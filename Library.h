#include <iostream>
#include <vector>
#include "Song.h"
#include "BST.h"

class Library{
    public:
        Library();
        BST<Song> songs;
        Node<Song>* root;
        Node<Song>* find(Node<Song>*& node, string title);
        void add_song(Song*);
        bool remove_song(string);
        void show_songs();

        Node<Song> * remove(string x, Node<Song>* t);
        Node<Song> * findMin(Node<Song> * n);
};