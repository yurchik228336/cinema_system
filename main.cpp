#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

// Глобальные константы
const int HALLS = 2;              // Количество залов
const int ROWS = 5;               // Количество рядов в каждом зале
const int SEATS_PER_ROW = 10;     // Мест в ряду

// Глобальные массивы для хранения данных о местах в каждом зале
vector<vector<vector<bool>>> occupied(HALLS, vector<vector<bool>>(ROWS, vector<bool>(SEATS_PER_ROW, false)));
vector<vector<vector<int>>> prices(HALLS, vector<vector<int>>(ROWS, vector<int>(SEATS_PER_ROW, 0)));
vector<vector<vector<string>>> customerNames(HALLS, vector<vector<string>>(ROWS, vector<string>(SEATS_PER_ROW, "")));
vector<string> movieNames(HALLS);
vector<string> showTimes(HALLS);

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void printHallLayout(int hallIndex) {
    cout << "\nЗал " << hallIndex + 1 << ": Фильм - " << movieNames[hallIndex] 
         << ", Время: " << showTimes[hallIndex] << "\n\n";
    cout << "    ";
    for (int i = 0; i < SEATS_PER_ROW; ++i) {
        cout << setw(3) << i + 1;
    }
    cout << "\n";

    for (int i = 0; i < ROWS; ++i) {
        cout << setw(3) << i + 1 << " ";
        for (int j = 0; j < SEATS_PER_ROW; ++j) {
            if (occupied[hallIndex][i][j]) {
                cout << "[X]";
            } else {
                cout << "[ ]";
            }
        }
        cout << "\n";
    }
}

void initializeHalls() {
    // Инициализация первого зала
    movieNames[0] = "Мстители";
    showTimes[0] = "19:00";
    
    // Инициализация второго зала
    movieNames[1] = "Властелин колец";
    showTimes[1] = "20:30";
    
    // Установка цен на места для каждого зала
    for (int hall = 0; hall < HALLS; ++hall) {
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < SEATS_PER_ROW; ++j) {
                if (i < 2) prices[hall][i][j] = 500;      // Передние ряды
                else if (i < 4) prices[hall][i][j] = 300; // Средние ряды
                else prices[hall][i][j] = 200;            // Задние ряды
            }
        }
    }
}

bool buyTicket(int hallIndex, int row, int seat) {
    if (row < 1 || row > ROWS || seat < 1 || seat > SEATS_PER_ROW) {
        cout << "Неверный номер ряда или места!\n";
        return false;
    }
    
    row--; seat--; // Преобразование в индексы массива
    
    if (occupied[hallIndex][row][seat]) {
        cout << "Это место уже занято!\n";
        return false;
    }
    
    string name;
    cout << "Введите имя покупателя: ";
    getline(cin, name);
    
    occupied[hallIndex][row][seat] = true;
    customerNames[hallIndex][row][seat] = name;
    cout << "Билет успешно продан! Цена: " << prices[hallIndex][row][seat] << " руб.\n";
    return true;
}

void infiniteOutput() {
    int counter = 0;
    while (true) {
        cout << "Debug output #" << counter++ << endl;
        #ifdef _WIN32
            Sleep(1000);
        #else
            usleep(1000000);
        #endif
    }
}

int main() {
    char mode;
    cout << "Выберите режим (D для отладки, любая другая клавиша для обычного режима): ";
    cin >> mode;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (mode == 'D' || mode == 'd') {
        infiniteOutput();
        return 0;
    }

    initializeHalls();

    while (true) {
        clearScreen();
        cout << "\n=== Система бронирования билетов ===\n";
        cout << "1. Показать схему зала 1\n";
        cout << "2. Показать схему зала 2\n";
        cout << "3. Купить билет\n";
        cout << "4. Выход\n";
        cout << "Выберите действие: ";
        
        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (choice == 4) break;
        
        switch(choice) {
            case 1:
                clearScreen();
                printHallLayout(0);
                cout << "\nНажмите Enter для продолжения...";
                cin.get();
                break;
                
            case 2:
                clearScreen();
                printHallLayout(1);
                cout << "\nНажмите Enter для продолжения...";
                cin.get();
                break;
                
            case 3: {
                clearScreen();
                cout << "Выберите зал (1 или 2): ";
                int hallChoice;
                cin >> hallChoice;
                
                if (hallChoice < 1 || hallChoice > HALLS) {
                    cout << "Неверный номер зала!\n";
                    cout << "Нажмите Enter для продолжения...";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin.get();
                    break;
                }
                
                hallChoice--; // Преобразование в индекс массива
                clearScreen();
                printHallLayout(hallChoice);
                
                int row, seat;
                cout << "Введите номер ряда (1-" << ROWS << "): ";
                cin >> row;
                cout << "Введите номер места (1-" << SEATS_PER_ROW << "): ";
                cin >> seat;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                buyTicket(hallChoice, row, seat);
                
                cout << "Нажмите Enter для продолжения...";
                cin.get();
                break;
            }
            
            default:
                cout << "Неверный выбор!\n";
                cout << "Нажмите Enter для продолжения...";
                cin.get();
                break;
        }
    }
    
    return 0;
}
