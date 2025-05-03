
#ifndef UPDATED_E_VOTING_SYSTEM_H
#define UPDATED_E_VOTING_SYSTEM_H
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

// User Class
class User 
{
protected:
    int id;
    string name;
    string email;
    string pass;
public:
    virtual void show() = 0;
    virtual bool loginE(string) = 0;
    virtual bool loginP(string) = 0;
    virtual ~User() {}
};

class Voter : public User 
{
private:
    int voted;
public:
    Voter() 
    {
        id = 0;
        name = "";
        email = "";
        pass = "";
        voted = 0;
    }
    Voter(int i, string n, string e, string p, int v) 
    {
        id = i;
        name = n;
        email = e;
        pass = p;
        voted = v;
    }

    Voter(int i, string n, string e, string p) 
    {
        id = i;
        name = n;
        email = e;
        pass = p;
        voted = 0;

        ofstream fout1("votedNationals.txt", ios::app);
        ofstream fout2("votedLocals.txt", ios::app);

        if (fout1.is_open() && fout2.is_open()) 
        {
            fout1 << id << "\t" << name << "\t" << email << "\t" << pass << "\t" << voted << endl;
            fout2 << id << "\t" << name << "\t" << email << "\t" << pass << "\t" << voted << endl;
            fout1.close();
            fout2.close();
        }
        else 
        {
            cout << "\nError opening one of the files!\n\n";
        }
    }

    void markVoted(const string& filename) {
        voted++;

        ifstream fin(filename);
        ofstream temp("temp.txt");
        string line;

        while (getline(fin, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            int fileId, fileVoted;
            string fileName, fileEmail, filePass;

            ss >> fileId;
            ss.ignore(); 
            getline(ss, fileName, '\t');
            getline(ss, fileEmail, '\t');
            getline(ss, filePass, '\t');
            ss >> fileVoted;

            // Ensure all parts are present before writing
            if (fileName.empty() || fileEmail.empty() || filePass.empty()) continue;

            if (fileId == id) {
                fileVoted = voted;
            }

            temp << fileId << "\t" << fileName << "\t" << fileEmail << "\t" << filePass << "\t" << fileVoted << '\n';
        }

        fin.close();
        temp.close();

        remove(filename.c_str());
        rename("temp.txt", filename.c_str());
    }

    void removeVote() 
    {
        voted--;
    }

    bool hasVoted() 
    {
        if (voted == 1)
            return true;
        return false;
    }
    bool hasDoubleVoted() 
    {
        if (voted == 2)
            return true;
        return false;
    }

    int getID() 
    {
        return id;
    }

    string getName() 
    {
        return name;
    }
    string getEmail() {
        return email;
    }
    int getVotes() {
        return voted;
    }

    void show() 
    {
        cout << "Voter ID: " << id << ", Name: " << name << ", Voted: " << (voted ? "Yes" : "No") << endl;
    }

    bool loginE(string e) 
    {
        return e == email;
    }

    bool loginP(string p) 
    {
        return p == pass;
    }
};

//admin class
class Admin : public User 
{
public:
    Admin() 
    {
        id = 0;
        name = "";
        email = "";
        pass = "";
    }

    Admin(int i, string n, string e, string p) 
    {
        id = i;
        name = n;
        email = e;
        pass = p;
    }

    bool loginE(string e) 
    {
        return e == email;
    }

    bool loginP(string p) 
    {
        return p == pass;
    }

    void show() 
    {
        cout << "Admin: " << name << ", Email: " << email << endl;
    }
};

// Candidate Class 
class Candidate 
{
private:
    string name;
    string party;
    int votes;
public:
    Candidate() : name(""), party(""), votes(0) {}

    Candidate(string n, string p) : name(n), party(p), votes(0) {}

    void incrementVotes(int candidateNumberToVote, const string& filename) 
    {
        votes++;
        ifstream fin(filename);
        ofstream temp("temp.txt");
        string line;

        while (getline(fin, line)) 
        {
            stringstream ss(line);
            int candidateNumber, votesFromFile;
            string nameFromFile, partyFromFile;

            ss >> candidateNumber;
            ss.ignore();
            getline(ss, nameFromFile, '\t');
            getline(ss, partyFromFile, '\t');
            ss >> votesFromFile;

            if (candidateNumber == candidateNumberToVote) 
            {
                votesFromFile++;
            }

            temp << candidateNumber << "\t" << nameFromFile << "\t" << partyFromFile << "\t" << votesFromFile << endl;
        }

        fin.close();
        temp.close();

        remove(filename.c_str());
        rename("temp.txt", filename.c_str());
        cout << "\nVoted Succesfully !\n\n";
    }

