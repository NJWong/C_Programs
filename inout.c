#include <stdio.h>

/* Version 1 */
/*
int main() {
	// an 'int' is used here because a simple 'char' is not large enough to hold the EOF value.
	int c;
	c = getchar();
	// (c != 97) - where 97 is the ascii decimal number for 'a'
	// EOF is an integer defined in <stdio.h> and is a symbolic constant
	while (c != EOF) {
		putchar(c);
		c = getchar();
	}
	return 0;
}
*/

void exercise1();
void exercise2();

/* Version 2 */
/*
	Any assignment such as 'c = getchar()' is an expression and has a value.
	This value is the left hand side after the assignment.
	The parentheses around the 'c = getchar()' is required since the != has
	a higher precedence.

	Note:

	'c = getchar() != EOF'

	is equivalent to

	'c = (getchar() != EOF)'

	which will set 'c' to either 1 or 0 depending on if it encounters end of file.
*/
int main() {
	printf("--- START main ---\n");
	int c;
	while((c = getchar()) != EOF) {
		putchar(c);
	}

	exercise1();
	exercise2();

	printf("--- END main ---\n");
}

/* Exercise 1 - Verify that the expression 'getchar() != EOF' is 0 or 1 */
void exercise1() {
	printf("--- START exercise1 ---\n");

	int c;
	if (c = (getchar() == EOF)) {
		// will print 1 when main the while loop in main reaches end of file.
		printf("value of c: %d\n", c);
	}

	printf("--- END exercise1 ---\n");
}

/* Exercise 2 - Print the value of EOF */
void exercise2() {
	printf("--- START exercise2 ---\n");

	printf("value of EOF: %d\n", EOF);

	printf("--- END exercise2 ---\n");
}