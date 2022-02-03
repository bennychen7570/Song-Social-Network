#ifndef NODE_H
#define NODE_H
#include <iostream>

template <typename T>
class Node{
 public:
  T* data;
  Node* left;
  Node* right;

  Node();
  Node(T*);
};

template<typename T>
Node<T>::Node(){
    data = nullptr;
    left = nullptr;
    right = nullptr;
}

template <typename T>
Node<T>::Node(T* s){
    data = s;
    left = nullptr;
    right = nullptr;
}

#endif