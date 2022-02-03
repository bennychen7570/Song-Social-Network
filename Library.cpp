#include "Library.h"

Library::Library(){
    root = NULL;
}

void Library::add_song(Song* song){
    songs.insert(root, song);
}

bool Library::remove_song(string title){
    Node<Song>* temp = find(root, title);
    if (temp == NULL){
        cout << "Error: " + title + " is already not in your library!" << endl;
        return false;
    }

    root = remove(title,root);
    songs.size--;
    if (songs.size <= 0)
        root = nullptr;
    cout << "Successfully removed " + title + " from your library!" << endl;
    return true;
}

void Library::show_songs(){
    if(songs.size <= 0)
        cout << "No songs to show!" << endl;
    else
        songs.show(root);
}

Node<Song>* Library::find(Node<Song>*& node, string title) {
    if (node == NULL || title == node->data->getTitle()){
        return node;
    }
    if (title > node->data->getTitle()){
        return find(node->right, title);
    }
    return find(node->left, title);
}

Node<Song> * Library::remove(string x, Node<Song>* t){
    Node<Song>* temp;
    if (t == nullptr){
        return nullptr;
    }
    else if (x < t->data->getTitle()) {
        t->left = remove(x, t->left);
    }
    else if (x > t->data->getTitle()) {
        t->right = remove(x, t->right);
    }
    else if (t->left != nullptr && t->right != nullptr){
        temp = findMin(t->right);
        t->data = temp->data;
        t->right = remove(t->data->getTitle(),t->right);
    }
    else{
        temp = t;
        if (t->left == nullptr)
            t = t->right;
        else if (t->right == nullptr)
            t = t->left;
        delete temp;
    }
    return t;
}

Node<Song> * Library::findMin(Node<Song> * n) {
    if (n == nullptr)
        return nullptr;
    else if (n->left == nullptr)
        return n;
    else
        return findMin(n-> left);
}