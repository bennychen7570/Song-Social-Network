#pragma once
#include <iostream>

using namespace std;

class Song{
public:
    Song();
    Song(string title);
    Song(const Song& other);
    Song& operator=(const Song& other);
    void setTitle(string t);
    string getTitle();
    void play(int t);
    int getPlays();
    void setIndex(int i);
    int getIndex();
    void resetPlays();

    friend std::ostream& operator<<(std::ostream& os, const Song& song);
    friend bool operator>(const Song &s1, const Song &s2);
    friend bool operator<(const Song &s1, const Song &s2);
private:
    string title;
    int plays;
    int index;
};