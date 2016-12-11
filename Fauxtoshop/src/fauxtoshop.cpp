// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own.
// TODO: rewrite this comment

#include <iostream>
#include <cstdlib>
#include <algorithm>
#include "console.h"
#include "gwindow.h"
#include "grid.h"
#include "simpio.h"
#include "strlib.h"
#include "gbufferedimage.h"
#include "gevents.h"

using namespace std;

static const int    WHITE = 0xFFFFFF;
static const int    BLACK = 0x000000;
static const int    GREEN = 0x00FF00;

static bool     openImageFromFilename(GBufferedImage &img, string filename);
static bool 	saveImageToFilename(const GBufferedImage &img, string filename);
static void     getMouseClickLocation(int &row, int &col);

/* Depending on how you approach your problem decomposition, you
 * may want to rewrite some of these lines, move them inside loops,
 * or move them inside helper functions, etc.
 *
 * TODO: rewrite this comment.
 */
void scatter (GBufferedImage& img, const Grid<int>& grid, int radius){
    Grid<int> dup = grid;
    int numCols = grid.numCols();
    int numRows = grid.numRows();
    int ranRow=numRows, ranCol=numCols;
    for (int r = 0; r < numRows; r++)
        for (int c = 0; c < numCols; c++) {
            while (c+ranCol<0 || c+ranCol>=numCols || r+ranRow<0 || r+ranRow>=numRows){
                ranCol = randomInteger(-radius,radius);
                ranRow = randomInteger(-radius,radius);
            }
            dup[r][c] = grid[r+ranRow][c+ranCol];
            ranRow=numRows;
            ranCol=numCols;
        }
    img.fromGrid(dup);
}

bool colorDiff(GBufferedImage& img, const Grid<int>& grid, int row, int col, int threhold) {
    int red, green, blue;
    int mred, mgreen, mblue;
    img.getRedGreenBlue(grid[row][col], mred, mgreen, mblue);
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (grid.inBounds(row+i, col+j)) {
                img.getRedGreenBlue(grid[row+i][col+j], red, green, blue);
                if (max(abs(mred-red),max(abs(mgreen-green), abs(mblue-blue))) > threhold)
                    return true;
            }
        }
    }
    return false;
}

void edge (GBufferedImage& img, const Grid<int>& grid, int threhold){
    Grid<int> dup = grid;
    int numCols = grid.numCols();
    int numRows = grid.numRows();
    for (int r = 0; r < numRows; r++) {
        for (int c = 0; c < numCols; c++) {
            if (colorDiff(img, grid, r, c, threhold)) {
                dup[r][c] = BLACK;
            } else {
                dup[r][c] = WHITE;
            }
        }
    }
    img.fromGrid(dup);
}


bool isGreen(GBufferedImage& img, Grid<int>& stkGrid, int row, int col, int threhold) {
    int red, green, blue, red1, green1, blue1;
    img.getRedGreenBlue(GREEN,red,green,blue);
    img.getRedGreenBlue(stkGrid[row][col],red1,green1,blue1);
    if (max(abs(red-red1),max(abs(green-green1), abs(blue-blue1))) <= threhold)
        return true;
    else
        return false;
}

void greenScreen(GBufferedImage& img, Grid<int>& imgGrid, Grid<int>& stkGrid, int row, int col, int threhold) {
    Grid<int> dup = imgGrid;
    int numCols = stkGrid.numCols();
    int numRows = stkGrid.numRows();
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            if (imgGrid.inBounds(row+i,col+j)) {
                if (!isGreen(img,stkGrid,i,j,threhold)) {
                    dup[row+i][col+j] = stkGrid[i][j];
                }
            }
        }
    }
    img.fromGrid(dup);
}

