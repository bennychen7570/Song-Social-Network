#ifndef BST_H
#define BST_H
#include "Node.h"

template <typename T>
class BST{
public:
    Node<T>* root;
    int size;

    BST();
    void insert(Node<T> *&, T*);
    void show(Node<T>*);
    ~BST();
};

using namespace std;

template <typename T>
BST<T>::BST(){
    root = nullptr;
    size = 0;
}

template <typename T>
void BST<T>::insert(Node<T>*& root, T* s){
    if(root == NULL){
        root = new Node<T>(s);
        size++;
        return;
    }
    if(*s < *(root->data)){
        insert(root->left, s);
    }
    else if(*s > *(root->data)){
        insert(root->right, s);
    }
}

template <typename T>
void BST<T>::show(Node<T>* node){
    if(node == nullptr){
        return;
    }
    show(node->left);
    cout << *(node->data) << endl;
    show(node->right);
}

template <typename T>
BST<T>::~BST(){
    delete root;
}
#endif