# λ-calculus: syntax

## Student Credits

- Marc Hoek (s3157520)
- Joshua J. Lelipaly (s3329941)

## Known Problems

The program has all of the required functionality but lacks some optional features that were outlined in the assignment document as **may**-features

The program does not...

- accept Unicode lambda-characters (λ) as of yet
- support the dot-operator as of yet
- support the use of characters other than alphanumeric ASCII-characters, `(`, `)`, `\\`, and whitespace in expression strings
- output an expression with the least amount of whitespace and parentheses as of yet

## Assignment Deviations

We also chose to implement a way for users to quit the program without the use of Ctrl+C.
This can be done by typing the phrase `exit`.
This is (probably) the only way for a normal user (no file input) to exit the program with an exit code of 0 without the use of Ctrl+C.

## Quick start

Compile the program and run it using the following commands:

```bash
make
./syntax
```

To run the program with a particular text-file as input, use one of the following commands:

```bash
./syntax < path/to/file.txt
cat path/to/file.txt | ./syntax
```

## Program Description

This program lexically analyses and parses a piece of text on compliance with the grammar of the lambda calculus.
The lambda calculus grammar can be defined using the following Backus-Naur grammar:

```html
<expr> ::== <var> | '(' <expr> ')' | '\' <var> <expr> | <expr> <expr>
```

`<var>` is a string of alphanumerical characters that doesn't start with a digit

Examples of accepted strings include:

- myVar (a (b (c d e f g) h) i j k) l m
- (my123Var my456Var) (\ x abcd)
- myVar \  urVar ( a b c ) \a theirVar letters123
- justAVar

More text files with example-strings can be found in the positive.tar.gz(with accepted strings) and negative.tar.gz(rejected strings) compressed archives in this submission.

The program reads given input until the user exits the program, until a the program has accepted every expression in an input-file or until an error has been found. Until then the program will print out the lines that came out of the analysis succesfully in its own format. If a syntax/grammar error has been found the program will print a (hopefully) helpful error message and halt.

*Note:* A string that contains characters apart from the following (alphanumeric characters, '(', ')', '\\' and whitespace) will generate an error upon getting detected and will stop the program.
