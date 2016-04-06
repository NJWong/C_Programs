CC 	   = gcc
CFLAGS = -g -Wall -I.

all: clean_bins rledecode

rledecode: rledecode.c
	$(CC) $(CFLAGS) -o rledecode rledecode.c arg_parser.c

test: arg_parser_test

arg_parser_test: tests/arg_parser_test.c
	$(CC) $(CFLAGS) -o tests/arg_parser_test tests/arg_parser_test.c

clean: clean_bins clean_tests

clean_bins:
	$(RM) rledecode

clean_tests:
	$(RM) tests/arg_parser_test