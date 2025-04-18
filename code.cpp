
#include <iostream>
using namespace std;

class User {
protected:
    int id;
    string name;
public:
    virtual void show() = 0;
};

class Voter : public User {
    bool voted;
public:
    Voter() {
        id = 0;
        name = "";
        voted = false;
    }

    Voter(int i, string n) {
        id = i;
        name = n;
        voted = false;
    }

    void markVoted() {
        voted = true;
    }

    bool hasVoted() {
        return voted;
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
};
