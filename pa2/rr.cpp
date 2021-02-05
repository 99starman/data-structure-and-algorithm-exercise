/**
 * @file rr.cpp
 * Implementation of rainbowRipple.
 
 *
 */
#include "rr.h"

// This shows all the function declarations (and some of the
// functions) that I used to implement rainbowRipple.
// You can create additional functions and/or delete/replace functions
// as you wish.

void ERROR_badGaps(string sgc) {
// Report error and exit.

//
  cerr << "ERROR: Unable to parse string specifying gaps and colors: "
       << "\"" << sgc << "\""
       << endl;
  exit(-1);
}

vector<pair<int,RGBAPixel>> parseGaps(string sgc) {
// Convert a string containing a sequence of number,character pairs,
// where number is an integer >= 0 and character is one of roygbiv,
// into a vector<pair<int,RGBAPixel>> of the sequence of pairs except
// the characters are replaced by the corresponding RGBAPixel.
// For example, the string "0r10g7b" converts to the vector
// [(0,RGBAPixel(255,0,0)),(10,RGBAPixel(0,255,0)),(7,RGBAPixel(0,0,255))]

//
  vector<pair<int,RGBAPixel>> vgc;
  RGBAPixel c;
  size_t idx = 0;
  size_t i = 0;
  int g;
  while( idx < sgc.size() ) {
    try {
      g = stoi(sgc.substr(idx), &i);
    } catch( invalid_argument& ia ) {
      ERROR_badGaps(sgc);
    }
    idx += i;
    if( g < 0 || idx >= sgc.size() ) ERROR_badGaps(sgc);
    switch( sgc[idx] ) {
    case 'R':
    case 'r':
      c =  RGBAPixel(255,0,0);	//red
      break;
    case 'O':
    case 'o':
      c = RGBAPixel(255,165,0);	//orange
      break;
    case 'Y':
    case 'y':
      c = RGBAPixel(255,255,0);	//yellow
      break;
    case 'G':
    case 'g':
      c = RGBAPixel(0,255,0);	//green
      break;
    case 'B':
    case 'b':
      c = RGBAPixel(0,0,255);	//blue
      break;
    case 'I':
    case 'i':
      c = RGBAPixel(75,0,130);	//indigo
      break;
    case 'V':
    case 'v':
      c = RGBAPixel(238,130,238);//violet
      break;
    default:
      ERROR_badGaps(sgc);
    }
    vgc.push_back(make_pair(g,c));
    idx++;
  }
  return vgc;
}

bool closeEnough(RGBAPixel c1, RGBAPixel c2) {
// Returns true iff color c1 is close enough to color c2

//
  int dist = (c1.r-c2.r)*(c1.r-c2.r) + (c1.g-c2.g)*(c1.g-c2.g) +
    (c1.b-c2.b)*(c1.b-c2.b);

  return (dist <= 80);
}

bool good(PNG & image, vector<vector<int>> & D,
		pair<int,int> curr, pair<int,int> next) {
// Returns true iff a neighbor "next" of a pixel "curr" is:
// 1. within the image,
// 2. unvisited, and
// 3. close in color to "curr".
// An entry in distance table D is -1 only if the pixel has not been 
// visited.

  // @todo Replace the following line with your code.
  
  if (next.second >= (int) D.size() || next.first >= (int) D[0].size()) {    
    return false;
  } 
  if (next.first < 0 || next.second < 0) {
    return false;
  }   
  if (D[next.second][next.first] != -1) return false;   
  RGBAPixel * pixel1 = image.getPixel(next.first, next.second);    
  RGBAPixel * pixel2 = image.getPixel(curr.first, curr.second);
  if (!closeEnough(*pixel1, *pixel2)) return false;
  return true;
}

vector<pair<int,int>> neighbors(pair<int,int> curr) {

  vector<pair<int,int>> n;

  // @todo Add your code here.
  int x = curr.first;
  int y = curr.second;
  
  n.push_back(make_pair(x-1,y));
  n.push_back(make_pair(x, y+1));
  n.push_back(make_pair(x+1, y));
  n.push_back(make_pair(x, y-1));

  return n;
}


void rainbowRipple(PNG & image, pair<int,int> start, string sgc) {
// Color certain pixels in "image" based on their distance from "start"
// using the gaps-and-colors string "sgc" to specify the color pattern.
// The distance is determined by Breadth-First-Search which is a
// Level-Order Traversal of the tree of shortest paths from "start"
// whose edges are between good neighbors.  The neighbors of pixel
// (x,y) are (x-1,y), (x,y+1), (x+1,y), (x,y-1).  The good neighbors
// of (x,y) are those neighbors that
// 1. lie within the image,
// 2. are close to the color of pixel (x,y), and
// 3. have not been visited before.
// BFS visits "start", then the good neighbors of "start", then the
// good neighbors of these good neighbors, etc.  This forms a tree,
// level by level. The distance of a pixel from "start" is its level
// in this tree (or -1 if it hasn't been visited).  As soon as BFS
// visits a pixel it knows its distance from "start" and it can color
// the pixel based on the gaps-and-colors string "sgc".  (* However,
// it should wait to color the pixel until after its good neighbors
// are computed, since the original pixel color determines what
// neighbors are good. *)
// Coloring:
// Suppose (gap_1,color_1), (gap_2,color_2), ..., (gap_n,color_n) is the
// sequence of number,color pairs in "sgc".  Those pixels at distance
// gap_1 from "start" are colored color_1, those at distamce
// gap_1 + 1 + gap_2 are colored color_2, etc.  The gaps are the
// number of uncolored pixels between colored pixels along a shortest
// path.  The sequence specified by "sgc" should be viewed as
// repeating forever, so a pixel at distance 33 from "start" would be
// colored blue using the sequence (9,red),(4,blue),(3,green) since
// 33 = (9+1+4+1+3+1)+9+1+4.

  // @todo Add your code here.
  Queue<pair<int,int>> q;
  vector<vector<int>> D(image.height(), vector<int>(image.width(), -1));
  vector<pair<int,RGBAPixel>> seq = parseGaps(sgc);

  q.enq(start);
  int d = -1;
  int d_prev = 0;
  D[start.second][start.first] = d;
  int colorIndex = 0;
  int colorCurr = 0;

  RGBAPixel color;

  int sum = (int) seq[0].first;

  for (int i = 1; i < (int) seq.size(); i++) {
    sum = sum + 1 + seq[i].first;
    seq[i].first = sum;
  }
  sum ++;

  while (!q.empty()) { 

    pair<int,int> s = q.deq();

    vector<pair<int,int>> n = neighbors(s);
    d = 1 + D[s.second][s.first];
    
    for (int i = 0; i < 4; i++) {
      if (good(image, D, s, n[i])) {         
        q.enq(n[i]);
        D[n[i].second][n[i].first] = d;
      }
    }

    // coloring
    

    int mod = d % sum;

    for (int j = 0; j < (int) seq.size(); j++) {
      if (mod == seq[j].first) {
        color = seq[j].second;
        *image.getPixel(s.first, s.second) = color;        
        break;
      }
    }      
    

 
      
  }  


}
