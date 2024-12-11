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

const int HALLS = 3;              
const int ROWS = 5;               
const int SEATS_PER_ROW = 10;     


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
    cout << "\n" << string(40, '=') << "\n";
    cout << "Зал " << hallIndex + 1 << " | Фильм: " << movieNames[hallIndex]
         << " | Время: " << showTimes[hallIndex] << "\n";
    cout << string(40, '=') << "\n\n";
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
    movieNames[0] = "Мстители";
    showTimes[0] = "19:00";

    movieNames[1] = "Властелин колец";
    showTimes[1] = "20:30";

    movieNames[2] = "Гарри Поттер";
    showTimes[2] = "18:00";

    for (int hall = 0; hall < HALLS; ++hall) {
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < SEATS_PER_ROW; ++j) {
                if (i < 2) prices[hall][i][j] = 500;
                else if (i < 4) prices[hall][i][j] = 300;
                else prices[hall][i][j] = 200;
            }
        }
    }
}

bool autoBuyTickets(int hallIndex, int ticketCount, int preferredRow, vector<pair<int, int>>& seats) {
    preferredRow--; 


    for (int col = 0; col <= SEATS_PER_ROW - ticketCount; ++col) {
        bool canPlace = true;
        for (int k = 0; k < ticketCount; ++k) {
            if (occupied[hallIndex][preferredRow][col + k]) {
                canPlace = false;
                break;
            }
        }
        if (canPlace) {
            for (int k = 0; k < ticketCount; ++k) {
                seats.emplace_back(preferredRow, col + k);
            }
            return true;
        }
    }

    for (int row = 0; row < ROWS; ++row) {
        if (row == preferredRow) continue;
        for (int col = 0; col <= SEATS_PER_ROW - ticketCount; ++col) {
            bool canPlace = true;
            for (int k = 0; k < ticketCount; ++k) {
                if (occupied[hallIndex][row][col + k]) {
                    canPlace = false;
                    break;
                }
            }
            if (canPlace) {
                for (int k = 0; k < ticketCount; ++k) {
                    seats.emplace_back(row, col + k);
                }
                return true;
            }
        }
    }

    return false;
}

void printSuccessMessage(int totalCost, int hallIndex) {
    cout << "\n" << string(40, '=') << "\n";
    cout << "\033[1;32mБИЛЕТЫ УСПЕШНО ПРОДАНЫ!\033[0m\n"; 
    cout << "Общая стоимость: " << totalCost << " руб.\n";
    cout << string(40, '=') << "\n";
    cout << "\033[1;34mЧто хотите сделать дальше?\033[0m\n";
    cout << "1. Вернуться в главное меню\n";
    cout << "2. Показать схему зала " << hallIndex + 1 << "\n";
    cout << "\033[0mВыберите действие: ";
}


void printCancelMessage() {
    cout << "\n" << string(40, '=') << "\n";
    cout << "\033[1;31mПокупка ОТМЕНЕНА!\033[0m\n"; 
    cout << string(40, '=') << "\n";
    cout << "\033[1;34mЧто хотите сделать дальше?\033[0m\n";
    cout << "1. Вернуться в главное меню\n";
    cout << "2. Показать схему зала\n";
    cout << "\033[0mВыберите действие: ";
}

