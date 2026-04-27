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

    string fin = "f1.bin";
    fstream file(fin, ios::in | ios::binary);
    if (!file.is_open()) {
        ofstream create(fin, ios::binary);
        create.close();
        file.open(fin, ios::in | ios::binary);
    }

    file.seekg(0, ios::end);
    streamsize oldSize = file.tellg();
    int key = 0;

    cout << "Сервер запущен. Ожидание запросов..." << endl;

    while (key != 27) {
        file.clear();
        file.seekg(0, ios::end);
        streamsize newSize = file.tellg();

        if (newSize > oldSize) {
            file.seekg(oldSize);

            while (file.tellg() < newSize) {
                Student st;
                file.read((char*)&st, sizeof(Student));

                if (file.gcount() == sizeof(Student)) {
                    string surname = st.surname;
                    double avg = (st.grades[0] + st.grades[1] + st.grades[2] + st.grades[3]) / 4.0;

                    cout << "Обработка: " << surname << " (" << avg << ")" << endl;

                    string response;
                    int minGrade = 5;
                    for (int i = 0; i < 4; i++) {
                        if (st.grades[i] < minGrade) {
                            minGrade = st.grades[i];
                        }
                    }
                    if (minGrade < 3) {
                        response = format("Есть задолженности. Средний балл: {}. Стипендии нет.", avg);
                    }
                    else if (minGrade == 3) {
                        response = format("Задолженностей нет. Балл: {}. Стипендии нет.", avg);
                    }
                    else if (minGrade == 4) {
                        response = format("Задолженностей нет. Балл: {}. Стипендия: 2200р.", avg);
                    }
                    else {
                        response = format("Задолженностей нет. Балл: {}. Стипендия: 3300р.", avg);
                    }

                    Response res;
                    memset(&res, 0, sizeof(Response));
                    strncpy_s(res.message, response.c_str(), _TRUNCATE);

                    ofstream resFile(format("{}.bin", surname), ios::binary); // f2.txt
                    if (resFile.is_open()) {
                        resFile.write((char*)&res, sizeof(Response));
                        resFile.close();
                        cout << "Ответ записан." << endl << endl;
                    }
                }
            }
            oldSize = newSize;
        }

        if (_kbhit()) key = _getch();
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    return 0;
}