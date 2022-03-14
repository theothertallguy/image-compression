#include "stats.h"


#include "stats.h"

/**
 * @param a value of vector at vect[d-1][d-1]
 * @param b value of vector at vect[d][d-1]
 * @param c value of vector at vect[d-1][d]
 * @param d value of colour at im.getPixel(d,d)
 * 
 * value at vect[d][d] is b + c - a + d
 * e = b + c - a + d
 * e - b - c + a = d
 * @param im the image to get stats for
 */
stats::stats(PNG & im){
    int width = im.width();
    int height = im.height();

    sumRed = vector<vector<long>>(width, vector<long>(height));
    sumGreen = vector<vector<long>>(width, vector<long>(height));
    sumBlue = vector<vector<long>>(width, vector<long>(height));
    sumsqRed = vector<vector<long>>(width, vector<long>(height));
    sumsqGreen = vector<vector<long>>(width, vector<long>(height));
    sumsqBlue = vector<vector<long>>(width, vector<long>(height));

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            setSums(im,i,j);
        }
    }
}

void stats::setSums(PNG & im, int x, int y) {
    RGBAPixel * xyPixel = im.getPixel(x,y);
    int r = xyPixel->r;
    int g = xyPixel->g;
    int b = xyPixel->b;

    pair<int,int> ul(x, y);

    setVectRect(sumRed,ul,r);
    setVectRect(sumGreen,ul,g);
    setVectRect(sumBlue,ul,b);
    setVectRect(sumsqRed,ul,(r*r));
    setVectRect(sumsqGreen,ul,(g*g));
    setVectRect(sumsqBlue,ul,(b*b));
}

void stats::setVectRect(vector<vector<long>> vect, pair<int,int> ul, int colVal) {
    int right = ul.first;
    int bottom = ul.second;
    int top = ul.second - 1;
    int left = ul.first - 1;
    long a = 0;
    long b = 0;
    long c = 0;
    long d = 0;
    if (left >= 0) {
        if (top >= 0) {
            a = vect[left][top];
            b = vect[right][top];
        }
        c = vect[left][bottom];
    } else {
        if (top >= 0) {
            b = vect[right][top];
        }
    }
    d = colVal;

    vect[right][bottom] = b + c - a + d;
}

/**
 * @brief return sum of channel vals over given rect 
 */
long stats::getSum(char channel, pair<int,int> ul, int w, int h){
    switch (channel) {
        case 'r':
            return getVectRect(sumRed,ul,w,h);
            break;
        case 'g':
            return getVectRect(sumGreen,ul,w,h);
            break;
        case 'b':
            return getVectRect(sumBlue,ul,w,h);
            break;
    }
}

/**
 * @brief return sum of squares of channel vals over given rect 
 */
long stats::getSumSq(char channel, pair<int,int> ul, int w, int h){
    switch (channel) {
        case 'r':
            return getVectRect(sumsqRed,ul,w,h);
            break;
        case 'g':
            return getVectRect(sumsqGreen,ul,w,h);
            break;
        case 'b':
            return getVectRect(sumsqBlue,ul,w,h);
            break;
    }
}

/**
 * @brief 
 * 
 * @param vect vector to pull values from
 * @param ul upper left corner of rect to get from vect
 * @param w width of rect
 * @param h height of rect
 * @return long 
 */
long stats::getVectRect(vector<vector<long>> vect, pair<int,int> ul, int w, int h) {
    int right = ul.first + w - 1;
    int bottom = ul.second  + h - 1;
    int top = ul.second - 1;
    int left = ul.first - 1;
    long a = 0;
    long b = 0;
    long c = 0;
    long d = 0;
    if (left >= 0) {
        if (top >= 0) {
            a = vect[left][top];
            b = vect[right][top];
        }
        c = vect[left][bottom];
    } else {
        if (top >= 0) {
            b = vect[right][top];
        }
    }
    d = vect[right][bottom];
    
    return d - b + a - c;
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int,int> ul, int w, int h){
    double r = getColVar('r', ul ,w, h);
    double g = getColVar('g', ul ,w, h);
    double b = getColVar('b', ul ,w, h);
    return r + g + b;
}

// given a rectangle, compute its sum of squared deviations from mean, over a color channel.
// see written specification for a description of this function.
double stats::getColVar(char channel, pair<int,int> ul, int w, int h) {
    long sqrSum = getSumSq(channel,ul,w,h);
    long linSum = getSum(channel,ul,w,h);
    long r = w * h;
    long sqr = pow(linSum, 2);
    double ans = sqrSum - (sqr / r);
    return ans;
}
		
RGBAPixel stats::getAvg(pair<int,int> ul, int w, int h){
    int r = getSum('r', ul ,w, h) / (w*h);
    int g = getSum('g', ul ,w, h) / (w*h);
    int b = getSum('b', ul ,w, h) / (w*h);
    return RGBAPixel(r,g,b);
}