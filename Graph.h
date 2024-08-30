//
// Created by bashirov on 8/5/24.
//

#ifndef PROJECT3_GRAPH_H
#define PROJECT3_GRAPH_H
#pragma once
#include "Node.h"
#include <set>
#include <map>
#include <stack>
#include <sstream>
#include <algorithm>
#include <iostream>
using namespace std;


class Graph {

private:

    map<int,Node> nodes;
    set <int> visited;

public:

    Graph(int size) {
        for (int nodeID = 0; nodeID < size; nodeID++)
            nodes[nodeID] = Node();
    }

    void addEdge(int fromNodeID, int toNodeID) {
        nodes[fromNodeID].addEdge(toNodeID);
    }
    string toString() {
        stringstream out;
        for (auto &pair: nodes) {
            int nodeID = pair.first;
            Node node = pair.second;
            out << "R" << to_string(nodeID) << ":" << node.toString();
        }
        return out.str();
    }

    stack <int> dfs_tree(int node, stack <int> post_order){
        if (visited.find(node) != visited.end()){
            return post_order;
        }
        visited.insert(node);
        for (auto &_Node : nodes[node].getAdjacentNodeIDs()){
            if (visited.find(_Node) == visited.end()){
                post_order = dfs_tree(_Node, post_order);
            }
        }
        post_order.push(node);
        return post_order;

    }

    stack <int> dfs_Forest(Graph graph){
        stack <int> post_order = stack <int>();
        set <int> empty_set = set<int>();
        visited = empty_set;
        for (auto &node: graph.getNodes()){
            if (visited.find(node.first) == visited.end()){
                post_order = dfs_tree(node.first, post_order);
            }
        }
        return post_order;
    }

    vector <int> dfs_tree_scc(int node, vector <int> SCC){
        if (visited.find(node) != visited.end()){
            return SCC;
        }
        visited.insert(node);
        for (auto &_Node : nodes[node].getAdjacentNodeIDs()){
            if (visited.find(_Node) == visited.end()){
                SCC = dfs_tree_scc(_Node, SCC);
            }
        }
        SCC.push_back(node);
        return SCC;
    }

    vector <vector<int>> dfs_Forest_scc(Graph graph, stack<int> post_order){
        vector <vector<int>> result;
        vector <int> SCC = vector <int>();
        set <int> empty_set = set<int>();
        visited = empty_set;
        for (int i = 0; i < graph.getNodes().size(); i++){
            if (visited.find(post_order.top()) == visited.end()){
                vector <int> unit;
                int num = post_order.top();
                SCC = dfs_tree_scc(post_order.top(), unit);
                post_order.pop();
                result.push_back(SCC);
            }
        }
        return result;
    }

    const map<int, Node> &getNodes() const {
        return nodes;
    }
};


#endif //PROJECT3_GRAPH_H
