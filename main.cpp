// header files
#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
using namespace std;


// global variables
const string CoNm[] = {"FHC", "Filipinas Hotel Corporation"};
const string acct[][4] = { // account table
// email, name, password, reservation number
	{"dbale@gmail.com", "Dina Bale", "123456", "0xS43253"},
	{"mgaya@gmail.com", "Mally Gaya", "password", "None"},
	{"tlee@gmail.com", "Toto Lee", "qwerty", "None"},
	{"mmahal@gmail.com", "Mina Mahal", "123123", "0xD54682"},
	{"cpot@gmail.com", "Cory Pot", "letmein", "None"}
};
// ! ROOM RESEVATION VECTOR: int, ctime, string
// ! ROOM INFORMATION VECTOR: int, string, double, string


// function prototypes
char homeScreen(char choice);
char validateChoice(char choice, char options[], int size);


// CLASSES
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


// * MAIN
int main() {
	// // Example 1D array
	// string textArr[] = {
	// 		"S -> Start Booking",
	// 		"Q -> Quit",
	// };
	// PrintUI ui1(textArr);
	// ui1.print();

	// // Example 2D array
	// string myStrings[][3] = {
	// 		{"Room #", "Type", "Price (PhP)"},
	// 		{"102", "Standard", "3250.00"},
	// 		{"107", "Standard", "3525.00"},
	// 		{"111", "Standard", "3275.00"},
	// 		{"115", "Standard", "3500.00"},
	// 		{"205", "Standard", "3550.00"},
	// 		{"215", "Standard", "3200.00"},
	// 		{"306", "Deluxe", "4750.00"},
	// 		{"519", "Penthouse", "3550.00"},
	// 		{"", "", ""},
	// 		{"Press F to Filter Rooms", "Press B to Book a Reservation", "Press X to Exit"},
	// };
	// PrintUI ui2(myStrings);

	// // Print the multidimensional array in table format using static function
	// PrintUI::printTable(myStrings);


	// initialize variables
	char choice;
	int usrIdx = -1;

	// show home screen
	choice = homeScreen(choice);
  switch (choice) {
    case 'S': // validate use
      // ! TO IMPLEMENT
      if (usrIdx == -1) {
        return 1; // exit program if invalid user
      }
      break;
    case 'Q': // quit program
      cout << "Quiting program" << endl;
      return 0;
    default: // exit program if invalid choice
      cout << "Invalid choice" << endl;
      return 1;
  }

	// verify admin access
	bool adminAccess = false; // no admin access
	// ! TO IMPLEMENT

	// start admin access
	// ! TO IMPLEMENT

	// start customer access
	// ! TO IMPLEMENT


	return 0;
}


// * FUNCTION DEFINITIONS
// MAIN
char homeScreen(char choice) {
	// home screen options
	char options[2] = {'S', 'Q'};
	int size = sizeof(options) / sizeof(options[0]);
	string textArr[] = {
		"S -> Start Booking",
		"Q -> Quit",
	};

	// print screen contents
	PrintUI ui(textArr);
	ui.print();

	// get, validate, and return choice
	choice = validateChoice(choice, options, size);
	return choice;
}


// PRINTUI
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
	// Calculate maximum length
	int maxLen = text[1].length(); // start with company name length
	for (size_t i = 0; i < text.size(); ++i) {
		if (text[i].length() > maxLen) {
			maxLen = text[i].length();
		}
	}

	// print contents inside a box
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

// static function to print a table
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

	// print company names centered with borders
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

	// print the table
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


// HELPER
// validate user input for menu options
char validateChoice(char choice, char options[], int size) {
  // get user choice
  char input;
  cout << "Choice: ";
  cin >> input;
  input = toupper(input); 

  // validate input
  for (int i = 0; i < size; i++) {
    if (input == toupper(options[i])) {
      return input;
    }
  }

  // if input is invalid
  cout << "Invalid input" << endl;

  // clear input buffer and prompt again
  cin.clear();
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
  return validateChoice(choice, options, size);
}