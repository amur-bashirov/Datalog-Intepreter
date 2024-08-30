//
// Created by deck on 6/29/24.
//

//#ifndef LAB1_TOKEN_H
//#define LAB1_TOKEN_H
#pragma once
#include <string>
#include <sstream>


enum TokenType {
    COMMA, PERIOD, Q_MARK, LEFT_PAREN, RIGHT_PAREN, COLON, COLON_DASH, MULTIPLY, ADD, SCHEMES,
    FACTS, RULES, QUERIES, ID, STRING, COMMENT, UNDEFINED, END
};
class Token{
private:
    TokenType type;
    int line;
    std::string value;
public:
    Token(TokenType type, std::string value, int line) : type(type), value(value), line(line){}
    std::string toString() const {
        std::stringstream out;
        out << "(" << typeName(type) << "," << "\"" << value << "\"" << "," << line << ")";
        return out.str();
    }
    std::string typeName(TokenType type) const {
        switch (type){
            case TokenType::COMMA:      return "COMMA";
            case TokenType::PERIOD:     return "PERIOD";
            case TokenType::Q_MARK:     return "Q_MARK";
            case TokenType::LEFT_PAREN: return "LEFT_PAREN";
            case TokenType::RIGHT_PAREN:return "RIGHT_PAREN";
            case TokenType::COLON:      return "COLON";
            case TokenType::COLON_DASH: return "COLON_DASH";
            case TokenType::MULTIPLY:   return "MULTIPLY";
            case TokenType::ADD:        return "ADD";
            case TokenType::SCHEMES:    return "SCHEMES";
            case TokenType::FACTS:      return "FACTS";
            case TokenType::RULES:      return "RULES";
            case TokenType::QUERIES:    return "QUERIES";
            case TokenType::ID:         return "ID";
            case TokenType::STRING:     return "STRING";
            case TokenType::COMMENT:    return "COMMENT";
            case TokenType::UNDEFINED:  return "UNDEFINED";
            case TokenType::END:        return "END";
            default:                    return "UNKNOWN";
        }
    }
    TokenType getType() const { return type; }

    const std::string getValue() const { return value; }

    int getLine() const { return line; }
};
//#endif //LAB1_TOKEN_H