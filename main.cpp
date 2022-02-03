#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <vector>
#include <sstream>
#include <fstream>
#include "Song.h"
#include <algorithm>
#include <random>
#include <queue>
#include "BST.h"
#include "User.h"
#include "Heap.h"
#include "Library.h"
#include "Parser.h"

template <typename T>
Node<T>* find(Node<T>*&, string);
void listen(string, int, Node<Song>*& root, Heap& heap);
void make_friends(string, string, Node<User>*& u_root);
void show_friends(string, Node<User>*& u_root);
void break_friendship(string, string, Node<User>*& u_root);
bool isInEFN(string, User, Node<User>* u_root, const BST<User>&, int radius);
void recommend(int, Library&, Heap*);
void add_user(string, Node<User>*&, BST<User>&);

const string PRIMARY_USERNAME = "PRIMARY_USER";

struct EFN {
    int get_rad() const {return radius;}
    void set_rad(int radius){this->radius = radius;}
private:
    int radius = 1;
};

using namespace std;
// getSongFilenames takes a directory name and returns the names of all .txt files in that directory.
std::vector<std::string> getSongFilenames(std::string inputdir_name) {
	if (inputdir_name == "") return std::vector<std::string>();
	// cite - https://www.delftstack.com/howto/cpp/how-to-get-list-of-files-in-a-directory-cpp/
	DIR* dir;
	struct dirent* dir_entry;
	std::vector<std::string> filenames;
	if ((dir = opendir(inputdir_name.c_str())) != nullptr) {
		while ((dir_entry = readdir(dir)) != nullptr) {
			if (std::string(dir_entry->d_name).find(".txt") != std::string::npos) {
				filenames.push_back(dir_entry->d_name);
			}
		}
		closedir(dir);
	}
	return filenames;
}

