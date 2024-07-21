// ** HEADER FILES
#include "var.h"

// ** CLASSES
// printing contents with UI
class PrintUI {
private:
    vector<string> text;

public:
    template<size_t N> // 1D array constructor
    PrintUI(const string (&textArr)[N]);

    template<size_t M, size_t N> // 2D array constructor
    PrintUI(const string (&textArr)[M][N]);

    void print() const; // print content with UI

    static void printTable(const vector<vector<string> >& vec); // print contents in table format
};


// ** DEFINITIONS
// * PRINTUI
// 1D array constructor
template<size_t N>
PrintUI::PrintUI(const string (&textArr)[N]) {
    text.insert(text.end(), CoNm, CoNm + 2); // insert company name
    for (size_t i = 0; i < N; ++i) {
        text.push_back(textArr[i]); // insert text from array
    }
}


// 2D array constructor
template<size_t M, size_t N>
PrintUI::PrintUI(const string (&textArr)[M][N]) {
    text.insert(text.end(), CoNm, CoNm + 2); // insert company name
    for (size_t i = 0; i < M; ++i) {
        for (size_t j = 0; j < N; ++j) {
            text.push_back(textArr[i][j]); // insert text from array
        }
    }
}


// print content with UI
void PrintUI::print() const {
    // calculate maximum length
    int maxLen = text[1].length(); // let the company name be the initial max
    for (size_t i = 0; i < text.size(); ++i) { // for all lines of text
        if (text[i].length() > maxLen) {
            maxLen = text[i].length(); // update max length
        }
    }

    // print contents inside a box
    string border = string(maxLen + 10, '-'); // create border that is 10 characters longer than the max length
    string paddingLine = "|" + string(maxLen + 8, ' ') + "|"; // create padding line that is 8 characters longer than the max length
    cout << border << endl; 
    for (size_t i = 0; i < text.size(); ++i) { // for all lines of text
        int leftPadding = (maxLen - text[i].size()) / 2; // calculate left padding
        int rightPadding = maxLen - text[i].size() - leftPadding; // calculate right padding
        if (i == 2) { // print border and padding line after the company name
            cout << border << endl << paddingLine << endl;
        }
        cout << "|" << setw(4 + leftPadding) << setfill(' ') << " " // print the left border and padding
            << setw(text[i].size()) << left << text[i] // print the text
            << setw(rightPadding + 4) << " " << "|" << endl; // print the right padding and right border
    }
    cout << paddingLine << endl; 
    cout << border << endl;
}


// print contents in table format
void PrintUI::printTable(const vector<vector<string> >& vec) {
    if (vec.empty() || vec[0].empty()) return; // return if the vector is empty

    size_t M = vec.size(); // get the number of rows
    size_t N = vec[0].size(); // get the number of columns

    int maxLen = 0;
    for (size_t i = 0; i < M; ++i) { // for all rows
        for (size_t j = 0; j < N; ++j) { // for all columns
            if (vec[i][j].length() > maxLen) { 
                maxLen = vec[i][j].length(); // update max length
            }
        }
    }

    // print company names centered with borders
    string companyName1 = CoNm[0]; 
    string companyName2 = CoNm[1];
    int maxCompanyNameLen = max(companyName1.length(), companyName2.length()); // get the maximum length of the company names
    string border = string(maxLen * N + 10, '-'); // create border that is 10 characters longer than the (max length * number of columns)
    string paddingLine = "|" + string(maxLen * N + 8, ' ') + "|"; // create padding line that is 8 characters longer than the (max length * number of columns)
    cout << border << endl;
    cout << "|" << setw((maxLen * N + 32 - maxCompanyNameLen) / 2) << setfill(' ') << " "
        << companyName1 << setw((maxLen * N + 32 - maxCompanyNameLen + 1) / 2) << " " << "|" << endl; // print the company abbreviation
    cout << "|" << setw((maxLen * N + 8 - maxCompanyNameLen) / 2) << setfill(' ') << " "
        << companyName2 << setw((maxLen * N + 8 - maxCompanyNameLen + 1) / 2) << " " << "|" << endl; // print the company name
    cout << border << endl;

    // print the table
    for (size_t i = 0; i < M; ++i) { // for all rows
        if (i == 1 || i == M - 1) { // print border after the company name and before the last row
            cout << border << endl;
        }
				
        cout << "| "; // print the left border
        for (size_t j = 0; j < N; ++j) { // for all columns
            cout << setw(maxLen) << left << vec[i][j] << " | "; // print the text and right border
        }
        cout << endl;
    }
    cout << border << endl;
}