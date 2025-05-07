#include "Updated_E_Voting_System.h"
#include <iostream>
#include <conio.h> 
#include <string>
#include <sstream>
#include <fstream>
#include <windows.h>

using namespace std;


void rules()
{
    system("cls");
    cout << "Rules...\n";
    // to be written yet.
    system("pause");
}

string getMaskedPassword()
{
    string password;
    char ch;
    while (true)
    {
        password.clear();
        cout << "Enter password (at least 4 characters): ";
        while (true)
        {
            ch = _getch();
            if (ch == '\r')
            {
                cout << endl;
                break;
            }
            else if (ch == '\b')
            {
                if (!password.empty())
                {
                    password.pop_back();
                    cout << "\b \b";
                }
            }
            else
            {
                password += ch;
                cout << '*';
            }
        }
        if (password.length() < 4)
        {
            cout << "Password too short. Please enter at least 4 characters.\n";
            Sleep(1000);
            system("cls");
        }
        else
        {
            break;
        }
    }
    return password;
}

int loadVotersFromFile(Voter* voters[], int index, const string& filename)
{
    fstream fin(filename);
    if (!fin.is_open())
    {
        cout << "Error opening file: " << filename << endl;
        return index;
    }

    string line;
    while (getline(fin, line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);
        int id, voted;
        string name, email, pass;

        ss >> id;
        ss.ignore();
        getline(ss, name, '\t');
        getline(ss, email, '\t');
        getline(ss, pass, '\t');
        ss >> voted;

        voters[index++] = new Voter(id, name, email, pass, voted);
    }

    fin.close();
    return index;
}

string getElectionTitle(bool isLocal)
{
    string title;
    char choice;

    do
    {
        cout << "\nDo you want to continue with default Title? (y/n)\n";
        cout << "[Warning]: changing title will change default values of title!\n";
        choice = _getch();
        cout << choice << endl;

        switch (choice)
        {
        case 'y':
        case 'Y':
        {
            ifstream file("electionTitles.txt");
            if (file.is_open())
            {
                string line1, line2;
                getline(file, line1);
                getline(file, line2);
                file.close();

                if (isLocal)
                    title = line1;
                else
                    title = line2;
                if (title.empty())
                    cout << "Warning: Default title not found in file.\n";
                else
                    cout << "Default title fetched: " << title << endl;
            }
            else
            {
                cout << "Error: Could not open electionTitles.txt\n";
            }
            break;
        }
        case 'n':
        case 'N':
        {
            cout << "Enter title for creating election:\n";
            cin.ignore();
            getline(cin, title);

            string line1 = "", line2 = "";
            ifstream fileIn("electionTitles.txt");
            if (fileIn.is_open())
            {
                getline(fileIn, line1);
                getline(fileIn, line2);
                fileIn.close();
            }

            if (isLocal)
                line1 = title;
            else
                line2 = title;

            ofstream fileOut("electionTitles.txt");
            if (fileOut.is_open())
            {
                fileOut << line1 << endl;
                fileOut << line2 << endl;
                fileOut.close();
                cout << "Default title updated in file.\n";
            }
            else
            {
                cout << "Error: Could not write to electionTitles.txt\n";
            }
            break;
        }
        default:
            cout << "Invalid input. Please enter 'y' or 'n'.\n";
        }
    } while (choice != 'y' && choice != 'Y' && choice != 'n' && choice != 'N');

    return title;
}

void createLocalElection()
{
    int num;
    string title = getElectionTitle(true);

    cout << "Enter the number of candidates you want to add: ";
    cin >> num;
    if (cin.fail() || num <= 0)
    {
        cin.clear(); cin.ignore(1000, '\n');
        cout << "Invalid number of candidates!\n";
        return;
    }
    cin.ignore();

    Election* e = new LocalElection(title, num);
    e->begin();
    delete e;

}

void createNationalElection()
{
    int num;
    string title = getElectionTitle(false);

    cout << "Enter the number of candidates you want to add: ";
    cin >> num;
    if (cin.fail() || num <= 0)
    {
        cin.clear(); cin.ignore(1000, '\n');
        cout << "Invalid number of candidates!\n";
        return;
    }
    cin.ignore();

    Election* e = new NationalElection(title, num);
    e->begin();
    delete e;
}

void registerVoters()
{
    Voter* voters[50];
    int index = 0;
    index = loadVotersFromFile(voters, index, "votedLocals.txt");

    int count;
    cout << "Enter the number of voters you want to register: ";
    cin >> count;
    if (cin.fail() || count <= 0 || count > 50 - index) {
        cin.clear(); cin.ignore(1000, '\n');
        cout << "Invalid input. Returning to menu...\n";
        Sleep(1000);
        return;
    }
    cin.ignore();

    string name, email, pass;
    for (int j = 0; j < count; j++)
    {
        system("cls");
        cout << "\n--- Registering Voter number " << index << " ---\n";
        cout << "Set name: ";
        getline(cin, name);

        bool emailExists = false;
        do
        {
            emailExists = false;
            cout << "Set Email: ";
            getline(cin, email);

            for (int k = 0; k < index; k++)
            {
                if (voters[k]->getEmail() == email)
                {
                    cout << "Error: This email is already registered. Please enter a different email.\n";
                    emailExists = true;
                    break;
                }
            }
        } while (emailExists);

        cout << "Set Password: ";
        getline(cin, pass);

        voters[index] = new Voter(index, name, email, pass);
        index++;
        Sleep(500);
    }

    cout << "\nAll voters registered successfully!\n";
    system("pause");

    for (int j = 0; j < index; j++)
    {
        delete voters[j];
    }
}