int main() {
    GWindow gw;
    gw.setTitle("Fauxtoshop");
    gw.setVisible(true);
    GBufferedImage img;    
    while (true){
        cout << "Welcome to Fauxtoshop!" << endl;
        string imgName;
        while (!openImageFromFilename(img,imgName)){
            cout << "Enter name of image file to open (or blank to quit): ";
            getline(cin, imgName);
            if (imgName.empty())
                exit(0);
        }
        cout << "Opening img file, may take a minute..." << endl;
        gw.setSize(img.getWidth(), img.getHeight());    // set size of windows canvas
        gw.add(&img,0,0);   // add the real image
        Grid<int> original = img.toGrid();  // convert img to grid<int> structure

        char choice;
        bool choiceValid = false;
        while (!choiceValid){
            cout << "Which image filter would you like to apply?" << endl
                 << "\t1 - Scatter" << endl
                << "\t2 - Edge detection" << endl
                << "\t3 - \"Green screen\" with another image" << endl
                << "\t4 - Comapre image with another image" << endl
                << "Your choice:";
            cin >> choice;
            switch (choice) {
            case '1':
                choiceValid = true;
                int radius;
                cout << "Enter degree of scatter [1-100]: ";
                cin >> radius;
                scatter (img,original,radius);
                break;
            case '2':
                choiceValid = true;
                int threhold;
                cout << "Enter threhold for edge detection: ";
                cin >> threhold;
                edge(img,original,threhold);
                break;
            case '3': {
                cin.clear();
                cin.ignore(10000, '\n');
                choiceValid = true;
                int threhold2;
                string stkName;
                string location;
                int srow = -1, scol = -1;
                char a, b;
                GBufferedImage stkImg;
                cout << "Now choose another file to add to your background file." << endl;
                while (!openImageFromFilename(stkImg, stkName)) {
                    cout << "Enter name of sticker image file to open: ";
                    getline(cin,stkName);
                }
                cout << "Opening sticker img file, may take a minute..." << endl;
                Grid<int> stkGrid = stkImg.toGrid();
                cout << "Enter tolerence threhold:";
                cin >> threhold2;
                cin.clear();
                cin.ignore(10000, '\n');
                while (srow < 0 && scol < 0) {
                    cout << "Enter location to place the sticker img as \"(row,col)\" (or blank to use mouse):";
                    getline(cin,location);
                    if (location != "") {
                        stringstream ss(location);
                        ss >> a >> srow >> b >> scol;
                    } else {
                        getMouseClickLocation(srow, scol);
                    }
                    cout << endl << "Get location: (" << srow << ", " << scol << ")." << endl;
                }
                greenScreen(img,original,stkGrid,srow,scol,threhold2);
                cout << "Press Enter...";
                break;
            }
            default:
                cout << "Invalid choice.Please choose again." << endl;
                break;
            }

        }
        // save img
        string saveImg;
        cin.ignore(256,'\n');
        while (true){
            saveImg = getLine("enter filename to save img(or blank to skip:");
            if (saveImg.empty())
                break;
            if (saveImageToFilename(img,saveImg))
                break;
        }
        // clear and repeat
        gw.clear();
        cout << endl;
    }
//    int row, col;
//    getMouseClickLocation(row, col);
    return 0;
}


/* STARTER CODE HELPER FUNCTION - DO NOT EDIT
 *
 * Attempts to open the image file 'filename'.
 *
 * This function returns true when the image file was successfully
 * opened and the 'img' object now contains that image, otherwise it
 * returns false.
 */
static bool openImageFromFilename(GBufferedImage &img, string filename) {
    try { img.load(filename); }
    catch (...) { return false; }
    return true;
}

/* STARTER CODE HELPER FUNCTION - DO NOT EDIT
 *
 * Attempts to save the image file to 'filename'.
 *
 * This function returns true when the image was successfully saved
 * to the file specified, otherwise it returns false.
 */
static bool saveImageToFilename(const GBufferedImage &img, string filename) {
    try {
        img.save(filename);
    } catch (...) {
        return false;
    }
    return true;
}

/* STARTER CODE HELPER FUNCTION - DO NOT EDIT
 *
 * Waits for a mouse click in the GWindow and reports click location.
 *
 * When this function returns, row and col are set to the row and
 * column where a mouse click was detected.
 */
static void getMouseClickLocation(int &row, int &col) {
    GMouseEvent me;
    do {
        me = getNextEvent(MOUSE_EVENT);
    } while (me.getEventType() != MOUSE_CLICKED);
    row = me.getY();
    col = me.getX();
}
