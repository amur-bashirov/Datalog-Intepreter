//
// Created by bashirov on 8/5/24.
//

#ifndef PROJECT3_NODE_H
#define PROJECT3_NODE_H
#pragma once
#include <set>
#include <sstream>
#include <iostream>
using namespace std;



class Node {

private:

    set<int> adjacentNodeIDs;

public:
    void addEdge(int adjacentNodeID) {
        adjacentNodeIDs.insert(adjacentNodeID);
    }

    string toString(){
        stringstream out;
        auto it = adjacentNodeIDs.begin();
        while (it != adjacentNodeIDs.end()){
            out << "R"<< *it;
            if (++it != adjacentNodeIDs.end()){
                out << ",";
            }
        }
        out << endl;
        return out.str();
    }

    const set<int> &getAdjacentNodeIDs() const {
        return adjacentNodeIDs;
    }

};

#endif //PROJECT3_NODE_H
