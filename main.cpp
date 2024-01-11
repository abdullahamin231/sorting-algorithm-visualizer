#include <SFML/Graphics.hpp>
#include<iostream>
#include<fstream>
#include<ctime>

using namespace std;
using namespace sf;

Color blockColor(209,150,197);
Color current(202,71,84);

void swap(int& a, int& b){
    int temp = a;
    a = b;
    b = temp;
}

class dataset{
private:
    int size = 50;
    int *arr = new int[size];
    RectangleShape* blocks = new RectangleShape[size];
    float gap = 1.5f;
    float paddingRight = 10.0f, paddingleft = 5.0f;
public:
    bool sorted = false;
    void setSorted(){sorted = !sorted;}
    dataset(){
        for(int i = 0; i < size; i++){
            arr[i] = rand()%50+1;
        }
        for(int i = 0; i < size; i++){
            Vector2f blockSize{10.0f, arr[i]*10.0f};
            blocks[i].setFillColor(blockColor);
            blocks[i].setSize(blockSize);
             // + 10.f is padding from left
             // 2.5f is gap between blocks
            blocks[i].setPosition(i*(1850.0f/size + gap) + paddingleft - paddingRight, 5.0f);
        }
    }
    void draw(RenderWindow& window){
        for(int i = 0; i < size; i++){
            // -5.0 accomodates for gap between blocks
            Vector2f blockSize{(1850.0f/size -(2*gap)), arr[i]*11.0f};
            // Vector2f blockSize{25.0f, arr[i]*10.0f};
            blocks[i].setSize(blockSize);
            window.draw(blocks[i]);
        }
    }
    ~dataset(){
        delete[] arr;
        delete[] blocks;
    }
    void randomize(){
        for(int i = 0; i < size; i++){
            Color r{rand()%255+1, rand()%255+1, rand()%255+1};
            blocks[i].setFillColor(r);
            arr[i] = rand()%50+1;
        }
    }
    
    int* getArr(){
        return arr;
    }
    int getSize(){
        return size;
    }
    RectangleShape* getBlocks(){
        return blocks;
    }
    void print(){
        for(int i = 0; i < size; i++){
            cout << arr[i] << " ";
        }
        cout << endl;
    }


};

