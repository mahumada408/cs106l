#include <algorithm>
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
    std::string dict_word;

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

    std::string current_family;
    std::string family;
    for (int i = 0; i < word_size; ++i) {
      current_family += "-";
      family += "-";
    }
    
    std::vector<char> guessed_letters;
    bool success = false;
    while (guesses != 0) {
      std::cout << current_family << " guesses remaining: " << guesses << " words remaining: " << words_in_max_family << std::endl;

      char letter;
      std::cout << "Guess letter! ";
      std::cin >> letter;

      // Check to see if the letter has been used already.
      while (std::find(guessed_letters.begin(), guessed_letters.end(), letter) != guessed_letters.end()) {
        // Letter has already been guessed.
        std::cout << "Letter has already been used!" << std::endl;
        std::cin >> letter;
      }
      guessed_letters.push_back(letter);

      // Build up the different families based on the guessed letter.
      std::map<std::string, std::vector<std::string>> the_families;
      family = current_family;
      for (const auto& word : available_words) {
        int i = 0;
        for (const auto& c : word) {
          if (c == letter) {
            family.at(i) = c;
          }
          ++i;
        }
        std::map<std::string, std::vector<std::string>>::iterator families_itr = the_families.find(family);
        if (families_itr == the_families.end()) {
            // Family is not there so add it.
            the_families[family] = std::vector<std::string>{word};
        } else {
            families_itr->second.push_back(word);
        }
        // Reset family.
        family = current_family;
      }

      // Get largest family.
      words_in_max_family = 0;
      for (const auto& pair : the_families) {
        if (pair.second.size() > words_in_max_family) {
          current_family = pair.first;
          words_in_max_family = pair.second.size();
          available_words = pair.second;
        }
      }

      // Decrement family.
      guesses--;
      
      // int init_guesses = guesses;
      int success_letters = 0;
      for (auto& letter : current_family) {
        if (letter != '-') {
          success_letters++;
        }
      }
      if (success_letters == word_size) {
        guesses = 0;
        success = true;
      }
    }

    if (success) {
      std::cout << "You did it! Word was: " << available_words[0] << std::endl;
    } else {
      std::cout << "Sorry the word was: " << available_words[std::rand() % word_size] << std::endl;
    }
    return 0;
}