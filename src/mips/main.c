#include <stdio.h>
#include <unistd.h>

void parser ( ) {
	char *buf[255];
	read ( 1, buf, 255 );
}

int main ( int argc, char **argv ) {
	if ( argc < 2 ) return -1;
	char *s = argv[1];
	printf ( "%s\n", s );
	parser ( );
}