class algos : public dataset{
private:
    int waitingPeriod = 50;
    int *p; int size;
    RectangleShape* blocks;
    RenderWindow& window;
    dataset& data;
public:
    algos(int* p, int size, RenderWindow& window, dataset& data, RectangleShape* blocks):p(p), size(size), window(window), data(data), blocks(blocks){};
    void setWaitingPeriod(int waitingPeriod){
        this->waitingPeriod = waitingPeriod;
    }
    void repeatable(Clock& clock, RenderWindow& window, dataset& data, int size, RectangleShape* blocks){
        while (clock.getElapsedTime().asMilliseconds() < waitingPeriod) {
                // Update the display during the waiting period
                window.clear(Color::White);
                data.draw(window);
                window.display();
            }
            for (int j = 0; j < size; j++) {
                blocks[j].setFillColor(blockColor);
            }
    
            window.clear(Color::White);
            data.draw(window);
            window.display();
    }
    // Selection and Bubble Sort
    void bubbleSort(int* p, int size, RectangleShape* blocks, RenderWindow& window, dataset& data) {
        Clock clock;

        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - i - 1; j++) {
                if (p[j] > p[j + 1]) {
                    swap(p[j], p[j + 1]);
                    blocks[j].setFillColor(current);
                    blocks[j + 1].setFillColor(current);
                }
            }
            clock.restart();
            repeatable(clock, window, data, size, blocks);
        }
    }
    void selectionSort(){ 
        int i, j, minIndex; 
        Clock clock;
        for (i = 0; i < size - 1; i++) { 
            minIndex = i; 
            for (j = i + 1; j < size; j++) { 
                if (p[j] < p[minIndex]) 
                    minIndex = j; 
            }
            if (minIndex != i){
                swap(p[minIndex], p[i]);
                blocks[i].setFillColor(current);
                blocks[minIndex].setFillColor(current);
            }
            clock.restart();
            repeatable(clock, window, data, size, blocks);
            
        } 
    } 
    // Merge Sort
    void merge(int *a, int low, int mid, int high){
        int rightSize = high - mid;
        int leftSize = mid - low + 1;
        int *L = new int[leftSize]; int *M = new int[rightSize];
        for(int i = 0; i < leftSize; i++)
            L[i] = a[low+i];
        for(int j = 0; j < rightSize; j++)
            M[j] = a[mid+j+1];
        
        int i, j, k;
        i = 0;
        j = 0;
        k = low;

        while(i < leftSize && j < rightSize){
            if(L[i] <= M[j]){
                a[k] = L[i]; i++;
            } else {
                a[k] = M[j]; j++;

            }
            k++;
        }
        while(i < leftSize){
            a[k] = L[i];
            i++;
            k++;
        }
        while(j < rightSize){
            a[k] = M[j];
            j++;
            k++;
        }
        delete[] L; delete[] M;
    }
    void mergeSort(int* a, int low, int high){
        Clock clock;
        if(low < high){
            int mid = (low+high)/2;
            mergeSort(p, low, mid);
            mergeSort(p, mid+1, high);
            merge(p, low, mid, high);
            clock.restart();
            while(clock.getElapsedTime().asMilliseconds() < waitingPeriod){
                window.clear(Color::White);
                data.draw(window);
                window.display();
            }
            // didn't apply repeatable here because needed to do low-high
            for(int i = low; i < high; i++){
                blocks[i].setFillColor(current);
            }
            window.clear(Color::White);
            data.draw(window);
            window.display();
        }
    }

    // Quick Sort
    int partition(int* a, int left, int right){
        int pivot = a[right];
        int i = (left - 1);
        for (int j = left; j < right; j++) {
            if (a[j] <= pivot) {
                i++;
                swap(a[i], a[j]);
            }
        }
        swap(a[i + 1], a[right]);
        return (i+1);
    }
    void visualise(RectangleShape* blocks, int left, int right){
        window.clear(Color::White);
            // Draw blocks
        for (int i = 0; i < data.getSize(); ++i) {
            if (i >= left && i <= right) {
                // Highlight the current portion being processed
                blocks[i].setFillColor(current);
            } else {
                blocks[i].setFillColor(blockColor);
            }
            window.draw(blocks[i]);
        }
        window.display();
    }
    void quickSort(int *a, int left, int right){
        Clock clock;
        if(left < right){ 
            int pi = partition(a, left, right);
            quickSort(a, left, pi-1);
            quickSort(a, pi+1, right);

            visualise(blocks, left, right);

            clock.restart();
            while(clock.getElapsedTime().asMilliseconds() < waitingPeriod){
                visualise(blocks, left, right);
            }
        }
    }

    void insertionSort(int *a) {
        Clock clock;
        for (int i = 1; i < size; i++) {
            int key = a[i];
            int j = i - 1;
            while (key < a[j] && j >= 0) {
            a[j + 1] = a[j];
            --j;
            }
            a[j + 1] = key;
            blocks[j + 1].setFillColor(current);
            blocks[j].setFillColor(current);

            clock.restart();
            while(clock.getElapsedTime().asMilliseconds() < waitingPeriod)
                repeatable(clock, window, data, size, blocks);  
        }
    }


};

