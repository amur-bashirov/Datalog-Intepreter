//
// Created by deck on 7/8/24.
//

#ifndef PROJECT_1_PARSER_H
#define PROJECT_1_PARSER_H
#pragma once
#include "Scanner.h"
#include <vector>
#include "Token.h"
#include <iostream>
#include <sstream>
#include <utility>
#include <set>
#include <algorithm>


class Parameter{
private:
    string parameter;
    bool isconst = false;
public:
    Parameter(string parameter): parameter(parameter){
        if (parameter[0] == '\''){
            this->isconst = true;
    }
}
    std::string toString() const {
        std::stringstream out;
        out<< parameter;
        return out.str();
    }

    const string &getParameter() const {
        return parameter;
    }

    bool isIsconst() const {
        return isconst;
    }
};



class Predicate{
private:
    std::string id;
    std::vector<Parameter> parameters;
    //std::vector<pair<string, bool>> _pairs;
public:
    const string &getId() const {
        return id;
    }

    const vector<Parameter> &getParameters() const {
        return parameters;
    }

    Predicate(std::string id, std::vector<Parameter> para){
        this->id = id;
        this->parameters = para;

    }

    std::string toString() const {
        std::stringstream out;
        out << id << "(";
        for (size_t i = 0; i < parameters.size(); ++i) {
            out << parameters[i].toString();
            if (i < parameters.size() - 1) {
                out << ",";
            }
        }
        out << ")";
        return out.str();
    }

};






class Rule1{
private:
    Predicate head_predicate;
    std::vector<Predicate> rules;
public:
    Rule1(const Predicate& head_predicate, const std::vector<Predicate>& rules)
        : head_predicate(head_predicate), rules(rules) {}
    const std::vector<Predicate>& getRules() const {
        return rules;
    }

    const Predicate &getHeadPredicate() const {
        return head_predicate;
    }
    void addBodyPredicate(Predicate predicate){
        rules.push_back(predicate);
    }

    std::string toString() const {
        std::stringstream out;
        out << head_predicate.toString() << " :- ";

        for (size_t i = 0; i < rules.size(); ++i) {
            out << rules[i].toString();
            if (i < rules.size() - 1) {
                out << ",";
            }
        }
        out << ".";
        return out.str();
    }
};

class Parser {
private:
    std::vector<Token> tokens;
    std::vector<Predicate> schemes;
    std::vector<Predicate> facts;
    std::vector<Rule1> Rules;
    std::vector<Predicate> queries;
    std::set<std::string> domainSet;

public:
    Parser() {}
    Parser(const std::vector<Token>& tokens) : tokens(tokens) { }

