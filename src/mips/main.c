#include <stdio.h>

int main ( int argc, char **argv ) {
	if ( argc < 2 ) return -1;
	char *s = argv[1];
	printf ( "%s\n", s );
}
