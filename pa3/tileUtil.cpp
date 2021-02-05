
// File:        tileUtil.cpp
// Author:      Cinda
// Date:        2020-10-31
// Description: file handling functions that prepare data for the tiling algorithm


#include "tileUtil.h"

/**
 * Function tile:
 * @param PNG & target: an image to use as base for the mosaic. it's pixels will be
 *                      be replaced by thumbnail images whose average color is close
 *                      to the pixel.
 * @param rgbtree & ss: a kd-tree of RGBAPixels, used as a query structure for
 *                      nearest neighbor search. 
 * @param map<RGBAPixel, string> & photos: a map that takes a color key and returns the
 *                      filename of an image whose average color is that key.
 *
 * returns: a PNG whose dimensions are TILESIZE times that of the target. Each
 * pixel in the target is used as a query to ss.findNearestNeighbor, and the response
 * is used as a key in photos. 
 */

PNG tiler::tile(PNG & target, const rgbtree & ss, map<RGBAPixel,string> & photos){
    int h = target.height();
    int w = target.width();
    
    PNG finalImg(w*TILESIZE, h*TILESIZE);  // setup the final image to return
    
    for (int i = 0; i < w; i++) {
        
        for (int j = 0; j < h; j++) {
            RGBAPixel key = ss.findNearestNeighbor(*target.getPixel(i, j)); //!!!!!
            PNG curr;           // setup the thumbail image  
            curr.readFromFile(photos[key]);
            
            for (int ty = 0; ty < TILESIZE; ty++) {
                for (int tx = 0; tx < TILESIZE; tx++) {
                    *finalImg.getPixel(i * TILESIZE + tx, j * TILESIZE + ty) = *curr.getPixel(tx, ty);
                }
            }
            
        } 
         

    }
    return finalImg;

}

/* buildMap: function for building the map of <key, value> pairs, where the key is an
 * RGBAPixel representing the average color over an image, and the value is 
 * a string representing the path/filename.png of the TILESIZExTILESIZE image
 * whose average color is the key.
 * 
 * We've provided a bit of the C++ code that allows you to iterate over the files
 * in a directory. It is up to you to figure out what it means, and how to use it.
 * 
 * @param path is the subdirectory in which the tiles can be found. In our examples
 * this is imlib.
 *
*/
map<RGBAPixel, string> tiler::buildMap(string path) {

    map < RGBAPixel, string> thumbs;
    for (const auto & entry : fs::directory_iterator(path)) {
        PNG curr; curr.readFromFile(entry.path());

        int h = curr.height();
        int w = curr.width();
        int dim = h * w;
        int sum[3];
        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {
                sum[0] += curr.getPixel(i,j)->r;
                sum[1] += curr.getPixel(i,j)->g;
                sum[2] += curr.getPixel(i,j)->b;
            }
        }
                
        thumbs[RGBAPixel(sum[0]/dim, sum[1]/dim, sum[2]/dim)] = entry.path(); 
    }
    return thumbs;
}


