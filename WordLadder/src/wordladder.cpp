// Copyright [2016] <Huang Jiaming>
/*
 * A word ladder is a connection from one word to another formed by changing
 * letter at a time with the constraint that at each step the sequence of
 * letters still form a valid word. FIND SHORTEST ONE with BFS.
 */

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "simpio.h"
#include "set.h"
#include "stack.h"
#include "queue.h"

using namespace std;

void wordLadder(Set<string>& dict, string word1, string word2) {

    Queue<Stack<string>> queue;
    Stack<string> root;
    Set<string> got;
    root.add(word1);
    queue.enqueue(root);
    string alph = "abcdefghijklmnopqrstuvwxyz";
    while (!queue.isEmpty()) {
        Stack<string> tmpstack = queue.dequeue();
        string tmpword = tmpstack.peek();
        for (int i = 0; i < tmpword.length(); i++) {
            string tmpword2 = tmpword;
            for (auto c : alph) {
                tmpword2[i] = c;
                if (dict.contains(tmpword2) && !got.contains(tmpword2)) {
                    got.add(tmpword2);
                    if (tmpword2 != word2) {
                        Stack<string> tmpstack2 = tmpstack;
                        tmpstack2.add(tmpword2);
                        queue.enqueue(tmpstack2);
                    }
                    else {
                        tmpstack.add(tmpword2);
                        cout << "A connection found:" << endl;
                        while (!tmpstack.isEmpty()) {
                            cout << tmpstack.pop() << ", ";
                        }
                    }
                }
            }
        }

    }

}

int main() {

    cout << "Welcome to CS106B Word Ladder." << endl;
    cout << "Please give me two English words, and I will change the first into" << endl;
    cout << "the second by changing one letter at a time." << endl;
    string dictFile;
    ifstream ifs;
    while (!ifs.is_open()) {
        dictFile = getLine("Dictionary File Name? ");
        ifs.open(dictFile);
        if (!ifs.is_open())
            cout << "Unable to open that file. Try again." << endl;
    }
    // set dictionary
    Set<string> dict;
    string word;
    while(ifs >> word) {
        dict.add(word);
    }

    while(1) {

        string word1, word2;
        bool isWord = false;
        while (!isWord) {
            word1 = toLowerCase(getLine("word #1: "));
            if (dict.contains(word1))
                isWord = true;
        }
        isWord = false;
        while (!isWord) {
            word2 = toLowerCase(getLine("word #2: "));
            if (dict.contains(word2) && word1.length() == word2.length() && word1 != word2)
                isWord = true;
        }
        wordLadder(dict,word1,word2);
        cout << endl;
    }


    ifs.close();
    return 0;
}
