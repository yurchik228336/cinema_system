
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <regex> 
using namespace std;

const int HALLS = 3;              
const int ROWS = 5;               
const int SEATS_PER_ROW = 10;     

vector<vector<vector<bool>>> occupied(HALLS, vector<vector<bool>>(ROWS, vector<bool>(SEATS_PER_ROW, false)));
vector<vector<vector<int>>> prices(HALLS, vector<vector<int>>(ROWS, vector<int>(SEATS_PER_ROW, 0)));
vector<vector<vector<string>>> customerNames(HALLS, vector<vector<string>>(ROWS, vector<string>(SEATS_PER_ROW, "")));
vector<string> movieNames(HALLS);
vector<string> showTimes(HALLS);


int getMenuChoice() {
    string input;
    int choice;

    while (true) {
        cout << "Выберите действие: ";
        cin >> input;

        if (regex_match(input, regex("^[+-]?[0-9]+$"))) {
            choice = stoi(input); 
            if (choice >= 1 && choice <= 4) {
                return choice;
            }
        }

        cout << "Неверный ввод! Пожалуйста, введите целое число от 1 до 4.\n";
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

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void displaySchedule() {
    cout << string(40, '=') << "\n";
    cout << "РАСПИСАНИЕ ФИЛЬМОВ:\n";
    for (int hall = 0; hall < HALLS; ++hall) {
        cout << "Зал " << hall + 1 << ": " << movieNames[hall] << " в " << showTimes[hall] << "\n";
    }
    cout << string(40, '=') << "\n";
}

void printHallLayout(int hallIndex) {
    cout << "\n" << string(40, '=') << "\n";
    cout << "Зал " << hallIndex + 1 << " | Фильм: " << movieNames[hallIndex]
         << " | Время: " << showTimes[hallIndex] << "\n";
    cout << string(40, '=') << "\n\n";

    cout << "\033[1;32m[ ]\033[0m - Свободное место (500р)\n";
    cout << "\033[1;38;5;214m[ ]\033[0m - Свободное место (300р)\n"; 
    cout << "\033[1;34m[ ]\033[0m - Свободное место (200р)\n"; 
    cout << "\033[1;31m[X]\033[0m - Занято\n";
    cout << "\n";
    cout << "   ";
    for (int i = 0; i < SEATS_PER_ROW; ++i) {
        cout << setw(3) << i + 1;
    }
    cout << "\n";
    
    for (int i = 0; i < ROWS; ++i) {
        cout << setw(3) << i + 1 << " ";
        for (int j = 0; j < SEATS_PER_ROW; ++j) {
            if (occupied[hallIndex][i][j]) {
                cout << "\033[1;31m[X]\033[0m";  
            } else {
                if (i < 2) {
                    cout << "\033[1;32m[ ]\033[0m";  
                } else if (i < 4) {
                    cout << "\033[1;38;5;214m[ ]\033[0m";  
                } else {
                    cout << "\033[1;34m[ ]\033[0m"; 
                }
            }
        }
        cout << "\n";
    }
}



bool autoBuyTickets(int hallIndex, int ticketCount, int preferredRow, vector<pair<int, int>>& seats) {
    preferredRow--;  


    int ticketsPlaced = 0;
    for (int row = preferredRow; row < ROWS && ticketsPlaced < ticketCount; ++row) {
        for (int col = 0; col < SEATS_PER_ROW && ticketsPlaced < ticketCount; ++col) {
            if (!occupied[hallIndex][row][col]) {
                seats.emplace_back(row, col);
                ticketsPlaced++;
            }
        }
    }

    for (int row = 0; row < ROWS && ticketsPlaced < ticketCount; ++row) {
        if (row == preferredRow) continue;  
        for (int col = 0; col < SEATS_PER_ROW && ticketsPlaced < ticketCount; ++col) {
            if (!occupied[hallIndex][row][col]) {
                seats.emplace_back(row, col);
                ticketsPlaced++;
            }
        }
    }

    return ticketsPlaced == ticketCount;
}

void printSuccessMessage(int totalCost, int hallIndex) {
    clearScreen();
    cout << "\n" << string(40, '=') << "\n";
    cout << "\033[1;32mБИЛЕТ(Ы) УСПЕШНО ПРОДАН(Ы)!\033[0m\n"; 
    cout << "Общая стоимость: " << totalCost << " руб.\n";
    cout << string(40, '=') << "\n";
    
}

void printCancelMessage() {
    clearScreen();
    cout << "\n" << string(40, '=') << "\n";
    cout << "\033[1;31mПокупка ОТМЕНЕНА!\033[0m\n"; 
    cout << string(40, '=') << "\n";

}

int countAvailableSeats(int hallIndex) {
    int availableSeats = 0;
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < SEATS_PER_ROW; ++j) {
            if (!occupied[hallIndex][i][j]) {
                ++availableSeats;
            }
        }
    }
    return availableSeats;
}

