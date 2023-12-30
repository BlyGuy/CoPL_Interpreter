<!-- # This README is perfect, DONT CHANGE🗿 true not clickbait

## Reasons

1. Just because
2. Refer back reason 1
3. [**THE BEST REASON**](https://youtu.be/dQw4w9WgXcQ?si=OC_GlE4yt-o7mba8) -->

# λ-calculus: Interpreter

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
- have configurable reduction strategies

## Assignment Deviations

There are no deviations to the assignment.

## Quick start

Compile the program and run it using the following commands:

```bash
make
./interpreter path/to/FileWithExpression.txt
```

where FileWithExpression.txt is a text-file containing an expression in the lambda calculus language.

## Program Description

This program lexically analyses and parses a piece of text from a text file (one expression per file) on compliance with the grammar of the lambda calculus.
If applicable the program will perform beta-reductions to the expression and if a beta reduction would lead to a captured variable
then the program will perform alpha-conversions. The program will continue to beta-reduce until no longer possible or when
a limit of 1000 has been reached.
The lambda calculus grammar can be defined using the following Backus-Naur grammar:

```html
<expr> ::== <var> | '(' <expr> ')' | '\' <var> <expr> | <expr> <expr>
```

`<var>` is a string of alphanumerical characters that doesn't start with a digit

Examples of accepted strings include:

- \x\y(x\z y)
- (\x x)(\y y)
- (\x \y x)(\z y)
- (\x y)((\x (x x))(\x (x x)))
- (\x x x)(\x x x)

More text files with example-expressions can be found in the positive.tar.gz(with accepted expressions) and negative.tar.gz(rejected expressions) compressed archives in this submission.

The program reads given input from a textfile and exits when the program is able to reduce the expression successfully or when an error has been found. Until then the program will print out the lines that came out of the reduction process successfully in its own format. If a syntax/grammar/reduction error has been found the program will print a helpful error message and halt.

*Note:* A string that contains characters apart from the following (alphanumeric characters, '(', ')', '\\' and whitespace) will generate an error upon getting detected and will stop the program.

## Beta Reductions and Alpha Conversions

Our method of implementing beta reductions is top-down. We begin at the root and we reduce its children recursively. When we have found a lambda we firstly check
for bound variables and compare them to see if they form conflicts. If they do we resolve them by performing alpha-conversions. The way we implemented alpha-conversions is we simply append a substring 'A' to the conflicting variable and loop through all letters of the alphabet for this substring until there is a substring that, when appended to the name of the conflicting variable, would not be included in the list of bound variables.

Next we perform beta reductions and reconnect the nodes of the tree.
We also decided to make a function to clean the tree to reduce the amount of excessive brackets and save memory.
