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
        cout << "Ââåäèòå ôàìèëèþ è 4 öåëî÷èñëåííûå îöåíêè ÷åðåç ïðîáåë: ";
        string surname;
        int g[4];
        if (!(cin >> surname >> g[0] >> g[1] >> g[2] >> g[3])) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Îøèáêà ââîäà! Ïîïðîáóéòå åùå ðàç." << endl;
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
            cout << "Îøèáêà: Ââåäåíî ñëèøêîì ìíîãî îöåíîê" << endl;
            continue;
        }

        // Óäàëÿåì ñòàðûé ôàéë ðåçóëüòàòà, åñëè îí ñóùåñòâóåò
        string filename = format("{}.txt", surname); // f2.txt
        if (exists(filename))
            remove(filename);

        // Çàïèñü çàïðîñà â îáùèé ôàéë
        fstream requests("f1.txt", ios::app | ios::out);
        if (!requests.is_open()) {
            cerr << "Îøèáêà îòêðûòèÿ f1.txt" << endl;
            return 1;
        }
        requests << surname << " " << g[0] << " " << g[1] << " " << g[2] << " " << g[3] << endl;
        requests.close();

        cout << "Çàïðîñ îòïðàâëåí. Îæèäàíèå îòâåòà..." << endl;

        // Îæèäàíèå ïîÿâëåíèÿ ôàéëà ñ ðåçóëüòàòîì
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

        // ×òåíèå ðåçóëüòàòà
        ifstream result(filename);
        if (result.is_open()) {
            string responseLine;
            if (getline(result, responseLine)) {
                cout << "Îòâåò ñåðâåðà: " << responseLine << endl;
            }
            result.close();
            // Óäàëÿåì ôàéë ïîñëå ïðî÷òåíèÿ, ÷òîáû íå ìåøàë ñëåäóþùèì çàïðîñàì
            remove(filename);
        }
        else {
            cerr << "Íå óäàëîñü îòêðûòü ôàéë ðåçóëüòàòà." << endl;
        }

        // Ïðîâåðêà íàæàòèÿ Esc ïîñëå âûâîäà
        if (_kbhit())
            key = _getch();
    }

    cout << "Êëèåíò çàâåðøèë ðàáîòó." << endl;
    return 0;
}