int main()
{
    srand((unsigned) time(0));
    RenderWindow window(VideoMode(2000, 600), "Sorting Algorithms Visualiser");
    
    Color textColorRest(100,102,105);
    
    Font timeFont;
    timeFont.loadFromFile("font/RobotoMono-Regular.ttf");

    bool inMenu = true;
    string menus[] = {"{M}: Merge Sort", "{S}: Selection Sort", "{B}: Bubble Sort", "{Q}: Quick Sort", "{I}: Insertion Sort", "{R}: Randomize", "{Esc}: Menu", "{E} : Exit"};
    Text menu[7];
    Font font;
    font.loadFromFile("font/PlayfairDisplay-ExtraBold.ttf");
    for(int i = 0; i < 7; i++){
        menu[i].setFont(font);
        menu[i].setCharacterSize(44);
        menu[i].setFillColor(textColorRest);
        menu[i].setString(menus[i]);
        menu[i].setPosition(window.getSize().x / 2.0f - menu[i].getGlobalBounds().width / 2.0f, window.getSize().y / 2.0f - menu[i].getGlobalBounds().height / 2.0f + i*55.0f - 150.0f);
    }

    Text title;
    title.setFont(font);
    title.setCharacterSize(64);
    title.setFillColor(textColorRest);
    title.setString("Sorting Algorithms Visualiser");
    title.setPosition(window.getSize().x / 2.0f - title.getGlobalBounds().width / 2.0f, window.getSize().y / 2.0f - title.getGlobalBounds().height / 2.0f - 250.0f);

    Text timeText;
    timeText.setFont(timeFont);
    timeText.setCharacterSize(24);
    timeText.setFillColor(textColorRest);
    timeText.setPosition(10.0f, 550.0f);


    fstream file("time.txt", ios::app);
    dataset data;
    algos solver(data.getArr(), data.getSize(), window, data, data.getBlocks());
    while (window.isOpen())
    {
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();
            else if(e.type == Event::KeyPressed){
                inMenu = false;
                if (e.key.code == Keyboard::Escape){
                    inMenu = true;
                } else if(e.key.code == Keyboard::E){
                    window.close();
                } else if(e.key.code == Keyboard::R){
                    data.randomize();
                    data.sorted = false;
                } else if(e.key.code == Keyboard::M && data.sorted == false){

                    cout << "merge\n";
                    Clock timer;
                    solver.mergeSort(data.getArr(), 0, data.getSize()); data.setSorted();
                    float mergeSortTime = timer.getElapsedTime().asSeconds();
                    file << "Merge Sort: " << mergeSortTime << endl;
                    timer.restart();
                    timeText.setString("Time Taken (merge sort): " + to_string(mergeSortTime) + "s");

                } else if (e.key.code == Keyboard::S && data.sorted == false){
                    
                    cout << "Selection\n";
                    Clock timer;
                    solver.selectionSort();
                    float selectionSortTime = timer.getElapsedTime().asSeconds(); data.setSorted();
                    file << "Selection Sort: " << selectionSortTime << endl;
                    timer.restart();
                    timeText.setString("Time Taken (selection sort): " + to_string(selectionSortTime) + "s");

                } else if (e.key.code == Keyboard::B && data.sorted == false){

                    cout << "Bubble\n";
                    Clock timer; 
                    solver.bubbleSort(data.getArr(), data.getSize(), data.getBlocks(), window, data);
                    float bubbleSortTime = timer.getElapsedTime().asSeconds(); data.setSorted();
                    file << "Bubble Sort: " << bubbleSortTime << endl;
                    timer.restart();
                    timeText.setString("Time Taken (bubble sort): " + to_string(bubbleSortTime) + "s");

                }   else if (e.key.code == Keyboard::Q && data.sorted == false){

                    cout << "quick\n";
                    Clock timer; 
                    solver.quickSort(data.getArr(), 0, data.getSize());
                    float quickSortTime = timer.getElapsedTime().asSeconds(); data.setSorted();
                    file << "Quick Sort: " << quickSortTime << endl;
                    timer.restart();
                    timeText.setString("Time Taken (quick sort): " + to_string(quickSortTime) + "s");

                } else if (e.key.code == Keyboard::I && data.sorted == false){

                    cout << "insertion\n";
                    Clock timer; 
                    solver.insertionSort(data.getArr());
                    float insertionSortTime = timer.getElapsedTime().asSeconds(); data.setSorted();
                    file << "Insertion Sort: " << insertionSortTime << endl;
                    timer.restart();
                    timeText.setString("Time Taken (insertion sort): " + to_string(insertionSortTime) + "s");

                } 
            }
        }
        window.clear(Color::White);

        if(inMenu){
            window.draw(title);
            for(int i = 0; i < 7; i++){
                window.draw(menu[i]);
            }
        } else{
            window.draw(timeText);
            data.draw(window);
        }
        
        window.display();
    }
    file.close();

    return 0;
}
