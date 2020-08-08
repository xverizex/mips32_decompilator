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
unsigned int global_pointer;
const char *global_file_buffer;

static struct mips32_regs {
	unsigned int r[32];
} cpuc;

#define COLOR_ADDRESS          0x0
#define COLOR_OPERATE          0x1
#define COLOR_NUMBER           0x2
#define COLOR_REGISTER         0x3
#define COLOR_NOP              0x4
#define COLOR_BE               0x5
#define COLOR_LOAD_SAVE        0x6

char current_buffer[5][255];


static char *colored_string ( const char *p, const int color, int index ) {
	switch ( color ) {
		case COLOR_ADDRESS:
			snprintf ( current_buffer[index], 255, "\033[00;32m%s\033[00m", p );
			return current_buffer[index];
		case COLOR_OPERATE:
			snprintf ( current_buffer[index], 255, "\033[01;33m%s\033[00m", p );
			return current_buffer[index];
		case COLOR_REGISTER:
			snprintf ( current_buffer[index], 255, "\033[00;36m%s\033[00m", p );
			return current_buffer[index];
		case COLOR_NOP:
			snprintf ( current_buffer[index], 255, "\033[00;34m%s\033[00m", p );
			return current_buffer[index];
		case COLOR_BE:
			snprintf ( current_buffer[index], 255, "\033[01;32m%s\033[00m", p );
			return current_buffer[index];
		case COLOR_LOAD_SAVE:
			snprintf ( current_buffer[index], 255, "\033[01;35m%s\033[00m", p );
			return current_buffer[index];


	}
}
static char *colored_num ( const unsigned int p, const int color, int index ) {
	switch ( color ) {
		case COLOR_ADDRESS:
			snprintf ( current_buffer[index], 255, "\033[00;32m0x%08x\033[00m", p );
			return current_buffer[index];
		case COLOR_NUMBER:
			snprintf ( current_buffer[index], 255, "\033[01;33m0x%x\033[00m", p );
			return current_buffer[index];
	}
}

static unsigned int get_address_offset ( unsigned int p ) {
	int *ad = ( int * ) ( global_file_buffer + p );
	return *ad;
}

static void print_comment_function ( const unsigned int offset ) {
	for ( int i = 0; i < sym.count; i++ ) {
		if ( sym.offset[i].addr == offset ) {
			if ( sym.offset[i].name[0] == 0 ) continue;
			if ( !strncmp ( sym.offset[i].name, "_MIPS_STUBS_", 13 ) ) continue;
			printf ( " ; %s", sym.offset[i].name );
			return;
		}
	}
}

static void mips32_cpuc_set_register ( unsigned int reg, unsigned int number ) {
}

void mips32_operate_abs_s ( struct mips32_registers *mr, short static_number ) {
}

void mips32_operate_add ( struct mips32_registers *mr, short static_number ) {
}

void mips32_operate_add_s ( struct mips32_registers *mr, short static_number ) {
}

void mips32_operate_addi ( struct mips32_registers *mr, short static_number ) {
}

void mips32_operate_addiu ( struct mips32_registers *mr, short static_number ) {
	cpuc.r[mr->rt] = cpuc.r[mr->rs] + static_number;
}

void mips32_operate_sw ( struct mips32_registers *mr, short static_number ) {
}

void mips32_operate_or ( struct mips32_registers *mr, short static_number ) {
}

void mips32_operate_lui ( struct mips32_registers *mr, short static_number ) {
	cpuc.r[mr->rt] = 0;
	cpuc.r[mr->rt] = ( static_number << 16 ) & 0xffff0000;
}

void mips32_operate_lw ( struct mips32_registers *mr, short static_number ) {
	cpuc.r[mr->rt] = cpuc.r[mr->base] + static_number;
	unsigned int p = cpuc.r[mr->rt] & 0xffff;
	unsigned int offset_to_address = get_address_offset ( p );
	print_comment_function ( offset_to_address );
}

void mips32_operate_slti ( struct mips32_registers *mr, short static_number ) {
}

void mips32_operate_beq ( struct mips32_registers *mr, short static_number ) {
}

void mips32_operate_nop ( struct mips32_registers *mr, short static_number ) {
}

void mips32_operate_jalr ( struct mips32_registers *mr, short static_number ) {
}

void mips32_operate_jr ( struct mips32_registers *mr, short static_number ) {
}


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

