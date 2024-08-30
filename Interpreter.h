//
// Created by bashirov on 7/23/24.
//

#ifndef PROJECT3_INTERPRETER_H
#define PROJECT3_INTERPRETER_H
#pragma once
#include "Database.h"
#include "Parser.h"
#include "Relation.h"
#include <iostream>
#include <map>
#include <stack>
#include <set>
#include <sstream>
#include <string>
using namespace std;


class Interpreter{
private:
    Database database;
    Parser parser;
public:
    Interpreter (Parser pip){
        stringstream out;
        this->parser=pip;
        for ( auto &scheme: pip.getSchemes()){
            string name1 = scheme.getId();
            vector<Parameter> parameters = scheme.getParameters();
            vector<string> attributes;
            for (auto parameter: parameters){
                string attribute = parameter.getParameter();
                attributes.push_back(attribute);
            }

            Relation relation = Relation(name1, attributes);
            for (auto &fact: pip.getFacts()){
                string name2 = fact.getId();
                vector<Parameter> t_parameters = fact.getParameters();
                vector<string>tuples;
                for (auto parameter: t_parameters){
                    string tuple = parameter.getParameter();
                    tuples.push_back(tuple);
                }
                if (name1 == name2){
                    Tuple new_tuple = Tuple(tuples);
                    relation.addTuple(new_tuple);
                }
            } database.Add(name1, relation);
        }
    }




    static Graph makeGraph(const vector<Rule1>& rules) {

        Graph graph(rules.size());
        for (int i = 0; i < rules.size(); i++){
            //cout << "from rule R" << to_string(i) << ": " << rules.at(i).toString() << endl;
            for (int j = 0; j < rules.at(i).getRules().size();j++){
                //cout << "from body predicate: " << rules.at(i).getRules().at(j).toString() << endl;
                for (int k = 0; k < rules.size();k++){
                    //cout << "to rule R" << to_string(k) << ": " << rules.at(k).toString() << endl;
                    if (rules.at(k).getHeadPredicate().getId() == rules.at(i).getRules().at(j).getId()) {
                        //cout << "dependecy found: (R" << to_string(i) <<",R" << to_string(k) << ")" << endl;
                        graph.addEdge(i,k);
                    }
                }
            }
        }
        return graph;

    }

    static Graph reverseGraph(Graph graph) {
        Graph new_graph = Graph(graph.getNodes().size());
        for (auto &pair: graph.getNodes()) {
            int index = pair.first;
            Node nodes = pair.second;
            for (auto &node : nodes.getAdjacentNodeIDs()) {
                new_graph.addEdge(node, index);
            }
        }
        return new_graph;
    }





    void evaluateRule(Rule1 Rule, Parser p){
        string empty_string = "";
        vector <string> empty_vector = vector<string>();
        empty_vector.push_back(empty_string);
        Scheme empty_scheme = Scheme(empty_vector);
        Relation result = Relation(empty_string, empty_scheme);
        vector <Relation> Relations = vector<Relation>();
        for (auto &predicate : Rule.getRules()){
            map<string, int> variables = map<string, int>();
            vector <string> variable_order = vector <string>();
            set<string> strings = set<string>();
            vector <int> int_order = vector <int>();
            string name = predicate.getId();
            vector<Parameter> parameters = predicate.getParameters();
            vector<string> attributes = vector<string>();
            Relation relation = database.getRelation(name);
            for (size_t i = 0; i < parameters.size();i++) {
                string attribute = parameters[i].getParameter();
                attributes.push_back(attribute);
                bool isconst = parameters[i].isIsconst();
                if (isconst) {
                    relation = relation.select1(i, attribute);

                } else {
                    if (strings.find(attribute) != strings.end()) {
                        relation = relation.select2(i, variables[attribute]);
                    } else {
                        variables.insert({attribute, i});
                        strings.insert(attribute);
                        variable_order.push_back(attribute);
                        int_order.push_back(i);
                    }
                }
            }
            vector<int> nums = vector<int>();
            vector<string> names = vector<string>();
            for (const auto &variable: variables) {
                int num = variable.second;
                string str = variable.first;
                nums.push_back(num);
                names.push_back(str);
            }
            relation = relation.project(int_order, attributes);
            relation = relation.rename(variable_order);
            Relations.push_back(relation);
        }
        if (Relations.size() > 1){
            for (int i = 1; i < Relations.size(); i++){
                Relations[0] = Relations[0].join(Relations[i]);
                result = Relations[0];
            }
        } else {
            for (auto &relation: Relations){
                result = relation;
            }
        }
        Predicate head = Rule.getHeadPredicate();
        string name = head.getId();
        vector <string> names = vector<string>();
        vector <int> indexes = vector<int>();
        for ( auto parameter : head.getParameters()){
            names.push_back(parameter.getParameter());
        }
        for (auto &n : names){
            for ( int i = 0; i < result.getScheme().size(); i++){
                if (result.getScheme().at(i) == n){
                    indexes.push_back(i);
                }
            }
        }

        result = result.project(indexes, result.getScheme().getNames());
        vector <string> new_names = vector<string>();
        for ( auto &scheme : p.getSchemes()){
            if (scheme.getId() == name) {
                for (auto &parameter: scheme.getParameters()) {
                    new_names.push_back(parameter.getParameter());
                }
            }
        }
        result = result.rename(new_names);
        result = database.getRelation(name).unite(result);
        database.Add2(name, result);
    }






