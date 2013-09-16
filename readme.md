Songoku
-------

A simple C program that applies a couple of naive reduction techniques to resolve sudokus.

I wrote this program in 2005 to automatically solve sudokus for a "El Mundo" newspaper sudoku contest.

Nothing very fancy...


How to run it
-------------

To add a sudoku, just input the starting number from stdin (for example through the terminal) in the following way:

    column row number

You can also feed a file:

    ./songoku < sudoku/19

The sudoku files shows an evil sudoku that will defeat the program:

    ./songoku < sudoku/evil

Aaaand... that's all!
