//
// Created by deck on 7/2/24.
//
//#ifndef TOKEN_H
//#define TOKEN_H
#pragma once
#include <string>
using namespace std;
#include <vector>
#include <cctype>
#include <sstream>
#include "Token.h"

class Scanner {
private:
    string input;
    int line;
    std::vector<Token> _tokens;
public:
    Scanner(const string& input) : input(input), line(1) {}




    Token scanToken() {
        while (!input.empty()) {
            if (isspace(input.at(0))) {
                if (input.at(0) == '\n') {
                    line++;
                }
                input = input.substr(1);
            } else if (input.at(0) == ',') {
                TokenType type = COMMA;
                string value = ",";
                input = input.substr(1);
                return Token(type, value, line);
            } else if (input.at(0) == '.') {
                TokenType type = PERIOD;
                string value = ".";
                input = input.substr(1);
                return Token(type, value, line);
            } else if (input.at(0) == '?') {
                TokenType type = Q_MARK;
                string value = "?";
                input = input.substr(1);
                return Token(type, value, line);
            } else if (input.at(0) == '(') {
                TokenType type = LEFT_PAREN;
                string value = "(";
                input = input.substr(1);
                return Token(type, value, line);
            } else if (input.at(0) == ')') {
                TokenType type = RIGHT_PAREN;
                string value = ")";
                input = input.substr(1);
                return Token(type, value, line);
            } else if (input.at(0) == ':') {
                if (input.size() > 1 && input.at(1) == '-') {
                    TokenType type = COLON_DASH;
                    string value = ":-";
                    input = input.substr(2);
                    return Token(type, value, line);
                } else {
                    TokenType type = COLON;
                    string value = ":";
                    input = input.substr(1);
                    return Token(type, value, line);
                }
            } else if (input.at(0) == '*') {
                TokenType type = MULTIPLY;
                string value = "*";
                input = input.substr(1);
                return Token(type, value, line);
            } else if (input.at(0) == '+') {
                TokenType type = ADD;
                string value = "+";
                input = input.substr(1);
                return Token(type, value, line);
            } else if (isalpha(input.at(0))) {
                size_t end = 0;
                while (end < input.size() && (isalnum(input[end]) || input[end] == '_')) {
                    end++;
                }
                string value = input.substr(0, end);
                input = input.substr(end);

                if (value == "Queries") return Token(QUERIES, value, line);
                if (value == "Rules") return Token(RULES, value, line);
                if (value == "Schemes") return Token(SCHEMES, value, line);
                if (value == "Facts") return Token(FACTS, value, line);

                return Token(ID, value, line);
            } else if (input.at(0) == '\'') {
                size_t end = 1;
                std::string value = "'";
                bool terminated = false;
                while (end < input.size()) {
                    if (input[end] == '\'') {
                        value += '\'';
                        if (end + 1 < input.size() && input[end + 1] == '\'') {
                            value += '\'';
                            end += 2;
                        } else {
                            terminated = true;
                            end++;
                            break;
                        }
                    } else {
                        if (input[end] == '\n') {
                            line++;
                        }
                        value += input[end];
                        end++;
                    }
                }
                input = input.substr(end);
                if (terminated) {
                    return Token(STRING, value, line);
                } else {
                    return Token(UNDEFINED, value, line);
                }
            } else if (input.at(0) == '#') {
                std::string value = "#";
                size_t end = 1;
                if (end < input.size() && input[end] == '|') {
                    value += '|';
                    end++;
                    bool terminated = false;
                    while (end < input.size()) {
                        if (input[end] == '|' && end + 1 < input.size() && input[end + 1] == '#') {
                            value += "|#";
                            end += 2;
                            terminated = true;
                            break;
                        } else {
                            if (input[end] == '\n') {
                                line++;
                            }
                            value += input[end];
                            end++;
                        }
                    }
                    input = input.substr(end);
                    if (terminated) {
                        //return Token(COMMENT, value, line);
                    } else {
                        //return Token(UNDEFINED, value, line);
                    }
                } else {
                    while (end < input.size() && input[end] != '\n') {
                        value += input[end];
                        end++;
                    }
                    input = input.substr(end);
                    //return Token(COMMENT, value, line);
                }
            } else {
                std::string value(1, input.at(0));
                input = input.substr(1);
                return Token(UNDEFINED, value, line);
            }
        }
        return Token(END, "", line);
    }
};

//#endif // TOKEN_H

