// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own.
// TODO: rewrite this comment

#include <iostream>
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
void scatter (GBufferedImage & img, Grid<int> & grid, int radius){
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
        gw.setSize(img.getWidth(), img.getHeight());
        gw.add(&img,0,0);
        Grid<int> original = img.toGrid();

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
                cout << "Please input a radius[1-100]:";
                cin >> radius;
                scatter (img,original,radius);
                break;
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
