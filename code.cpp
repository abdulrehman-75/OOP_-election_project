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
        cout << "Voter ID: " << id << ", Name: " << name << ", Voted: " << (voted ? "Yes" : "No") << endl;
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
}  
