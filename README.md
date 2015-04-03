# compositor
PSET 4, CPSC 223, Yale University
My solution implemented linked lists to represent each 'row' of a final table that represented the characters stored at each 2D coordinate. Using a 2D array would fail large test cases, since some inputs may represent images at very far coordinates (eg. (10^6, 10^6)). A 2D array would allocate far too much memory than is necessary. A simple linked list representation wouldn't work well either: storing a character at (10^6, 10^6) would still take up 10^6 nodes to represent the 'space' characters preceding the target character. So my linked list implementation represents all characters given as inputs except 'space' characters, such that even if an input placed a '%' character at (10^6, 10^6), my linked list would skip allocating the spaces preceding the '%' and would allocate a single node of a linked list {data: '%', next: NULL, x: 10^6}. Then, when all inputs are received and the whole composite picture must be printed, my printPic function (via the missingSpaces calculation) prints out the necessary characters with the properly deduced implicit space characters between them, given that each character has an 'x' coordinate value.  

## 7.4.1 An ASCII art compositor

For this assignment you are to write a program that takes from stdin a sequence of instructions for pasting ASCII art pictures together, reads those pictures from files, and writes the combined picture to stdout.

Each instruction is of the form row column filename, suitable for reading with scanf("%d %d %s", &row, &col, filename);, where row and col are declared as ints and filename is a suitably large buffer of chars. Such an instruction means to paste the contents of file filename into the picture with each character shifted row rows down and column columns to the right of its position in file filename. When pasting an image, all characters other than space (' ', or ASCII code 32) overwrite any characters from earlier files at the same position. Spaces should be treated as transparent, having no effect on the final image.

For example, suppose that the current directory contains these files:
<pre>
    # # #
\==========/
 \......../
examples/2015/hw/4/ship
  /\
 /vv\
/vvvv\
  ||
examples/2015/hw/4/tree
     * * *
 ____|_|_|_____ 
|_____________|
|___HAPPY_____|
|__BIRTHDAY___|
|_____________|
examples/2015/hw/4/cake
Then this is what we should get from executing the command:

$ echo "1 1 ship 3 5 ship 3 19 tree 7 2 ship 13 4 ship 4 22 tree 5 6 cake" | ./compositor

     # # #
 \==========/
  \......#.# #       /\
     \==========/   /vv\/\
      \....*.*.*   /vvv/vv\
       ____|_|_|_____|/vvvv\
      |_____________|   ||
  \===|___HAPPY_____|
   \..|__BIRTHDAY___|
      |_____________|


        # # #
    \==========/
     \......../
examples/2015/hw/4/example.out

</pre>
## 7.4.2 Submitting your assignment

For this assignment, you may submit whatever source files you like, along with a file Makefile that will generate the program compositor when make is called with no arguments (see the instructions for using make.)

You can test your submitted assignment using the public test script with

/c/cs223/bin/testit 4 public
You may also test your unsubmitted assignment in the current working directory with

/c/cs223/Hwk4/test.public
The test script is intended mostly to guard against trivial errors in output format and is not necessarily exhaustive.

## 7.4.3 Notes

###7.4.3.1 Input

For parsing the commands on stdin, we recommend using scanf. You can test for end of file by checking if scanf correctly parsed all three arguments, as in

    int row;
    int col;
    char filename[BUFFER_SIZE];

    while(scanf("%d %d %s", &row, &col, filename) == 3) {
        /* do something with this file */
    }
You may assume that row and col are always non-negative.

Your program should exit with a non-zero error code if it cannot open a file for reading. Because scanf's %s conversion specifier only reads up to the next whitespace character, you may assume that filenames do not contain whitespace. You may also assume that no filename appearing in the input will require more than 2048 bytes to store (including the terminal null character).30

You may assume that the input contains no null characters.

### 7.4.3.2 Output

Your output should include newline and space characters to put the composited characters in the appropriate rows and columns. It should not include any more of such characters than are absolutely necessary.

For example, there should never be a space at the end of a line (even if there is a space at the end of a line in one of the input files). Similarly, there should not be any blank lines at the end of your output. You may, however, find it necessary to add a newline to the end of the last line to avoid having the output end in the middle of a line.

### 7.4.3.3 General

You may assume that the final picture is not so big that you can't store a row or column number for one of its characters in an int.
