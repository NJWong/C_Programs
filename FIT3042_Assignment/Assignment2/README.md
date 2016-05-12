--------------------------------------------------------------------------------
           FIT3042 - System Tools and Programming Languages (S1-2016)

                        Assignment 2 - SDL Video Player

--------------------------------------------------------------------------------

                            Nicholas Wong, 22601600

--------------------------------------------------------------------------------

1. Compiling & Running
-----------------------
From the main folder, run 'make' to compile the program. An executable 'ppmplayer'
will be created which can be run as expected. It accepts 2, 3, and 4 command line
arguments as shown:

    $ ./ppm <delay_ms> <brightness_factor> <contrast_factor> <saturation_factor>

    <delay_ms> - the delay between each window update in milliseconds.
    <brightness_factor> - value between 0 and 100
    <contrast_factor> - value between 0 and 100
    <saturation_factor> - value between 0 and 100

The arg_parser module will validate the command line arguments to ensure there
is the correct number of arguments, and that each argument has valid values.


2. Functionality
-----------------
The core functionality to play a video from a stream of decoded ppm frames is
working as prescribed in the assignment specification. Run the program as
follows:

    $ ./rledecode test.rle - | ./ppmplayer 10

This will play each ppm frame with a delay of 10 ms between each frame.

Important: This program must be run with decoded ppm frames that are piped in
through the stdin stream as shown above.

This program supports ppm images have width and height dimensions of up to
and including 5 digits. This is a safe assumption because most screens generally
have 4 digit width and heights e.g. 1920x1080.

Before the program reads in the frame pixel data, it reads and checks the header
for valid information. It expects "P6" as the magic number, and 255 as the max
pixel int value. It also checks that the width and height values have not
changed from frame to frame. It also checks for comment lines anywhere from
after the "P6" line to before the 255 line.

Implementation details can be found in the documentation in video_player.h.

    2.2 Extended Functionality
    ---------------------------
    As mentioned above, the program takes all three optional command line
    arguments and validates them to ensure they are in the expected range.
    The implementation for each of the following can be seen in 
    image_manipulator.c

        2.2.1 Brightness
        ----------------
        Has been implemented by manipulating the RGB pixel values. The command
        line argument for this is the <brightness_factor>, between 0 and 100.

        Value ranges:
            [0, 49]   - Decrease the brightness. 0 is darkest.
            [50]      - Do nothing.
            [51, 100] - Increase the brightness. 100 is the lightest.

        2.2.2 Contrast
        ----------------
        Has been implemented by converting the RGB values to HSV and then
        manipulating the S and V values.

        Value ranges:
            [0, 49]   - Decrease the contrast. 0 is least contrasted.
            [50]      - Do nothing.
            [51, 100] - Increase the contrast. 100 is most contrasted.

        2.2.3 Saturation
        ----------------
        Has been implemented by converting the RGB values to HSV and then
        manipulating the S value.

        Value ranges:
            [0, 49]   - Decrease the saturation. 0 is least saturated.
            [50]      - Do nothing.
            [51, 100] - Increase the saturation. 100 is most saturated.

3. Cleaning
------------
To remove all of the *.ppm files, *.rle files,  and binaries in the root
directory, run the following:

    $ make clean
