#include "User.h"

User::User(){
    user_name = "";
    user_index = user_count;
    user_count++;
}
User::User(std::string name){
    user_name = name;
    user_index = user_count;
    user_count++;
}

void User::make_friendship(User* user){
    if (user_name == user->user_name){
        std::cout << "A user cannot be friends with themselves" << std::endl;
        return;
    }
    friends.push_back(user);
}

void User::show_friends(){
    if (friends.empty()){
        std::cout << user_name + " has no friends :(" << std::endl;
        return;
    }
    if (user_name == "PRIMARY_USER")
        std::cout << "Your friends: ";
    else
        std::cout << "Friends of " + user_name + ": ";
    std::vector<User*>::iterator iter;
    for(iter = friends.begin(); iter != friends.end()-1; iter++){
        if ((*iter)->user_name == "PRIMARY_USER")
            std::cout << "*You*" << ", ";
        else
         std::cout << (*iter)->user_name << ", ";
    }
    if ((*iter)->user_name == "PRIMARY_USER")
        std::cout << "*You*" << std::endl;
    else
        std::cout << (*iter)->user_name << std::endl;
}

void User::unfriend(std::string name){
    std::vector<User*>::iterator iter;
    for(iter = friends.begin(); iter != friends.end(); iter++){
        if((*iter)->user_name == name){
            friends.erase(iter);
            //std::cout << name << " is not your friend now." << std::endl;
            return;
        }
    }
}

std::string User::getTitle() {
    return user_name;
}

bool User::areFriends(User* user){
    for (int i = 0; i < friends.size(); i++){
        if(friends[i]->user_name == user->user_name){
            return true;
        }
    }
    return false;
}

std::ostream& operator<<(std::ostream& os, const User& user)
{
    int width = 50;
    os << std::left << "User name: " << std::setw(width) << user.user_name << " Friends: " << std::to_string(user.friends.size());
    return os;
}
bool operator>(const User &u1, const User &u2) {
    return u1.user_name > u2.user_name;
}
bool operator<(const User &u1, const User &u2){
    return u1.user_name < u2.user_name;
}

int User::user_count = 0;