#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <tuple>

#define blank "█"

using namespace std;

unsigned int get_word_length() {
    while (true) {
        unsigned int temp = 0;
        cout << "Please enter the desired word length: " << endl;
        cin >> temp;
        if (!(temp > 0 && temp < 29)) {
            cout << "That is not a valid input, please try again!" << endl;
        } else {
            return temp;
        }
    }
}

unsigned int get_guesses() {
    while (true) {
        unsigned int temp = 0;
        cout << "Please enter the desired number of guesses: " << endl;
        cin >> temp;
        if (temp <= 0) {
            cout << "That is not a valid input, please try again!" << endl;
        } else {
            return temp;
        }
    }
}

bool get_total_number_of_words_remaining_on() {
    while (true) {
        char temp;
        cout << "Please enter the wether to display the total number of words remaining (y/n): " << endl;
        cin >> temp;
        if (!(temp == 'y' || temp == 'n')) {
            cout << "That is not a valid input, please try again!" << endl;
        } else {
            if (temp == 'y') {
                return true;
            }
            return false;
        }
    }
}

vector<string> get_words_from_txt(unsigned int length = 0) {
    ifstream txt("dictionary.txt");
    vector<string> lines;
    if (!txt) {
        cout<<"Error opening dictionary file"<<endl;
        exit(-1);
    }
    string line;
    while (getline(txt, line)) {
        if (!length == 0) {
            if (line.length() == length) {
                lines.push_back(line);
            }
        } else {
            lines.push_back(line);
        }
    }
    return lines;
}

char get_next_letter() {
    while (true) {
        char temp;
        cout << "Please enter the next letter: " << endl;
        cin >> temp;
        if (!isalpha(temp)) {
            cout << "That is not a valid input, please try again!" << endl;
        } else {
            return temp;
        }
    }
}

tuple<vector<string>, vector<int>> process_next_letter(char letter, vector<string> list_of_words, unsigned int word_length) {
    vector<string> new_list;

    //this is in indexes
    map<string, long> positions ={};

    //generate all possible possitions

    for (string& word : list_of_words) {
        vector<int> char_locations;

        for(size_t i = 0; i < word.size(); ++i) {
            if (word[i] == letter) {
                char_locations.push_back(i);
            }
        }

        string char_loc_str = "";
        for (int char_loc : char_locations) {
            char_loc_str.append(to_string(char_loc));
        }

        if (positions.find(char_loc_str) == positions.end()) {
            positions[char_loc_str] = 1;
        } else {
            positions[char_loc_str] ++;
        }
        new_list.push_back(word);
    }

    vector<int> num_of_positions;

    tuple<string, int> max_val = make_tuple("", -1);
    for ( const auto &[key, value]: positions ) {
        if (value > get<1>(max_val)) {
            max_val = make_tuple(key, value);
        }
    }

    vector<string> final_list;
    for (string& word : list_of_words) {
        vector<int> char_locations;

        for(size_t i = 0; i < word.size(); ++i) {
            if (word[i] == letter) {
                char_locations.push_back(i);
            }
        }

        string char_loc_str = "";
        for (int char_loc : char_locations) {
            char_loc_str.append(to_string(char_loc));
        }


        if (char_loc_str == get<0>(max_val)) {
            final_list.push_back(word);
        }
    }

    string temp = get<0>(max_val);
    vector<int> final_positions;
    for (char c : temp) {
        int x = c - '0';
        final_positions.push_back(x);
    }
//    int* fp = &final_positions[0];
    return make_tuple(final_list, final_positions);
}

string change_word(string old_word, vector<int> positions, char letter){
    string new_word = old_word;
    for (int i : positions) {
        new_word[i] = letter;
    }
    return new_word;
}

void print_letters_guessed(vector<char> letters) {
    for (char letter : letters) {
        cout << letter << ", ";
    }
    cout << endl;
}

bool get_play_again() {
    while (true) {
        char temp;
        cout << "Do you want to play again (y/n): " << endl;
        cin >> temp;
        if (!(temp == 'y' || temp == 'n')) {
            cout << "That is not a valid input, please try again!" << endl;
        } else {
            if (temp == 'y') {
                return true;
            }
            return false;
        }
    }
}


int main() {
    unsigned int word_length = get_word_length();
    unsigned int guesses = get_guesses();
    bool total_number_of_words_remaining_on = get_total_number_of_words_remaining_on();
    vector<string> list_of_words = get_words_from_txt(word_length);
    bool play_again = true;
    string current_word = "";
    for (int i = 0; i < word_length; i++) {
        current_word.append("*");
    }

    vector <char> letters_guessed;
    while (play_again) {
        if (total_number_of_words_remaining_on) {
            unsigned long number_of_words_remaining = list_of_words.size();
            cout << "Number of words remaining: " << number_of_words_remaining << endl;
        }
        cout << "Number of guesses remaining: " << guesses << endl;
        print_letters_guessed(letters_guessed);
        cout << "Current word: " << current_word << endl;
        if (current_word.find('*') == string::npos) {
            cout << "You won!!\n" << "Word was: " << current_word << endl;
            play_again = get_play_again();
        } else if (guesses > 0) {
            char next_letter = get_next_letter();
            letters_guessed.push_back(next_letter);
            vector<int> postions;
            tie(list_of_words, postions) = process_next_letter(next_letter, list_of_words, word_length);
            current_word = change_word(current_word, postions, next_letter);
        } else {
            current_word = list_of_words[0];
            cout << "You failed\n" << "Word was: " << current_word << endl;
            play_again = get_play_again();
        }

        guesses--;
    }
}
