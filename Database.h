//
// Created by bashirov on 7/22/24.
//

#ifndef PROJECT3_DATABASE_H
#define PROJECT3_DATABASE_H
#pragma once
#include "Relation.h"
#include <map>
#include <set>
#include <memory>
#include <string>
using namespace std;


class Database {
private:
    string _name;
    map<string, Relation> relations;
public:
    void Add(const string& name, Relation relation){
        //relations[name] = relation;
        relations.insert({name, relation});
}
    void Add2(const string& name, Relation relation){
        relations.at(name) = relation;
    }

    Relation getRelation (string name){
        Relation relation = relations.at(name);
        return relation;
    }

//    string toString() const {
//        stringstream out;
//        for (const auto &pair: relations) {
//            out << pair.first;
//            Relation relation = Relation(pair.second);
//            out << relation.toString() << endl;
//        }
//    }
};
#endif //PROJECT3_DATABASE_H
