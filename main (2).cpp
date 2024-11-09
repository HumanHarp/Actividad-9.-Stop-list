#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <chrono>
#include <cctype>

using namespace std;
using namespace std::chrono;

unordered_set<string> loadStopList(const string& stopListFile) {
    unordered_set<string> stopList;
    ifstream file(stopListFile);
    string word;
    while (file >> word) {
        stopList.insert(word);
    }
    file.close();
    return stopList;
}

unordered_map<string, int> processText(const string& inputFile, const unordered_set<string>& stopList) {
    unordered_map<string, int> wordCount;
    ifstream file(inputFile);
    string word;

    while (file >> word) {
        for (char& c : word) c = tolower(c);
        word.erase(0, word.find_first_of("abcdefghijklmnopqrstuvwxyz"));
        word.erase(word.find_last_of("abcdefghijklmnopqrstuvwxyz") + 1);

        if (stopList.find(word) == stopList.end() && word.size() > 1) {
            wordCount[word]++;
        }
    }
    file.close();
    return wordCount;
}

unordered_map<string, int> filterLowFrequencyWords(const unordered_map<string, int>& wordCount, int minFrequency) {
    unordered_map<string, int> filteredWords;
    for (const auto& pair : wordCount) {
        if (pair.second >= minFrequency) {
            filteredWords[pair.first] = pair.second;
        }
    }
    return filteredWords;
}

void saveDictionary(const unordered_map<string, int>& dictionary, const string& outputFile) {
    ofstream file(outputFile);
    for (const auto& pair : dictionary) {
        file << pair.first << ": " << pair.second << endl;
    }
    file.close();
}

int main() {
    string inputFile = "input.txt";
    string stopListFile = "stop_list.txt";
    string outputFile = "dictionary.txt";
    string logFile = "a9_matricula.txt";

    unordered_set<string> stopList = loadStopList(stopListFile);

    auto start = high_resolution_clock::now();
    unordered_map<string, int> wordCount = processText(inputFile, stopList);

    int minFrequency = 2;
    unordered_map<string, int> filteredWords = filterLowFrequencyWords(wordCount, minFrequency);
    saveDictionary(filteredWords, outputFile);

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);

    ofstream log(logFile);
    log << "Tiempo total de procesamiento: " << duration.count() << " ms" << endl;
    log.close();

    cout << "Proceso completado. Diccionario guardado en " << outputFile << endl;
    cout << "Log de tiempos guardado en " << logFile << endl;

    return 0;
}
