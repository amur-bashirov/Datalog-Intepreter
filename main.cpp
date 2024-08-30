#include <iostream>
#include "Scanner.h"
#include <fstream>
#include "Parser.h"
#include "Token.h"
#include "Scheme.h"
#include<stack>
#include "Graph.h"
#include "Interpreter.h"
#include "Node.h"
//#include "Database.h"
void processInput(const std::string& input) { //) {
    Scanner scanner(input);
    std::vector<Token> tokens;
    Token token = scanner.scanToken();
    tokens.push_back(token);
    while (token.getType() != END) {


        token = scanner.scanToken();
        tokens.push_back(token);
    }


    Parser p = Parser(tokens);

    p.parse();
    Interpreter interpret = Interpreter(p);
    vector<Predicate> queries = p.getQueries();
    interpret.rules_loop(p);
    cout << "Query Evaluation" << endl;
    for (auto &query : queries){
        interpret.evaluateQuery(query);
    }
}



 int main(int argc, char* argv[]) {
     std::ifstream inFile(argv[1]);
     std::string input((std::istreambuf_iterator<char>(inFile)),
                       std::istreambuf_iterator<char>());
     inFile.close();

//     std::string outputFileName(argv[2]);
//     std::ofstream outFile(argv[2]);

     processInput(input);


     return 0;
 }




