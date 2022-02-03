#include <iostream>
#include <iomanip>
#include "Song.h"

using namespace std;

Song::Song() : title(""), plays(0), index(-1) {};

Song::Song(string title) : plays(0), index(-1) {
    this->title = title;
}
Song::Song(const Song& other) {
    title = other.title;
    plays = other.plays;
}
Song& Song::operator=(const Song& other) {
    title = other.title;
    plays = other.plays;
    return *this;
}

int Song::getPlays() {
    return plays;
}

void Song::play(int t) {
    plays += t;
}

void Song::setTitle(string t){
    title = t;
}

string Song::getTitle(){
    return title;
}

void Song::resetPlays(){
    plays = 0;
}

void Song::setIndex(int i) {index = i;}
int Song::getIndex() {return index;}

ostream& operator<<(std::ostream& os, const Song& song)
{
    int width = 50;
    os << left << "Title: " << setw(width) << song.title << " Listens: " << to_string(song.plays);
    return os;
}

bool operator>(const Song &s1, const Song &s2) {
    return s1.title > s2.title;
}
bool operator<(const Song &s1, const Song &s2){
    return s1.title < s2.title;
}