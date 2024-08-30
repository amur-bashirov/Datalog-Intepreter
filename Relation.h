//
// Created by deck on 7/21/24.
//

#ifndef RELATION_H
#define RELATION_H
#pragma once
#include <iostream>

#include "Tuple.h"
#include "Scheme.h"
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <set>
using namespace std;

class Relation {

private:

    string _name;
    Scheme scheme;
    set<Tuple> _tuples;

public:

    Relation(const string& name, const Scheme& scheme)
      : _name(name), scheme(scheme) { }

    void addTuple(const Tuple& tuple) {
        _tuples.insert(tuple);
    }

    const set<Tuple> &getTuples() const {
        return _tuples;
    }

    const Scheme &getScheme() const {
        return scheme;
    }

    string toString() const {
        stringstream out;
        // add code to print the Tuples, one per line
        for (Tuple t : _tuples) {
            out << t.toString(scheme) << endl;
        }
        return out.str();
    }

    Relation select1(int index, const string& value) const {
        Relation result(_name, scheme);
        // add tuples to the result if they meet the condition
        for (const Tuple& t:   _tuples) {
            if (t.at(index) == value) {
                result.addTuple(t);
            }
        }
        return result;
    }

    Relation select2(int index1, int index2) const {
        Relation result(_name, scheme);
        for (const Tuple& t: _tuples) {
            if (t.at(index1) == t.at(index2)) {
                result.addTuple(t);
            }
        }
        return result;
    }

    Relation project(vector <int> attributes, vector <string> parameters) {
        vector<string> new_atttributes = vector <string>();
        map <string, vector<Tuple>> map_relation = map<string, vector<Tuple>>();
        for (const Tuple& t:   _tuples) {
            vector <string> values = vector<string>();
            for (int i = 0; i < attributes.size(); i++) {
                int num = attributes.at(i);
                string value = t.at(num);
                values.push_back(value);
            }
            for (int i = 0; i < attributes.size(); i++) {
                int num = attributes.at(i);
                string new_attr = parameters.at(num);
                if (map_relation.find(new_attr) == map_relation.end()) {
                    map_relation[new_attr] = { values };
                    new_atttributes.push_back(new_attr);
                } else {
                    map_relation[new_attr].push_back(values);
                }
            }
        }
        Scheme new_scheme = Scheme(new_atttributes);
        Relation new_relation = Relation(_name, new_scheme);
//        for (const auto &pair: map_relation) {
//            string attribute = pair.first;
//            vector<Tuple> tuples = pair.second;
//            for (auto &tuple : tuples){
//                Tuple new_tuple = Tuple(tuple);
//                new_relation.addTuple(new_tuple);}
//        }
        for (auto &parameter : parameters){
            vector <Tuple> tuples = map_relation[parameter];
            for (auto &tuple : tuples){
                Tuple new_tuple = Tuple(tuple);
                new_relation.addTuple(new_tuple);}
        }
        return new_relation;
    }

    Relation rename(vector<string> names) {
        Scheme new_scheme = Scheme(names);
        set <Tuple> Tuples = set<Tuple>();
        Relation new_relation = Relation(_name, new_scheme);
        for (auto &tuple : _tuples){
            Tuples.insert(tuple);
        }
        new_relation._tuples = Tuples;
        return new_relation;
    }

    static bool joinable(const Scheme& leftScheme, const Scheme& rightScheme,
                         const Tuple& leftTuple, const Tuple& rightTuple) {

        for (int i = 0; i < leftScheme.getNames().size();i++) {
            for (int j = 0; j < rightScheme.getNames().size(); j++) {
                if (leftScheme.getNames().at(i) == rightScheme.getNames().at(j)){
                    if (leftTuple.at(i) != rightTuple.at(j)){
                        return false;
                    }
                }
            }
        }
        return true;
    }

