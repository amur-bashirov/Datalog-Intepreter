//
// Created by deck on 7/21/24.
//

#ifndef TUPLE_H
#define TUPLE_H
#pragma once
#include <iostream>
#include <vector>
#include "Scheme.h"
#include <sstream>
using namespace std;
class Tuple {

private:

    vector<string> values;

public:

    Tuple(vector<string> values) : values(values) { }

    unsigned size() const {
        return values.size();
    }

    const string& at(int index) const {
        return values.at(index);
    }

    const vector<string> &getValues() const {
        return values;
    }

    bool operator<(const Tuple t) const {
        return values < t.values;
    }

    string toString(const Scheme& scheme) const {
        const Tuple& tuple = *this;
        stringstream out;
        for (size_t i = 0; i < scheme.size(); ++i) {
            out << scheme.at(i) << "=" << tuple.at(i)<< "";
            if (i < scheme.size() - 1) {
                out << ", ";
            }
        }


        // fix the code to print "name=value" pairs
        // out << scheme.size();
        // out << scheme.at(0);
        // out << tuple.size();
        // out << tuple.at(0);
         return out.str();
    }

    // TODO: add more delegation functions as needed

};
#endif //TUPLE_H
