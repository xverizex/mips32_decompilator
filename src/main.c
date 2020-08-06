#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "controller.h"

static char *read_program ( const char const *in_file ) {
	struct stat st;
	stat ( in_file, &st );

	char *program_buffer = malloc ( st.st_size );

	FILE *fp = fopen ( in_file, "r" );
	fread ( program_buffer, 1, st.st_size, fp );
	fclose ( fp );
	return program_buffer;
}

int main ( int argc, char **argv ) {
	if ( argc < 2 ) return -1;

	int param;
	char *out_file = "out.c";
	char *in_file = NULL;

	do {
		param = getopt ( argc, argv, "i:o:" );
		switch ( param ) {
			case 'i':
				in_file = optarg;
				break;
			case 'o':
				out_file = optarg;
				break;
		}

	} while ( param != -1 );

	if ( access ( in_file, F_OK ) ) {
		fprintf ( stderr, "File program not found %s\n", in_file );
		exit ( EXIT_FAILURE );
	}

	char *program_buffer = read_program ( in_file );

	controller_analisys_and_decompile ( program_buffer, out_file );
}
