CC 	   = gcc
CFLAGS = -g -Wall -I.
MODE   = -std=c99

all: clean_bins rledecode

rledecode: rledecode.c driver.c 
	$(CC) $(CFLAGS) $(MODE) -o rledecode rledecode.c driver.c

test: clean_tests rledecode_test

arg_parser_test: tests/arg_parser_test.c
	$(CC) $(CFLAGS) $(MODE) -o tests/arg_parser_test tests/arg_parser_test.c

rledecode_test: tests/rledecode_test.c
	$(CC) $(CFLAGS) $(MODE) -o tests/rledecode_test tests/rledecode_test.c

clean: clean_bins clean_output clean_tests

# Clean up output ppm files
clean_output:
	$(RM) ./*.ppm

# Clean up compiled output files for program files
clean_bins:
	$(RM) rledecode
	$(RM) ./*rle

# Clean up compiled output files for unit test files
clean_tests:
	$(RM) tests/arg_parser_test
	$(RM) tests/rledecode_test
	$(RM) ./*rle
	$(RM) tests/*.rle
	$(RM) rlefiles/test*.rle