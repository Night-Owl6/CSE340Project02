/*
 * Copyright (C) Mohsen Zohrevandi, 2017
 *               Rida Bazzi 2019
 * Do not share this file with anyone
 */
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "lexer.h"
#include "set"
#include <algorithm>
#include <vector>
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

/*
Input Format

Grammar ! Rule-list HASH
Rule-list ! Rule Rule-list | Rule
Id-list ! ID Id-list | ID
Rule ! ID ARROW Right-hand-side STAR
Right-hand-side ! Id-list | epsilon

ID = letter (letter + digit)*
STAR = '*'
HASH = #
ARROW = ->

Input Example:

decl -> idList colon ID *
idList -> ID idList1 *
idList1 -> *
idList1 -> COMMA ID idList1 *
#

Non-Terminals = { decl, idList, idList1 }
Terminals = { colon, ID, COMMA }

Task one simply outputs the list of terminals in the order in which they appear in the grammar rules followed by the list of
non-terminals in the order in which they appear in the grammar rules.

Output Example:
colon ID COMMA decl idList idList1

*/

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
    cout << "2\n";
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

//============================================================================

/*
Input Format
The following context-free grammar specifies the input format:

Grammar ! Rule-list HASH
Rule-list ! Rule Rule-list | Rule
Id-list ! ID Id-list | ID
Rule ! ID ARROW Right-hand-side STAR
Right-hand-side ! Id-list | epsilon

The input consists of a rule list. Each rule has a lefthand side which is an ID, which is followed by an arrow and is followed by
a sequence of zero more IDs and terminated with a STAR. The meaning of the input is explained in the Semantics section below.
The tokens used in the above grammar description are defined by the following regular expressions:

ID = letter (letter + digit)*
STAR = '*'
HASH = #
ARROW = ->

Where digit is the digits from 0 through 9 and letter is the upper and lower case letters a through z and A through Z.
Tokens are case-sensitive. Tokens are space separated and there is at least one whitespace character between any two successive
tokens. We provide a lexer with a geToken() function to recognize these tokens. You should use the provided lexer in you solution.
*/

/*
Input Example:

decl -> idList colon ID *
idList -> ID idList1 *
idList1 -> *
idList1 -> COMMA ID idList1 *
#

The list of non-terminal symbols in the order in which they appear in the grammar is:
Non-Terminals = { decl, idList, idList1 }
The list of terminal symbols in the order in which they appear in the grammar is:
Terminals = { colon, ID, COMMA }

Task one simply outputs the list of terminals in the order in which they appear in the grammar rules followed by the list of
non-terminals in the order in which they appear in the grammar rules.

Output Example:
colon ID COMMA decl idList idList1

*/

/*
NT              UD                  NT           T
decl            decl                decl        colon
idList          idlist              idlist      ID
idList1         colon               idlist1     COMMA
                ID
                idlist1
                COMMA

*/
