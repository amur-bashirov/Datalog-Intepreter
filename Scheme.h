//
// Created by bashirov on 7/22/24.
//

#ifndef PROJECT3_SCHEME_H
#define PROJECT3_SCHEME_H
#pragma once
#include <iostream>
#include <vector>

#include <sstream>
using namespace std;

class Scheme {

private:

    vector<string> names;

public:

    Scheme(vector<string> names) : names(names) { }

    unsigned size() const {
        return names.size();
    }

    const string& at(int index) const {
        return names.at(index);
    }

    const vector<string> &getNames() const {
        return names;
    }
    // TODO: add more delegation functions as needed

};


#endif //PROJECT3_SCHEME_H
