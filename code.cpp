#include <iostream>
using namespace std;

//user class
class User 
{
protected:
    int id;
    string name;
public:
    virtual void show() = 0;
};

//voter class
class Voter : public User 
{
    bool voted;
public:
    Voter() 
    {
        id = 0;
        name = "";
        voted = false;
    }

    Voter(int i, string n) 
    {
        id = i;
        name = n;
        voted = false;
    }

    void markVoted() 
    {
        voted = true;
    }

    bool hasVoted() 
    {
        return voted;
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
        cout << "Voter ID: " << id << ", Name: " << name << ", Voted: ";  
        if (voted) {
            cout << "YES" << endl;
        } else {
            cout << "NO" << endl;
        }
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
