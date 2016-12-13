// This is the .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

// Please feel free to add any other #includes you need!
#include "recursionproblems.h"
#include <cmath>
#include <iostream>
#include "hashmap.h"
#include "map.h"
#include "random.h"
using namespace std;

static int nnnum = 0;
static int mmmum = 0;
double weightOnKnees(int row, int col, const Vector<Vector<double> >& weights) {
    // TODO: write this function
    if (row < 0 || col < 0 || col >= weights[row].size()) return 0;
    return weights[row][col] + weightOnKnees(row-1, col-1, weights)/2.0 + weightOnKnees(row-1, col, weights)/2.0;
}

double weightOnKneesFaster(int row, int col, const Vector<Vector<double> >& weights,
                           Vector<Vector<double> >& memos) {
    // TODO: write this function
    if (row < 0 || col < 0 || col >= weights[row].size()) return 0.0;
    if (memos[row][col] != 0.0) return memos[row][col];
    double ret = weights[row][col] + weightOnKneesFaster(row-1, col-1, weights,memos)/2.0 + weightOnKneesFaster(row-1, col, weights,memos)/2.0;
    memos[row][col] = ret;
    return ret;
}

double weightOnKneesFaster(int row, int col, const Vector<Vector<double> >& weights) {
    // TODO: write this function
    Vector<Vector<double> > mem;
    for (int i = 0; i < weights.size(); i++) {
        Vector<double> vrow;
        for (int j = 0; j < weights[i].size(); j++) {
            vrow.add(0.0);
        }
        mem.add(vrow);
    }
    return weightOnKneesFaster(row,col,weights,mem);
}

void drawSierpinskiTriangle(GWindow& gw, double x, double y, int size, int order) {
    // TODO: write this function
    if (order == 0) return;

    double h = sqrt(3)/2*size;

    if (order == 1) {
        gw.drawLine(x,y,x+size,y);
        gw.drawLine(x,y,x+size/2,y+h);
        gw.drawLine(x+size,y,x+size/2,y+h);
    }
    int nsize = size/2;
    drawSierpinskiTriangle(gw,x,y,nsize,order-1);
    drawSierpinskiTriangle(gw,x+size/2,y,nsize,order-1);
    drawSierpinskiTriangle(gw,x+size/4,y+h/2,nsize,order-1);

}

