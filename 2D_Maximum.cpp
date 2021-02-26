// Finding a local maximum element in a n×n two-dimensional array M filled with intergers
// Uses divide and conquer ("Prune and Search"), and takes O(n) time, instead of the common O(nlogn) or O(n^2) time.
// (A local maximum is one with value larger than or eaqual to its neighbours to the left, right, top and bottom)
// Implementation of CPSC 320 assignment 2, spring 2021. Reference: https://courses.csail.mit.edu/6.006/spring11/rec/rec02.pdf

#include <stdio.h>
#include <iostream>
#include<stdlib.h>
using namespace std; 
#include <tuple>
#include <vector>
#include <iterator>


// helper function to search maximum element in a single row r between column c0 and c1 (inclusive), returns <max value, row position, column position>
tuple <int, int, int> searchRowHelper(int **M, int r, int c0, int c1) {
    int r_maxVal = M[r][c0]; 
    int r_maxPos = c0;
    for (int c = c0 + 1; c < c1 + 1; c++) {
        if (M[r][c] > r_maxVal) {  // update maximum and its position
          r_maxVal = M[r][c]; 
          r_maxPos = c;
        } 
    }
    return make_tuple(r_maxVal, r, r_maxPos);    
}

// helper function to search maximum element in a single column c between row r0 and r1 (inclusive), returns <max value, row position, column position>
tuple <int, int, int> searchColHelper(int **M, int c, int r0, int r1) {
    int c_maxVal = M[c][r0]; 
    int c_maxPos = r0;
    for (int r = r0 + 1; r < r1 + 1; r++) {
        if (M[r][c] > c_maxVal) {  // update maximum and its position
          c_maxVal = M[r][c]; 
          c_maxPos = r;
        } 
    }
    return make_tuple(c_maxVal, c_maxPos, c);    
}


// helper function to find the maximum in the "window frame" of a matrix with row range [r0, r1] and column range [c0, c1], 
//                 returns <max value, row position, column position>
tuple <int, int, int> findMaxInFrame(int **M, int r0, int r1, int c0, int c1) {
    vector <int> frame;
   int currMax, rowPos, colPos;
   int r_mid = (r0 + r1) / 2;
   int c_mid = (c0 + c1) / 2;

   tuple <int, int, int> curr1 = searchRowHelper(M, r0, c0, c1);            // maximum of the first row
   tuple <int, int, int> curr2 = searchRowHelper(M, r_mid, c0, c1);         // maximum of the middle row
   tuple <int, int, int> curr3 = searchRowHelper(M, r1, c0, c1);            // maximum of the last row

   tuple <int, int, int> curr4 = searchColHelper(M, c0, r0, r1);            // maximum of the first column
   tuple <int, int, int> curr5 = searchColHelper(M, c_mid, r0, r1);         // maximum of the middle column
   tuple <int, int, int> curr6 = searchColHelper(M, c1, r0, r1);            // maximum of the last column

   vector<tuple<int, int, int>> tup;
   tup.insert(tup.end(), {curr1, curr2, curr3, curr4, curr5, curr6});
   vector<int> vec;
   for (tuple<int, int, int>& t : tup) {
       tie(currMax, rowPos, colPos) = t;
       vec.push_back(currMax);
       vec.push_back(rowPos);
       vec.push_back(colPos);
   }

   for (int i = 0; i < (signed) vec.size(); i = i+3) {
       if (vec[i] > currMax) {
           currMax = vec[i];
           rowPos = vec[i+1];
           colPos = vec[i+2];
       }
   }
   return make_tuple(currMax, rowPos, colPos);     
      
}


