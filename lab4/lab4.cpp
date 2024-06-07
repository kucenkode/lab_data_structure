#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <chrono>
#include <locale>
#include <codecvt>
#include <set>

using namespace std;

// Функция для загрузки словаря из файла
unordered_map<wstring, bool> loadDictionary(const string& filename) {
    wifstream file(filename);
    file.imbue(locale(file.getloc(), new codecvt_utf8<wchar_t>));
    unordered_map<wstring, bool> dict;
    wstring word;

    while (file >> word) {
        dict[word] = true; // Храним слово как ключ и значение
    }

    return dict;
}

// Функция для поиска слов в словаре
vector<wstring> findWords(const wstring& letters, const unordered_map<wstring, bool>& dict) {
    vector<wstring> foundWords;

    // Преобразуем слово в набор букв
    set<wchar_t> letterSet(letters.begin(), letters.end());

    // Проверяем каждое слово в словаре
    for (const auto& pair : dict) {
        const wstring& word = pair.first;
        set<wchar_t> wordSet(word.begin(), word.end());

        // Проверяем, что количество букв в слове не превышает количество букв в наборе букв исходного слова
        if (wordSet.size() <= letterSet.size() &&
            includes(letterSet.begin(), letterSet.end(), wordSet.begin(), wordSet.end())) {
            foundWords.push_back(word);
        }
    }

    // Сортировка по уменьшению длины слова
    sort(foundWords.begin(), foundWords.end(), [](const wstring& a, const wstring& b) {
        return a.size() > b.size();
        });

    return foundWords;
}

int main() {
    setlocale(LC_ALL, "en_US.UTF-8");
    wcout.imbue(locale("en_US.UTF-8"));

    // Инициализация словаря
    auto startInit = chrono::high_resolution_clock::now();
    unordered_map<wstring, bool> dict = loadDictionary("D:\\C++\\lab_data_structure\\lab4\\russian_nouns.txt");
    auto endInit = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsedInit = endInit - startInit;
    wcout << L"Время инициализации словаря: " << elapsedInit.count() << L" секунд." << endl;

    // Обработка нового слова
    wstring newWord = L"абажур";
    auto startProcessing = chrono::high_resolution_clock::now();
    vector<wstring> words = findWords(newWord, dict);
    auto endProcessing = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsedProcessing = endProcessing - startProcessing;

    // Вывод найденных слов
    for (const wstring& word : words) {
        wcout << word << endl;
    }

    wcout << L"Время обработки слова: " << elapsedProcessing.count() << L" секунд." << endl;

    return 0;
}