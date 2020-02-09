#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

void PrintDictionary(std::map<int, std::vector<std::string>>& dictionary_length) {
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
}

int main() {
    // Open dictionary file.
    std::fstream dictionary;
    dictionary.open("dictionary.txt");
    std::string word;

    std::map<int, std::vector<std::string>> dictionary_length;

    if (dictionary.is_open()) {
        while (std::getline(dictionary, dict_word))
        {
            std::map<int, std::vector<std::string>>::iterator itr = dictionary_length.find(dict_word.size());
            if (itr == dictionary_length.end()) {
                // Key is not there, so add it.
                dictionary_length[dict_word.size()] = std::vector<std::string>{dict_word};
            } else {
                itr->second.push_back(dict_word);
            }
        }
        
    }

    // Prompt the user for size of word.
    int word_size;
    int words_in_max_family;
    std::cout << "Please enter word size: ";
    std::cin >> word_size;
    std::cout << "You entered: " << word_size << std::endl;

    bool valid_word = false;
    std::vector<std::string> available_words;
    while (!valid_word) {
        std::map<int, std::vector<std::string>>::iterator itr = dictionary_length.find(word_size);
        if (itr == dictionary_length.end()) {
            // Key is not there, so add it.
            std::cout << "Invalid word length, try again." << std::endl;
            std::cout << "Please enter word size: ";
            std::cin >> word_size;
        } else {
            std::cout << "Valid word length." << std::endl;
            valid_word = true;
            available_words = itr->second;
            words_in_max_family = available_words.size();
        }
    }

    // Prompt the user for number of guesses.
    int guesses;
    std::cout << "Please enter number of guesses: ";
    std::cin >> guesses;
    std::cout << "You entered: " << guesses << std::endl;

    for (int i = 0; i < word_size; ++i) {
        std::cout << "-";
    }
    std::cout << " guesses remaining: " << guesses << " words remaining: " << words_in_max_family << std::endl;

    char letter;
    std::cout << "Guess letter! ";
    std::cin >> letter;

    for (const auto& word : available_words) {
        for (const auto& c : word) {
        }
    }



    return 0;
}