    TokenType tokenType() const {
        return tokens.at(0).getType();
    }
    void advanceToken() {
        tokens.erase(tokens.begin());
    }
    void throwError() {
        throw tokens.at(0);
        //throw tokens.at(0).getType();
       // std::cout << "error" << endl;
    }
    void match(TokenType t) {
        //cout << "match: " << t << endl;
        TokenType p = tokens.at(0).getType();
        if (p == t ){
            advanceToken();
        }
        else{
            //std::cerr << "Syntax error: Invalid TokenType, expected: " << t << "'" << std::endl;
            throwError();
        }

    }
    void parameter(){
        if (tokenType() == STRING){

            match(STRING);
        }
        if (tokenType() == ID){
            match(ID);
        }
    }
    void predicate(std::vector<Predicate>& predicates){
        if (tokenType() == ID){
            string name = tokens.at(0).getValue();
            match(ID);
            match(LEFT_PAREN);
            std::vector<std::string> parameters;
            parameters.push_back(tokens.at(0).getValue());
            parameter();
            parameterList(parameters);
            std::vector<Parameter> new_parameters;
            for (int i = 0; i < parameters.size();i++){
                auto token = tokenType();
                if (tokenType() == STRING){
                    Parameter parameter = Parameter(parameters[i]);
                    new_parameters.push_back(parameter);
                } else{
                    Parameter parameter = Parameter(parameters[i]);
                    new_parameters.push_back(parameter);
                }
            }
            Predicate p(name,new_parameters);
            predicates.push_back(p);
            match(RIGHT_PAREN);
        }
    }
    void headPredicate(Predicate& h_predicate){
        if (tokenType() == ID){
            string name = tokens.at(0).getValue();
            match(ID);
            match(LEFT_PAREN);
            std::vector<std::string>parameters;
            parameters.push_back(tokens.at(0).getValue());
            match(ID);
            idList(parameters);
            std::vector<Parameter> new_parameters;
            for (int i = 0; i < parameters.size();i++){
                if (tokenType() == STRING){
                    Parameter parameter = Parameter(parameters[i]);
                    new_parameters.push_back(parameter);
                } else{
                    Parameter parameter = Parameter(parameters[i]);
                    new_parameters.push_back(parameter);
                }
            }
            h_predicate = Predicate(name, new_parameters);
            match(RIGHT_PAREN);

        }
    }
    void parameterList(std::vector<std::string>& parameters){
        if (tokenType() == COMMA){
            match(COMMA);
            parameters.push_back(tokens.at(0).getValue());
            parameter();
            parameterList(parameters);

        }
    }
    void idList(std::vector<std::string>& names) {
        if (tokenType() == COMMA) {
            match(COMMA);
            names.push_back(tokens.at(0).getValue());
            match(ID);
            idList(names);
        }
    }
    void stringList(std::vector<std::string>& strings){
        if (tokenType() == COMMA){
            match(COMMA);
            strings.push_back(tokens.at(0).getValue());
            //TokenType token = tokens.at(0).getType()
           // _tokens.push_back(token);
            match(STRING);
            stringList(strings);
        }
    }
    void predicateList(std::vector<Predicate>& predicates){
        if (tokenType() == COMMA){
            match(COMMA);
            predicate(predicates);
            predicateList(predicates);
        }
    }
    void schemeList(){
        if (!tokens.empty() && tokenType() == ID){
            scheme();
            schemeList();
        }
        //is it correct implementation?
    }
    void factList(){
        if (!tokens.empty() && tokenType() == ID){
            fact();
            factList();
        }
    }
    void ruleList(){
        if (!tokens.empty() && tokenType() == ID){
            rule();
            ruleList();
        }
    }
    void queryList(){
        if (!tokens.empty() && tokenType() == ID){
            query();
            queryList();
        }
    }
    void scheme(){

            std::string name = tokens.at(0).getValue();
            match(ID);
            match(LEFT_PAREN);
            std::vector<std::string> names;
            names.push_back(tokens.at(0).getValue());
            match(ID);
            idList(names);
            std::vector<Parameter> new_parameters;
            for (int i = 0; i < names.size();i++){
                if (tokenType() == STRING){
                    Parameter parameter = Parameter(names[i]);
                    new_parameters.push_back(parameter);
                } else{
                    Parameter parameter = Parameter(names[i]);
                    new_parameters.push_back(parameter);
                }
            }
            Predicate p(name, new_parameters);
            schemes.push_back(p);
            match(RIGHT_PAREN);

    }
    void fact(){

            std::string name = tokens.at(0).getValue();
            match(ID);
            match(LEFT_PAREN);
            std::vector<std::string> strings;
            strings.push_back(tokens.at(0).getValue());
            //std::vector <TokenType> _tokens;
            //_tokens.push_back(tokenType());
            match(STRING);
            //TokenType empty_token;
            stringList(strings);
            std::vector<Parameter> new_parameters;
            for (int i = 0; i < strings.size();i++){
                //if (_tokens[i] == STRING){
                    Parameter parameter = Parameter(strings[i]);
                    new_parameters.push_back(parameter);
                //} else{
                   // Parameter parameter = Parameter(strings[i], false);
                   // new_parameters.push_back(parameter);
                //}
            }
            Predicate p(name, new_parameters);
            facts.push_back(p);
            match(RIGHT_PAREN);
            match(PERIOD);

    }
    void rule(){

            std::vector<Predicate> predicates;
            string name;
            std::vector<string>strings;
            std::vector<Parameter> new_parameters;
            for (int i = 0; i < strings.size();i++){
                if (tokenType() == STRING){
                    Parameter parameter = Parameter(strings[i]);
                    new_parameters.push_back(parameter);
                } else{
                    Parameter parameter = Parameter(strings[i]);
                    new_parameters.push_back(parameter);
                }
            }
            Predicate h_predicate(name,new_parameters);
            headPredicate(h_predicate);
            match(COLON_DASH);
            predicate(predicates);
            predicateList(predicates);//??
//            string empty;
//            string empties;
//            Predicate p(empty, empt)
//            for (const auto& predicate : predicates){
//                rules.push_back(predicate);
//            }
            Rule1 p(h_predicate, predicates);
            Rules.push_back(p);
            match(PERIOD);


    }
    void query(){
        //do i need to check if it is predicate? How?

            std::vector<Predicate> predicates;
            predicate(predicates);
            for (const auto& predicate : predicates){
                queries.push_back(predicate);
            }
            match(Q_MARK);

    }
    void datalogProgram(){
        if (!tokens.empty()){
            match(SCHEMES);
            match(COLON);
            scheme();
            schemeList();
            match(FACTS);
            match(COLON);
            factList();
            match(RULES);
            match(COLON);
            ruleList();
            match(QUERIES);
            match(COLON);
            query();
            queryList();
            match(END);
        }
        else{
            throwError();
        }
    }
    std::string toString() {
        std::stringstream out;

        // Print schemes
        out << "Schemes(" << schemes.size() << "):" << std::endl;
        for (const auto& scheme : schemes) {
            out << "  " << scheme.toString() << std::endl;
        }

        // Print facts
        out << "Facts(" << facts.size() << "):" << std::endl;
        for (const auto& fact : facts) {
            out << "  " << fact.toString() << "." << std::endl;
        }

        // Print rules
        out << "Rules(" << Rules.size() << "):" << std::endl;
        for (const auto& rule : Rules) {
            out << "  " << rule.toString() << std::endl;
        }

        // Print queries
        out << "Queries(" << queries.size() << "):" << std::endl;
        for (const auto& query : queries) {
            out << "  " << query.toString() << "?" << std::endl;
        }



        return out.str();
    }
//    void collectDomain() {
//        for (const auto& fact : facts) {
//            for (const auto& param : fact.getParameters()) {
//                if (isStringConstant(param)) {
//                    domainSet.insert(param);
//                }
//            }
//        }

//        for (const auto& query : queries) {
//            for (const auto& param : query.getParameters()) {
//                if (isStringConstant(param)) {
//                    domainSet.insert(param);
//                }
//            }
//        }


//    bool isStringConstant(const std::string& param) const {
//        return !param.empty() && param[0] == '\'' && param.back() == '\'';
//    }
//
//    void printDomain() const {
//        std::vector<std::string> domain(domainSet.begin(), domainSet.end());
//        std::sort(domain.begin(), domain.end());
//
//        std::cout << "Domain(" << domain.size() << "):" << std::endl;
//        for (const auto& value : domain) {
//            std::cout << "  " << value << std::endl;
//        }
//    }
    void parse() {
        try {
            datalogProgram();

            //std::cout << "Success!" << std::endl;

//            std::cout << toString();
//            collectDomain();
//            printDomain();
;

        } catch (Token token) {
            std::cout << "Failure!" << std::endl;
            std::cout << token.toString() << std::endl;
        }
    }

    const vector<Predicate> &getSchemes() const {
        return schemes;
    }

    const vector<Predicate> &getFacts() const {
        return facts;
    }

    const vector<Rule1> &getRules() const {
        return Rules;
    }


    const vector<Predicate> &getQueries() const {
        return queries;
    }
};





#endif //PROJECT_1_PARSER_H