    int getVoteCount() const 
    {
        return votes;
    }
    string getCandidateInfo() const 
    {
        return "Name: " + name + ", Party: " + party + ", Votes: " + to_string(votes);
    }
    void setName(string n) 
    { 
        name = n; 
    }
    void setParty(string p) 
    { 
        party = p; 
    }
    void setVotes(int v) 
    { 
        votes = v; 
    }
    string getName() const {  return name; }
    string getParty() const { return party; }
    int getVotes() const { return votes; }
};

class Election {
    protected:
        string title;
        int localCount;
        int nationalCount;
        Candidate* localCandidates;
        Candidate* nationalCandidates;
        int nextLocalIndex;
        int nextNationalIndex;
    
    public:
        Election() : title(""), localCount(0), nationalCount(0), localCandidates(nullptr), nationalCandidates(nullptr), nextLocalIndex(0), nextNationalIndex(0) {}
        virtual ~Election() {
            if (localCandidates) delete[] localCandidates;
            if (nationalCandidates) delete[] nationalCandidates;
        }
    
        virtual void begin() = 0;
        virtual void loadCandidates() = 0;
        virtual void addCandidates() = 0;
        virtual void vote(int index) = 0;
        virtual void showResults() const = 0;
    
        int getLocalCandidateCount() const { return nextLocalIndex; }
        int getNationalCandidateCount() const { return nextNationalIndex; }
        int getLocalCapacity() const { return localCount; }
        int getNationalCapacity() const { return nationalCount; }
    };
    
    class LocalElection : public Election {
    protected:
        int countExistingCandidates(const string& filename) {
            ifstream fin(filename);
            string line;
            int count = 0;
            while (getline(fin, line)) if (!line.empty()) count++;
            return count;
        }
    
    public:
        LocalElection(string t, int count) {
            title = t;
            int existing = countExistingCandidates("localCandidates.txt");
            localCount = existing + count;
            localCandidates = new Candidate[localCount];
            nextLocalIndex = 0;
        }
    
        void begin() override {
            loadCandidates();
            addCandidates();
            cout << "\nAdded successfully !\n\n";
        }
    
        void loadCandidates() override {
            ifstream fin("localCandidates.txt");
            if (!fin.is_open()) return;
    
            string line;
            while (getline(fin, line) && nextLocalIndex < localCount) {
                string idStr, name, party;
                int votes;
                stringstream ss(line);
                ss >> idStr >> name;
                ss >> ws;
                getline(ss, party, '\t');
                ss >> votes;
                localCandidates[nextLocalIndex].setName(name);
                localCandidates[nextLocalIndex].setParty(party);
                localCandidates[nextLocalIndex].setVotes(votes);
                nextLocalIndex++;
            }
            fin.close();
        }
    
        void addCandidates() override {
            ofstream fout("localCandidates.txt", ios::app);
            if (!fout.is_open()) return;

            int newCandidates = localCount - nextLocalIndex;
            for (int i = 0; i < newCandidates; ++i) {
                string name, party;
                int candidateNumber = nextLocalIndex + 1;

                cout << "Enter name of local candidate " << candidateNumber << ": ";
                cin.ignore();
                getline(cin, name);

                char partyChoice;
                do {
                    cout << "Select party for candidate " << candidateNumber << ":\n";
                    cout << "1. PMLN\n";
                    cout << "2. PTI\n";
                    cout << "3. PPP\n";
                    cout << "Enter choice (1/2/3): ";
                    partyChoice = _getch();
                    cout << partyChoice << endl;

                    switch (partyChoice) {
                    case '1':
                        party = "PMLN";
                        break;
                    case '2':
                        party = "PTI";
                        break;
                    case '3':
                        party = "PPP";
                        break;
                    default:
                        cout << "Invalid choice. Please select 1, 2, or 3.\n";
                    }
                } while (partyChoice != '1' && partyChoice != '2' && partyChoice != '3');

                localCandidates[nextLocalIndex].setName(name);
                localCandidates[nextLocalIndex].setParty(party);
                localCandidates[nextLocalIndex].setVotes(0);
                fout << candidateNumber << "\t" << name << "\t" << party << "\t0\n";
                nextLocalIndex++;
            }

            fout.close();
        }
    
