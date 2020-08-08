#include "mips32.h"
#include "values.h"
#include <capstone/mips.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mips32_struct.h"

struct info {
	char *name;
	unsigned int addr;
};

struct sym {
	struct info *offset;
	int count;
} sym;

int global_engian;
unsigned int address;
int global_entry_point;


static unsigned int found_bool_sym_name ( const char * const name, const int type ) {
	for ( int i = 0; i < sym.count; i++ ) {
		if ( !strncmp ( sym.offset[i].name, name, strlen ( name ) + 1 ) ) return 1;
	}
	return 0;
}

static unsigned int get_offset_by_name ( const char * const name ) {
	for ( int i = 0; i < sym.count; i++ ) {
		if ( !strncmp ( sym.offset[i].name, name, strlen ( name ) + 1 ) ) return sym.offset[i].addr;
	}
	return 0;
}

static unsigned int get_info_op ( unsigned int op, int start, int end ) {
	unsigned int num = 0;
	op >>= start;
	int eend = end - start + 1;
	int bit = 1 << eend - 1; 
	int count = eend - 1;
	for ( int i = 0; i < eend; i++ ) {
		num |= ( op & bit );
		bit >>= 1;
	}

	return num;
}

static int get_index_op_first ( const int first ) {
	for ( int i = 0; i < mips32_ops_count; i++ ) {
		if ( mips32_op[i].check == FIRST )
			if ( mips32_op[i].special == first ) return i;
	}
	return -1;
}

static int get_index_op_last ( const int last ) {
	for ( int i = 0; i < mips32_ops_count; i++ ) {
		if ( mips32_op[i].check == BOTH || mips32_op[i].check == LAST )
			if ( mips32_op[i].special == last ) return i;
	}
	return -1;
}

static char *get_name_register ( const unsigned int reg ) {
	for ( int i = 0; i < mips32_cpu_count; i++ ) {
		if ( cpu[i].r == reg ) { 
			return cpu[i].name;
		}
	}
	return NULL;
}

static void scheme ( const int index, const int op ) {
	switch ( mips32_op[index].scheme ) {
		case 1: {
			}
			break;
		case 2: {
				unsigned int special = get_info_op ( op, 26, 31 );
				unsigned int rs = get_info_op ( op, 21, 25 );
				unsigned int rt = get_info_op ( op, 16, 20 );
				unsigned int rd = get_info_op ( op, 11, 15 );
				unsigned int zero = get_info_op ( op, 6, 10 );
				unsigned int operate = get_info_op ( op, 0, 5 );

				printf ( "%s %s, %s, %s\n", mips32_op[index].special_cmd,
						get_name_register ( rd ),
						get_name_register ( rs ),
						get_name_register ( rt )
				       );
			}
			break;
		case 3: {
			}
			break;
		case 4: {
				unsigned int operate = get_info_op ( op, 26, 31 );
				unsigned int rs = get_info_op ( op, 21, 25 );
				unsigned int rt = get_info_op ( op, 16, 20 );
				unsigned int immediate = get_info_op ( op, 0, 15 );

				printf ( "%s %s, %s, -0x%x\n", mips32_op[index].special_cmd,
						get_name_register ( rs ),
						get_name_register ( rt ),
						0x10000 - immediate
				       );
			}
			break;
		case 5: {
				unsigned int operate = get_info_op ( op, 26, 31 );
				unsigned int base = get_info_op ( op, 21, 25 );
				unsigned int rt = get_info_op ( op, 16, 20 );
				unsigned int offset = get_info_op ( op, 0, 15 );

				printf ( "%s %s, 0x%x(%s)\n", mips32_op[index].special_cmd,
						get_name_register ( rt ),
						offset,
						get_name_register ( base )
				       );
			}
			break;
		case 6: {
				unsigned int operate = get_info_op ( op, 26, 31 );
				unsigned int zero = get_info_op ( op, 21, 25 );
				unsigned int rt = get_info_op ( op, 16, 20 );
				unsigned int immediate = get_info_op ( op, 0, 15 );

				printf ( "%s %s, 0x%x\n", mips32_op[index].special_cmd,
						get_name_register ( rt ),
						immediate
				       );
			}
			break;
	}
}

