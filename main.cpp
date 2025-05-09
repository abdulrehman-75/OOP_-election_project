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
       cout << "1. Only admin can register voters & candidates. " << endl;
       cout << "2. Only admin can start election period, voters can ony vote when election period is in start mode. " << endl;
       cout << "3. Admin can not register more tha 1 voter with same email, system can automatically detect it. " << endl;
       cout << "4. If admin change election title, it changes over all title for that category. " << endl;
       cout << "5. Each voter can cast 2 votes in local election categories but both of candidates should be unique, casting both votes to same candidate gives error. " << endl;
       cout << "6. Each voter can cast only single vote in national election category." << endl;
       cout << "7. Admin Can view results of election even during elections or after election. " << endl;
       cout << "8. Results show the ranking of each party , by summing votes of individuals of that party. " << endl;
       cout << "9. Results also show votes of each candidate of each category. " << endl;
       cout << "10. Admin must set password of voters account atleast 4 characters long, because when voter logg in there is a password constraint which gives error on password entered is less than 4 characters. " << endl;
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
            createLocalElection(); 
            system("pause");
            break;
        case '2':
            createNationalElection(); 
            system("pause");
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


int countVoterVotes(const string& votedFile, const string& voterEmail) {
    int count = 0;
    ifstream fin(votedFile);
    string line;

    if (!fin.is_open()) {
        cout << "Warning: Could not open " << votedFile << " to count votes." << endl;
        return count;
    }

    while (getline(fin, line)) {
        stringstream ss(line);
        string fileEmail;
        getline(ss, fileEmail, '\t');

        if (fileEmail == voterEmail) {
            count++;
        }
    }
    fin.close();

    return count;
}

void getVotedCandidates(const string& votedFile, const string& voterEmail, string votedNames[], int& count) {
    count = 0;
    ifstream fin(votedFile);
    string line;

    if (!fin.is_open()) {
        cout << "Warning: Could not open " << votedFile << " to retrieve voted candidates." << endl;
        return;
    }

    while (getline(fin, line)) {
        stringstream ss(line);
        string fileEmail, candidateName;
        getline(ss, fileEmail, '\t');
        getline(ss, candidateName);

        if (fileEmail == voterEmail) {
            votedNames[count++] = candidateName;
        }
    }
    fin.close();
}


void updateVoterVoteCount(const string& voterEmail, const string& voterFile) {
    const int MAX_VOTERS = 100;
    string tempFile = "temp_" + voterFile;
    ifstream fin(voterFile);
    ofstream fout(tempFile);

    if (!fin.is_open() || !fout.is_open()) {
        cout << "Error: Could not open voter file for updating vote count." << endl;
        return;
    }

    string line;
    bool found = false;

    while (getline(fin, line)) {
        stringstream ss(line);
        string id, name, email, pass, voteCount;

        getline(ss, id, '\t');
        getline(ss, name, '\t');
        getline(ss, email, '\t');
        getline(ss, pass, '\t');
        getline(ss, voteCount);

        if (email == voterEmail) {
            int count = stoi(voteCount);
            count++;
            voteCount = to_string(count);
            found = true;
        }

        fout << id << '\t' << name << '\t' << email << '\t' << pass << '\t' << voteCount << endl;
    }

    fin.close();
    fout.close();
    if (found) {
        remove(voterFile.c_str());
        rename(tempFile.c_str(), voterFile.c_str());
        cout << "Voter vote count updated successfully." << endl;
    }
    else {
        remove(tempFile.c_str());
        cout << "Voter not found in the file." << endl;
    }
}


void voteLocal(int index, Voter* voters[]) {
    processVote(index, voters, "localCandidates.txt", "localVoteRecords.txt", 2, true);
}

void voteNational(int index, Voter* voters[]) {
    processVote(index, voters, "nationalCandidates.txt", "nationalVoteRecords.txt", 1, false);
}


void votersDashboard(int index, Voter* votersLocal[], Voter* votersNational[]) {
    char choice;
    do {
        system("cls");

        string voterEmail = votersLocal[index]->getEmail();

        int localVotesUsed = countVoterVotes("localVoteRecords.txt", voterEmail);
        int nationalVotesUsed = countVoterVotes("nationalVoteRecords.txt", voterEmail);

        cout << "==== RULES ====\nWelcome Voter! you can cast your vote to one and only one candidate \nstanding in national elections where as you can cast total 2 votes to two \ndifferent candidates standing in local elections." << endl << endl;
        cout << "Your votes remaining: \n";
        cout << "Local Elections: " << (2 - localVotesUsed) << " out of 2\n";
        cout << "National Elections: " << (1 - nationalVotesUsed) << " out of 1\n\n";
        cout << "=============================\n";
        cout << "        VOTER DASHBOARD      \n";
        cout << "=============================\n";
        cout << "1. Vote Local Candidate" << (localVotesUsed >= 2 ? " (No votes remaining)" : "") << "\n";
        cout << "2. Vote National Candidate" << (nationalVotesUsed >= 1 ? " (No votes remaining)" : "") << "\n";
        cout << "3. Logout\n";
        cout << "\nEnter Your Choice\n";
        choice = _getch();
        cout << choice << endl;

        switch (choice) {
        case '1':
            if (localVotesUsed >= 2) {
                cout << "\nYou have already used all your local votes!\n";
                system("pause");
            }
            else {
                voteLocal(index, votersLocal);
            }
            break;
        case '2':
            if (nationalVotesUsed >= 1) {
                cout << "\nYou have already used your national vote!\n";
                system("pause");
            }
            else {
                voteNational(index, votersNational);
            }
            break;
        case '3':
            cout << "\nLogging out from voters dashboard...";
            Sleep(700); cout << "......"; Sleep(700); cout << "....."; Sleep(300);
            break;
        default:
            cout << "\nInvalid choice. Please try again.\n";
            Sleep(1000);
        }
    } while (choice != '3');
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