bool isNumber(const string& str)
{
    for (char const &c : str) {
        if (std::isdigit(c) == 0) return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    //Assumes there is a local directory named "songsdir." Names of text files (minus the .txt part) in this directory will be added as songs in the system.
	if (argc != 1) {
		std::cout << "default input directory : songsdir" << std::endl;
		exit(1);
	}
    std::string inputdir_name = "songsdir";

	// song_files vector stores the names of the files in the input directory.
	std::vector<std::string> song_files = getSongFilenames(inputdir_name);
	//randomize to avoid alphabetic order
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(song_files), std::end(song_files), rng);

	std::vector<Song> songs;
	BST<Song> songs_BST;
	Heap* songs_heap = new Heap;
    struct EFN efn;

//	//make a list of song objects from file names
    for (string& it : song_files){
        songs.push_back(Song(it.substr(0,it.find_first_of('.'))));
    }

//    Low song size testing (comment out above loop)
//    songs.push_back(Song("ONE"));
//    songs.push_back(Song("TWO"));
//    songs.push_back(Song("THREE"));

    //add song references into heap
    for (Song& it : songs){
        songs_heap->insert(&it);
    }
    //Add song references into songs_BST
    Node<Song>* root = NULL;
    songs_BST.insert(root, &songs[0]);
    for (int i = 1; i < songs.size(); i++){
        songs_BST.insert(root, &songs[i]);
    }
    //Make BST of users and insert primary user at root
    BST<User> users_BST;
    User primary_user(PRIMARY_USERNAME);
    Node<User>* u_root = NULL;
    users_BST.insert(u_root, &primary_user);

    //instantiate primary user library
    Library library;

	cout<<"Enter one of the following commands."<<endl;
	cout<< "1. user <user name> \nAdd a new user to system (spaces not allowed).\n" << endl;
	cout<< "2. friend <user1> [<user2>]\nFriend user 1. If user2 is specified, make a friendship between user1 and user2.\n" << endl;
	cout<< "3. unfriend <user1> [<user2>]\nUnfriend user 1. If user2 is specified, user1 unfriends user2.\n" << endl;
	cout<< "4. listen <user> <n> <song title>\n User listens to specified song n times.\n" << endl;
	cout<< "5. radius <k>\nSet EFN radius to k. k must be a positive integer.\n" << endl;
	cout<< "6. recommend <k>\nRecommend and add to your library, the top k played songs by users in the EFN radius.\n" << endl;
	cout<< "7. remove <song title>\n Remove specified song from the Primary User's library. \n" << endl;
	cout<< "8. show friends [<user name>]\nShow all your friends. If user name is specified, show friends for that user.\n" << endl;
	cout<< "9. show songs\nShow songs in your library.\n" << endl;
	cout<< "10. show system\nShow songs in the system but not in your library. Also shows the number of relevant listens by users in the EFN.\n" << endl;
	cout<< "11. exit\nExits the program." << endl;

	bool exit = false;
	string line;
	while (exit == false){
        getline(cin,line);
        Parser command(line);

        if(command.getOperation() == "user"){
            if (command.getArg1() == ""){
                cout << "Error: no name specified!" << endl;
                continue;
            }
            if (command.getArg2() != ""){
                cout << "Error: usernames may not contain spaces!" << endl;
                continue;
            }
            if (command.getArg1() == PRIMARY_USERNAME || command.getArg1() == "FLAG"){
                cout << "Error: that username is reserved!" << endl;
                continue;
            }
            add_user(command.getArg1(),u_root,users_BST);
        }
        else if (command.getOperation() == "friend"){
            if (command.getArg2() == ""){
                Node<User>* temp = find(u_root, command.getArg1());
                if (temp == NULL) {
                    cout << "Error: " + command.getArg1()  + " is not an active user" << endl;
                    continue;
                }
                if (primary_user.areFriends(temp->data)){
                    cout << "You and " + command.getArg1() + " are already friends!" << endl;
                    continue;
                }
                primary_user.make_friendship(temp->data);
                temp->data->make_friendship(&primary_user);
                cout << "You and " + command.getArg1() + " are now friends!" << endl;
            }
            else {
                make_friends(command.getArg1(),command.getArg2(), u_root);
            }
        }
        //unfriend <user1> [<user2>]\nUnfriend user 1. If user2 is specified, user1 unfriends user2.
        else if (command.getOperation() == "unfriend"){
            if (command.getArg2() == ""){
                break_friendship(PRIMARY_USERNAME, command.getArg1(), u_root);
                continue;
            }
            break_friendship(command.getArg1(), command.getArg2(), u_root);
        }
        //listen <user> <n> <song title> User listens to specified song n times.
        else if (command.getOperation() == "listen"){
            if (command.getArg3() == ""){
                cout << "Error: not enough arguments!" << endl;
                continue;
            }
            if (isNumber(command.getArg2())){
                if (find(u_root, command.getArg1()) == NULL){
                    cout << "Error: " + command.getArg1() + "is not an active user!" << endl;
                    continue;
                }
                if(library.find(library.root, command.getArg3()) != NULL){
                    cout << "Listens ignored: " + command.getArg3() + " is already in your library!" << endl;
                    continue;
                }
                if (!isInEFN(command.getArg1(),primary_user, u_root, users_BST,efn.get_rad())){
                    if (stoi(command.getArg2()) == 1)
                        cout << command.getArg1() + " listened to " + command.getArg3() + "one time! (but is not in the EFN so who cares?)" << endl;
                    else
                        cout << command.getArg1() + " listened to " + command.getArg3() + " " + command.getArg2() + " times! (but is not in the EFN so who cares?)" << endl;
                    continue;
                }
                if (stoi(command.getArg2()) >= 0) {
                    listen(command.getArg3(),stoi(command.getArg2()), root, *songs_heap); //LISTENING HAPPENING HERE
                    continue;
                }
            }
            cout << "Error: n must be a positive integer!" << endl;
        }
        //radius <k> Set EFN radius to k. k must be a positive integer.
        else if (command.getOperation() == "radius"){
            if (command.getArg1() == ""){
                cout << "Error: please specify a radius." << endl;
                continue;
            }
            if (isNumber(command.getArg1())){
                if (stoi(command.getArg1()) >= 0) {
                    efn.set_rad(stoi(command.getArg1()));
                    cout << "EFN radius set to " + command.getArg1() + "!" << endl;
                    continue;
                }
            }
            cout << "Error: radius must only be a positive integer!" << endl;

        }
        //recommend <k> Recommend and add to your library, the top k played songs by users in the EFN radius
        else if (command.getOperation() == "recommend"){
            if (command.getArg1() == ""){
                cout << "Error: please specify number of songs to recommend." << endl;
                continue;
            }
            if (isNumber(command.getArg1())){
                if (stoi(command.getArg1()) >= 0) {
                    recommend(stoi(command.getArg1()), library, songs_heap);
                    continue;
                }
            }
            cout << "Error: number of songs to recommend must be a positive integer!" << endl;
        }

        //show friends [<user name>] Show all your friends. If user name is specified, show friends for that user.
        else if (command.getOperation() == "show"){
            if(command.getArg1() == "friends"){
                if (command.getArg2() == ""){
                    primary_user.show_friends();
                    continue;
                }
                Node<User>* user = find(u_root,command.getArg2());
                if (user == NULL){
                    cout << "Error: " + command.getArg2() + " is not an active user!" << endl;
                    continue;
                }
                user->data->show_friends();
                continue;
            }
            //show songs Show songs in your library.
            if(command.getArg1() == "songs"){
                library.show_songs();
                continue;
            }
            //show system Show songs in the system but not in your library. Also shows the number of relevant listens by users in the EFN.
            if(command.getArg1() == "system"){
                songs_heap->displayHeap();
                continue;
            }
            cout << "Error: \"show\" must preceded a relevant parameter; \"friends,\" \"songs,\" \"system.\"" << endl;

        }
        //remove <song title> Remove specified song from the Primary User's library.
        else if (command.getOperation() == "remove"){
            static const char* WHITESPACE = " \t\n\r";
            string song = command.getArg1() + " " + command.getArg2() + " " + command.getArg3();
            song.erase(0, song.find_first_not_of(WHITESPACE));
            song.erase(song.find_last_not_of(WHITESPACE) + 1);
            if (command.getArg1() != "") {
                if(library.remove_song(song)) {
                    Node<Song> *temp = find(root, song);
                    songs_heap->insert(temp->data);
                    temp->data->resetPlays();
                }
            }
            else
                cout << "Error: Please specify song to remove!" << endl;
        }
        else if (command.getOperation() == "exit")
            exit = true;
        else
            cout << "Error: Invalid command." <<endl;
	}

	delete songs_heap;

}

