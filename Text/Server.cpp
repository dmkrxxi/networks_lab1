#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
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

    // Создаём файл запросов, если его нет
    fstream("f1.txt", ios::app).close();

    fstream file("f1.txt", ios::in | ios::binary);
    if (!file.is_open()) {
        cerr << "Не удалось открыть файл f1.txt" << endl;
        return 1;
    }

    int key = 0;
    streamsize oldSize = 0;
    file.seekg(0, ios::end);
    oldSize = file.tellg();

    std::cout << "Сервер запущен. Ожидание запросов..." << endl;

    while (key != 27) { // 27 — код Esc
        file.clear();
        file.seekg(0, ios::end);
        streamsize newSize = file.tellg();

        if (newSize > oldSize) {
            // Появились новые данные
            file.seekg(oldSize);
            string line;
            (getline(file, line)); {
                if (line.empty()) continue;
                istringstream iss(line);
                string surname;
                int g[4];
                if (iss >> surname >> g[0] >> g[1] >> g[2] >> g[3]) {
                    std::cout << "Получен запрос: " << surname << " "
                        << g[0] << " " << g[1] << " " << g[2] << " " << g[3] << endl;

                    string response;
                    double avg = (g[0] + g[1] + g[2] + g[3]) / 4.0;
                    int minGrade = 5;
                    for (int i = 0; i < 4; i++) {
                        if (g[i] < minGrade) {
                            minGrade = g[i];
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

                    string filename = format("{}.txt", surname);
                    ofstream result(filename); // перезаписываем файл ответа
                    if (result.is_open()) {
                        result << response << endl;
                        result.close();
                        std::cout << "Ответ записан в " << filename << endl << endl;
                    }
                }
                else {
                    cerr << "Некорректная строка: " << line << endl << endl;
                }
            }
            oldSize = file.tellg();
            if (!file) file.clear(); // сброс eof и ошибок
        }

        if (_kbhit()) {
            key = _getch();
        }

    }

    file.close();
    std::cout << "Сервер завершил работу." << endl;
    return 0;
}
