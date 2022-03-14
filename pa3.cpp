// File:        pa3.cpp
// Author:      cs221 instructors
// Date:        2021-03-03
// Description: Partial test of PA3 functionality
//              Reads Point data from external files
//              Produces PNG images of the point sets

#include "sqtree.h"

int main() {

  // read in image
  PNG orig;
  orig.readFromFile("images/orig/small1.png");
  /*
  orig.readFromFile("images/orig/small2.png");
  orig.readFromFile("images/orig/pacman.png");
  orig.readFromFile("images/orig/huckleberry_hound.png");
  orig.readFromFile("images/orig/rocky.png");
  orig.readFromFile("images/orig/rocky_bullwinkle.png");
  orig.readFromFile("images/orig/colour.png");
  orig.readFromFile("images/orig/klee-flora-on-sand.png");
  orig.readFromFile("images/orig/stanley-totem-poles.png");
  */
  
  // use it to build a sqtree (try other tolerances)
  cout << "here" << endl;
  SQtree t(orig,10.0);
  cout << "here" << endl;

  // copy the tree (to show you can)
  SQtree tCopy(t);
  cout << "here" << endl;

  // render the sqtree
  PNG pic = t.render();
  cout << "here" << endl;

  // write it out
  pic.writeToFile("images/out/small1.png");

  return 0;
}