template <typename T>
Node<T>* find(Node<T>*& node, string title) {
    if (node == NULL || title == node->data->getTitle()){
        return node;
    }
    if (title > node->data->getTitle()){
        return find(node->right, title);
    }
    return find(node->left, title);
}

void listen(string title, int times, Node<Song>*& root, Heap& heap) {
    if(times < 0){
        cout << "Error: A song may only be listened to a positive amount!" << endl;
        return;
    }
    Node<Song>* song = find(root, title);
    if (song == NULL){
        cout << "Error: " + title + " was not found in the system." << endl;
    }
    else{
        song->data->play(times);
        heap.increaseKey(song->data->getIndex(),song->data);
        if (times == 1)
            cout << title + " was listened to one time!" << endl;
        else
            cout << title + " was listened to " + to_string(times) + " times!" << endl;
    }
}

void make_friends(string name1, string name2, Node<User>*& u_root){
    Node<User>* temp1 = find(u_root, name1);
    Node<User>* temp2 = find(u_root, name2);
    if (temp1 == NULL){
        cout << "Error: " + name1 + " is not an active user" << endl;
        return;
    }
    if (temp2 == NULL){
        cout << "Error: " + name2 + " is not an active user" << endl;
        return;
    }
    if (temp1->data->areFriends(temp2->data)){
        cout << name1 + " and " + name2 + " are already friends!" << endl;
        return;
    }

    temp1->data->make_friendship(temp2->data);
    temp2->data->make_friendship(temp1->data);

    cout << name1 + " and " + name2 + " are now friends!" << endl;
}

