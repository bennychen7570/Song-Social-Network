#ifndef USER_H
#define USER_H
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

class User{
public:

    static int user_count;

    std::string user_name;
    std::vector<User *> friends;
    User();
    User(std::string);
    std::string getTitle();
    bool areFriends(User*);
    void make_friendship(User*);
    void show_friends();
    void unfriend(std::string);
    int user_index;

    friend std::ostream& operator<<(std::ostream& os, const User& user);
    friend bool operator>(const User &u1, const User &u2);
    friend bool operator<(const User &u1, const User &u2);
};

#endif