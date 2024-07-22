// ** HEADER FILES
#include "admin.h"

// ** PROTOTYPES
// starting functions
char validateChoice(char choice, char options[], int size);
char homeScreen();
int identifyUser();
char transacScreen(string access);


// general transaction helper functions
char roomType(string method, string access, int usrIdx);
string determineFilter(char roomChoice);


// view room functions
char viewRooms(string access, string filterType, int usrIdx);


// book room functions
string generateReservationNumber(string roomType);
char bookRoom(string access, int usrIdx);
void processReservation(int roomNumber, int usrIdx, tm checkInDate, tm checkOutDate);
char seeReservation(string access, int usrIdx);
char cancelReservation(string access, int usrIdx);


// admin functions
int selectUser();
char editRoomInfo(string access);


// ** DEFINITIONS
// * STARTING FUNCTIONS
// validate user input for menu options
char validateChoice(char choice, char options[], int size) {
  char input; // get user input
  cout << "Choice: ";
  cin >> input;
  input = toupper(input); 

  for (int i = 0; i < size; i++) { // for all options
    if (input == toupper(options[i])) {
      return input; // return if input is in options
    }
  }

  cout << "Invalid input" << endl; // print error message

  cin.clear(); // clear input buffer
  cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignore input buffer
  return validateChoice(choice, options, size); // prompt again
}


// show home screen
char homeScreen() {
	char choice, options[2] = {'S', 'Q'}; // initailize and print options
	int size = sizeof(options) / sizeof(options[0]);

	string textArrHS[] = { // home screen prompt
		"S -> Start Booking",
		"Q -> Quit",
	};

	PrintUI uiHS(textArrHS); // print UI
	uiHS.print();

	choice = validateChoice(choice, options, size); // get, validate, and return choice

  if (choice == 'Q') { // quit program
    cout << "Quiting program" << endl;
    exit(0);
  }

  if (choice != 'S') { // invalid choice
    cout << "Invalid choice" << endl;
    exit(1);
  }

	return choice;
}


// identify user from accounts
int identifyUser() {
  system("clear"); // clear screen

  string usrEmail, usrPw; // initialize variables
  int usrIdx = -1, attempts = 3;


  string textArrIU1[] = { // email prompt
		"Enter Email: ",
		"__________",
	};

    string textArrIU2[] = { // password prompt
    "Enter Password: ",
    "______________",
  };

	PrintUI uiIU1(textArrIU1); // print UI
	uiIU1.print();

  for (;;) { // loop until correct email is entered or attempts are used up
    cout << endl << ":: "; // ask for email
    cin >> usrEmail;

    if (usrEmail.compare(admin.getEmail()) == 0) { // if user is admin
      int adminIdx = validateAdmin(); // validate admin password
      return adminIdx;
    }

    for (int i = 0; i < sizeof(acct) / sizeof(acct[0]); ++i) { // if user is not admin
      if (acct[i].getEmail().compare(usrEmail) == 0) { // if email is found in accounts
        usrIdx = i; // set variables
        attempts = 3;
        break;
      } 
    }

    if (usrIdx != -1) {
      break; // break if email is found
    }

    attempts = decAttempt(attempts); // if not found, decrement attempts and ask for email again
  }

  PrintUI uiIU2(textArrIU2); // print UI
  uiIU2.print(); 

  for (;;) { // loop until correct password is entered or attempts are used up
    cout << endl << ":: "; // ask for password
    cin >> usrPw;

    if (acct[usrIdx].getPassword().compare(usrPw) == 0){ 
      return acct[usrIdx].getIdx(); // return user index if password is correct
    } else {
      attempts = decAttempt(attempts); // decrement attempts and ask for password again
    }
  }
}


