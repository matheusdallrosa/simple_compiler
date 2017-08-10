## Simple Compiler
This is a simple compiler for a given simple language.

## Lexical:
First use the determ.cpp to create a new deterministic automata, the input file for should be like this:

number_of_separators

separator_{0}, separator_{1}, ....... , separator_{number_of_separators}

number_of_tokens

number_of_lines_for_the_grammar_of_the_next_token

label_of_the_token

regular_grammar_of_the_token

A regular grammar should follow the example on the file: lexical/determ_input_sample.txt

## Sample for the determ.cpp input on the lexical folder.

The determ.cpp will output a deterministic automata on a file named qwer.csv on the current dir.

Compile and execute:
g++ determ.cpp -o determ

./determ < code.txt

The lexical_analysis.cpp receives a code as input, during it's execution the lexical_analysis.cpp loads the automata 
on the qwer.csv file after, make the lexical anlysis, and gives as output the tokens, the value of each token, 
and the line where the token was found.

Compile and execute:
g++ lexycal_analysis.cpp -o lexycal_analysis

./lexycal_analysis < code.txt > tokens.txt

Syntax:
The syntax.py loads a LALR table generated from gold parser, and without the HEAD and BOTTOM comments.

Execute:
python syntax.py < code.txt
