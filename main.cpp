#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

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

vector<string> process_next_letter(char letter, vector<string> list_of_words, unsigned int word_length) {
    vector<string> new_list;

    //this is in indexes
    unordered_map<string, long> positions ={};

    //generate all possible possitions

    for (string& word : list_of_words) {
        size_t nCharacterOffset = word.find (letter, 0);
        vector<int> char_locations;
        while (nCharacterOffset != string::npos) {
            char_locations.push_back(static_cast<int>(nCharacterOffset));
            size_t nCharSearchOffset = nCharacterOffset + 1;
            nCharacterOffset = word.find(word, nCharSearchOffset);
        }

        string char_loc_str = "";
        for (int char_loc : char_locations) {
            char_loc_str.append(to_string(char_loc));
        }

        if (char_loc_str.length() < 1) {
            //do nothing
        } else if (positions.find(char_loc_str) == positions.end()) {
            positions[char_loc_str] = 1;
        } else {
            positions[char_loc_str] ++;
        }
        new_list.push_back(word);
    }

    for ( const auto &[key, value]: positions ) {
        std::cout << key <<":" << value << '\n';
    }

    return(new_list);
}


int main() {
    unsigned int word_length = get_word_length();
    unsigned int guesses = get_guesses();
    bool total_number_of_words_remaining_on = get_total_number_of_words_remaining_on();
    vector<string> list_of_words = get_words_from_txt(word_length);
    bool play_again = true;
//    for (string& s: list_of_words) {
//        cout << s << endl;
//    }
    string current_word = "";
    for (int i = 0; i < word_length; i++) {
        current_word.append(blank);
        current_word.append(" ");
    }
    while (play_again) {
        if (total_number_of_words_remaining_on) {
            unsigned long number_of_words_remaining = list_of_words.size();
            cout << "Number of words remaining: " << number_of_words_remaining << endl;
        }
        cout << "Number of guesses remaining: " << guesses << endl;
        cout << "Current word: " << current_word << endl;
        char next_letter = get_next_letter();
        list_of_words = process_next_letter(next_letter, list_of_words, word_length);

        int i;
        cin >> i;

        guesses--;
    }
}