void displayVoters()
{
    Voter* localVoters[50];
    Voter* nationalVoters[50];
    int localCount = 0, nationalCount = 0;

    localCount = loadVotersFromFile(localVoters, 0, "votedLocals.txt");
    nationalCount = loadVotersFromFile(nationalVoters, 0, "votedNationals.txt");

    cout << "Combined Voter Information (Local + National):\n";
    cout << "--------------------------------------------------\n";

    for (int i = 0; i < localCount; i++)
    {
        int voterId = localVoters[i]->getID();
        string name = localVoters[i]->getName();
        int localVotes = localVoters[i]->getVotes();

        int nationalVotes = 0;
        for (int j = 0; j < nationalCount; j++)
        {
            if (nationalVoters[j]->getID() == voterId)
            {
                nationalVotes = nationalVoters[j]->getVotes();
                break;
            }
        }

        cout << "Voter ID: " << voterId << ", Name: " << name
            << ", Local Votes: " << localVotes
            << ", National Votes: " << nationalVotes << "\n";
    }
    system("pause");

    for (int i = 0; i < localCount; i++)
        delete localVoters[i];
    for (int i = 0; i < nationalCount; i++)
        delete nationalVoters[i];
}

bool isVotingPeriodActive(const string& filename = "votingPeriod.txt")
{
    ifstream inFile(filename);
    if (!inFile.is_open())
    {
        cerr << "Error opening " << filename << "\n";
        return false;
    }

    char status;
    inFile >> status;
    inFile.close();

    return status == '1';
}

void setVotingPeriodStatus(bool isActive, const string& filename = "votingPeriod.txt") {
    ofstream outFile(filename, ios::trunc);
    if (!outFile.is_open())
    {
        cerr << "Error opening " << filename << " for writing.\n";
        return;
    }

    outFile << (isActive ? '1' : '0');
    outFile.close();
    cout << "Voting Period Changes successfully !\n";
    system("pause");
}

void displayElectionResults(const string& candidateFile, const string& title, Candidate* candidates, int& totalCount, int MAX)
{
    ifstream fin(candidateFile);
    string line;
    totalCount = 0;

    while (getline(fin, line) && totalCount < MAX)
    {
        stringstream ss(line);
        int id, votes;
        string name, party;

        ss >> id;
        ss.ignore();
        getline(ss, name, '\t');
        getline(ss, party, '\t');
        ss >> votes;

        candidates[totalCount].setName(name);
        candidates[totalCount].setParty(party);
        candidates[totalCount].setVotes(votes);
        totalCount++;
    }
    fin.close();

    string* parties = new string[MAX];
    int* voteCounts = new int[MAX];
    int partyCount = 0;

    for (int i = 0; i < totalCount; ++i)
    {
        string party = candidates[i].getParty();
        int j;
        for (j = 0; j < partyCount; ++j)
        {
            if (parties[j] == party)
            {
                voteCounts[j] += candidates[i].getVoteCount();
                break;
            }
        }
        if (j == partyCount)
        {
            parties[partyCount] = party;
            voteCounts[partyCount] = candidates[i].getVoteCount();
            partyCount++;
        }
    }

    for (int i = 0; i < partyCount - 1; ++i)
    {
        for (int j = i + 1; j < partyCount; ++j)
        {
            if (voteCounts[j] > voteCounts[i])
            {
                swap(voteCounts[i], voteCounts[j]);
                swap(parties[i], parties[j]);
            }
        }
    }

    cout << "\n=== " << title << " ===\n";
    cout << "Party Rankings:\n";
    for (int i = 0; i < partyCount; ++i)
    {
        cout << i + 1 << ". " << parties[i] << " - " << voteCounts[i] << " votes\n";
    }

    cout << "\nDetailed Candidate Results:\n";
    for (int i = 0; i < totalCount; ++i)
    {
        cout << candidates[i].getCandidateInfo() << endl;
    }

    delete[] parties;
    delete[] voteCounts;
}

void viewResults()
{
    const int MAX = 50;
    Candidate* candidates = new Candidate[MAX];

    ifstream finTitle("electionTitles.txt");
    string localTitle, nationalTitle;
    getline(finTitle, localTitle);
    getline(finTitle, nationalTitle);
    finTitle.close();

    int count;
    displayElectionResults("localCandidates.txt", localTitle, candidates, count, MAX);
    displayElectionResults("nationalCandidates.txt", nationalTitle, candidates, count, MAX);
    system("pause");

    delete[] candidates;
}

void adminDashboard()
{
    char choice;
    do
    {
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
            createLocalElection(); system("pause");
            break;
        case '2':
            createNationalElection(); system("pause");
            break;
        case '3':
            registerVoters();
            break;
        case '4':
            displayVoters();
            break;
        case '5':
            cout << "\nLogging out from admin dashboard...";
            Sleep(700);
            cout << "......";
            Sleep(700);
            cout << ".....";
            Sleep(300);
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
    } while (choice != '5');
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

    if (admin->loginE(email) && admin->loginP(pass))
    {
        admin->show();
        system("pause");
        adminDashboard();
    }
    else
    {
        cout << "\nEntered email or password is incorrect!\n";
        cout << "Returning back to main menu...";
        Sleep(1500);
        cout << "...\n";
    }

    delete admin;
}

void votersPortal()
{
    // to be implemented yet.
}

int main()
{
    char choice;
    do
    {
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

        switch (choice)
        {
        case '1':
            adminPortal();
            break;
        case '2':
            votersPortal();
            break;
        case '3':
            rules();
            break;
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