#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
using namespace std;

// Global variables
const string CoNm[] = {"FHC", "Filipinas Hotel Corporation"};

// Class definition for PrintUI
class PrintUI {
private:
    vector<string> text;

public:
    // Constructors
    template<size_t N>
    PrintUI(const string (&textArr)[N]);

    template<size_t M, size_t N>
    PrintUI(const string (&textArr)[M][N]);

    // Function to print the UI
    void print() const;

    // Static function to print a table
    template<size_t M, size_t N>
    static void printTable(const string (&arr)[M][N]);
};


// * Main function
int main() {
    // Example 1D array
    string textArr[] = {
        "S -> Start Booking",
        "Q -> Quit",
    };
    PrintUI ui1(textArr);
    ui1.print();

    // Example 2D array
    string myStrings[][3] = {
        {"Room #", "Type", "Price (PhP)"},
        {"102", "Standard", "3250.00"},
        {"107", "Standard", "3525.00"},
        {"111", "Standard", "3275.00"},
        {"115", "Standard", "3500.00"},
        {"205", "Standard", "3550.00"},
        {"215", "Standard", "3200.00"},
        {"306", "Deluxe", "4750.00"},
        {"519", "Penthouse", "3550.00"},
        {"", "", ""},
        {"Press F to Filter Rooms", "Press B to Book a Reservation", "Press X to Exit"},
    };
    PrintUI ui2(myStrings);



    // Print the multidimensional array in table format using static function
    PrintUI::printTable(myStrings);

    return 0;
}



// * Function definitions
// Constructor for 1D array
template<size_t N>
PrintUI::PrintUI(const string (&textArr)[N]) {
    text.insert(text.end(), CoNm, CoNm + 2); // Insert company name
    for (size_t i = 0; i < N; ++i) {
        text.push_back(textArr[i]); // Insert text from array
    }
}

// Constructor for 2D array
template<size_t M, size_t N>
PrintUI::PrintUI(const string (&textArr)[M][N]) {
    text.insert(text.end(), CoNm, CoNm + 2); // Insert company name
    for (size_t i = 0; i < M; ++i) {
        for (size_t j = 0; j < N; ++j) {
            text.push_back(textArr[i][j]); // Insert text from array
        }
    }
}

// Function to print the UI
void PrintUI::print() const {
    // Calculate maximum length
    int maxLen = text[1].length(); // Start with company name length
    for (size_t i = 0; i < text.size(); ++i) {
        if (text[i].length() > maxLen) {
            maxLen = text[i].length();
        }
    }

    // Print UI
    string border = string(maxLen + 10, '-');
    string paddingLine = "|" + string(maxLen + 8, ' ') + "|";
    cout << border << endl;
    for (size_t i = 0; i < text.size(); ++i) {
        int leftPadding = (maxLen - text[i].size()) / 2;
        int rightPadding = maxLen - text[i].size() - leftPadding;
        if (i == 2) {
            cout << border << endl << paddingLine << endl;
        }
        cout << "|" << setw(4 + leftPadding) << setfill(' ') << " "
             << setw(text[i].size()) << left << text[i]
             << setw(rightPadding + 4) << " " << "|" << endl;
    }
    cout << paddingLine << endl;
    cout << border << endl;
}

// Static function to print a table
template<size_t M, size_t N>
void PrintUI::printTable(const string (&arr)[M][N]) {
    int maxLen = 0;
    for (size_t i = 0; i < M; ++i) {
        for (size_t j = 0; j < N; ++j) {
            if (arr[i][j].length() > maxLen) {
                maxLen = arr[i][j].length();
            }
        }
    }

    // Print company names centered with borders
    string companyName1 = CoNm[0];
    string companyName2 = CoNm[1];
    int maxCompanyNameLen = max(companyName1.length(), companyName2.length());
    string border = string(maxLen * N + 10, '-');
    string paddingLine = "|" + string(maxLen * N + 8, ' ') + "|";
    cout << border << endl;
    cout << "|" << setw((maxLen * N + 32 - maxCompanyNameLen) / 2) << setfill(' ') << " "
         << companyName1 << setw((maxLen * N + 32 - maxCompanyNameLen + 1) / 2) << " " << "|" << endl;
    cout << "|" << setw((maxLen * N + 8 - maxCompanyNameLen) / 2) << setfill(' ') << " "
         << companyName2 << setw((maxLen * N + 8 - maxCompanyNameLen + 1) / 2) << " " << "|" << endl;
    cout << border << endl;

    // Print the table
    for (size_t i = 0; i < M; ++i) {
      if (i == 1 or arr[i][0] == "") {
        cout << border << endl;
        if (arr[i][0] == "") {
          continue;
        }
      }
        cout << "| ";
        for (size_t j = 0; j < N; ++j) {
            cout << setw(maxLen) << left << arr[i][j] << " | ";
        }
        cout << endl;
    }
    cout << border << endl;
}