// show transaction screen
char transacScreen(string access) {
  system("clear"); // clear screen

  char choice, options[6] = {}; // initialize options

  options[0] = 'V'; // set options
  options[1] = 'B';
  options[2] = 'S';
  options[3] = 'C';
  options[4] = 'Q';

	string textArrTS[] = { // transaction screen prompt
		"V -> View all available rooms",
    "B -> Book a reservation",
    "S -> See reservation",
    "C -> Cancel reservation",
		"Q -> Quit",
    " ",
    " ",
    "Enter transaction type: _____",
	};


  if (access.compare("admin") == 0) { // add content for admin
    options[5] = 'E';
    textArrTS[0] = "V -> View all rooms";
    textArrTS[4] = "E -> Edit room information";
    textArrTS[5] = "Q -> Quit";
    textArrTS[6] = " ";
    textArrTS[7] = "Enter transaction type: _____";
  }

  int size = sizeof(options) / sizeof(options[0]); // get size of options

	PrintUI uiTS(textArrTS); // print UI
	uiTS.print();

	choice = validateChoice(choice, options, size); // get, validate, and return choice
  choice = (choice == 'Q') ? 'X' : (choice == 'S') ? 'R' : choice; // change choice to 'X' if 'Q' and 'R' if 'S'

  return choice;
}


// ** GENERAL TRANSACTION HELPER FUNCTIONS
// ask user to choose room type
char roomType(string method, string access, int usrIdx) {
  char choice, options[5] = {}; // initialize options

  options[0] = 'S'; // set options
  options[1] = 'D';
  options[2] = 'P';
  options[3] = 'X';

  string textArrRT[] = { // room type prompt
    "Choose Room Type",
    "S -> Standard",
    "D -> Deluxe",
    "P -> Penthouse",
    " ",
    " ",
    "Press X to Exit",
  };

  if (method.compare("indirect") == 0) { // add content for indirect method (called from viewRooms function)
    options[4] = 'R';
    textArrRT[5] = "Press R to Return";
  }

  int size = sizeof(options) / sizeof(options[0]); // get size of options

  PrintUI uiRT(textArrRT); // print UI
  uiRT.print(); 

  choice = validateChoice(choice, options, size); // get, validate, and return choice

  if (choice == 'R') { // return to previous screen
    choice = viewRooms(access, "unfiltered", usrIdx);
  }

  return choice;
}


// determine filter (room type)
string determineFilter(char roomChoice) {
  // determine filter
  string filter = (roomChoice == 'S') ? "Standard" :
          (roomChoice == 'D') ? "Deluxe" :
          (roomChoice == 'P') ? "Penthouse" :
          (roomChoice == 'X') ? "S" :
          "X";  // default value if none of the conditions are met

  return filter;
}


// ** VIEW ROOM FUNCTIONS
// view rooms
char viewRooms(string access, string filterType, int usrIdx) {
  int sizeTbl = sizeof(rooms) / sizeof(rooms[0]); // get size of rooms array
  char roomChoice; // initialize options for choosing room type
  string filter;

  if (filterType.compare("filtered") == 0) { // check if function is called with filter on
    roomChoice = roomType("indirect", access, usrIdx); // get room type as char
    filter = determineFilter(roomChoice); // get room type as string
  }

  vector<vector<string> > roomData; // declare vector to store room data

  // insert data into the vector using temporary vectors
  // header
  vector<string> header;
  header.push_back("Room Number"), header.push_back("Type"), header.push_back("Price Per Night (PHP)"); // insert headers
  if (access.compare("admin") == 0) { // add header if admin
    header.push_back("Available");
  }
  roomData.push_back(header); // insert header into roomData

  // data
  // !! TABLE ISSUE: Right border is not aligned when admin access is used
  for (int i = 0; i < sizeTbl; ++i) { // for all rooms
    if (access.compare("admin") != 0) { // enable filter for user
      if (access.compare("user") == 0 && rooms[i].availability.compare("Available") != 0) {
        continue; // skip if room is not available
      } 
    }

    if (filterType.compare("filtered") == 0) {
      if (rooms[i].type.compare(filter) != 0) {
        continue; // skip if room is not of the selected type
      }
    }

    vector<string> row;
    row.push_back(to_string(rooms[i].roomNumber)); // add room data
    row.push_back(rooms[i].type);
    row.push_back(to_string(rooms[i].pricePerNight));

    if (access.compare("admin") == 0) { // add availability if admin
      row.push_back(rooms[i].availability);
    }

    roomData.push_back(row); // insert row into roomData
  }

  // footer
  vector<string> footer;
  if (filterType.compare("filtered") == 0) { // add footer if filter is on
    footer.push_back("Press C to Clear Filter");
  } else { // add footer if filter is off
    footer.push_back("Press F to Filter Rooms");
  }

  footer.push_back("Press B to Book a Reservation"); // add other footers
  footer.push_back("Press X to Exit");

  if (access.compare("admin") == 0) { // add footer if admin
    footer.push_back(" ");
  }

  roomData.push_back(footer); // insert footer into roomData

  system("clear"); // clear screen
  PrintUI::printTable(roomData); // print table

  char choice, options[4] = {'C', 'F', 'B', 'X'}; // initialize options
  int sizeOpt = sizeof(options) / sizeof(options[0]);
  choice = validateChoice(choice, options, sizeOpt); // get, validate, and return choice

  switch(choice) { // process choice
    case 'C':
      viewRooms(access, "unfiltered", usrIdx); // clear filter
      break;
    case 'F':
      viewRooms(access, "filtered", usrIdx); // filter rooms
      break;
    case 'B':
      choice = bookRoom(access, usrIdx); // book a reservation (indirect method)
      return choice;
    case 'X': // go back to transaction screen
      return 'S';
    default: // invalid choice
      cout << "Invalid choice" << endl;
      exit(1);
  }

  return 'S';
}


