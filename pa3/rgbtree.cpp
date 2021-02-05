/**
 * @file rgbtree.cpp
 * Implementation of rgbtree class.
 */

#include <utility>
#include <algorithm>
#include "rgbtree.h"

using namespace std;

rgbtree::rgbtree(const map<RGBAPixel,string>& photos)
{
  int size = photos.size();
  if (size == 0) return;
  for (auto const& element : photos) {
    tree.push_back(element.first);
  }
  rgbtreeHelper(0, size-1, 0);
 

}

void rgbtree::rgbtreeHelper(int start, int end, int d) {
  if (start == end) return;
  int k = floor((start+end)/2);
  quickSelect(start, end, k, d);
  if (start < k) {
    rgbtreeHelper(start, k-1, (d+1)%3);    
  }
  if (end > k) {
    rgbtreeHelper(k+1, end, (d+1)%3);
  }

}

RGBAPixel rgbtree::findNearestNeighbor(const RGBAPixel & query) const
{
  RGBAPixel rootMin = tree[(floor(tree.size())-1)/2];
  return findNNhelper(query, 0, tree.size()-1, 0, rootMin);
}


RGBAPixel rgbtree::findNNhelper(const RGBAPixel& query, int start, int end, int dim, RGBAPixel& bestPixel) const {
  if (start == end) {
    return tree[end];
  }                                   // base case
  int mid = floor((start+end)/2);
  int d0 = distToPoint(query, bestPixel);     // best distance so far
  RGBAPixel rootMin = tree[mid];
  int d1 = distToPoint(query, rootMin);       // distance from root to query
  if (d1 < d0) {
    bestPixel = rootMin;                      // if < d0, update    
  }
  if (smallerByDim(query, rootMin, dim)) {    // if query is in left subtree
    RGBAPixel inMin = findNNhelper(query, start, mid-1, (1+dim)%3, bestPixel);    // candidate in the same subtree
    int d2 = distToPoint(query, inMin);
    if (d2 < distToPoint(query, bestPixel)) {
      bestPixel = inMin;                                          // if inMin is closer, update
    }
    int d3 = distToSplit(query, rootMin, dim);                    // distance to split plane 

    if (d3 < distToPoint(query, bestPixel)) {                     // if need to check the other side
      RGBAPixel outMin = findNNhelper(query, mid+1, end, (1+dim)%3, bestPixel);   // find candidate from the other side (right subtree)
      if (distToPoint(query, outMin) < distToPoint(query, bestPixel)) {
        bestPixel = outMin;                                                  // if outMin is closer, update
      }
    }
  } else {
    RGBAPixel inMin = findNNhelper(query, mid+1, end, (1+dim)%3, bestPixel);    // candidate in the same subtree (right)
    int d2 = distToPoint(query, inMin);
    if (d2 < distToPoint(query, bestPixel)) {
      bestPixel = inMin;                                          // if inMin is closer, update
    }
    int d3 = distToSplit(query, rootMin, dim);                    // distance to split plane 

    if (d3 < distToPoint(query, bestPixel)) {                     // if need to check the other side
      RGBAPixel outMin = findNNhelper(query, start, mid-1, (1+dim)%3, bestPixel);   // find candidate from the other side (left subtree)
      if (distToPoint(query, outMin) < distToPoint(query, bestPixel)) {
        bestPixel = outMin;                                                  // if outMin is closer, update
      }
    }
  }
  return bestPixel;





  /* if (start == end) return tree[1];
  int mid = floor((start+end)/2);
  //cout << "find nn line45" << endl;
  RGBAPixel rootMin = tree[mid];                             // median as the root
  //cout << "find nn line47" << endl;
                           
  //cout << "find nn line49" << endl;
  int d1 = distToPoint(query, rootMin);                     // d1: distance from query to root
  if (d1 == 0) return rootMin;
  //cout << "find nn line52" << endl;
  if (smallerByDim(query, rootMin, dim)) {     // if query in left subtree
                     
    RGBAPixel inMin = findNNhelper(query, start, mid-1, (1+dim)%3, bestPixel);   // find inMin, the candidate in the same subtree (left)
    int d2 = distToPoint(query, inMin);                               //        and its distance from query, d2
    int d3 = distToSplit(query, rootMin, dim);                        // distance to split plane
    int d = min(d1, d2);
    
    if (d > d3) {                                          // need to consider outMin
      cout << "find nn line61" << endl;
      RGBAPixel outMin = findNNhelper(query, mid+1, end, (1+dim)%3, bestPixel); // find outMin from the other side (right)
      if (distToPoint(query, outMin) < d) {
        return outMin;                                               // outMin is the nearest
      } else {
        if (d == d1) {                                               // if d1 <= d2, rootMin is the nearest
        return rootMin;
      } else {
        return inMin;
      }
      }

    } else {
      if (d == d1) {
        return rootMin;
      } else {
        return inMin;
      }
    }

  } else {                                 // if query in right subtree

    RGBAPixel inMin = findNNhelper(query, mid+1, end, (1+dim)%3, bestPixel);
    int d2 = distToPoint(query, inMin);
    int d3 = distToSplit(query, rootMin, dim);
    int d = min(d1, d2);
    
    if (d > d3) {
      cout << "find nn line89" << endl;
      RGBAPixel outMin = findNNhelper(query, start, mid-1, (1+dim)%3, bestPixel);
      if (distToPoint(query, outMin) < d) {
        return outMin;
      } else {
        if (d == d1) {
        return rootMin;
      } else {
        return inMin;
      }
      }

    } else {
      if (d == d1) {
        return rootMin;
      } else {
        return inMin;
      }
    }
  }
  cout << "find nn end" << endl; */
  


}