    Relation join(const Relation& right){
        const Relation &left = *this;
        Relation result = Relation(_name, scheme);
        vector <string> attributes = vector <string>();
        vector <string> left_attributes = vector <string>();
        vector <string> Right_Tuples = vector<string>();
        for (auto &attribute : left.getScheme().getNames()){
            attributes.push_back(attribute);
            left_attributes.push_back(attribute);
        }
        for (auto &attribute : right.getScheme().getNames()){
            auto it = find(attributes.begin(), attributes.end(), attribute);
            if (it == attributes.end()){
                attributes.push_back(attribute);
            }
        }
        Scheme new_scheme = Scheme(attributes);
        result = Relation(_name, new_scheme);
        for (auto &left_tuple : left.getTuples()){
            for (auto &right_tuple : right.getTuples()){
                if (joinable(left.getScheme(), right.getScheme(), left_tuple, right_tuple)){
                    for (int i = 0; i < right.getScheme().getNames().size(); i++){
                        auto it = find(left_attributes.begin(), left_attributes.end(), right.getScheme().getNames().at(i));
                        if (it == left_attributes.end()){
                            //left_attributes.push_back(right.getScheme().getNames().at(i));

                            Right_Tuples.push_back(right_tuple.at(i));
                        }
                    }
                    vector <string> new_tuple = vector<string>();
                    for (int i = 0; i < left_tuple.size(); i++){
                        new_tuple.push_back(left_tuple.at(i));
                    }
                    for (int i = 0; i < Right_Tuples.size();i++){
                        new_tuple.push_back(Right_Tuples.at(i));
                    }
                    vector <string> empty_vector = vector<string>();
                    Right_Tuples = empty_vector;
                    Tuple New_Tuple = Tuple(new_tuple);

                    result.addTuple(New_Tuple);
                }
            }
        }
        return result;

    }


    Relation unite(Relation right){
        const Relation &left = *this;
        Relation result = Relation(_name, scheme);
            for (auto &tuple2: right.getTuples()){
                if (_tuples.insert(tuple2).second){
                    cout << "  " << tuple2.toString(left.getScheme()) << endl;
                }
            }
        return left;
    }

//    Relation join(const Relation& right) {
//        const Relation &left = *this;
//        Relation result(_name, scheme);
//        map<string, vector<Tuple>> Map;
//        vector<string> Tuples;
//        set <string> attributes;
//        set<string> Real;
//        set<string> right_set;
//        vector<Tuple> vector_of;
//
//
//        for (auto &tuple: _tuples) {
//            bool add = true;
//            for (int y = 0; y < tuple.size(); y++) {
//                string l_Attribute = left.getScheme().at(y);
//                if (attributes.find(l_Attribute) == attributes.end()) {
//                    attributes.insert(l_Attribute);
//                    Real.insert(l_Attribute);
//                    Tuples.push_back(tuple.at(y));
//                }
//                vector <Tuple> right_tuples;
//                vector <string> values;
//                for (auto &_Tuple: right.getTuples()) {
//                    bool found = false;
//
//                    for (int j = 0; j < _Tuple.size(); j++) {
//                        string r_Attribute = right.getScheme().getNames().at(j);
//                        if (attributes.find(r_Attribute) == attributes.end()) {
//                            attributes.insert(r_Attribute);
//                            Real.insert(r_Attribute);
//                            values.push_back(tuple.at(j));
//
//                        } else if (l_Attribute== r_Attribute && tuple.at(y) != _Tuple.at(j)) {
//                            add = false;
//                            values.push_back(tuple.at(j));
//                        } else if (l_Attribute== r_Attribute && tuple.at(y) == _Tuple.at(j)){
//                            values.push_back(tuple.at(j));
//                            found = true;
//                        }
//
//                    }
//                    if (!add) {
//                        for (int j = 0; j < _Tuple.size(); j++) {
//                            values.pop_back();
//                        }
//                    } else if (found){
//                        Tuple tuply = Tuple(values);
//                        right_tuples.push_back(tuply);
//
//                    }
//                    //                        for (auto &_Tuple: right.getTuples()) {
//                    //                            if (right_set.find(_Tuple) == right_set.end()) {
//                    //                                right_set.insert(_Tuple);
//                    //                                cout << "right tuple: " << _Tuple.toString(right.getScheme()) << endl;
//                    //                            }
//                    //                            for (auto &r_parameter: _Tuple.getValues()) {
//                    //                                if (attributes.find(r_attribute) == attributes.end()) {
//                    //                                    attributes.insert(r_attribute);
//                    //                                    Attributes.insert(r_attribute);
//                    //                                }
//                    //                            }
//                    //                        }
//                }
//            }
//            set<string> empty_set;
//            attributes = empty_set;
//            Tuple new_tuple = Tuple(Tuples);
//            result.addTuple(new_tuple);
//
//        }
//
//        vector <string> new_attributes;
//        for (auto &attribute: Real) {
//            new_attributes.push_back(attribute);
//        }
//        Scheme new_scheme = Scheme(new_attributes);
//        result = Relation(_name, new_scheme);
//        return result;
//
//    }

};

#endif //RELATION_H
