#include <iostream>
using namespace std;

//user class
class User {
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

class Voter : public User {
private:
    int voted;
public:
    Voter() {
        id = 0;
        name = "";
        email = "";
        pass = "";
        voted = 0;
    }
    Voter(int i, string n, string e, string p, int v) {
        id = i;
        name = n;
        email = e;
        pass = p;
        voted = v;
    }

    Voter(int i, string n, string e, string p) {
        id = i;
        name = n;
        email = e;
        pass = p;
        voted = 0;

        ofstream fout1("votedNationals.txt", ios::app);
        ofstream fout2("votedLocals.txt", ios::app);

        if (fout1.is_open() && fout2.is_open()) {
            fout1 << id << "\t" << name << "\t" << email << "\t" << pass << "\t" << voted << endl;
            fout2 << id << "\t" << name << "\t" << email << "\t" << pass << "\t" << voted << endl;
            fout1.close();
            fout2.close();
        }
        else {
            cout << "\nError opening one of the files!\n\n";
        }
    }


    void markVoted(const string& filename) {
        voted++;

        ifstream fin(filename);
        ofstream temp("temp.txt");
        string line;

        while (getline(fin, line)) {
            stringstream ss(line);
            int fileId, fileVoted;
            string fileName, fileEmail, filePass;

            ss >> fileId;
            ss.ignore(); // Ignore tab
            getline(ss, fileName, '\t');
            getline(ss, fileEmail, '\t');
            getline(ss, filePass, '\t');
            ss >> fileVoted;

            if (fileId == id) {
                fileVoted = voted;
            }

            temp << fileId << "\t" << fileName << "\t" << fileEmail << "\t" << filePass << "\t" << fileVoted << endl;
        }

        fin.close();
        temp.close();

        remove(filename.c_str());
        rename("temp.txt", filename.c_str());
    }

    void removeVote() {
        voted--;
    }

    bool hasVoted() {
        if (voted == 1)
            return true;
        return false;
    }
    bool hasDoubleVoted() {
        if (voted == 2)
            return true;
        return false;
    }

    int getID() {
        return id;
    }

    string getName() {
        return name;
    }

    void show() {
        cout << "Voter ID: " << id << ", Name: " << name << ", Voted: " << (voted ? "Yes" : "No") << endl;
    }

    bool loginE(string e) {
        return e == email;
    }

    bool loginP(string p) {
        return p == pass;
    }
};

// Admin class
class Admin : public User
{
    string pass;
public:
    Admin()
    {
        id = 0;
        name = "";
        pass = "";
    }

    Admin(int i, string n, string p)
    {
        id = i;
        name = n;
        pass = p;
    }

    bool login(string p)
    {
        return p == pass;
    }

    void show()
    {
        cout << "Admin: " << name << endl;
    }
};

// Election base class
class Election {
protected:
    string title;
    int cCount;
    string* cands;
    int* votes;

public:
    Election() {
        title = "";
        cCount = 0;
        cands = nullptr;
        votes = nullptr;
    }

    Election(string t, int c) {
        title = t;
        cCount = c;
        cands = new string[cCount];
        votes = new int[cCount];
        for (int i = 0; i < cCount; i++) {
            votes[i] = 0;
            cands[i] = "";
        }
    }

    virtual ~Election() {
        delete[] cands;
        delete[] votes;
    }

    virtual void begin() = 0;

    void setCand(int index, string n) {
        if (index >= 0 && index < cCount)
            cands[index] = n;
    }

    void vote(int choice) {
        if (choice >= 0 && choice < cCount)
            votes[choice]++;
    }

    void results() {
        cout << "\n=== Results for " << title << " ===\n";
        for (int i = 0; i < cCount; i++) {
            cout << cands[i] << " - Votes: " << votes[i] << endl;
        }
    }

    int getCount() {
        return cCount;
    }

    string getCand(int i) {
        if (i >= 0 && i < cCount)
            return cands[i];
        return "";
    }
};

// derived classes: 
// national election , local election (todo)
