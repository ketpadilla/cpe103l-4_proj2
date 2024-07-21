// ** HEADER FILES
#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <ctime>
#include <string.h> 
#include <sstream>
using namespace std;


// ** COMPANY NAME
const string CoNm[] = {"FHC", "Filipinas Hotel Corporation"};


// ** PROTOTYPES
// time functions
tm create_tm(int year, int month, int day);
void printDate(tm date, string label);
bool validateDate(tm date);
tm getDate();
tm getDate(tm checkIn);


// ** CLASSES
// user information
class User {
  private:
    string email;
    string name;
    string password;
    string reservationNumber;
    int idx;

  public:
    User(string email, string name, string password, string reservationNumber, int idx = -1) : email(email), name(name), password(password), reservationNumber(reservationNumber), idx(idx) {} // parameterized constructor

    // getters
    string getEmail() { return email; }
    string getName() { return name; }
    string getPassword() { return password; }
    string getReservationNumber() { return reservationNumber; }
    int getIdx() { return idx; }

    // setters
    void setEmail(string email) { this->email = email; }
    void setName(string name) { this->name = name; }
    void setPassword(string password) { this->password = password; }
    void setReservationNumber(string reservationNumber) { this->reservationNumber = reservationNumber; }
    void setIdx(int idx) { this->idx = idx; }
};


// ** STRUCTURES
struct RoomInfo { // room information
    int roomNumber;
    string type;
    double pricePerNight;
    string availability;
};


struct RoomRes { // room reservation information
  int roomNumber;
  tm startDate;
  tm endDate;
  string reservationNumber;
};


// ** GLOBAL VARIABLES
User acct[] = { // accounts
// email, name, password, reservation number
  User("dbale@gmail.com", "Dina Bale", "123456", "S43253", 0),
  User("mgaya@gmail.com", "Mally Gaya", "password", "None", 1),
  User("tlee@gmail.com", "Toto Lee", "qwerty", "None", 2),
  User("mmahal@gmail.com", "Mina Mahal", "123123", "D54682", 3),
  User("cpot@gmail.com", "Cory Pot", "letmein", "None", 4)
};


User admin("admin@gmail.com", "Admin", "admin", "admin", -99); // admin account


RoomInfo rooms[] = { // room information 
// room number, type, price per night, availability
  {102, "Standard", 3250.00, "Available"},
  {107, "Standard", 3525.00, "Available"},
  {111, "Standard", 3275.00, "Available"},
  {115, "Standard", 3500.00, "Reserved"},
  {205, "Standard", 3550.00, "Available"},
  {215, "Standard", 3200.00, "Available"},
  {306, "Deluxe", 4750.00, "Reserved"},
  {315, "Deluxe", 5200.00, "Available"},
  {405, "Deluxe", 5750.00, "Available"},
  {519, "Penthouse", 12500.00, "Available"},
  {532, "Penthouse", 10250.00, "Available"},
  {607, "Penthouse", 15000.00, "Available"},
};


RoomRes res[] = { // room reservations
// room number, start date, end date, reservation number
    {115, create_tm(2021, 7, 10), create_tm(2021, 7, 15), "S43253"},
    {306, create_tm(2021, 7, 12), create_tm(2021, 7, 17), "D54682"}
};


// ** DEFINITIONS
// * TIME FUNCTIONS
// create tm struct with only day, month, and year
tm create_tm(int year, int month, int day) {
    tm time = {};
    time.tm_year = year - 1900;  // years since 1900
    time.tm_mon = month - 1;     // months since January (0-11)
    time.tm_mday = day;          // day of the month (1-31)
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    return time;
}


// print date
void printDate(tm date, string label) {
    if (label.compare("in") == 0) {
        cout << endl; // add new line before check-in date
    }

    char buffer[80];
    strftime(buffer, 80, "%m/%d/%Y", &date); // format date
    cout << "Check-" << label << " Date: " << buffer << endl; // print date

    if (label.compare("out") == 0) {
        cout << endl; // add new line after check-out date
    }
}


// validate date
bool validateDate(tm date) {
    tm date_copy = date; // copy the input date

    time_t t = mktime(&date_copy); // convert tm to time_t
    if (t == -1) { // check if the date is valid
        return false;
    }

    // print date today
    time_t now = time(0); ///
    tm* today = localtime(&now); // get current date

    if (date.tm_year < today->tm_year ||
        (date.tm_year == today->tm_year && date.tm_mon < today->tm_mon) ||
        (date.tm_year == today->tm_year && date.tm_mon == today->tm_mon && date.tm_mday < today->tm_mday)) { // check if the date is in the past
        return false;
    }

    tm* dt = localtime(&t); // convert time_t to tm

    return (dt->tm_year == date.tm_year && 
            dt->tm_mon == date.tm_mon && 
            dt->tm_mday == date.tm_mday); // return true if the date is valid
}


// get date (check-in)
tm getDate() {
    int year, month, day; // ask for the date day, month, and year
    cout << endl << "Enter check-in date: " << endl;
    cout << "Day: ";
    cin >> day;
    cout << "Month: ";
    cin >> month;
    cout << "Year: ";
    cin >> year;

    tm date = create_tm(year, month, day); // create tm struct

    if (validateDate(date)) { // return the date if it is valid
      return date;
    } else {
      cout << "Invalid date" << endl;
      return getDate(); // ask for the date again if it is invalid
    }
}


// get date (check-out)
tm getDate(tm checkIn) {
    int year, month, day; // ask for the date day, month, and year
    cout << endl << "Enter check-out date: " << endl;
    cout << "Day: ";
    cin >> day;
    cout << "Month: ";
    cin >> month;
    cout << "Year: ";
    cin >> year;

    tm checkOut = create_tm(year, month, day); // create tm struct

    if (validateDate(checkOut)) { // check if the date is valid
      if (difftime(mktime(&checkOut), mktime(&checkIn)) > 0) {
        return checkOut; // return if the date is later than the check-in date
      } else {
        cout << "Invalid date" << endl;
        return getDate(checkIn); // ask for the date again if it is invalid
      }
    } else {
      cout << endl << "Invalid date" << endl;
      return getDate(checkIn); // ask for the date again if it is invalid
    }

  return checkIn;
}