void show_friends(string name, Node<User>*& u_root){
    Node<User>* user = find(u_root, name);
    if (user == NULL){
        cout << "Error: " + name + " is not an active user" << endl;
        return;
    }
    user->data->show_friends();
}

void break_friendship(string name1, string name2, Node<User>*& u_root){
    Node<User>* temp1 = find(u_root, name1);
    Node<User>* temp2 = find(u_root, name2);
    if (temp1 == NULL){
        cout << "Error: " + name1 + " is not an active user" << endl;
        return;
    }
    if (temp2 == NULL){
        cout << "Error: " + name2 + " is not an active user" << endl;
        return;
    }
    if (name1 == PRIMARY_USERNAME){
        name1 = "You";
    }
    User* u1 = temp1->data;
    User* u2 = temp2->data;

    if (!u1->areFriends(u2)){
        cout << name1 + " and " + name2 + " are already not friends!" << endl;
        return;
    }
    u1->unfriend(u2->user_name);
    u2->unfriend(u1->user_name);
    cout << name1 + " and " + name2 + " are no longer friends!" << endl;
}

bool isInEFN(string name, User primary_user, Node<User>* u_root, const BST<User>& users_bst, int radius){
    Node<User>* temp = find(u_root, name);
    if (temp == NULL){
        cout << "Error: " + name + " is not an active user" << endl;
        return false;
    }
    User* user = temp->data;

    //BEGIN BFS
    int level = 0;
    User flag("FLAG");
    vector<bool> visited(users_bst.size, false);
    queue<User> Q;
    Q.push(primary_user);
    Q.push(flag);
    visited[primary_user.user_index] = true;
    while((!Q.empty()) && level < radius){
        User v = Q.front();
        Q.pop();
        if(v.getTitle() == flag.getTitle()){
            level++;
            Q.push(flag);
            if(Q.front().getTitle() == flag.getTitle())
                return false;
            else
                continue;
        }
        for (User* u  : v.friends){
            if (!visited[u->user_index]) {
                Q.push(*u);
                visited[u->user_index] = true;
            }
        }
    } //END BFS

    if (visited[user->user_index]){
        return true;
    }
    return false;
}

void recommend(int k, Library& library, Heap* heap){
    if (heap->getCurrentSize() < k){
        cout << "Error: Not enough songs in system to recommend " + to_string(k) + " songs!" << endl;
        cout << "Songs in system: " + to_string(heap->getCurrentSize()) << endl;
        return;
    }
    if (heap->peek()->getPlays() == 0){
        cout << "No songs with listens, 0 songs added." << endl;
        return;
    }
    for (int i = 0; i < k; i++){
        if (heap->peek()->getPlays() == 0){
            cout << "Added " + to_string(i) + " songs. Songs with no listens not added." << endl;
            return;
        }
        library.add_song(heap->extractMax());
    }
    if (k == 1)
        cout << "Added 1 song to your library!" << endl;
    else
        cout << "Added " + to_string(k) + " songs to your library!" << endl;
}

void add_user(string name, Node<User>*& u_root, BST<User>& users_BST){
    Node<User>* temp = find(u_root, name);
    if (temp != NULL){
        cout << "Error: The username " + name + " has already been taken!" << endl;
        return;
    }
    User* user = new User(name);
    users_BST.insert(u_root, user);
    cout << "Added user " + name + " to the system!" << endl;
}