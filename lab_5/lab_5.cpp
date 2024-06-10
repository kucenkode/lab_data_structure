#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <chrono>
#include <Windows.h>
#include <cctype>

using namespace std;

vector<string> loadDictionary(const string& filename) {
    ifstream file(filename);
    vector<string> file_text;
    string word;

    if (file.is_open()) {
        while (file >> word) {
            transform(word.begin(), word.end(), word.begin(), ::tolower);
            word.erase(remove_if(word.begin(), word.end(), [](char c) { return ispunct(c); }), word.end());
            file_text.push_back(word);
        }
        file.close();
    }

    return file_text;
}

vector<string> find_words(const string& query, const vector<string>& file_text, int max_amount_of_searched_words) {
    string lower_query = query;
    transform(lower_query.begin(), lower_query.end(), lower_query.begin(), ::tolower);

    unordered_map<string, int> words_q;
    for (const string& word : file_text) {
        words_q[word]++;
    }

    vector<string> sorted_words;
    for (const auto& pair : words_q) {
        sorted_words.push_back(pair.first);
    }
    sort(sorted_words.begin(), sorted_words.end(), [&words_q](const string& a, const string& b) {
        return words_q[a] > words_q[b];
        });

    vector<string> found_words;
    for (const string& word : sorted_words) {
        if (word.find(lower_query) != string::npos && find(found_words.begin(), found_words.end(), word) == found_words.end()) {
            found_words.push_back(word);
            if (found_words.size() == max_amount_of_searched_words) {
                break;
            }
        }
    }

    return found_words;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    vector<string> words = loadDictionary("D:\\C++\\lab_data_structure\\lab_5\\voina-i-mir.txt");

    while (true) {
        string query;
        cout << "Введите запрос (не менее 3-х символов): ";
        getline(cin, query);

        if (query.size() < 3) {
            cout << "Не короче 3-х символов!" << std::endl;
            continue;
        }

        auto start_time = chrono::steady_clock::now();
        vector<string> found_words = find_words(query, words, 20);

        cout << "Найденные слова:" << endl;
        for (const string& word : found_words) {
            cout << word << endl;
        }

        auto end_time = chrono::steady_clock::now();
        auto execution_time = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count() / 1000.0;
        cout << "Время выполнения: " << execution_time << " сек." << std::endl;

        string choice;
        cout << "Продолжить поиск? (да/нет): ";
        getline(cin, choice);
        if (choice != "да") {
            break;
        }
    }

    return 0;
}