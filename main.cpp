// ** HEADER FILES
#include "includes/helper.h"

// *** MAIN
int main() {
	string access = "user"; // set default access
	char usrChoice = homeScreen(); // show home screen
	int usrIdx = identifyUser(); // identify user and index from accounts array
	
	if (usrIdx == -99) { // set access to admin if user is admin
		access = "admin";
	}

	for (;;) { // process user choice until user exits
		if (usrChoice == 'S') { // show transaction screen when user starts the program or exits from previous screen
			usrChoice = transacScreen(access);
		}

		switch (usrChoice) { // determine user choice
			case 'V': // view rooms 
				usrChoice = viewRooms(access, "unfiltered", usrIdx);
				break;
			case 'B': // book a reservation
				usrChoice = bookRoom(access, usrIdx);
				break;
			case 'R': // show reservation/s
				usrChoice = seeReservation(access, usrIdx);
				break;
			case 'C': // cancel a reservation
				usrChoice = cancelReservation(access, usrIdx);
				break;
			case 'E': // edit room information
				usrChoice = editRoomInfo(access);
				break;
			case 'X': // exit program
				cout << "Exiting program" << endl;
				return 0;
			default: // invalid any other choice
				cout << "Invalid choice" << endl;
				return 1;
		}
	}

	return 0;
}