static void parse_operation ( const int op ) {
	int first = get_info_op ( op, 26, 31 );
	int last = get_info_op ( op, 0, 5 );

	switch ( first ) {
		case MIPS_SPECIAL_CUSTOM: {
						  int index = get_index_op_last ( last );
						  if ( index == -1 ) {
							  printf ( "not found index\n" );
							  exit ( EXIT_FAILURE );
						  }
						  scheme ( index, op );
					  }
					  break;
		case MIPS_COP1_CUSTOM: {
				       }
				       break;
		default: {
				 int index = get_index_op_first ( first );
				 if ( index == -1 ) {
					 printf ( "not found index\n" );
					 exit ( EXIT_FAILURE );
				 }
				 scheme ( index, op );
			 }
			 break;
				
	}
}

static void decompile ( const char * const program_buffer, const int size_of_section_code ) {
	const int end_address = address + size_of_section_code;

	for ( int pointer = address; pointer < end_address; pointer++ ) {
		unsigned int offset = pointer & 0xfffff;
		const unsigned int *operation = ( const unsigned int * ) ( program_buffer + offset );
		printf ( "0x%08x: ", pointer );
		parse_operation ( *operation );
		pointer += 3;
	}
}

void mips32_exec ( const Elf32_Ehdr * const program_header, const char * const program_buffer, const int engian, const char * const out_file ) {
	sym.count = 0;
	sym.offset = calloc ( 0, 0 );
	if ( !sym.offset ) {
		fprintf ( stderr, "No alloc memomy.\n" );
		exit ( EXIT_FAILURE );
	}

	global_engian = engian;

	Elf32_Shdr *section_header = ( Elf32_Shdr * ) &program_buffer[program_header->e_shoff];

	const int sections_header_count = program_header->e_shnum;
	Elf32_Shdr *section_string_table = ( Elf32_Shdr * ) &section_header[program_header->e_shstrndx];
	Elf32_Shdr *section_sym = NULL;
	Elf32_Shdr *section_code = NULL;
	int size_of_section_code = 0;
	Elf32_Sym *section_sym_tab = NULL;
	Elf32_Shdr *section = section_header;
	Elf32_Shdr *str_table = NULL;

	for ( int i = 0; i < sections_header_count; i++ ) {
		const char *name = program_buffer + section_string_table->sh_offset + section_header->sh_name;
		if ( section_header->sh_type == SHT_SYMTAB ) {
			section_sym = section_header;
		}
		if ( !strncmp ( name, ".text", 6 ) ) {
			section_code = section_header;
			size_of_section_code = section_code->sh_size;
		}
		if ( !strncmp ( name, ".strtab", 8 ) ) {
			str_table = section_header;
		}
		//printf ( "%d: sh_name: %x %s\n", i, section_header->sh_offset, name );
		section_header++;
	}
	printf ( "\n" );

	section_sym_tab = ( Elf32_Sym * ) ( program_buffer + section_sym->sh_offset );
	const int count = section_sym->sh_size / sizeof ( Elf32_Sym );
	for ( int i = 0; i < count; i++ ) {
		int ty = ELF32_ST_TYPE(section_sym_tab->st_info);
		const char *name = program_buffer + str_table->sh_offset + section_sym_tab->st_name;
		switch ( ty ) {
			case STT_SECTION:
			case STT_OBJECT:
			case STT_FUNC: {
					       int index = sym.count++;
					       sym.offset = realloc ( sym.offset, sizeof ( struct info ) * sym.count );
					       if ( !sym.offset ) {
						       fprintf ( stderr, "sym.offset realloc %s:%d\n", __FILE__, __LINE__ );
						       exit ( EXIT_FAILURE );
					       }
					       sym.offset[index].name = calloc ( strlen ( name ) + 1, 1 );
					       if ( !sym.offset[index].name ) {
						       fprintf ( stderr, "sym.offset[index].name calloc %s:%d\n", __FILE__, __LINE__ );
						       exit ( EXIT_FAILURE );
					       }
					       strncpy ( sym.offset[index].name, name, strlen ( name ) );
					       sym.offset[index].addr = section_sym_tab->st_value;
				       }
				break;
			default:
				break;
		}
		section_sym_tab++;
	}

	if ( found_bool_sym_name ( "main", STT_FUNC ) ) {
		address = get_offset_by_name ( "main" );
	} else {
		address = get_offset_by_name ( ".text" );
	}

	decompile ( program_buffer, size_of_section_code );
}
