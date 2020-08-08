#include <elf.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "values.h"
#include "mips32.h"

static void check_elf_mag_and_exit_if_fail ( const Elf32_Ehdr * const program_header ) {
	if ( strncmp ( program_header->e_ident, ELFMAG, SELFMAG ) ) {
		fprintf ( stderr, "Analisys program only elf.\n" );
		exit ( EXIT_FAILURE );
	}
}

static unsigned int get_elf_machine ( const Elf32_Ehdr * const program_header ) {
	return program_header->e_machine;
}

static unsigned int get_elf_type ( const Elf32_Ehdr * const program_header ) {
	return program_header->e_type;
}

static unsigned int get_elf_version ( const Elf32_Ehdr * const program_header ) {
	return program_header->e_version;
}

static unsigned int get_elf_class_bits ( const Elf32_Ehdr * const program_header ) {
	return program_header->e_ident[EI_CLASS];
}

static void print_machine ( const int machine ) {
	switch ( machine ) {
		case EM_MIPS:
			printf ( "machine: mips r3000\n" );
			break;
	}
}

static void print_type ( const int type ) {
	switch ( type ) {
		case ET_EXEC:
			printf ( "type: exec program\n" );
			break;
		case ET_DYN:
			printf ( "type: shared library\n" );
			break;
	}
}

static void print_engian ( const int engian ) {
	switch ( engian ) {
		case BIG_ENGIAN:
			printf ( "engian: big engian\n" );
			break;
		case LITTLE_ENGIAN:
			printf ( "engian: little engian\n" );
			break;
	}
}

static void print_bits ( const int bits ) {
	switch ( bits ) {
		case ELFCLASS32:
			printf ( "bits: 32\n" );
			break;
		case ELFCLASS64:
			printf ( "bits: 64\n" );
			break;
	}
}

static void print_info_program ( const int machine, const int type, const int engian, const int bits ) {
	print_machine ( machine );
	print_type ( type );
	print_engian ( engian );
	print_bits ( bits );
}

int global_engian = 0;

static int xchange ( int val ) {
	if ( global_engian == ELFDATA2MSB ) {
		int high = ( val >> 8 ) & 0xff;
		int low = val & 0xff;
		int number = ( low << 8 ) | ( high & 0xff );
		return number;
	} else {
		return val;
	}
}

static void controller_allocation ( 
		const Elf32_Ehdr * const program_header, 
		const char * const program_buffer,
		const int machine,
		const int type,
		const int engian,
		const char * const out_file ) {

	int cmd = -1;
	const int bits = get_elf_class_bits ( program_header );

	print_info_program ( machine, type, engian, bits );

	if ( type == ET_EXEC && machine == EM_MIPS && engian == BIG_ENGIAN && bits == ELFCLASS32 ) { cmd = 0; goto switch_cmd; }
	if ( type == ET_EXEC && machine == EM_MIPS && engian == LITTLE_ENGIAN && bits == ELFCLASS32 ) { cmd = 1; goto switch_cmd; }
	if ( type == ET_DYN && machine == EM_MIPS && engian == BIG_ENGIAN && bits == ELFCLASS32 ) { cmd = 2; goto switch_cmd; }
	if ( type == ET_DYN && machine == EM_MIPS && engian == LITTLE_ENGIAN && bits == ELFCLASS32 ) { cmd = 3; goto switch_cmd; }

switch_cmd:
	switch ( cmd ) {
		case 0:
			mips32_exec ( program_header, program_buffer, BIG_ENGIAN, out_file );
			break;
		case 1:
			mips32_exec ( program_header, program_buffer, LITTLE_ENGIAN, out_file );
			break;
		default:
			mips32_exec ( program_header, program_buffer, BIG_ENGIAN, out_file );
			break;

	}
}


void controller_analisys_and_decompile ( const char * const program_buffer, const char * const out_file ) {
	const Elf32_Ehdr *program_header = ( Elf32_Ehdr * ) program_buffer;

	check_elf_mag_and_exit_if_fail ( program_header );

	int machine = get_elf_machine ( program_header );
	int type = get_elf_type ( program_header );
	int version = get_elf_version ( program_header );
	global_engian = program_header->e_ident[EI_DATA];
	machine = xchange ( machine );
	type = xchange ( type );
	version = xchange ( version );

	switch ( machine ) {
		case EM_MIPS:
			controller_allocation ( program_header, program_buffer, machine, type, BIG_ENGIAN, out_file );
			break;
		case EM_MIPS_RS3_LE:
			controller_allocation ( program_header, program_buffer, machine, type, LITTLE_ENGIAN, out_file );
			break;
		default:
			fprintf ( stderr, "The program should be mips.\n" );
			exit ( EXIT_FAILURE );
	}
}
