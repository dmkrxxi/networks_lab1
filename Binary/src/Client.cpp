#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <conio.h>
#include <format>
#include <filesystem>
#include <cstring>

using namespace std;
using namespace std::filesystem;

struct Student {
    char surname[32];
    int grades[4];
};

struct Response {
    char message[128];
};

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int key = 0;
    while (key != 27) {
        cout << "Введите фамилию и 4 оценки через пробел: ";
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

        Student st;
        memset(&st, 0, sizeof(Student));
        strncpy_s(st.surname, surname.c_str(), _TRUNCATE);
        for (int i = 0; i < 4; i++) st.grades[i] = g[i];

        string filename = format("{}.bin", surname); // f2.txt
        if (exists(filename))
            remove(filename);

        fstream requests("f1.bin", ios::app | ios::binary);
        requests.write((char*)&st, sizeof(Student));
        requests.close();

        cout << "Запрос отправлен. Ожидание ответа..." << endl;

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

        ifstream result(filename, ios::binary);
        if (result.is_open()) {
            Response res;
            result.read((char*)&res, sizeof(Response));
            if (result.gcount() == sizeof(Response)) {
                cout << "Ответ сервера: " << res.message << endl << endl;
            }
            result.close();
            remove(filename);
        }

        if (_kbhit())
            key = _getch();
    }

    cout << "Клиент завершил работу." << endl;
    return 0;
}