int rgbtree::distToPoint(const RGBAPixel& first, const RGBAPixel& second) const {
  return pow(first.r - second.r, 2) + pow(first.g - second.g, 2) + pow(first.b - second.b, 2);
}



bool rgbtree::smallerByDim(const RGBAPixel & first,
                                const RGBAPixel & second, int curDim) const
{
  if (curDim == 0) {
    if (first.r <= second.r) {
      return true;
    } else return false;
  }
  if (curDim == 1) {
    if (first.g <= second.g) {
      return true;
    } else return false;
  }
  else {
    if (first.b <= second.b) {
      return true;
    } else return false;
  }
 

}

/**
 * This function splits the trees[start..end] subarray at position start k
 */
void rgbtree::quickSelect(int start, int end, int k, int d)
{
  if (start >= end) return;
  int p = partition(start, end, d);
  if (k < p) {
    quickSelect(start, p-1, k, d);
  } else if (k > p) {
    quickSelect(p+1, end, k, d);
  } else {
    return;
  }


}


/**
 * This method does a partition around pivot and will be used 
 * in quick select. It uses tree[lo] as the default pivot.
 * It returns the index of the pivot in the updated vector.
 * You will likely need to modify and complete this code.
 */
int rgbtree::partition(int lo, int hi, int d) 
{   

    int p = lo;

    if (d == 0) {
     for( int i=lo+1; i <= hi; i++ )
      if( (tree[i]).r < (tree[lo]).r ) { p++; swap(tree[p], tree[i]); }
    swap(tree[lo], tree[p]); 
    }

    if (d == 1) {
     for( int i=lo+1; i <= hi; i++ )
      if( (tree[i]).g < (tree[lo]).g ) { p++; swap(tree[p], tree[i]); }
    swap(tree[lo], tree[p]); 
    }

    if (d == 2) {
     for( int i=lo+1; i <= hi; i++ )
      if( (tree[i]).b < (tree[lo]).b ) { p++; swap(tree[p], tree[i]); }
    swap(tree[lo], tree[p]); 
    }
    
    return p;

}


/**
 * Helper function to help determine if the nearest neighbor could 
 * be on the other side of the KD tree.
 */
int rgbtree::distToSplit(const RGBAPixel& query, const RGBAPixel& curr, int dimension) const
{
  if (dimension == 0) {
    return pow(query.r - curr.r, 2);
  } 
  if (dimension == 1) {
    return pow(query.g - curr.g, 2);
  } 
  else {
    return pow(query.b - curr.b, 2);
  } 

}

