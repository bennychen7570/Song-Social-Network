#ifndef HEAP
#define HEAP
#include <assert.h>
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

class Heap{
private:
    vector<Song*> heapVector;
    int currentSize;

    /*
    * Return Left Child
    */
    int left(int parent){
        int l = 2 * parent + 1;
        if(l < heapVector.size())
            return l;
        else
            return -1;
    }

    /*
    * Return Right Child
    */
    int right(int parent){
        int r = 2 * parent + 2;
        if(r < heapVector.size())
            return r;
        else
            return -1;
    }

    /*
    * Return Parent
    */
    int parent(int child){
        int p = (child - 1)/2;
        if(child == 0)
            return -1;
        else
            return p;
    }

public:
    Heap() : currentSize(0) {

    }

    Heap(vector<Song*> heapVector) : currentSize(0) {
        this->heapVector = heapVector;
        this->currentSize = 0;
    }

    void insert(Song* x){
        heapVector.push_back(x);
        (*x).setIndex(currentSize);
        currentSize++;
    }

    void increaseKey(int i, Song* key){
        assert(currentSize != 0);
        if ((*key).getPlays() < (*heapVector[i]).getPlays()){ //check if already smaller
            cout << "Could not increase key" << endl;
            return ;
        }
        heapVector[i] = key;
        while (i > 0 && heapVector[parent(i)]->getPlays() < heapVector[i]->getPlays()){
            swap(heapVector[i],heapVector[parent(i)]);

            //swap song indexes
            int temp = heapVector[i]->getIndex();
            heapVector[i]->setIndex(heapVector[parent(i)]->getIndex());
            heapVector[parent(i)]->setIndex(temp);

            i = parent(i);
        }

    }

    Song* extractMax(){
        assert(currentSize != 0);

        Song* max = heapVector[0];

        int i = 0;
        while (left(i) != -1 && right(i) != -1){
            if (left(i) == -1){
                //swap song indexes
                heapVector[i]->setIndex(right(i));
                heapVector[right(i)]->setIndex(i);

                swap(heapVector[i],heapVector[right(i)]);
                i = right(i);
            }
            else if (right(i) == -1){
                //swap song indexes
                heapVector[i]->setIndex(left(i));
                heapVector[left(i)]->setIndex(i);
                swap(heapVector[i],heapVector[left(i)]);
                i = left(i);
            }
            else{
                if (heapVector[left(i)]->getPlays() > heapVector[right(i)]->getPlays()){
                    heapVector[i]->setIndex(left(i));
                    heapVector[left(i)]->setIndex(i);
                    swap(heapVector[i],heapVector[left(i)]);
                    i = left(i);
                }
                else{
                    heapVector[i]->setIndex(right(i));
                    heapVector[right(i)]->setIndex(i);
                    swap(heapVector[i],heapVector[right(i)]);
                    i = right(i);
                }
            }
        }

        heapVector[heapVector.size()-1]->setIndex(i);
        heapVector[i]->setIndex(heapVector[heapVector.size()-1]->getIndex());
        swap(heapVector[i],heapVector[heapVector.size()-1]);
        currentSize--;
        heapVector.pop_back();
        return heapVector[heapVector.size()];
    }

    void displayHeap(){
        typename vector<Song*>::iterator pos = heapVector.begin();
        cout << "Number of songs in the system: " + to_string(currentSize) + "\n";
        while (pos != heapVector.end())
        {
            cout<< (**pos) <<"\n";
            pos++;
        }
        cout<<endl;
    }

    int getCurrentSize() const {
        return currentSize;
    }

    Song* peek(){
        return heapVector[0];
    }

};
#endif