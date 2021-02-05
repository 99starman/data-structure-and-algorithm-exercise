/**
 * @file main.cpp
 * A simple C++ program that manipulates an image.
 *
 * @author CS 221: Basic Algorithms and Data Structures
**/

#include "chain.h"
#include "block.h"
#include <iostream>
#include "cs221util/PNG.h"
using namespace cs221util;
using namespace std;

void TestFlip()
{
   PNG ht;
   ht.readFromFile("images/rbgsmall.png");
   Block b;
   b.build(ht, 0,0, 60, 60);
    cout << "got here" << endl;
   b.flip();
    cout << "got here" << endl;

   PNG ht_p = ht;
   b.render(ht_p, 0,0);
    cout << "got here" << endl;
   ht_p.writeToFile("images/hyperthink_p.png");
}
int main()
{
   /**
    * Write your own functions to print out variants of different images!
    * You'll find the outputs in the "images" folder.
    * A sample function has been written for you.
    */
   TestFlip();
   return 0;
}
