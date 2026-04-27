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

    // Ñîçäà¸ì ôàéë çàïðîñîâ, åñëè åãî íåò
    fstream("f1.txt", ios::app).close();

    fstream file("f1.txt", ios::in | ios::binary);
    if (!file.is_open()) {
        cerr << "Íå óäàëîñü îòêðûòü ôàéë f1.txt" << endl;
        return 1;
    }

    int key = 0;
    streamsize oldSize = 0;
    file.seekg(0, ios::end);
    oldSize = file.tellg();

    std::cout << "Ñåðâåð çàïóùåí. Îæèäàíèå çàïðîñîâ..." << endl;

    while (key != 27) { // 27 — êîä Esc
        file.clear();
        file.seekg(0, ios::end);
        streamsize newSize = file.tellg();

        if (newSize > oldSize) {
            // Ïîÿâèëèñü íîâûå äàííûå
            file.seekg(oldSize);
            string line;
            (getline(file, line)); {
                if (line.empty()) continue;
                istringstream iss(line);
                string surname;
                int g[4];
                if (iss >> surname >> g[0] >> g[1] >> g[2] >> g[3]) {
                    std::cout << "Ïîëó÷åí çàïðîñ: " << surname << " "
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
                        response = format("Åñòü çàäîëæåííîñòè. Ñðåäíèé áàëë: {}. Ñòèïåíäèè íåò.", avg);
                    }
                    else if (minGrade == 3) {
                        response = format("Çàäîëæåííîñòåé íåò. Áàëë: {}. Ñòèïåíäèè íåò.", avg);
                    }
                    else if (minGrade == 4) {
                        response = format("Çàäîëæåííîñòåé íåò. Áàëë: {}. Ñòèïåíäèÿ: 2200ð.", avg);
                    }
                    else {
                        response = format("Çàäîëæåííîñòåé íåò. Áàëë: {}. Ñòèïåíäèÿ: 3300ð.", avg);
                    }

                    string filename = format("{}.txt", surname);
                    ofstream result(filename); // ïåðåçàïèñûâàåì ôàéë îòâåòà
                    if (result.is_open()) {
                        result << response << endl;
                        result.close();
                        std::cout << "Îòâåò çàïèñàí â " << filename << endl << endl;
                    }
                }
                else {
                    cerr << "Íåêîððåêòíàÿ ñòðîêà: " << line << endl << endl;
                }
            }
            oldSize = file.tellg();
            if (!file) file.clear(); // ñáðîñ eof è îøèáîê
        }

        if (_kbhit()) {
            key = _getch();
        }

    }

    file.close();
    std::cout << "Ñåðâåð çàâåðøèë ðàáîòó." << endl;
    return 0;
}
