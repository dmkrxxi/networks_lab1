#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <conio.h>
#include <format>
#include <filesystem>

using namespace std;
using namespace std::chrono;
using namespace std::filesystem;

int main() {
    //setlocale(LC_ALL, "RU");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int key = 0;
    bool exitFlag = false;

    while (key != 27) {
        cout << "Введите фамилию и 4 целочисленные оценки через пробел: ";
        string surname;
        int g[4];
        if (!(cin >> surname >> g[0] >> g[1] >> g[2] >> g[3])) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Ошибка ввода! Попробуйте еще раз." << endl;
            continue;
        }
        string extra;
        getline(cin, extra);
        bool hasExtra = false;
        for (char c : extra) {
            if (!isspace(static_cast<unsigned char>(c))) {
                hasExtra = true;
                break;
            }
        }
        if (hasExtra) {
            cout << "Ошибка: Введено слишком много оценок" << endl;
            continue;
        }

        // Удаляем старый файл результата, если он существует
        string filename = format("{}.txt", surname); // f2.txt
        if (exists(filename))
            remove(filename);

        // Запись запроса в общий файл
        fstream requests("f1.txt", ios::app | ios::out);
        if (!requests.is_open()) {
            cerr << "Ошибка открытия f1.txt" << endl;
            return 1;
        }
        requests << surname << " " << g[0] << " " << g[1] << " " << g[2] << " " << g[3] << endl;
        requests.close();

        cout << "Запрос отправлен. Ожидание ответа..." << endl;

        // Ожидание появления файла с результатом
        bool exitFlag = false;
        while (!exists(filename)) {
            if (_kbhit()) {
                if (_getch() == 27) {
                    exitFlag = true;
                    break;
                }
            }
            this_thread::sleep_for(chrono::milliseconds(200));
        }

        if (exitFlag) break;

        // Чтение результата
        ifstream result(filename);
        if (result.is_open()) {
            string responseLine;
            if (getline(result, responseLine)) {
                cout << "Ответ сервера: " << responseLine << endl;
            }
            result.close();
            // Удаляем файл после прочтения, чтобы не мешал следующим запросам
            remove(filename);
        }
        else {
            cerr << "Не удалось открыть файл результата." << endl;
        }

        // Проверка нажатия Esc после вывода
        if (_kbhit())
            key = _getch();
    }

    cout << "Клиент завершил работу." << endl;
    return 0;
}
