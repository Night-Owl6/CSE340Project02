/*
 * Copyright (C) Mohsen Zohrevandi, 2017
 *               Rida Bazzi 2019
 * Do not share this file with anyone
 */
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "lexer.h"
#include <set>
#include <vector>
#include <string>
#include <map>

using namespace std;

struct Rule {
    string LHS;
    vector<string> RHS;
};

vector<Rule> rules;
LexicalAnalyzer lexer;
vector<string> terminals;
vector<string> temp_nonterminals;
vector<string> nonterminals;
vector<string> unordered_terminals;

// read grammar
void ReadGrammar()
{
    Token token = lexer.GetToken();

    //parse the input grammar and build the rule list
    while (token.token_type != HASH) {
        Rule r;
        r.LHS = token.lexeme;   // the lexeme of the rule before ARROW
        
        // IF the lexeme is not in nonterminal list, add it to the nonterminal list
        if(find(temp_nonterminals.begin(), temp_nonterminals.end(), r.LHS) == temp_nonterminals.end())      // changed to temp_nonterminals.end() form nonterminals.end()
            temp_nonterminals.push_back(r.LHS);
        
        //IF the lexeme is not in unordered_terminals list, add it to the unordered_terminals list
        if (find(unordered_terminals.begin(), unordered_terminals.end(), r.LHS) == unordered_terminals.end())
            unordered_terminals.push_back(r.LHS);

        token = lexer.GetToken();   // consume the next token
                
        if (token.token_type == ARROW) {
            // Parse Right-hand-side
            token = lexer.GetToken();
            while (token.token_type != STAR && token.token_type != HASH) {
                //if lexeme is not in unordered_terminals list, add it to the unordered_terminals list
                if(find(unordered_terminals.begin(), unordered_terminals.end(), token.lexeme) == unordered_terminals.end())
                    unordered_terminals.push_back(token.lexeme);
                
                r.RHS.push_back(token.lexeme);
                token = lexer.GetToken();

                if(token.token_type != STAR) {
                    while (token.token_type != STAR) {
                        if (find(unordered_terminals.begin(), unordered_terminals.end(), token.lexeme) == unordered_terminals.end())
                            unordered_terminals.push_back(token.lexeme);
                        
                        r.RHS.push_back(token.lexeme);
                        token = lexer.GetToken();
                    }
                } else {
                    break;
                }
            }
            rules.push_back(r);
        } else {
            cout << "1Syntax error: I did not catch the arrow";
            return;
        }
        token = lexer.GetToken();
    }

    // build the terminal and nonterminal list
    for (const auto& t : unordered_terminals) {
        if (find(temp_nonterminals.begin(), temp_nonterminals.end(), t) == temp_nonterminals.end()) {
            terminals.push_back(t);
        }
        if(find(temp_nonterminals.begin(), temp_nonterminals.end(), t) != temp_nonterminals.end()) {
            nonterminals.push_back(t);
        }
    }
}

// Task 1
void printTerminalsAndNoneTerminals()
{
    for (const auto& t : terminals) {
        cout << t << " ";
    }

    for (const auto& t : nonterminals) {
        cout << t << " ";
    }
}

// Task 2
void RemoveUselessSymbols()
{
    map<string, bool> UniverseSymbols;
    map<string, bool> UniReachableSymbols;
    vector<Rule> RulesGen;

    //step 1: set all terminals as generating symbols
    for (const auto& t : terminals) {
        UniverseSymbols[t] = true;  // add terminals as their value as generating symbols
        UniReachableSymbols[t] = false; // step 4 - set all non-terminals as non-reachable symbols
    }
    for (const auto& t : nonterminals) {
        UniverseSymbols[t] = false;  // add non-terminals set their value as non generating symbols
        UniReachableSymbols[t] = false; // step 4 - set all non-terminals as non reachable symbols
    }

    bool changed = true;    // tracker variable if something is changed
    while (changed) {
            changed = false; 
            for( auto& r : rules) {
                if (!UniverseSymbols[r.LHS]) { // check if LHS symbol is already generating
                    if (r.RHS.empty()) {    // if RHS of rule is empty then LHS is generating - LHS -> epsilon
                        UniverseSymbols[r.LHS] = true;
                        changed = true;
                    } else {
                        bool allGenerating = true;
                        for (const auto& s : r.RHS) {
                            if (!UniverseSymbols[s]) {
                                allGenerating = false;
                                break;
                            }
                        }
                        if (allGenerating) {
                            UniverseSymbols[r.LHS] = true;
                            changed = true;
                        }
                    }
                }
            }
        }

    // 3) Remove all non-generating symbols - RulesGen that only have rules that are generating
    for (const auto& r : rules) {
        if(UniverseSymbols[r.LHS]) {
            bool allGenerating = true;
            for (const auto& s : r.RHS) {
                if (!UniverseSymbols[s]) {
                    allGenerating = false;
                    break;
                }
            }
            if (allGenerating) {
               RulesGen.push_back(r);
            }
        }
    }

    // 4) set all terminals and non-terminals as non-reachable symbols - done earlier

    // 5) set start symbol as reachable
    string startSymbol = RulesGen[0].LHS;
    UniReachableSymbols[startSymbol] = true; // set start symbol as reachable

    // 6) loop through all the rules
    for (const auto& r : rules) {
        if (r.LHS.compare(startSymbol) == 0 || UniReachableSymbols[r.LHS]) {
            for (const auto& r2 : r.RHS) {
                UniReachableSymbols[r2] = true; // set all RHS as reachable
            }
        } else {
            // step 7 - if that rule is not reachable, remove it from vector<Rule>RulesGen
            for (auto it = RulesGen.begin(); it != RulesGen.end(); ++it) {
                if (it->LHS == r.LHS && it->RHS == r.RHS) {
                    RulesGen.erase(it);
                    break;
                }
            }
        }
    }

    for (auto it = RulesGen.begin(); it != RulesGen.end(); ++it) {
        if(it->RHS.empty()) {
        it->RHS.push_back("#");
        }
    }

    for (const auto& t : RulesGen) {
        cout << t.LHS << " -> ";
        for (const auto& s : t.RHS) {
            cout << s << " ";
        }
        cout << endl;
    }

}

// Task 3
void CalculateFirstSets()
{
    cout << "3\n";
}

// Task 4
void CalculateFollowSets()
{
    cout << "4\n";
}

// Task 5
void CheckIfGrammarHasPredictiveParser()
{
    cout << "5\n";
}
    
int main (int argc, char* argv[])
{
    int task;

    if (argc < 2)
    {
        cout << "Error: missing argument\n";
        return 1;
    }

    /*
       Note that by convention argv[0] is the name of your executable,
       and the first argument to your program is stored in argv[1]
     */

    task = atoi(argv[1]);
    
    ReadGrammar();  // Reads the input grammar from standard input
                    // and represent it internally in data structures
                    // ad described in project 2 presentation file

    switch (task) {
        case 1: printTerminalsAndNoneTerminals();
            break;

        case 2: RemoveUselessSymbols();
            break;

        case 3: CalculateFirstSets();
            break;

        case 4: CalculateFollowSets();
            break;

        case 5: CheckIfGrammarHasPredictiveParser();
            break;

        default:
            cout << "Error: unrecognized task number " << task << "\n";
            break;
    }
    return 0;
}

/*
NT              UD                  NT           T
decl            decl                decl        colon
idList          idlist              idlist      ID
idList1         colon               idlist1     COMMA
                ID
                idlist1
                COMMA

*/