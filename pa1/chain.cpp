#include "chain.h"
#include "chain_given.cpp"
#include <cmath>
#include <iostream>

// PA1 functions

/**
 * Destroys the current Chain. This function should ensure that
 * memory does not leak on destruction of a chain.
 */
Chain::~Chain()
{
/* your code here */
   clear();
   head_ = NULL;
   length_ = 0;
   height_ = 0;
   width_ = 0;
}

/**
 * Inserts a new node at the back of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
void Chain::insertBack(const Block &ndata)
{
/* your code here */
   Node* newNode = new Node(ndata);
   if (empty()) {
       head_->next = newNode;
       newNode->next = head_;
       length_ += 1;
       return;
   }   
   walk(head_, length_)->next = newNode;
   newNode->next = head_;
   length_ += 1;
}


/**
 * Swaps the two nodes at the indexes "node1" and "node2".
 * The indexes of the nodes are 1 based.
 * assumes i and j are valid (in {1,length_} inclusive)
 * 
 */
void Chain::swap(int i, int j)
{
/* your code here */
   Node* prev1 = walk(head_, i-1);
   Node* prev2 = walk(head_, j-1);
   Node* node1 = prev1->next;
   Node* node2 = prev2->next;
   Node* next1 = node1->next;
   Node* next2 = node2->next;
   if (j-i > 1) {
       prev1->next = node2;
       node2->next = next1;
       prev2->next = node1;
       node1->next = next2;
   } else {
       prev1->next = node2;
       node2->next = node1;
       node1->next = next2;
   }
}

/**
 * Reverses the chain
 */
void Chain::reverse()
{
/* your code here */
   if (empty()) return;
   Node* prev = head_;
   Node* curr = head_->next;
   while (curr != head_) {
       Node* nextTemp = curr->next;
       curr->next = prev;
       prev = curr;
       curr = nextTemp;
   }
   head_->next = prev;
}

/*
* Modifies the current chain by "rotating" every group of k nodes by one position.
* In every k-node sub-chain, remove the first node, and place it in the last 
* position of the sub-chain. If the last sub-chain has length less than k,
* then don't change it at all. 
* Some examples with the chain a b c d e:
*   k = 1: a b c d e
*   k = 2: b a d c e
*   k = 3: b c a d e
*   k = 4: b c d a e
*/
void Chain::rotate(int k)
{
/* your code here */
   if (k == 1) return;
   int num = size() / k;
   for (int i = 0; i < num; i++) {
       int sta = i*k+1;
       int end = i*k+k;
       while (end - sta > 0) {
           swap(sta, end);
           end -= 1;
       }

   }
}

/**
 * Destroys all dynamically allocated memory associated with the
 * current Chain class.
 */
void Chain::clear()
{
/* your code here */
   if (head_ == NULL) return;
   Node* curr = head_->next;
   while (curr != head_) {
       Node* temp = curr;
       curr = curr->next;
       delete temp;
    }
    delete head_;
    head_ = NULL;
}

/* makes the current object into a copy of the parameter:
 * All member variables should have the same value as
 * those of other, but the memory should be completely
 * independent. This function is used in both the copy
 * constructor and the assignment operator for Chains.
 */
void Chain::copy(Chain const &other)
{
/* your code here */
   height_ = other.height_;
   width_ = other.width_;
   length_ = other.length_;
   head_ = new Node();
   head_->next = head_;
   Node* curr = head_;
   Node* otherCurr = other.head_;
   while (otherCurr->next != other.head_) {
       otherCurr = otherCurr->next;
       Node* newNode = new Node(otherCurr->data);
       curr->next = newNode;
       newNode->next = head_;
       curr = curr->next;      
   }

}
