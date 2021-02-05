
#ifndef BLOCK_H
#define BLOCK_H

#include <vector>
#include <iostream>
#include "cs221util/PNG.h"
#include "cs221util/HSLAPixel.h"
using namespace std;
using namespace cs221util;

class Block
{

public:
   /** 
    * Creates a block that is width x height pixels in size
    * starting at (x, y) on img. Assumes that the requested
    * block fits on the image. (inputs are feasible)
    */
   void build(PNG &img, int x, int y, int width, int height);

   /** 
    * Renders the given block onto img with its upper
    * left corner at (x, y). If the block doesn't fit onto 
    * the image, simply truncate it to fit!
    */
   void render(PNG &img, int x, int y) const;

   /**
    * Turns the block upside down by a simple reflection
    * across the horizontal axis through its middle.
    */
   void flip();

   // Return the width and height of the block.
   int width() const;
   int height() const;

private:
   vector<vector<HSLAPixel>> data;
};
#endif
