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

    void markVoted(const string& filename) 
    {
        voted++;

        ifstream fin(filename);
        ofstream temp("temp.txt");
        string line;

        while (getline(fin, line)) 
        {
            stringstream ss(line);
            int fileId, fileVoted;
            string fileName, fileEmail, filePass;

            ss >> fileId;
            ss.ignore(); // Ignore tab
            getline(ss, fileName, '\t');
            getline(ss, fileEmail, '\t');
            getline(ss, filePass, '\t');
            ss >> fileVoted;

            if (fileId == id) 
            {
                fileVoted = voted;
            }

            temp << fileId << "\t" << fileName << "\t" << fileEmail << "\t" << filePass << "\t" << fileVoted << endl;
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
    string getName() const 
    { 
        return name; 
    }
};