        void vote(int index) override {
            if (index >= 1 && index <= nextLocalIndex)
                localCandidates[index - 1].incrementVotes(index, "localCandidates.txt");
            else
                cout << "Invalid local candidate index!\n";
        }
    
        void showResults() const override {
            cout << "\n=== Local Election Results for " << title << " ===\n";
            for (int i = 0; i < nextLocalIndex; ++i)
                cout << localCandidates[i].getCandidateInfo() << endl;
        }
    };
    
    class NationalElection : public Election {
    protected:
        int countExistingCandidates(const string& filename) {
            ifstream fin(filename);
            string line;
            int count = 0;
            while (getline(fin, line)) if (!line.empty()) count++;
            return count;
        }
    
    public:
        NationalElection(string t, int count) {
            title = t;
            int existing = countExistingCandidates("nationalCandidates.txt");
            nationalCount = existing + count;
            nationalCandidates = new Candidate[nationalCount];
            nextNationalIndex = 0;
        }
    
        void begin() override {
            loadCandidates();
            addCandidates();
            cout << "\nAdded successfully !\n\n";
        }
    
        void loadCandidates() override {
            ifstream fin("nationalCandidates.txt");
            if (!fin.is_open()) return;
    
            string line;
            while (getline(fin, line) && nextNationalIndex < nationalCount) {
                string idStr, name, party;
                int votes;
                stringstream ss(line);
                ss >> idStr >> name;
                ss >> ws;
                getline(ss, party, '\t');
                ss >> votes;
                nationalCandidates[nextNationalIndex].setName(name);
                nationalCandidates[nextNationalIndex].setParty(party);
                nationalCandidates[nextNationalIndex].setVotes(votes);
                nextNationalIndex++;
            }
            fin.close();
        }
    
        void addCandidates() override {
            ofstream fout("nationalCandidates.txt", ios::app);
            if (!fout.is_open()) return;

            int newCandidates = nationalCount - nextNationalIndex;
            for (int i = 0; i < newCandidates; ++i) {
                string name, party;
                int candidateNumber = nextNationalIndex + 1;

                cout << "Enter name of national candidate " << candidateNumber << ": ";
                cin.ignore();
                getline(cin, name);

                char partyChoice;
                do {
                    cout << "Select party for candidate " << candidateNumber << ":\n";
                    cout << "1. PMLN\n";
                    cout << "2. PTI\n";
                    cout << "3. PPP\n";
                    cout << "Enter choice (1/2/3): ";
                    partyChoice = _getch();
                    cout << partyChoice << endl;

                    switch (partyChoice) {
                    case '1':
                        party = "PMLN";
                        break;
                    case '2':
                        party = "PTI";
                        break;
                    case '3':
                        party = "PPP";
                        break;
                    default:
                        cout << "Invalid choice. Please select 1, 2, or 3.\n";
                    }
                } while (partyChoice != '1' && partyChoice != '2' && partyChoice != '3');

                nationalCandidates[nextNationalIndex].setName(name);
                nationalCandidates[nextNationalIndex].setParty(party);
                nationalCandidates[nextNationalIndex].setVotes(0);
                fout << candidateNumber << "\t" << name << "\t" << party << "\t0\n";
                nextNationalIndex++;
            }

            fout.close();
        }
    
        void vote(int index) override {
            if (index >= 1 && index <= nextNationalIndex)
                nationalCandidates[index - 1].incrementVotes(index, "nationalCandidates.txt");
            else
                cout << "Invalid national candidate index!\n";
        }
    
        void showResults() const override {
            cout << "\n=== National Election Results for " << title << " ===\n";
            for (int i = 0; i < nextNationalIndex; ++i)
                cout << nationalCandidates[i].getCandidateInfo() << endl;
        }
    };

    
    #endif