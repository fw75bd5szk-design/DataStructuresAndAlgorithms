# DataStructuresAndAlgorithms

# CS 300 Data Structures and Algorithms

## What was the problem you were solving in the projects for this course?

The projects focused on building a course advising program for ABCU. The program needed to load course information from a file, print courses in alphanumeric order, and let users look up a course and its prerequisites. Project One focused on planning and comparing data structures, while Project Two involved building the working program in C++.

## How did you approach the problem?

I broke the program into smaller tasks: reading the file, organizing the courses, searching, and printing the results. I considered vectors, hash tables, and binary search trees before using a binary search tree for my final program. An in-order traversal lets the program print courses in order. This helped me understand why choosing a data structure matters. Each option has tradeoffs in search time, memory use, and how easily it produces sorted output.

## How did you overcome roadblocks?

One challenge was getting the input file to load correctly in Visual Studio. I worked through the file path issue and checked that I was entering the path at the correct prompt. I also worked on checking course records and prerequisites before inserting courses into the tree. Testing one menu option at a time helped me narrow down problems without trying to fix everything at once.

## How has this project expanded your approach to designing software?

This project got me thinking more about how the information should be organized before I start writing code. I used to focus more on getting the program running, but now I also consider how the design affects searching, sorting, and handling bad input. Working full time in HVAC/R, I am used to troubleshooting problems one step at a time. That same approach helped me work through this program.

## How has this project changed the way you write maintainable, readable, and adaptable programs?

I became more intentional about separating responsibilities into functions. File loading, searching, menu handling, and printing each have their own place in the program. Clear names and comments make the code easier to follow when I come back to it. I also included cleanup for the tree's allocated memory. These habits make it easier to change one part of the program without having to rewrite everything.