// ** BOOK ROOM FUNCTIONS
// generate reservation number
string generateReservationNumber(string roomType) {
  string startingLtr, reservationNumber; // initialize variables
  
  if (roomType.compare("Standard") == 0) { // determine starting letter based on room type
    startingLtr = "S";
  } else if (roomType.compare("Deluxe") == 0) {
    startingLtr = "D";
  } else if (roomType.compare("Penthouse") == 0) {
    startingLtr = "P";
  }

  reservationNumber = startingLtr + to_string(rand() % 90000 + 1000); // complete and return reservation number
  return reservationNumber;
}


// book a reservation
char bookRoom(string access, int usrIdx) {
  system("clear"); // clear screen

  tm checkInDate, checkOutDate; // initialize variables
  string filter;
  char roomChoice;
  int sizeTbl = sizeof(rooms) / sizeof(rooms[0]);

  for (;;) { // for admin: select user with no reservation
    if (access.compare("admin") == 0) {
      usrIdx = selectUser();

      if (usrIdx == -1) { // go back to transaction screen if admin exits
        return 'S';
      }

      if (acct[usrIdx].getReservationNumber().compare("None") != 0) { // check if user has a reservation
        system("clear"); 
        string textArrABR[] = { // prompt to select another user
          "User already has a reservation. Select another user.",
          " ",
          "Press any key to go select again."
        };

        PrintUI uiABR(textArrABR); // print UI
        uiABR.print();

        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // press any key to continue
        cin.get();

        usrIdx = -2; // set user index to -2 to select another user (user has a reservation)
      }
    }

    if (usrIdx != -2) {
      break; // break if user index is not -2 (no reservation)
    }
  }

  // for users: check if user already has a reservation
  if (acct[usrIdx].getReservationNumber().compare("None") != 0) {
    string textArrBR0[] = { // prompt if user has a reservation
      "User already has a reservation",
      " ",
      "Press any key to go back."
    };

    PrintUI uiBR0(textArrBR0); // print UI
    uiBR0.print();

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // press any key to continue and go back to transaction screen
    cin.get();
    return 'S';
  }

  // get check-in and check-out dates
  char dateOptions[2] = {'X', 'C'}, dateChoice; // initialize options for getting check-in and check-out dates
  int dateOptSize = sizeof(dateOptions) / sizeof(dateOptions[0]);
  string textArrBR1[] = { // check-in date prompt
    "Enter Check-in Date (MM/DD/YYYY):",
    " ",
    "_____",
    " ",
    "Press X to Exit",
    "Press C to Coninue",
  };

  PrintUI uiBR1(textArrBR1); // print UI
  uiBR1.print();

  cout << "Exit or Continue (X/C)?: "; // get, validate, and return choice
  if (validateChoice(dateChoice, dateOptions, dateOptSize) == 'X') {
    return 'S';
  }

  checkInDate = getDate(); // get check-in date

  string textArrBR2[] = { // check-out date prompt
    "Enter Check-out Date (MM/DD/YYYY):",
    " ",
    "_____",
    " ",
    "Press X to Exit",
    "Press C to Coninue",
  };

  PrintUI uiBR2(textArrBR2); // print UI
  uiBR2.print();

  cout << "Exit or Continue (X/C)?: "; // get, validate, and return choice
  if (validateChoice(dateChoice, dateOptions, dateOptSize) == 'X') {
    return 'S';
  }

  checkOutDate = getDate(checkInDate); // get, validate, and return check-out date

  printDate(checkInDate, "in"); // print dates
  printDate(checkOutDate, "out");

  // filter rooms
  roomChoice = roomType("direct", access, usrIdx);
  filter = determineFilter(roomChoice);

  if (filter.compare("S") == 0 || filter.compare("X") == 0 ) { // go back to transaction screen
    return 'S';
  }

  vector<vector<string> > roomData; // declare vector to store room data

  // insert data into the vector using temporary vectors
  // header
  vector<string> header;
  header.push_back(""), header.push_back("Room Number"), header.push_back("Price Per Night (PHP)");
  roomData.push_back(header);


  // data
  int roomCount = 0;
  for (int i = 0; i < sizeTbl; ++i) { // for all rooms    
    // filter out rooms that not of the selected type or reserved
    if (rooms[i].type.compare(filter) != 0 || rooms[i].availability.compare("Available") != 0) {
      continue; // skip if room is not of the selected type or reserved
    } 

    // count available rooms of the selected type
    if (rooms[i].type.compare(filter) == 0 && rooms[i].availability.compare("Available") == 0){  
      roomCount++; // count available rooms
    }
    
    vector<string> row;  // insert room data
    row.push_back(to_string(roomCount));
    row.push_back(to_string(rooms[i].roomNumber));
    row.push_back(to_string(rooms[i].pricePerNight));
    roomData.push_back(row);
  }

  // footer
  vector<string> footer; 
  footer.push_back("Press X to Exit");
  footer.push_back(" ");
  footer.push_back(" ");
  roomData.push_back(footer);

  system("clear");
  PrintUI::printTable(roomData); // print table

  // prompt user to book a room
  char choice, options[roomCount + 1]; // initialize options (X and 1 to roomCount)
  int sizeOpt = sizeof(options) / sizeof(options[0]);
  options[0] = 'X';
  for (int i = 1; i <= roomCount + 1; ++i) { // set options (1 to roomCount)
    options[i] = i + '0';
  }

  choice = validateChoice(choice, options, sizeOpt); // get, validate, and return choice
  if (choice == 'X') { // go back to transaction screen
    return 'S';
  }

  // confirm reservation
  int idx = int(choice) - 48; // convert choice to integer
  string textArrBR[] = { // reservation confirmation prompt
    "Room Number: " + roomData[idx][1],
    "Price Per Night: " + roomData[idx][2],
    "Check-in Date: " + to_string(checkInDate.tm_mon + 1) + "/" + to_string(checkInDate.tm_mday) + "/" + to_string(checkInDate.tm_year + 1900),
    "Check-out Date: " + to_string(checkOutDate.tm_mon + 1) + "/" + to_string(checkOutDate.tm_mday) + "/" + to_string(checkOutDate.tm_year + 1900),
    " ",
    "Press C to Confirm",
    "Press B to Back",
    "Press X to Exit",
  };

  PrintUI uiBR(textArrBR); // print UI
  uiBR.print();

  char confirmChoice, confirmOptions[3] = {'C', 'B', 'X'}; // initialize options
  int confirmSize = sizeof(confirmOptions) / sizeof(confirmOptions[0]);
  confirmChoice = validateChoice(confirmChoice, confirmOptions, confirmSize); // get, validate, and return choice

  switch (confirmChoice) { // process choice
    case 'C': // confirm reservation
      acct[usrIdx].setReservationNumber(generateReservationNumber(filter)); // generate reservation number to user
      processReservation(stoi(roomData[idx][1]), usrIdx, checkInDate, checkOutDate); // add reservation to room reservations array
      return 'S';
    case 'B': // go back to book reservation screen (indirect method)
      choice = bookRoom(access, usrIdx);
      return choice;
    case 'X': // go back to transaction screen
      return 'S';
    default: // invalid choice
      cout << "Invalid choice" << endl;
      exit(1);
  }

  return 'S';
}


