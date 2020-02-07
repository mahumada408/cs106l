#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>


int main() {
    // Open dictionary file.
    std::fstream dictionary;
    dictionary.open("dictionary.txt");
    std::string word;

    std::map<int, std::vector<std::string>> dictionary_length;

    if (dictionary.is_open()) {
        while (std::getline(dictionary, word))
        {
            std::map<int, std::vector<std::string>>::iterator itr = dictionary_length.find(word.size());
            if (itr == dictionary_length.end()) {
                // Key is not there, so add it.
                dictionary_length[word.size()] = std::vector<std::string>{word};
            } else {
                itr->second.push_back(word);
            }
        }
        
    }

    int i = 0;
    for (auto const& pair: dictionary_length) {
        std::cout << "{" << pair.first << " : ";
        for (const auto element : pair.second) {
            std::cout << element << " , ";
        }
        std::cout << std::endl;

        if (i == 3) {
            break;
        }
        ++i;
    }
    return 0;
}