// ** HEADER FILES
#include "printUI.h"

// ** PROTOTYPES
// starting functions
int decAttempt(int attempts);


// admin functions
int validateAdmin();


// ** DEFINITIONS
// * ADMIN FUNCTIONS
// validate if user is admin
int validateAdmin() {
  string adminPw; // initialize variables
  int attempts = 3;
  string uliVAtextArrVA[] = {
    "Enter Password: ",
    "______________",
  };

  PrintUI uliVA(uliVAtextArrVA); // print UI
  uliVA.print();

  for (;;) { // loop until correct password is entered or attempts are used up
    cout << endl << ":: "; // ask for password
    cin >> adminPw;

    if (admin.getPassword().compare(adminPw) == 0){
      return -99; // return admin index if password is correct
    } else {
      attempts = decAttempt(attempts); // decrement attempts and ask for password again
    }
  }
  
  return -1;
}


// ** STARTING FUNCTIONS
// decrement attempts until 0
int decAttempt(int attempts) { 
  attempts--; // decrement attempts
 
  if (attempts == 0) { // check if attempts are used up
    cout << "Login Invalid. Please Restart." << endl;
    exit(1); // exit program if attempts are used up
  }

  cout << "Attempts left: " << attempts; // else print and return attempts left
  return attempts;
}