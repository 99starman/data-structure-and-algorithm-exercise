#include "block.h"
#include <cmath>
#include <iostream>
using namespace std;

int Block::width() const
{
/* your code here */
   if (data.size() == 0) {
       return 0;
   } else {
       return data[0].size();
   }

}

int Block::height() const
{
/* your code here */
   return data.size();
}

void Block::render(PNG &im, int column, int row) const
{
/* your code here */
   int col_max;
   int row_max;
   if (column + width() <  (signed)im.width()) {
       col_max = column + width();
   } else {
       col_max = (signed) im.width();
   }

   if (row + height() <  (signed)im.height()) {
       row_max = row + height();
   } else {
       row_max = (signed) im.height();
   }   
      
   for (int x = column; x < col_max; x++) { 
       for (int y = row; y < row_max; y++) { 
           *(im.getPixel(x, y)) = data[y-row][x-column];
           
       }
   } 

}

void Block::build(PNG &im, int column, int row, int width, int height)
{
/* your code here */
   for (int x = row; x < row + height; x++) {
       vector<HSLAPixel> temp;      
       for (int y = column; y < column + width; y++) {
           HSLAPixel* pixel = im.getPixel(y, x);  
           temp.push_back(*pixel);          
       }
       data.push_back(temp);
   }
}

void Block::flip()
{
/* your code here */
    for (int i = 0; i < (int)data.size()/2; i++) {        
        data[i].swap(data[height()-i-1]);
    }


}
