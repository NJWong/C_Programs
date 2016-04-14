--------------------------------------------------------------------------------
           FIT3042 - System Tools and Programming Languages (S1-2016)

                        Assignment 1 - Image Decompressor

--------------------------------------------------------------------------------

                            Nicholas Wong, 22601600

--------------------------------------------------------------------------------

1. Compiling & Running
-----------------------
I have a 'makefile' that you can use to build my program. Simply run the 'make'
command from inside the assignment folder and let the magic happen. When this
command is run, I clean up all the binaries and any output *.rle files that may
have been created from the last build. It then compiles all the *.c files, and
produces a 'rledecode' executable.

Run the program using the following:

    $ ./rledecode <path-to-rle-file> (<file-prefix> | "-")

    <path-to-rle-file> is the path to the *.rle file you wish to decode
    <file-prefix> is the string prefix added to an output *.ppm file
    "-" indicates that the decoded data be output to stdout

2. Functionality
-----------------
I have implemented my own set of functions that handles validating and parsing
the command line arguments. They can be found in the 'arg_validator.c' file.
This includes the ability to check whether or not a file exists, whether the
correct number of arguments was passed in, and whether those arguments are
recognised as valid values.

The program currently supports validating the '--scale scale_factor' and the
'--tween tween_factor' optional arguments. It checks the third argument is
either '--scale' or '--tween', and that the factor is a positive integer.

A limitation is that the arg_validator can only handle one optional argument
and value, not both at the same time. So the following is not supported:

    $ ./rledecode test.rle prefix --scale 2 --tween 1

This current version of the program supports decoding an *.rle file and
outputting each frame to a *.ppm file, or to stdout. These functions can be
found in the 'decoder.c' file.

It supports videos with image width and height of up to 5 digits (a safe
assumption because most screens generally have 4 digit widths and heights e.g.
1920x1080), and videos that have at least 1 frame, and (theoretically) up to
9999 frames. I have not tested for this many frames, but this is a possible
limit when giving each *.ppm file a unique number.

It currently does not support decoding a *.rle with no frames.

When saving decoded frame data to a *.ppm file, the files are created in the
same directory as the 'rledecode' executable. They follow the prescribed
filename convention of 'prefix-xxxx.ppm', where prefix is taken from the
command line arguments, and xxxx is the .ppm file number starting at 0001
and going until 9999.

When outputting decoded data to stdout, all information that would be put in
a .ppm file is streamed to stdout. Since 'printf()' defaults to streaming to
stdout as well, I have made extensive use of 'fprintf()', where I specified
the stderr output stream for console messages that help the user. This includes
program status updates, and error messages. This helps ensure nothing I don't
want is streamed to stdout.

When compiling and running on Mac OSX, Ubuntu, and Xubuntu systems I did notice
some strange behaviour when streaming data to stdout. The data would stream as
expected and display (in ASCII) on the console, but after the program finished
there would be some "garbage values" placed in my input buffer.
    
    # after program completion
    $ ;2c1;2c1;2c1;2c1;2c1;2c1;2c1;2c1;2c1;2c

My only explanation for this is that data is getting streamed so fast that the 
system is having trouble processing the sheer amount and as a result a bunch
of garbage values are written to my command line input buffer. I tested this 
by piping the output to a .txt file:

    $ ./rledecode /rlefiles/single-sonic.rle - > test.txt

and I verified that all of the data was present by diff-ing it against the 
given 'sonic-test-0001.ppm' file. So I can safely say this functionality works.

    2.2 Extended Functionality
    ---------------------------
    I have partially implemented the tween and scale functionality. The
    program accepts either '--scale <factor>' or '--tween <factor>' from the
    command line arguments. It checks the value to ensure it is valid (i.e.
    is a positive integer) and then modifies values accordingly.

    Note: it does not accept both options at the same time correctly.

    The algorithms to scale and tween are not complete, and invoking them
    produces results that are not consistent with what is expected. However,
    they do not result in failure.

    Invoke scale by using:

    $ ./rledecode /rlefiles/single-sonic test --scale 2

    The result is stored in a 'test-0001.ppm' file

    Invoke tween by using:

    $ ./rledecode /rlefiles/virtua.rle test --tween 1

    The result is stored in the series of 'test-XXXX.ppm' files

2. Testing
-----------
I have tested the functionality using the folling commands:

    $ ./rledecode /rlefiles/single-sonic.rle test
    $ ./rledecode /rlefiles/single-sonic.rle -
    $ ./rledecode /rlefiles/single-virtua.rle test
    $ ./rledecode /rlefiles/single-virtua.rle -
    $ ./rledecode /rlefiles/sonic.rle test
    $ ./rledecode /rlefiles/sonic.rle -
    $ ./rledecode /rlefiles/virtua.rle test
    $ ./rledecode /rlefiles/virtua.rle -

Furthermore, I have created a unit test suite using the 'miniunit' framework
that can be found here: http://www.jera.com/techinfo/jtns/jtn002.html

These tests test the different command line argument inputs for the program.

To compile the tests:

    $ make test

Then to run the tests:

    $ /tests/rledecode_test

Running the following will clean the test binaries and test files:

    $ make clean_tests

3. Cleaning
------------
To remove all of the generated *.ppm files and binaries, run the following:

    $ make clean
