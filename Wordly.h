//
// UNFINISHED!!
//

#ifndef CPP_COURSE_BST_ITERATOR_WORDLY_H
#define CPP_COURSE_BST_ITERATOR_WORDLY_H

#include <iostream>
#include <vector>
#include <string>
#include <optional>
#include <algorithm>
#include <set>
#include <map>

#define ALPHABET_SIZE 26

char char_to_lower(char in) {
    if (in <= 'Z' && in >= 'A')
        return in - ('Z' - 'z');
    return in;
}

std::string to_lowercase(std::string str) {
    std::string new_str = str;
    std::transform(new_str.begin(), new_str.end(), new_str.begin(), char_to_lower);
    return new_str;
}

class Wordly {
private:
    const std::vector<std::string> words;
    std::size_t word_index;
public:
    // words -- allowed words
    // initial_word -- index of words in ‘words‘, which used for first game
    Wordly(const std::vector<std::string> &words, std::size_t initial_word) : words(words), word_index(initial_word) {}

    // Start new game with new word
    // word is index in original ‘words vector‘
    void reset_word(std::size_t word) {
        this->word_index = word;
    }

    // Legth of word for current game
    std::size_t word_length() const {
        return this->words[word_index].length();
    };

    // return nullopt is word have wrong length or vocabalory does not contain this word
    // return string of -/+/X characters iwth same length as ‘word‘ else
    std::optional<std::string> ask(const std::string &word) const {
        if (word.length() != this->word_length()) {
            return std::nullopt;
        }
        std::string result(this->word_length(), '-');
        const std::string my_word = to_lowercase(this->words[word_index]);
        const std::string target_word = to_lowercase(word);
        for (int i = 0; i < target_word.length(); i++) {
            if (my_word[i] == target_word[i]) {
                result[i] = 'X';
            } else if (my_word.find(target_word[i]) != std::string::npos) {
                result[i] = '+';
            }
        }
        return { result };
    };
};

class Player {
private:
    const std::vector<std::string> words;
    std::optional<std::set<size_t>> candidates;
    std::set<size_t> history;
    std::vector<std::set<char>> letter_positions;

    std::optional<std::string> find_candidate () {
        for (size_t index : *this->candidates) {
            const std::string word = this->words[index];
            bool word_valid = true;
            for (int i = 0; i < word.length(); i++) {
                char ch = word[i];
                if (this->letter_positions[i].find(ch) == this->letter_positions[i].end()) {
                    word_valid = false;
                }
            }
            if (word_valid) {
                if (this->history.find(index) != this->history.end()) {
                    history.insert(index);
                    return { word };
                }
            } else {
                this->candidates->erase(index);
            }
        }

        return std::nullopt;
    };

public:
    Player(const std::vector<std::string> &words) : words(words), candidates(std::nullopt) {}

    // New game
    void start(std::size_t word_length) {
        for (int i = 0; i < this->words.size(); i++) {
            std::string word = this->words[i];
            if (word.length() == word_length) {
                this->candidates->insert(i);
            }
        }

        for (int i = 0; i < word_length; i++) {
            std::set<char> positions;
            for (int j = 0; j < ALPHABET_SIZE; j++) {
                positions.insert('a' + j);
            }
            this->letter_positions[i] = positions;
        }
    };

    // Return nullopt if game is not started or player does not know any possible word
    // else return candidate words
    std::optional<std::string> ask() {
        return find_candidate();
    };

    // ‘word‘ -- word, was passed to Wordly::ask
    // ‘result‘ -- string of -/+/X, returned by Wordly::ask
    void consume_answer(const std::string &word, const std::string &result) {
        for (int i = 0; i < result.length(); i++) {
            char sym = result[i];
            switch (sym) {
                case '-':
                    // if letter is not in word - delete it from every position candidates set
                    for (int j = 0; j < result.length(); j++) {
                        this->letter_positions[j].erase(word[i]);
                    }
                    break;
                case '+':
                    // if letter is in word but not in its position - remove it from current index candidates
                    this->letter_positions[i].erase(word[i]);
                    break;
                case 'X':
                    // if letter on its position - current candidate set has only one candidate, and it is this letter
                    this->letter_positions[i].clear();
                    this->letter_positions[i].insert(word[i]);
                    break;
                default:
                    break;
            }
        }
    };
};

#endif //CPP_COURSE_BST_ITERATOR_WORDLY_H