    void rules_loop(Parser p){
        bool isuptaded = true;
        cout << "Rule Evaluation" << endl;
        int n = 0;
        map <string, bool> loop;
        cout << "Dependency Graph" << endl;
        Graph graph = Interpreter::makeGraph(p.getRules());
        cout << graph.toString() << endl;
        Graph copy_original = graph;
        graph = Interpreter::reverseGraph(graph);
        stack <int> post_order = graph.dfs_Forest(graph);
        vector <vector<int>> SCCs = graph.dfs_Forest_scc(graph, post_order);
        while (!isuptaded || n == 0){
            isuptaded = true;
            for (auto &SCC : SCCs) {
                map <int, Node> copy = copy_original.getNodes();
                int num = SCC[0];
                bool depends_on_itself = false;
                for (auto dependency :copy[num].getAdjacentNodeIDs()){
                    if (dependency == num){
                        depends_on_itself = true;
                    }
                }
                if (SCC.size() == 1 && !depends_on_itself){
                    cout << p.getRules()[num].toString() << endl;
                    evaluateRule(p.getRules()[num], p);
                    isuptaded = false;
                }
                else {
                    for (auto &index: SCC) {
                        Rule1 rule = p.getRules()[index];
                        string name = rule.getHeadPredicate().getId();
                        Relation copy_origin = database.getRelation(name);
                        cout << rule.toString() << endl;
                        evaluateRule(rule, p);


                        int begin = copy_origin.getTuples().size();
                        int end = database.getRelation(name).getTuples().size();


                        if (begin != end) {
                            isuptaded = false;
                        }
                    }
                    n = n + 1;
                }

            }
            n = n + 1;
        }
        cout << endl;
        cout << "Schemes populated after " << to_string(n) << " passes through the Rules." << endl;
        cout << endl;
    }


    void evaluateQuery(Predicate p){
        map<string, int> variables;
        vector <string> variable_order;
        set<string> strings;
        vector <int> int_order;
        string name = p.getId();
        vector<Parameter> parameters = p.getParameters();
        vector<string> attributes;
        Relation relation = database.getRelation(name);
        for (size_t i = 0; i < parameters.size();i++){
            string attribute = parameters[i].getParameter();
            attributes.push_back(attribute);
            bool isconst = parameters[i].isIsconst();
            if (isconst){
                relation = relation.select1(i,attribute);

            }
            else {
                if (strings.find(attribute) != strings.end()){
                    relation = relation.select2(i, variables[attribute]);
                } else{
                    variables.insert({attribute, i});
                    strings.insert(attribute);
                    variable_order.push_back(attribute);
                    int_order.push_back(i);
                }
//                    string empty = "empty";
//                    int integer = 100000000;
//                    variables.insert({integer,empty});
//                if (variables.empty()){
//                variables.insert({i, attribute});
//                strings.insert(attribute);
//                } else {
//                    for (const auto &variable: variables) {
//                        int num = variable.first;
//                        string str = variable.second;
//                        //                    auto it = variables.find(integer);
//                        //                    variables.erase(it);
//                        if (strings.find(attribute) != strings.end()) {
//                            relation = relation.select2(i, num);
//                        } else {
//                            variables.insert({i, attribute});
//                            strings.insert(attribute);
//                        }
//                    }
//                }
            }
        }
        vector<int> nums;
        vector<string> names;
        for (const auto &variable: variables) {
            int num = variable.second;
            string str = variable.first;
            nums.push_back(num);
            names.push_back(str);
        }
        stringstream out;
        if (strings.empty() && !relation.getTuples().empty()){
            string number = to_string(relation.getTuples().size());
            out << p.toString() << "? Yes" << "(" << number << ")" << endl;
        } else {
            relation = relation.project(int_order, attributes);
            relation = relation.rename(variable_order);
            string result;
            string num;
            if (relation.getTuples().empty()) {
                result = "No";
            } else {
                string number = to_string(relation.getTuples().size());
                num = "(" + number + ")";
                result = "Yes" + num;
            }
            out << p.toString() << "? " << result << endl;

            for (auto &tuple: relation.getTuples()) {
                out << "  " << tuple.toString(relation.getScheme()) << endl;
            }
        }
        cout << out.str();

    }







};


#endif //PROJECT3_INTERPRETER_H
