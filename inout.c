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
	int c;

	while((c = getchar()) != EOF) {
		putchar(c);
	}
}

