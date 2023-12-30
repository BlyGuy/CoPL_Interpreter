# λ-calculus: Type-checker

## Student Credits

Class: *Concepts of Programming Languages (2023-2024)*
Group: *Mijn Fantastische Groep*

- Marc Hoek (s3157520)
- Joshua J. Lelipaly (s3329941)
- Noah I. van Laarhoven (s3323854)

## System Details

Compiler versions:

- gcc (GCC) 13.2.1 20231205 (Red Hat 13.2.1-6)
- gcc (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0

Operating Systems:

- Fedora Linux 39 (KDE Plasma) x86_64
- Ubuntu 22.04.3 LTS on Windows 10 x86_64

## Known Problems

The program has all of the required functionality but lacks some optional features that were outlined in the assignment document as **may**-features

The program does not...

- accept Unicode lambda-characters (λ) as of yet
- support the dot-operator as of yet
- support the use of characters other than alphanumeric ASCII-characters, `(`, `)`, `\\`, and whitespace in expression strings
- output an expression with the least amount of whitespace and parentheses as of yet

## Quick start

Compile the program and run it using the following commands:

```bash
make
./type-checker path/to/FileWithJudgements.txt
```

where FileWithJudgements.txt is a text-file containing a judgement on each line in the file.

## Program Description

This program reads a judgement from a file into a character string and lexically analyses the judgement
into an abstract syntax tree on compliance with the grammar of the **sym*lambda calculus.
It also checks whether the judgement is derivable in the type system.
The simply typed lambda calculus grammar can be defined using the following Backus-Naur grammar:

```html
<judgement> ::= <expr> ':' <type>
<expr> ::= <lvar> | '(' <expr> ')' | '\' <lvar> '^' <type> <expr> | <expr> <expr>
<type> ::= <uvar> | '(' <type> ')' | <type> '->' <type>
```

`<lvar>` is a string of alphanumerical characters that starts with a lowercase letter
`<uvar>` is also a string of alphanumerical characters but this one starts with an uppercase letter

Examples of accepted strings include:

- (\x^A x):(A -> A)
- (\x^B (\x^A x)):(B -> (A -> A))
- (\x^A (\y^(A->B) (y ((\x^A x) x)))):(A -> ((A -> B) -> B))
- (\y^A (\x^(A -> (C -> A)) (x y))) : (A -> (A -> C -> A) -> C -> A)

More text files with example-strings can be found in the positive.tar.gz(with accepted strings) and negative.tar.gz(rejected strings) compressed archives in this submission.

The program reads given input until a the program has accepted every judgement in the input-file or until an error has been found. Until then the program will print out the lines that came out of the analysis succesfully in its own format. If a syntax/grammar/derivability error has been found the program will print a (hopefully) helpful error message and halt.

*Note:* A string that contains characters apart from the following (alphanumeric characters, '(', ')', '\\', '^', ':', "->" and whitespace) will generate an error upon getting detected and will stop the program.