bool buyTickets(int hallIndex) {
    clearScreen();
    printHallLayout(hallIndex);

    int totalAvailableSeats = countAvailableSeats(hallIndex);
    if (totalAvailableSeats == 0) {
        cout << "Извините, все места заняты в этом зале.\n";
        cout << "Нажмите Enter для возврата в меню...";
        cin.ignore();
        cin.get();
        clearScreen();
        return true;  
    }

    cout << "Выберите режим покупки:\n";
    cout << "1. Покупка по одному билету\n";
    cout << "2. Быстрая покупка нескольких билетов\n";
    cout << "\n3. Выйти в меню\n\n";
    cout << "Ваш выбор: ";
    int mode;
    while (!(cin >> mode) || mode > 3 || mode <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Неверный ввод! Пожалуйста, введите число: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (mode == 3) {
        clearScreen();
        return true;
    }

    if (mode == 1) {
        while (true) {
            int row, seat;
            cout << "Введите номер ряда (1-" << ROWS << "): ";
            cin >> row;
            cout << "Введите номер места (1-" << SEATS_PER_ROW << "): ";
            cin >> seat;

            if (row < 1 || row > ROWS || seat < 1 || seat > SEATS_PER_ROW) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Неверный номер ряда или места! Попробуйте снова.\n";
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            row--;
            seat--;

            if (occupied[hallIndex][row][seat]) {
                cin.clear();
                cout << "Это место уже занято! Попробуйте снова.\n";
                continue;
            }

            string name;
            do {
                cout << "Введите имя покупателя: ";
                getline(cin, name);

                if (name.empty()) {
                    cout << "Имя не может быть пустым! Попробуйте снова.\n";
                }
            } while (name.empty());

            occupied[hallIndex][row][seat] = true;
            customerNames[hallIndex][row][seat] = name;

            clearScreen();
            cout << "\nПодтвердите покупку:\n\n";
            cout << "Фильм: " << movieNames[hallIndex] << "\n";
            cout << "Зал: " << hallIndex + 1 << "\n";
            cout << "Ваш ряд: " << row + 1 << "\n";
            cout << "Ваше место: " << seat + 1 << "\n";
            cout << "Ваше имя: " << name << "\n";
            cout << "Цена билета: " << prices[hallIndex][row][seat] << " руб.\n";

            cout << "\nДля подтверждения введите 'y', для отмены любой другой символ/строку ";
            char choice;
            cin >> choice;
            if (choice == 'y' || choice == 'Y') {
                printSuccessMessage(prices[hallIndex][row][seat], hallIndex);
            } else {
                occupied[hallIndex][row][seat] = false;
                customerNames[hallIndex][row][seat] = "";
                printCancelMessage();
            }
            break;
        }
    } else if (mode == 2) {
        cout << "Доступно мест: " << totalAvailableSeats << "\n";

        int ticketCount;
        cout << "Введите количество билетов (максимум " << totalAvailableSeats << "): ";
        while (!(cin >> ticketCount) || ticketCount <= 0 || ticketCount > totalAvailableSeats) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Неверное количество билетов! Введите число от 1 до " << totalAvailableSeats << ": ";
        }

        int preferredRow;
        cout << "Введите предпочтительный ряд: ";
        while (!(cin >> preferredRow) || preferredRow < 1 || preferredRow > ROWS) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Неверный номер ряда! Введите число от 1 до " << ROWS << ": ";
        }

        vector<pair<int, int>> seats;
        if (autoBuyTickets(hallIndex, ticketCount, preferredRow, seats)) {
            int totalCost = 0;
            for (auto& seat : seats) {
                totalCost += prices[hallIndex][seat.first][seat.second];
                string name;
                do {
                    cout << "\nРяд: " << seat.first + 1 << ", Место: " << seat.second + 1 << "\n";
                    cout << "Цена билета: " << prices[hallIndex][seat.first][seat.second] << " руб.\n";
                    cout << "Введите имя покупателя: ";
                    cin.ignore();
                    getline(cin, name);

                    if (name.empty()) {
                        cout << "Имя не может быть пустым! Попробуйте снова.\n";
                    }
                } while (name.empty());

                occupied[hallIndex][seat.first][seat.second] = true;
                customerNames[hallIndex][seat.first][seat.second] = name;
            }

            cout << "\nОбщая стоимость: " << totalCost << " руб.\n";
            cout << "Хотите подтвердить покупку? (y/n): ";
            char choice;
            cin >> choice;
            if (choice == 'y' || choice == 'Y') {
                printSuccessMessage(totalCost, hallIndex);
            } else {
                for (auto& seat : seats) {
                    occupied[hallIndex][seat.first][seat.second] = false;
                    customerNames[hallIndex][seat.first][seat.second] = "";
                }
                printCancelMessage();
            }
        } else {
            printCancelMessage();
        }
    }
    return true;
}
void cancelReservation() {
    clearScreen();
    
    cout << "Выберите зал для отмены брони (1-" << HALLS << "): ";
    int hallIndex;
    while (!(cin >> hallIndex) || hallIndex < 1 || hallIndex > HALLS) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Неверный номер зала! Попробуйте снова.\n";
        cout << "Выберите зал для отмены брони (1-" << HALLS << "): ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');  
    hallIndex--;

 

    cout << "Введите имя покупателя для снятия брони: ";
    string name;
    cin.ignore();
    getline(cin, name);

    bool found = false;
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < SEATS_PER_ROW; ++j) {
            if (occupied[hallIndex][i][j] && customerNames[hallIndex][i][j] == name) {
                occupied[hallIndex][i][j] = false;
                customerNames[hallIndex][i][j] = "";
                cout << "Бронирование на место " << i + 1 << ", " << j + 1 << " успешно отменено.\n";
                found = true;
            }
        }
    }

    if (!found) {
        cout << "Не найдено бронирования на указанное имя.\n";
    }

    cout << "Нажмите Enter для возврата в меню...";
    cin.get();
}
int main() {
    setlocale(LC_ALL, "ru");
    initializeHalls();
    while (true) {
        displaySchedule();
        cout << "\n1. Купить билет\n";
        cout << "2. Посмотреть схему зала\n";
        cout << "3. Отменить бронь\n";
        cout << "4. Выход\n";

        int choice = getMenuChoice(); 

        if (choice == 4) break;
        if (choice == 3) {
            cancelReservation();
        } else if (choice == 1) {
            cout << "Выберите зал (1-" << HALLS << "): ";
            int hallIndex;
            while (!(cin >> hallIndex) || hallIndex < 1 || hallIndex > HALLS) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Неверный номер зала! Попробуйте снова.\n";
                cout << "Выберите зал (1-" << HALLS << "): ";
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  
            buyTickets(hallIndex - 1);
        } else if (choice == 2) {
            cout << "Выберите зал (1-" << HALLS << "): ";
            int hallIndex;
            while (!(cin >> hallIndex) || hallIndex < 1 || hallIndex > HALLS) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Неверный номер зала! Попробуйте снова.\n";
                cout << "Выберите зал (1-" << HALLS << "): ";
            }
            clearScreen();
            printHallLayout(hallIndex - 1);
            cout << "Нажмите Enter для возврата в меню...";
            cin.ignore();
            cin.get();
            clearScreen();
        } else {
            cout << "Неверный выбор! Попробуйте снова.\n";
        }
    }

    cout << "Программа завершена. Спасибо за использование!\n";
    return 0;
}