static int get_index_op_last ( const int last, unsigned int o ) {
	for ( int i = 0; i < mips32_ops_count; i++ ) {
		if ( mips32_op[i].check == BOTH || mips32_op[i].check == LAST )
			if ( mips32_op[i].special == last && mips32_op[i].o == o ) return i;
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

static void scheme ( const int index, const int op, const unsigned int pointer ) {
	struct mips32_registers mr;

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

				printf ( "%s %s, %s, %s", colored_string ( mips32_op[index].special_cmd, COLOR_OPERATE, 1 ),
						colored_string ( get_name_register ( rd ), COLOR_REGISTER, 2 ),
						colored_string ( get_name_register ( rs ), COLOR_REGISTER, 3 ),
						colored_string ( get_name_register ( rt ), COLOR_REGISTER, 4 )
				       );

				printf ( "\n" );
			}
			break;
		case 3: {
			}
			break;
		case 4: {
				unsigned int operate = get_info_op ( op, 26, 31 );
				unsigned int rs = get_info_op ( op, 21, 25 );
				unsigned int rt = get_info_op ( op, 16, 20 );
				unsigned short immediate = get_info_op ( op, 0, 15 );

				printf ( "%s %s, %s, %c%s", colored_string ( mips32_op[index].special_cmd, COLOR_OPERATE, 1 ),
						colored_string ( get_name_register ( rt ), COLOR_REGISTER, 2 ),
						colored_string ( get_name_register ( rs ), COLOR_REGISTER, 3 ),
						immediate < 0x8000 ? 0 : '-',
						colored_num ( immediate < 0x8000 ? (unsigned short) immediate : (short) ( 0x0 - immediate & 0xffff ), 
							COLOR_NUMBER, 4 )
				       );

				mr.rs = rs;
				mr.rt = rt;
				mips32_op[index].operate ( &mr, immediate < 0x8000 ? immediate : (short) immediate );

				printf ( "\n" );
			}
			break;
		case 5: {
				unsigned int operate = get_info_op ( op, 26, 31 );
				unsigned int base = get_info_op ( op, 21, 25 );
				unsigned int rt = get_info_op ( op, 16, 20 );
				unsigned short offset = get_info_op ( op, 0, 15 );

				printf ( "%s %s, %s(%s)", colored_string ( mips32_op[index].special_cmd, COLOR_LOAD_SAVE, 1 ),
						colored_string ( get_name_register ( rt ), COLOR_REGISTER, 2 ),
						colored_num ( offset < 0x8000 ? (unsigned short) offset : (short) ( 0x0 - offset & 0xffff ),
						       COLOR_NUMBER, 3 ),
						colored_string ( get_name_register ( base ), COLOR_REGISTER, 4 )
				       );

				mr.base = base;
				mr.rt = rt;
				mips32_op[index].operate ( &mr, offset < 0x8000 ? offset : (short) offset );

				printf ( "\n" );
			}
			break;
		case 6: {
				unsigned int operate = get_info_op ( op, 26, 31 );
				unsigned int zero = get_info_op ( op, 21, 25 );
				unsigned int rt = get_info_op ( op, 16, 20 );
				unsigned short immediate = get_info_op ( op, 0, 15 );

				printf ( "%s %s, %s", colored_string ( mips32_op[index].special_cmd, COLOR_OPERATE, 1 ),
						colored_string ( get_name_register ( rt ), COLOR_REGISTER, 2),
						colored_num ( immediate, COLOR_NUMBER, 3 )
				       );

				mr.rt = rt;
				mips32_op[index].operate ( &mr, immediate );

				printf ( "\n" );
			}
			break;

		case 7: {
				unsigned int operate = get_info_op ( op, 26, 31 );
				unsigned int rs = get_info_op ( op, 21, 25 );
				unsigned int rt = get_info_op ( op, 16, 20 );
				unsigned int offset = get_info_op ( op, 0, 15 );
				offset <<= 2;

				printf ( "%s %s, %s, %s", colored_string ( mips32_op[index].special_cmd, COLOR_BE, 1 ),
						colored_string ( get_name_register ( rs ), COLOR_REGISTER, 2 ),
						colored_string ( get_name_register ( rt ), COLOR_REGISTER, 3 ),
						colored_num ( pointer + offset + 4, COLOR_NUMBER, 4 )
				       );

				printf ( "\n" );
			}
			break;
		case 8: {
				if ( op == 0 ) {
					printf ( "%s\n", colored_string ( mips32_op[index].special_cmd, COLOR_NOP, 1 ) );
				}
			}
			break;
		case 9:
			{
				unsigned short special = get_info_op ( op, 26, 31 );
				unsigned short rs = get_info_op ( op, 21, 25 );
				unsigned short zero = get_info_op ( op, 16, 20 );
				unsigned short rd = get_info_op ( op, 11, 15 );
				unsigned short hint = get_info_op ( op, 6, 10 );
				unsigned short operate = get_info_op ( op, 0, 5 );

				printf ( "%s %s, %s", colored_string ( mips32_op[index].special_cmd, COLOR_OPERATE, 1 ),
						colored_string ( get_name_register ( rs ), COLOR_REGISTER, 2 ),
						colored_string ( get_name_register ( rd ), COLOR_REGISTER, 3 )
				       );

				printf ( "\n" );
			}
			break;
		case 10: 
			{
				unsigned short special = get_info_op ( op, 26, 31 );
				unsigned short rs = get_info_op ( op, 21, 25 );
				unsigned short operate = get_info_op ( op, 0, 5 );

				printf ( "%s %s", colored_string ( mips32_op[index].special_cmd, COLOR_OPERATE, 1 ),
						colored_string ( get_name_register ( rs ), COLOR_REGISTER, 2 )
				       );

				printf ( "\n" );
			}
			break;
	}
}

static void parse_operation ( const int op, const unsigned int pointer ) {
	int first = get_info_op ( op, 26, 31 );
	int last = get_info_op ( op, 0, 5 );

	switch ( first ) {
		case MIPS_SPECIAL_CUSTOM: {
						  int index = get_index_op_last ( last, MIPS_SPECIAL_CUSTOM );
						  if ( index == -1 ) {
							  printf ( "not found index\n" );
							  exit ( EXIT_FAILURE );
						  }
						  scheme ( index, op, pointer );
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
				 scheme ( index, op, pointer );
			 }
			 break;
				
	}
}

static void decompile ( const char * const program_buffer, const int size_of_section_code ) {
	const int end_address = address + size_of_section_code;
	global_file_buffer = program_buffer;

	for ( int pointer = address; pointer < end_address; pointer++ ) {
		unsigned int offset = pointer & 0xfffff;
		const unsigned int *operation = ( const unsigned int * ) ( program_buffer + offset );
		printf ( "%s: ", colored_num ( pointer, COLOR_ADDRESS, 0 ) );
		global_pointer = pointer;
		parse_operation ( *operation, pointer );
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