bool buyTickets(int hallIndex) {
    clearScreen();
    printHallLayout(hallIndex);

    cout << "Выберите режим покупки:\n";
    cout << "1. Покупка по одному билету\n";
    cout << "2. Быстрая покупка нескольких билетов\n";
    cout << "Ваш выбор: ";
    int mode;
    cin >> mode;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (mode == 1) {
        while (true) {
            int row, seat;
            cout << "Введите номер ряда (1-" << ROWS << "): ";
            cin >> row;
            cout << "Введите номер места (1-" << SEATS_PER_ROW << "): ";
            cin >> seat;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (row < 1 || row > ROWS || seat < 1 || seat > SEATS_PER_ROW) {
                cout << "Неверный номер ряда или места! Попробуйте снова.\n";
                continue;
            }

            row--;
            seat--;

            if (occupied[hallIndex][row][seat]) {
                cout << "Это место уже занято! Попробуйте снова.\n";
                continue;
            }

            string name;
            cout << "Введите имя покупателя: ";
            getline(cin, name);

            occupied[hallIndex][row][seat] = true;
            customerNames[hallIndex][row][seat] = name;

            cout << "Билет успешно продан! Цена: " << prices[hallIndex][row][seat] << " руб.\n";

            cout << "Хотите купить ещё один билет? (y/n): ";
            char more;
            cin >> more;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (more != 'y' && more != 'Y') break;
        }
        return true;
    } else if (mode == 2) {
        cout << "Введите количество билетов: ";
        int ticketCount;
        cin >> ticketCount;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Введите предпочитаемый ряд (1-" << ROWS << "): ";
        int preferredRow;
        cin >> preferredRow;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        vector<pair<int, int>> seats;
        if (!autoBuyTickets(hallIndex, ticketCount, preferredRow, seats)) {
            cout << "Невозможно разместить все билеты рядом.\n";
            return false;
        }

        vector<string> names(ticketCount);
        int totalCost = 0;

        for (int i = 0; i < ticketCount; ++i) {
            cout << "Введите имя покупателя для билета " << i + 1 << ": ";
            getline(cin, names[i]);
        }

        cout << "\nВы выбрали следующие места:\n";
        for (int i = 0; i < ticketCount; ++i) {
            int row = seats[i].first + 1;
            int col = seats[i].second + 1;
            totalCost += prices[hallIndex][seats[i].first][seats[i].second];
            cout << "Место: Ряд " << row << ", Место " << col << " - " << names[i] << "\n";
        }

        cout << "\nИтоговая стоимость: " << totalCost << " руб.\n";
        cout << "Подтвердите покупку (y/n): ";
        char confirm;
        cin >> confirm;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (confirm == 'y' || confirm == 'Y') {
            for (int i = 0; i < ticketCount; ++i) {
                int row = seats[i].first;
                int col = seats[i].second;
                occupied[hallIndex][row][col] = true;
                customerNames[hallIndex][row][col] = names[i];
            }

            printSuccessMessage(totalCost, hallIndex);


            while (true) {
                
                int choice;
                cin >> choice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                if (choice == 1) {
                    return true; 
                } else if (choice == 2) {
                    clearScreen();
                    printHallLayout(hallIndex); 
                    cout << "\nНажмите Enter для возврата в меню...\n";
                    cin.ignore(); 
                    break; 
                } else {
                    cout << "Неверный выбор. Попробуйте снова.\n";
                }
            }
        } else {
            printCancelMessage();

            while (true) {
                
                int choice;
                cin >> choice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                if (choice == 1) {
                    return true; 
                } else if (choice == 2) {
                    clearScreen();
                    printHallLayout(hallIndex); 
                    cout << "\nНажмите Enter для возврата в меню...\n";
                    cin.ignore(); 
                    break; 
                } else {
                    cout << "Неверный выбор. Попробуйте снова.\n";
                }
            }
        }

        return true;
    } else {
        cout << "Неверный выбор!\n";
        return false;
    }
}

void displaySchedule() {
    cout << string(40, '=') << "\n";
    cout << "РАСПИСАНИЕ ФИЛЬМОВ:\n";
    for (int hall = 0; hall < HALLS; ++hall) {
        cout << "Зал " << hall + 1 << ": " << movieNames[hall] << " в " << showTimes[hall] << "\n";
    }
    cout << string(40, '=') << "\n";
}

int main() {
    initializeHalls();
    while (true) {
        clearScreen();
        displaySchedule();
        cout << "\n1. Купить билет\n";
        cout << "2. Посмотреть схему зала\n";
        cout << "3. Выход\n";
        cout << "Выберите действие: ";
        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 3) break;

        if (choice == 1) {
            cout << "Выберите зал (1-" << HALLS << "): ";
            int hallIndex;
            cin >> hallIndex;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (hallIndex < 1 || hallIndex > HALLS) {
                cout << "Неверный номер зала! Попробуйте снова.\n";
                continue;
            }
            buyTickets(hallIndex - 1);
        } else if (choice == 2) {
            cout << "Выберите зал (1-" << HALLS << "): ";
            int hallIndex;
            cin >> hallIndex;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (hallIndex < 1 || hallIndex > HALLS) {
                cout << "Неверный номер зала! Попробуйте снова.\n";
                continue;
            }
            clearScreen();
            printHallLayout(hallIndex - 1);
            cout << "Нажмите Enter для возврата в меню...";
            cin.ignore();
        } else {
            cout << "Неверный выбор! Попробуйте снова.\n";
        }
    }

    cout << "Программа завершена. Спасибо за использование!\n";
    return 0;
}
