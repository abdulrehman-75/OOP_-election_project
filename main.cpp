#include "Updated_E_Voting_system.h"
#include <iostream>
#include <conio.h> 
#include <string>
#include <sstream>
#include <fstream>
#include <windows.h>

using namespace std;


void adminDashboard() 
{
     char choice;
     do {
         system("cls");
         cout << "=============================\n";
         cout << "        ADMIN DASHBOARD      \n";
         cout << "=============================\n";
         cout << "1. Create Local Election\n";
         cout << "2. Create National Election\n";
         cout << "3. Register New Voters\n";
         cout << "4. Display Existing Voters\n";
         cout << "5. Logout From Admin\n";
         cout << "6. Start Voting Period\n";
         cout << "7. End Voting Period\n";
         cout << "8. View Election Results\n";
         cout << "\nEnter Your Choice\n";

           choice = _getch();
           cout << choice << endl;

         switch (choice) 
         {
         case '1': 
             createLocalElection(); 
             break;
         case '2': 
             createNationalElection(); 
             break;
         case '3': 
             registerVoters(); 
             break;
         case '4': 
             displayVoters(); 
             break;
         case '5':
             cout << "\nLogging out from admin dashboard...";
             Sleep(700); cout << "......"; Sleep(700); cout << "....."; Sleep(300);
             break;
         case '6': 
             setVotingPeriodStatus(true); 
             break;
         case '7': 
             setVotingPeriodStatus(false);  
             break;
         case '8': 
             viewResults(); 
             break;
         default:
             cout << "\nInvalid choice. Please try again.\n";
             Sleep(1000);
         }
     } 
     while (choice != '5');
}

void adminPortal() 
{
    system("cls");
    User* admin = new Admin(15, "Aizaz Akmal", "admin123", "qwer");

    string email, pass;
    cout << "\n--- Admin Login ---\n";
    cout << "Enter Email: ";
    getline(cin, email);
    cout << "Enter Password: ";
    pass = getMaskedPassword();

    if (admin->loginE(email) && admin->loginP(pass)) {
        admin->show();
        system("pause");
        adminDashboard();
    }
    else {
        cout << "\nEntered email or password is incorrect!\n";
        cout << "Returning back to main menu...";
        Sleep(1500);
        cout << "...\n";
    }

    delete admin;
}

int main() 
{
    char choice;
    do {
        system("cls");
        cout << "==============================\n";
        cout << "   ELECTRONIC VOTING SYSTEM   \n";
        cout << "==============================\n";
        cout << "1. Login as Admin\n";
        cout << "2. Login as Voter\n";
        cout << "3. View Rules\n";
        cout << "4. Exit\n";
        cout << "\nEnter Your Choice\n";

        choice = _getch();
        cout << choice << endl;

        switch (choice) {
        case '1': adminPortal(); break;
        case '2': votersPortal(); break;
        case '3': rules(); break;
        case '4':
            cout << "\nExiting the program.\n";
            break;
        default:
            cout << "\nInvalid choice. Please try again.\n";
            Sleep(1000);
        }
    } while (choice != '4');

    return 0;
}