// process reservation into room reservations array
void processReservation(int roomNumber, int usrIdx, tm checkInDate, tm checkOutDate) {
    int size = 0; // get size of room reservations array
    while (res[size].roomNumber != 0) {
        size++;
    }

    // use pointers to add information to room reservations array
    RoomRes* pRes = &res[size]; // get pointer to room reservations array
    pRes->roomNumber = roomNumber; // add room and reservation numbers
    pRes->reservationNumber = acct[usrIdx].getReservationNumber();
    pRes->startDate = checkInDate; // add check-in and check-out dates
    pRes->endDate = checkOutDate;

    // update room availability
    int roomIdx; // find room index in rooms array
    size = sizeof(rooms) / sizeof(rooms[0]);
    for (int i = 0; i < size; ++i) {
        if (rooms[i].roomNumber == roomNumber) {
            roomIdx = i;
            break;
        }
    }

    RoomInfo* pRoom = &rooms[roomIdx]; // get pointer to room and update availability
    pRoom->availability = "Reserved";

    cout << endl << "Reservation confirmed!" << endl << endl; // print confirmation message
    cout << "Press any key to go back." << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// see reservation
char seeReservation(string access, int usrIdx) {
  system("clear"); // clear screen

  for (;;) { // for admin: select user with reservation
    if (access.compare("admin") == 0) {
      usrIdx = selectUser();

      if (usrIdx == -1) { //
        return 'S';
      }

      // select again if user has no reservation
      if (acct[usrIdx].getReservationNumber().compare("None") == 0) {
        system("clear");
        string textArrABR[] = { // prompt to select another user
          "User has no room reservation. Select another user.",
          " ",
          "Press any key to go select again."
        };

        PrintUI uiABR(textArrABR); // print UI
        uiABR.print();

        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // press any key to continue
        cin.get();

        usrIdx = -2; // set user index to -2 to select another user (user has a reservation)
      }
    }

    if (usrIdx != -2) { // break if user index is not -2 (no reservation)
      break;
    }
  }

  // for users: check if user has a reservation
  if (acct[usrIdx].getReservationNumber() == "None") {  // check if user has a reservation
    string textArrSR0[] = { // prompt if user has no reservation
    "User has no room reservation,",
    " ",
    "Press any key to go back."
  };

    PrintUI uiSR0(textArrSR0); // print UI
    uiSR0.print();

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // press any key to continue
    cin.get();

    return 'S';
  }

  string resNum = acct[usrIdx].getReservationNumber(); // get reservation number
  int size = sizeof(res) / sizeof(res[0]), roomIdx; // initialize variables
  string roomNm, pricePN, roomtyp;
  tm checkInDate, checkOutDate;

  for (int i = 0; i <= size; ++i) { // for all reservations
    if (res[i].reservationNumber.compare(resNum) == 0) {
      roomIdx = i; // find index of reservation number in rooms array
      break;
    }
  }

  roomNm = to_string(res[roomIdx].roomNumber); // get room number

  size = sizeof(rooms) / sizeof(rooms[0]); // find room details in rooms array from room number
  for (int i = 0; i <= size; ++i) {
    if (rooms[i].roomNumber == stoi(roomNm)) { // get room details
      pricePN = to_string(rooms[i].pricePerNight);
      roomtyp = rooms[i].type;
      checkInDate = res[roomIdx].startDate;
      checkOutDate = res[roomIdx].endDate;
      break;
    }
  }

  string textArrSR[] = { // reservation details prompt
    "Reservation Number: " + resNum,
    "Room Number: " + roomNm,
    "Room Type: " + roomtyp,
    "Price per Night: " + pricePN,
    "Check-in Date: " + to_string(checkInDate.tm_mon + 1) + "/" + to_string(checkInDate.tm_mday) + "/" + to_string(checkInDate.tm_year + 1900),
    "Check-out Date: " + to_string(checkOutDate.tm_mon + 1) + "/" + to_string(checkOutDate.tm_mday) + "/" + to_string(checkOutDate.tm_year + 1900),
    " ",
    "Press any key to go back."
  };

  PrintUI uiSR(textArrSR); // print UI
  uiSR.print();

  cin.ignore(numeric_limits<streamsize>::max(), '\n'); // press any key to continue
  cin.get();
  return 'S';
}


// cancel reservation
char cancelReservation(string access, int usrIdx) {
  system("clear"); // clear screen

  for (;;) { // for admin: select user with reservation
    if (access.compare("admin") == 0) {
      usrIdx = selectUser();

      if (usrIdx == -1) { // go back to transaction screen if admin exits
        return 'S';
      }

      if (acct[usrIdx].getReservationNumber().compare("None") == 0) { // check if user has a reservation
        system("clear");
        string textArrABR[] = { // prompt to select another user
          "User has no room reservation. Select another user.",
          " ",
          "Press any key to go select again."
        };

        PrintUI uiABR(textArrABR); // print UI
        uiABR.print(); 

        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // press any key to continue
        cin.get();

        usrIdx = -2; // set user index to -2 to select another user (user has no reservation)
      }
    }

    if (usrIdx != -2) { // break if user index is not -2 (has a reservation)
      break;
    }
  }

  // for users: check if user has a reservation
  if (acct[usrIdx].getReservationNumber() == "None") {
    string textArrCR0[] = { // prompt if user has no reservation
    "User has no room reservation,",
    " ",
    "Press any key to go back."
  };

    PrintUI uiCR0(textArrCR0); // print UI
    uiCR0.print();

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // press any key to continue
    cin.get();

    return 'S';
  }

  string resNum = acct[usrIdx].getReservationNumber(); // get reservation number

  int size = sizeof(res) / sizeof(res[0]), roomIdx; // find index of reservation number in res[] array
  for (int i = 0; i <= size; ++i) {
    if (res[i].reservationNumber.compare(resNum) == 0) {
      roomIdx = i; // get index of reservation number in res[] array
      break;
    }
  }

  int roomNumber = res[roomIdx].roomNumber; // get room number

  for (int i = roomIdx; i < size - 1; ++i) { // remove reservation from res[] array
    res[i] = res[i + 1];
  }
  --size; // decrement size of res[] array

  // clear the last row (default values)
  // !! Will leave garbage values in the last row per user (expandability issue)
  res[size].roomNumber = 0; 
  res[size].reservationNumber = "";
  res[size].startDate = create_tm(0, 0, 0);
  res[size].endDate = create_tm(0, 0, 0);

  // update room availability
  size = sizeof(rooms) / sizeof(rooms[0]); // find room index in rooms array
  for (int i = 0; i < size; ++i) {
      if (rooms[i].roomNumber == roomNumber) {
          roomIdx = i;
          break;
      }
  }
  
  // use pointers to update room availability
  RoomInfo* pRoom = &rooms[roomIdx]; // get pointer to room
  pRoom->availability = "Available"; // update room availability

  acct[usrIdx].setReservationNumber("None"); // update user reservation number

  string textArrCR[] = { // cancellation prompt
    "Reservation Cancelled",
    " ",
    "Press any key to go back."
  };

  PrintUI uiCR(textArrCR); // print UI
  uiCR.print();

  cin.ignore(numeric_limits<streamsize>::max(), '\n'); // press any key to continue
  cin.get();

  return 'S';
}


// ** ADMIN FUNCTIONS
// select user
int selectUser() {
  int size = sizeof(acct) / sizeof(acct[0]); // initialize options for selecting users
  char choice;
  string textArrSU[] = { // select user prompt
    "Select User",
    " ",
  };

  // insert data into the vector using temporary vectors
  // header
  vector<string> header;
  header.push_back("User Index"), header.push_back("Email"), header.push_back("Name");
  vector<vector<string> > userData;
  userData.push_back(header);

  // data
  for (int i = 0; i < size; ++i) {
    // insert data
    vector<string> row; 
    row.push_back(to_string(i + 1));
    row.push_back(acct[i].getEmail());
    row.push_back(acct[i].getName());
    userData.push_back(row);
  }

  // footer
  vector<string> footer;
  footer.push_back("Press X to Exit");
  footer.push_back(" ");
  footer.push_back(" ");
  userData.push_back(footer);

  PrintUI::printTable(userData); // print table

  char options[size + 1]; // initialize options (X and 1 to number of users)
  int sizeOpt = sizeof(options) / sizeof(options[0]);
  options[0] = 'X';
  for (int i = 1; i <= size; ++i) {
    options[i] = i + '0'; // set options (1 to number of users)
  }

  choice = validateChoice(choice, options, sizeOpt); // get, validate, and return choice

  if (choice == 'X') { // go back to transaction screen
    return -1;
  }

  return int(choice) - 49; // return user index
}


// edit room information 
char editRoomInfo(string access) {
  if (access.compare("admin") != 0) { // check if user is admin
    cout << "Invalid access" << endl;
    exit(1);
  }

  system("clear"); // clear screen

  tm checkInDate, checkOutDate; // initialize variables
  string filter;
  char roomChoice;
  int sizeTbl = sizeof(rooms) / sizeof(rooms[0]), roomCount = 0;
  char choice;

  // declare vector to store room data
  vector<vector<string> > roomData;

  // insert data into the vector using temporary vectors
  // header
  vector<string> header;
  header.push_back("");
  header.push_back("Room Number");
  header.push_back("Room Type");
  header.push_back("Price Per Night (PHP)");
  header.push_back("Availability");
  roomData.push_back(header);

  // data
  for (int i = 0; i < sizeTbl; ++i) {
    roomCount++; // count rooms
   
    // insert data
    vector<string> row; 
    row.push_back(to_string(roomCount));
    row.push_back(to_string(rooms[i].roomNumber));
    row.push_back(rooms[i].type);
    row.push_back(to_string(rooms[i].pricePerNight));
    row.push_back(rooms[i].availability);
    roomData.push_back(row);
  }

  // footer
  vector<string> footer;
  footer.push_back("Press X to Exit");
  footer.push_back(" ");
  footer.push_back(" ");
  footer.push_back(" ");
  footer.push_back(" ");
  roomData.push_back(footer);

  system("clear"); 
  PrintUI::printTable(roomData); // print table

  // prompt user to book a room
  char options[roomCount + 1]; // initialize options (X and 1 to roomCount)
  int sizeOpt = sizeof(options) / sizeof(options[0]);
  options[0] = 'X';
  for (int i = 1; i <= roomCount + 1; ++i) {
    options[i] = i + '0'; // set options (1 to roomCount)
  }

  choice = validateChoice(choice, options, sizeOpt); // get, validate, and return choice
  
  if (choice == 'X') { // go back to transaction screen
    return 'S';
  }

  int idx = int(choice) - 48; // convert choice to integer
  
  system("clear");
  string textArrERI[] = { // edit room information prompt
    "Room Number: " + roomData[idx][1],
    "Room Type: " + roomData[idx][2],
    "Price Per Night: " + roomData[idx][3],
    "Availability: " + roomData[idx][4],

    "Press any key to continue."
  };

  PrintUI uiERI(textArrERI); // print UI
  uiERI.print();

  cin.ignore(numeric_limits<streamsize>::max(), '\n'); // press any key to continue
  cin.get();

  // ask what admin wants to edit
  char editChoice; // edit room number
  cout << "Edit Room Number? (Y/N): ";
  cin >> editChoice;

  if (toupper(editChoice) == 'Y') {
    cout << "Enter new room number: ";
    cin >> rooms[idx - 1].roomNumber;
  }

  cout << "Edit Room Type? (Y/N): "; // edit room type
  cin >> editChoice;

  if (toupper(editChoice) == 'Y') {
    cout << "Enter new room type: ";
    cin >> rooms[idx - 1].type;
  }

  cout << "Edit Price Per Night? (Y/N): "; // edit price per night
  cin >> editChoice;

  if (toupper(editChoice) == 'Y') {
    cout << "Enter new price per night: ";
    cin >> rooms[idx - 1].pricePerNight;
  }

  system("clear"); // clear screen
  string textArrERI2[] = { // room information updated prompt
    "Room Information Updated",
    " ",
    "Room Number: " + to_string(rooms[idx - 1].roomNumber),
    "Room Type: " + rooms[idx - 1].type,
    "Price Per Night: " + to_string(rooms[idx - 1].pricePerNight),
    "Availability: " + rooms[idx - 1].availability,
    " ",
    "Press any key to go back."
  };

  PrintUI uiERI2(textArrERI2); // print UI
  uiERI2.print();

  cin.ignore(numeric_limits<streamsize>::max(), '\n'); // press any key to continue
  cin.get();

  return 'S';
}