// function to find a local maximum of a matrix with row range [r0, r1] and column range [c0, c1]
int findMax(int **M, int r0, int r1, int c0, int c1) {
    int g_val, g_row, g_col;
    tuple <int, int, int> g = findMaxInFrame(M, r0, r1, c0 ,c1);
    std::tie(g_val, g_row, g_col) = g;
    if (r1 - r0 < 3) {
        return g_val;
    }
    if (g_row == r0) {
        if (g_val >= M[g_row+1][g_col]) {
            return g_val;            
        } else {                // recurse onto a sub matrix
            if (g_col <= (c0+c1)/2) {
                findMax(M, r0, (r0+r1)/2, c0, (c0+c1)/2); 
            } else {
                findMax(M, r0, (r0+r1)/2, (c0+c1)/2, c1);
            }
        } 
    }
    else if (g_row == r1) {
        if (g_val >= M[g_row-1][g_col]) {
            return g_val;            
        } else {                // recurse onto a sub matrix
            if (g_col <= (c0+c1)/2) {
                findMax(M, (r0+r1)/2, r1, c0, (c0+c1)/2); 
            } else {
                findMax(M, (r0+r1)/2, r1, (c0+c1)/2, c1);
            }
        } 
    }
    else if (g_col == c0) {
        if (g_val >= M[g_row][g_col+1]) {
            return g_val;            
        } else {                // recurse onto a sub matrix
            if (g_row <= (r0+r1)/2) {
                findMax(M, r0, (r0+r1)/2, c0, (c0+c1)/2); 
            } else {
                findMax(M, (r0+r1)/2, r1, c0, (c0+c1)/2);
            }
        } 
    }
    else if (g_col == c1) {
        if (g_val >= M[g_row][g_col-1]) {
            return g_val;            
        } else {                // recurse onto a sub matrix
            if (g_row <= (r0+r1)/2) {
                findMax(M, r0, (r0+r1)/2, (c0+c1)/2, c1); 
            } else {
                findMax(M, (r0+r1)/2, r1, (c0+c1)/2, c1);
            }
        } 
    }
    else if (g_row == (r0+r1)/2) {
        int up = M[g_row-1][g_col];
        int down = M[g_row+1][g_col];
        if (g_val >= up && g_val >= down) {
            return g_val;
        } else {   // determine which one of the four sub matrices to recurse on
            if (up >= down) {
                if (g_col < (c0+c1)/2) {
                    findMax(M, r0, (r0+r1)/2, c0, (c0+c1)/2);
                } else {
                    findMax(M, r0, (r0+r1)/2, (c0+c1)/2, c1);
                }                
            } else {      // the element below g is greater, recurse to the sub matrix below                
                if (g_col < (c0+c1)/2) {
                    findMax(M, (r0+r1)/2, r1, c0, (c0+c1)/2);
                } else {
                    findMax(M, (r0+r1)/2, r1, (c0+c1)/2, c1);
                }            
            }
        }
    } else {              // g_col = (c0+c1)/2
        int left = M[g_row][g_col-1];
        int right = M[g_row][g_col+1];
        if (g_val >= left && g_val >= right) {
            return g_val;
        } else {   // determine which one of the four sub matrices to recurse on
            if (left >= right) {
                if (g_row < (r0+r1)/2) {
                    findMax(M, r0, (r0+r1)/2, c0, (c0+c1)/2);
                } else {
                    findMax(M, (r0+r1)/2, r1, c0, (c0+c1)/2);
                }                
            } else {      // the element to the right of g is greater, recurse to the right sub matrix               
                if (g_row < (r0+r1)/2) {
                    findMax(M, r0, (r0+r1)/2, (c0+c1)/2, c1);
                } else {
                    findMax(M, (r0+r1)/2, r1, (c0+c1)/2, c1);
                }            
            }
        }
    }
    return 0;     
}

// Input stream: an int indicating n, then followed by a n by n matrix
int main() 
{ 
    int n;
    std::cin >> n;    
    int** M = new int*[n];               // dynamic allocation
    for(int i = 0; i < n; i++){
		M[i] = new int [n];
	}

    for(int i = 0; i < n; i++){	
		for(int j = 0; j < n; j++){            
			std::cin >> M[i][j];			
		}
	}

    std::cout << findMax(M, 0, n-1, 0, n-1) << endl;

    for(int i = 0; i < n; i++) {        // clean up
    delete [] M[i];
    }
    delete [] M;

    return 0;
} 


