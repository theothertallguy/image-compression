/**
 *
 * shifty quadtree (pa3)
 * sqtree.cpp
 * This file will be used for grading.
 *
 */

#include "sqtree.h"

// First Node constructor, given.
SQtree::Node::Node(pair<int,int> ul, int w, int h, RGBAPixel a, double v)
  :upLeft(ul),width(w),height(h),avg(a),var(v),NW(NULL),NE(NULL),SE(NULL),SW(NULL)
{}

// Second Node constructor, given
SQtree::Node::Node(stats & s, pair<int,int> ul, int w, int h)
  :upLeft(ul),width(w),height(h),NW(NULL),NE(NULL),SE(NULL),SW(NULL) {
  avg = s.getAvg(ul,w,h);
  var = s.getVar(ul,w,h);
}

// SQtree destructor, given.
SQtree::~SQtree() {
  clear();
}

// SQtree copy constructor, given.
SQtree::SQtree(const SQtree & other) {
  copy(other);
}

// SQtree assignment operator, given.
SQtree & SQtree::operator=(const SQtree & rhs) {
  if (this != &rhs) {
    clear();
    copy(rhs);
  }
  return *this;
}

/**
 * SQtree constructor given tolerance for variance.
 */
SQtree::SQtree(PNG & imIn, double tol) {
  cout << "here" << endl;
  stats imgStats = stats(imIn);
  cout << "here" << endl;
  pair<int,int> tl(0,0);
  root = buildTree(imgStats,tl,imIn.width(),imIn.height(),tol);
}

/**
 * Helper for the SQtree constructor.
 */
SQtree::Node * SQtree::buildTree(stats & s, pair<int,int> & ul, int w, int h, double tol) {
  Node * rectNode = new Node(s,ul,w,h);
  cout << "here1" << endl;
  rectNode->avg = s.getAvg(ul,w,h);
  rectNode->var = s.getVar(ul,w,h);
  rectNode->SW = NULL;
  rectNode->SE = NULL;
  rectNode->SW = NULL;
  rectNode->SE = NULL;
  rectNode->width = w;
  rectNode->height = h;
  rectNode->upLeft = ul;
  cout << "here2" << endl;
  if ((rectNode->var > tol) && ((w > 1)||(h > 1))) {
    pair<int,int> splitPos = splitPoint(s,ul,w,h);
  cout << "here3" << endl;
    if ((w>1)&&(h>1)) {
  cout << "here4" << endl;
      pair<int,int> norW(ul.first,ul.second);
      pair<int,int> norE(splitPos.first,ul.second);
      pair<int,int> souW(ul.first,splitPos.second);
      pair<int,int> souE(splitPos.first,splitPos.second);
      int innerW = splitPos.first-ul.first;
      int innerH = splitPos.first-ul.first;
      rectNode->NW = buildTree(s,norW,innerW,innerH,tol);
      rectNode->NE = buildTree(s,norE,w - innerW,innerH,tol);
      rectNode->SW = buildTree(s,souW,innerW,h - innerH,tol);
      rectNode->SE = buildTree(s,souE,w - innerW,h - innerH,tol);
    } else if (w>1) {
  cout << "here5" << endl;
      pair<int,int> norW(ul.first,ul.second);
      pair<int,int> norE(splitPos.first,ul.second);
      int innerW = splitPos.first-ul.first;
      rectNode->NW = buildTree(s,norW,innerW,1,tol);
      rectNode->NE = buildTree(s,norE,w - innerW,1,tol);
    } else if (h>1) {
  cout << "here6" << endl;
      pair<int,int> norW(ul.first,ul.second);
      pair<int,int> souW(ul.first,splitPos.second);
      int innerH = splitPos.first-ul.first;
      rectNode->NW = buildTree(s,norW,1,innerH,tol);
      rectNode->SW = buildTree(s,souW,1,h - innerH,tol);
    }
  }

  return rectNode;
}

pair<int,int> SQtree::splitPoint(stats & s, pair<int,int> & ul, int w, int h) {
  int goodW = -1;
  int goodH = -1;
  double bestVar = __DBL_MAX__;

  if (w > 1) {
    for (int i = 1; i < w; i++) {
      pair<int,int> right(ul.first + i,ul.second);
      double varL = s.getVar(ul,i,1);
      double varR = s.getVar(right, w-i, 1);
      if (max(varL,varR) < bestVar) {
        bestVar = max(varL,varR);
        goodW = ul.first + i;
        goodH = ul.second;
      }
    }
  }

  if (h > 1) {
    for (int i = ul.second; i < ul.second + h; i++) {
      pair<int,int> bottom(ul.first,ul.second + i);
      double varU = s.getVar(ul,1,i);
      double varD = s.getVar(bottom, 1, h-i);
      if (max(varU,varD) < bestVar) {
        bestVar = max(varU,varD);
        goodW = ul.first;
        goodH = ul.second + i;
      }
    }
  }

  if ((w > 1)&&(h > 1)) {
    for (int i = ul.first; i < ul.first + w; i++) {
      for (int j = ul.second; j < ul.second + h; j++) {
        pair<int,int> ur(ul.first + i,ul.second);
        pair<int,int> bl(ul.first,ul.second + j);
        pair<int,int> br(ul.first + i,ul.second + j);
        double varUL = s.getVar(ul,i,j);
        double varUR = s.getVar(ur, w-i, j);
        double varBL = s.getVar(bl, i, h-j);
        double varBR = s.getVar(br, w-i, h-j);
        if (max(max(varUL,varBL),max(varUR,varBR)) < bestVar) {
          bestVar = max(max(varUL,varBL),max(varUR,varBR));
          goodW = ul.first + i;
          goodH = ul.second + j;
        }
      }
    }
  }

  return pair<int,int>(goodW,goodH);
}
  
/**
 * Render SQtree and return the resulting image.
 */
PNG SQtree::render() {
  PNG squArt = PNG(root->width,root->height);
  return renderNode(root, squArt);
}

PNG SQtree::renderNode(Node * node, PNG im) {
  cout << "here5.5" << endl;
  if (node == NULL) {
  cout << "here6" << endl;
    return im;
  } else if (((node->NE == NULL)&&(node->NW == NULL))&&((node->SE == NULL)&&(node->SW == NULL))) {
    int ulW = node->upLeft.first;
    int ulH = node->upLeft.second;
    for (int i = ulW; i < ulW + node->width; i++) {
      for (int j = ulH; i < ulH + node->height; i++) {
        RGBAPixel * xyPixel = im.getPixel(i,j);
        xyPixel->r = node->avg.r;
        xyPixel->g = node->avg.g;
        xyPixel->b = node->avg.b;
        xyPixel->a = node->avg.a;
      }
    }
    return im;
  } else {
    PNG returnIMG;
    returnIMG = renderNode(node->NE, im);
    returnIMG = renderNode(node->NW, returnIMG);
    returnIMG = renderNode(node->SE, returnIMG);
    returnIMG = renderNode(node->SW, returnIMG);

    return returnIMG;
  }
}

/**
 * Delete allocated memory.
 */
void SQtree::clear() {
  // Your code here.
}

void SQtree::copy(const SQtree & other) {
  // Your code here.
}

int SQtree::size() {
  return nodeSize(root);
}

/**
 * Returns size of given node
 */
int SQtree::nodeSize(Node* node) {
  if (node == NULL) {
    return 0;
  } else {
    int north = nodeSize(node->NE) + nodeSize(node->NW);
    int south = nodeSize(node->SE) + nodeSize(node->SW);
    return north + south + 1;
  }
}

