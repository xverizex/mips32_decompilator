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
	unsigned int vaddr;
	unsigned int type;
};

struct sym {
	struct info *offset;
	int count;
} sym;

struct sect {
	struct info *offset;
	int count;
} sect;

static void exec_pfaf_n ( int );
static void exec_pfaf_ns ( int );
static int xchange ( int num );
static short change ( short num );
static int exec_pfaf_num ( unsigned int num );
static int check_is_hex ( char *val );
static unsigned int get_hex ( unsigned char s );
static unsigned int get_hex_offset ( char *param );

int global_engian;
unsigned int address;
int global_entry_point;
unsigned int global_pointer;
unsigned int global_text_size;
const char *global_file_buffer;
unsigned int global_find_offset;
unsigned int global_pfaf_find_offset;
unsigned int global_pfaf_num_pointer;
int global_pfaf_found;
int switch_pfa;
int switch_pfaf;
int global_print;
extern int global_engian;
int index_section_text;
int switch_gp_offset;
int switch_pfaf_num;
int switch_pff;
int switch_to_end_function;
int stage_gp;
int global_dialog;

char temp_buffer[255];

#define NO_PRINT              1
#define PRINT                 0

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
#define COLOR_FUNCTION         COLOR_BE

char current_buffer[8][255];

char **stack;
int stack_index;
int stack_size;


static char *colored_string ( const char *p, const int color, int index ) {
	switch ( color ) {
		case COLOR_ADDRESS:
			snprintf ( current_buffer[index], 255, "\033[00;32m%s\033[00m", p );
			return current_buffer[index];
		case COLOR_OPERATE:
			snprintf ( current_buffer[index], 255, "\033[00;33m%s\033[00m", p );
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
			snprintf ( current_buffer[index], 255, "\033[00;31m%s\033[00m", p );
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
		case COLOR_BE:
			snprintf ( current_buffer[index], 255, "\033[01;32m%x\033[00m", p );
			return current_buffer[index];
	}
}

static unsigned int get_address_offset ( unsigned int p ) {
	unsigned int *ad = ( unsigned int * ) ( global_file_buffer + p );
	return *ad;
}

static void print_pff ( const unsigned int pp, unsigned int offset, unsigned int rt ) {
	for ( int i = 0; i < sym.count; i++ ) {
		if ( sym.offset[i].addr == offset ) {
			if ( sym.offset[i].name[0] == 0 ) continue;
			if ( !strncmp ( sym.offset[i].name, "_MIPS_STUBS_", 13 ) ) continue;
			if ( sym.offset[i].type == STT_FUNC ) printf ( "%s: %s\n", colored_num ( global_pointer, COLOR_ADDRESS, 1 ), 
					colored_string ( sym.offset[i].name, COLOR_FUNCTION, 2 ) );
			return;
		}
	}
	if ( rt == MIPS_REG_T9_CUSTOM ) {
		char buf[255];
		snprintf ( buf, 255, "func.0x%x", pp );
		printf ( "%s: %s\n", colored_num ( global_pointer, COLOR_ADDRESS, 1 ),
			colored_string ( buf, COLOR_ADDRESS, 2 ) 
				);
	}
}
static void print_comment_function ( const unsigned int pp, unsigned int offset ) {
	for ( int i = 0; i < sym.count; i++ ) {
		if ( sym.offset[i].addr == offset ) {
			if ( sym.offset[i].name[0] == 0 ) continue;
			if ( !strncmp ( sym.offset[i].name, "_MIPS_STUBS_", 13 ) ) continue;
			printf ( " ; [%08x:%08x] ", pp, offset );
			if ( sym.offset[i].type == STT_OBJECT ) printf ( "struct " );
			printf ( "%s", sym.offset[i].name );
			return;
		}
	}

	for ( int i = 0; i < sect.count - 1; i++ ) {
		if ( pp >= sect.offset[i].vaddr && pp <= sect.offset[i+1].vaddr ) {
			unsigned int off = ( sect.offset[i].addr & 0xffff0000 ) | ( pp & 0xffff );
			const char *str = ( global_file_buffer + off );
			if ( str[0] > 0 ) {
				printf ( "; %s", str );
				printf ( "func.%x", pp );
			} else {
				const int *n = (const int *) str;
				printf ( "; int=%d ;", *n );
				printf ( "func.%x", pp );
			}
			return;
		}
	}
}

static void mips32_cpuc_set_register ( unsigned int reg, unsigned int number ) {
}

void mips32_operate_abs_fmt ( struct mips32_registers *mr, short static_number, int sec_num ) {
}

void mips32_operate_add ( struct mips32_registers *mr, short static_number, int sec_num ) {
	if ( switch_gp_offset && mr->rd == MIPS_REG_GP_CUSTOM ) {
		cpuc.r[mr->rd] = cpuc.r[mr->rs] + cpuc.r[mr->rt];
		stage_gp++;
		if ( stage_gp >= 2 ) switch_gp_offset = 0;
	} else if ( mr->rt != MIPS_REG_GP_CUSTOM ) {
		cpuc.r[mr->rd] = cpuc.r[mr->rs] + cpuc.r[mr->rt];
		if ( switch_pfaf ) {
			if ( mr->rd == MIPS_REG_SP_CUSTOM && mr->rs == MIPS_REG_SP_CUSTOM ) {
				if ( static_number < 0x8000 ) {
					exec_pfaf_n ( global_pointer );		
				}
			}
		}
	}
}

void mips32_operate_add_fmt ( struct mips32_registers *mr, short static_number, int sec_num ) {
}

void mips32_operate_addi ( struct mips32_registers *mr, short static_number, int sec_num ) {
	if ( switch_gp_offset && mr->rt == MIPS_REG_GP_CUSTOM ) {
		cpuc.r[mr->rt] = cpuc.r[mr->rs] + static_number;
		stage_gp++;
		if ( stage_gp >= 2 ) switch_gp_offset = 0;
	} else if ( mr->rt != MIPS_REG_GP_CUSTOM ) {
		cpuc.r[mr->rt] = cpuc.r[mr->rs] + static_number;
		if ( switch_pfaf ) {
			if ( mr->rt == MIPS_REG_SP_CUSTOM && mr->rs == MIPS_REG_SP_CUSTOM ) {
				if ( static_number < 0x8000 ) {
					exec_pfaf_n ( global_pointer );		
				}
			}
		}
	}
}

void mips32_operate_andi ( struct mips32_registers *mr, short static_number, int sec_num ) {
}

void mips32_cop2_operate_bc ( struct mips32_registers *mr, short static_number, int sec_num ) {
	unsigned int offset = ( unsigned int ) ( (unsigned int ) static_number << 2 );
	offset += global_pointer + 4;
	if ( mr->nd == 0 && mr->tf == 0 ) {
		if ( global_dialog == PRINT )
		printf ( "%s %s, %s", colored_string ( "bc2f", COLOR_OPERATE, 1 ),
				colored_num ( mr->cc, COLOR_NUMBER, 2 ),
				colored_num ( offset, COLOR_NUMBER, 3 )
		       );
	}
	if ( mr->nd == 1 && mr->tf == 0 ) {
		if ( global_dialog == PRINT )
		printf ( "%s %s, %s", colored_string ( "bc2fl", COLOR_OPERATE, 1 ),
				colored_num ( mr->cc, COLOR_NUMBER, 2 ),
				colored_num ( offset, COLOR_NUMBER, 3 )
		       );
	}
	if ( mr->nd == 0 && mr->tf == 1 ) {
		if ( global_dialog == PRINT )
		printf ( "%s %s, %s", colored_string ( "bc2t", COLOR_OPERATE, 1 ),
				colored_num ( mr->cc, COLOR_NUMBER, 2 ),
				colored_num ( offset, COLOR_NUMBER, 3 )
		       );
	}
	if ( mr->nd == 1 && mr->tf == 1 ) {
		if ( global_dialog == PRINT )
		printf ( "%s %s, %s", colored_string ( "bc2tl", COLOR_OPERATE, 1 ),
				colored_num ( mr->cc, COLOR_NUMBER, 2 ),
				colored_num ( offset, COLOR_NUMBER, 3 )
		       );
	}
}
void mips32_cop1_operate_bc ( struct mips32_registers *mr, short static_number, int sec_num ) {
	unsigned int offset = ( unsigned int ) ( (unsigned int ) static_number << 2 );
	offset += global_pointer + 4;
	if ( mr->nd == 0 && mr->tf == 0 ) {
		if ( global_dialog == PRINT )
		printf ( "%s %s, %s", colored_string ( "bc1f", COLOR_OPERATE, 1 ),
				colored_num ( mr->cc, COLOR_NUMBER, 2 ),
				colored_num ( offset, COLOR_NUMBER, 3 )
		       );
	}
	if ( mr->nd == 1 && mr->tf == 0 ) {
		if ( global_dialog == PRINT )
		printf ( "%s %s, %s", colored_string ( "bc1fl", COLOR_OPERATE, 1 ),
				colored_num ( mr->cc, COLOR_NUMBER, 2 ),
				colored_num ( offset, COLOR_NUMBER, 3 )
		       );
	}
	if ( mr->nd == 0 && mr->tf == 1 ) {
		if ( global_dialog == PRINT )
		printf ( "%s %s, %s", colored_string ( "bc1t", COLOR_OPERATE, 1 ),
				colored_num ( mr->cc, COLOR_NUMBER, 2 ),
				colored_num ( offset, COLOR_NUMBER, 3 )
		       );
	}
	if ( mr->nd == 1 && mr->tf == 1 ) {
		if ( global_dialog == PRINT )
		printf ( "%s %s, %s", colored_string ( "bc1tl", COLOR_OPERATE, 1 ),
				colored_num ( mr->cc, COLOR_NUMBER, 2 ),
				colored_num ( offset, COLOR_NUMBER, 3 )
		       );
	}
}

void mips32_operate_addiu ( struct mips32_registers *mr, short static_number, int sec_num ) {
	if ( !switch_gp_offset && mr->rs == MIPS_REG_GP_CUSTOM && mr->rt == MIPS_REG_GP_CUSTOM ) return;
	if ( switch_gp_offset && mr->rs == MIPS_REG_GP_CUSTOM && mr->rt == MIPS_REG_GP_CUSTOM ) {
		cpuc.r[mr->rt] = cpuc.r[mr->rs] + static_number;
		stage_gp++;
		if ( stage_gp >= 2 ) switch_gp_offset = 0;
	} else if ( mr->rt != MIPS_REG_GP_CUSTOM ) {
		cpuc.r[mr->rt] = cpuc.r[mr->rs] + static_number;
		if ( switch_pfaf ) {
			if ( mr->rt == MIPS_REG_SP_CUSTOM && mr->rs == MIPS_REG_SP_CUSTOM ) {
				if ( static_number < 0x8000 ) {
					//exec_pfaf_n ( global_pointer );		
				}
			}
		}
	} else {
		cpuc.r[mr->rt] = cpuc.r[mr->rs] + static_number;
	}

	unsigned int p = cpuc.r[mr->rt] & 0xffff;
	for ( int i = 0; i < sect.count - 1; i++ ) {
		if ( sect.offset[i].vaddr >= cpuc.r[mr->rt] && sect.offset[i+1].vaddr <= cpuc.r[mr->rt] ) {
			p |= sect.offset[i].addr & 0xffff0000;
		}
	}
	unsigned int pp = cpuc.r[mr->rt];
	unsigned int offset_to_address = xchange ( get_address_offset ( p ) );
	if ( switch_pff ) {
		print_pff ( pp, offset_to_address, mr->rt );
	}
	if ( global_print ) print_comment_function ( pp, offset_to_address );
}

void mips32_operate_sw ( struct mips32_registers *mr, short static_number, int sec_num ) {
}

void mips32_operate_or ( struct mips32_registers *mr, short static_number, int sec_num ) {
}

void mips32_operate_lui ( struct mips32_registers *mr, short static_number, int sec_num ) {
	if ( switch_gp_offset && mr->rt == MIPS_REG_GP_CUSTOM ) {
		cpuc.r[mr->rt] = 0;
		cpuc.r[mr->rt] = ( static_number << 16 ) & 0xffff0000;
		stage_gp++;
		if ( stage_gp >= 2 ) switch_gp_offset = 1;
	} else if ( mr->rt != MIPS_REG_GP_CUSTOM ) {
		cpuc.r[mr->rt] = 0;
		cpuc.r[mr->rt] = ( static_number << 16 ) & 0xffff0000;
	} else if ( mr->rt == MIPS_REG_GP_CUSTOM ) {
		if ( switch_pfaf_num ) {
			if ( mr->rt == MIPS_REG_GP_CUSTOM  ) {
			//	if ( static_number < 0x8000 ) {
						exec_pfaf_ns ( global_pfaf_num_pointer );		
						if ( global_pfaf_found ) {
							printf ( "%s: %s\n", colored_num ( global_pointer, COLOR_BE, 1 ), temp_buffer );
						}
			//	}
			}
		} else if ( switch_pfaf ) {
#if 1
			if ( mr->rt == MIPS_REG_GP_CUSTOM  ) {
				if ( static_number < 0x8000 ) {
					exec_pfaf_n ( global_pointer );		
				}
			}
#endif
		}
	}
}

void mips32_operate_lw ( struct mips32_registers *mr, short static_number, int sec_num ) {
	if ( mr->rt == MIPS_REG_GP_CUSTOM ) return;
	cpuc.r[mr->rt] = 0;
	cpuc.r[mr->rt] = (unsigned int) ( cpuc.r[mr->base] + static_number );
	unsigned int p = cpuc.r[mr->rt] & 0xffff;

	for ( int i = 0; i < sect.count - 1; i++ ) {
		if ( sect.offset[i].vaddr >= cpuc.r[mr->rt] && sect.offset[i+1].vaddr <= cpuc.r[mr->rt] ) {
			p |= sect.offset[i].addr & 0xffff0000;
			break;
		}
	}

	unsigned int pp = cpuc.r[mr->rt];
	unsigned int offset_to_address = xchange ( get_address_offset ( p ) );
	//cpuc.r[mr->rt] = offset_to_address;
	if ( global_print ) {
		print_comment_function ( pp, offset_to_address );
	}
	if ( switch_pff ) {
		print_pff ( pp, offset_to_address, mr->rt );
	}
	if ( switch_pfa ) {
		if ( global_find_offset == offset_to_address ) {
			if ( !switch_pfaf_num ) { 
				exec_pfaf_num ( global_pointer );
			}
		}
	}
}

void mips32_operate_slti ( struct mips32_registers *mr, short static_number, int sec_num ) {
}

void mips32_operate_beq ( struct mips32_registers *mr, short static_number, int sec_num ) {
}

void mips32_operate_beql ( struct mips32_registers *mr, short static_number, int sec_num ) {
}

void mips32_operate_nop ( struct mips32_registers *mr, short static_number, int sec_num ) {
}

void mips32_operate_jalr ( struct mips32_registers *mr, short static_number, int sec_num ) {
	if ( mr->rs == MIPS_REG_T9_CUSTOM ) {
		unsigned int pp = cpuc.r[mr->rs];
		unsigned int p = cpuc.r[mr->rs] & 0xffff;
		for ( int i = 0; i < sect.count - 1; i++ ) {
			if ( sect.offset[i].vaddr >= cpuc.r[mr->rs] && sect.offset[i+1].vaddr <= cpuc.r[mr->rs] ) {
				p |= sect.offset[i].addr & 0xffff0000;
			}
		}
		unsigned int offset_to_address = xchange ( get_address_offset ( p ) );
		if ( global_print ) print_comment_function ( pp, offset_to_address );
	}
}

void mips32_operate_jr ( struct mips32_registers *mr, short static_number, int sec_num ) {
	if ( switch_to_end_function ) {
		if ( mr->rs == MIPS_REG_RA_CUSTOM ) {
			switch_to_end_function = 0;
			return;
		}
	}
	if ( mr->rs == MIPS_REG_T9_CUSTOM ) {
		unsigned int pp = cpuc.r[mr->rs];
		unsigned int p = cpuc.r[mr->rs] & 0xffff;
		for ( int i = 0; i < sect.count - 1; i++ ) {
			if ( sect.offset[i].vaddr >= cpuc.r[mr->rs] && sect.offset[i+1].vaddr <= cpuc.r[mr->rs] ) {
				p |= sect.offset[i].addr & 0xffff0000;
			}
		}
		unsigned int offset_to_address = xchange ( get_address_offset ( p ) );
		if ( global_print ) print_comment_function ( pp, offset_to_address );
	}
	if ( switch_pff ) {
		if ( mr->rs == MIPS_REG_RA_CUSTOM ) {
			switch_pff = 0;
			return;
		}
	}
}

void mips32_operate_blezl ( struct mips32_registers *mr, short static_number, int sec_num ) {
}
void mips32_operate_blez ( struct mips32_registers *mr, short static_number, int sec_num ) {
}
void mips32_operate_bgtz ( struct mips32_registers *mr, short static_number, int sec_num ) {
}
void mips32_operate_bgtzl ( struct mips32_registers *mr, short static_number, int sec_num ) {
}
void mips32_operate_bgez ( struct mips32_registers *mr, short static_number, int sec_num ) {
}
void mips32_operate_bgezl ( struct mips32_registers *mr, short static_number, int sec_num ) {
}
void mips32_operate_bgezal ( struct mips32_registers *mr, short static_number, int sec_num ) {
}
void mips32_operate_bgezall ( struct mips32_registers *mr, short static_number, int sec_num ) {
}

void mips32_operate_and ( struct mips32_registers *mr, short static_number, int sec_num ) {
}

void mips32_operate_jal ( struct mips32_registers *mr, short static_number, int sec_num ) {
	if ( switch_pfa ) {
		if ( global_find_offset == sec_num ) {
			printf ( "%s\n", colored_num ( global_pointer, COLOR_ADDRESS, 1 ) );
		}
	}
}

void mips32_operate_addu ( struct mips32_registers *mr, short static_number, int sec_num ) {
	if ( switch_gp_offset && mr->rd == MIPS_REG_GP_CUSTOM ) {
		cpuc.r[mr->rd] = cpuc.r[mr->rs] + cpuc.r[mr->rt];
	} else if ( mr->rd != MIPS_REG_GP_CUSTOM ) {
		cpuc.r[mr->rd] = cpuc.r[mr->rs] + cpuc.r[mr->rt];
	}
}

void mips32_operate_bltz ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_bltzal ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_bltzall ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_bltzl ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_bne ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_bnel ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_break ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_c_cond ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_cache ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_ceil_w ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_cfc1 ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_cfc2 ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_clo ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_clz ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_cop2 ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_ctc1 ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_ctc2 ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_cvt_d ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_cvt_s ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_cvt_w ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_deret ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_div ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_div_fmt ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_divu ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_eret ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_floor_w_fmt ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_j ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_lb ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_lbu ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_ldc1 ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_ldc2 ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_lh ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_lhu ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_ll ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_lwc1 ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_lwc2 ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_lwl ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_lwr ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_madd ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_maddu ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_mfc0 ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_mfc1 ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_mfc2 ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_mfhi ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_mflo ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_mov_fmt ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_movf ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_movf_fmt ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_movn ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_movn_fmt ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_movt ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_movt_fmt ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_movz ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_movz_fmt ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_msub ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_msubu ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_mtc0 ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_mtc1 ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_mtc2 ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_mthi ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_mtlo ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_mul ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_mul_fmt ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_mult ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_multu ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_neg_fmt ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_nor ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_ori ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_pref ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_round_w ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_sb ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_sc ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_sdbbp ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_sdc1 ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_sdc2 ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_sh ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_sll ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_sllv ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_slt ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_sltiu ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_sltu ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_sqrt_fmt ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_sra ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_srav ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_srl ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_srlv ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_ssnop ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_sub ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_sub_fmt ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_subu ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_swc1 ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_swc2 ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_swl ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_swr ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_sync ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_syscall ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_teq ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_teqi ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_tge ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_tgei ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_tgeiu ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_tgeu ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_tlbp ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_tlbr ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_tlbwi ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_tlbwr ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_tlt ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_tlti ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_tltiu ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_tltu ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_tne ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_tnei ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_trunc_w ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_wait ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_xor ( struct mips32_registers *mr, short static_number, int num ) {
}
void mips32_operate_xori ( struct mips32_registers *mr, short static_number, int num ) {
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

static unsigned int get_info_op_one_bit ( unsigned int op, const int pos ) {
	op >>= pos;
	unsigned int num = op & 1;

	return num;
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

static int get_index_op_cop1_cond ( const int operate, unsigned int o ) {
	for ( int i = 0; i < mips32_ops_count; i++ ) {
		if ( mips32_op[i].check == ONLY_COP1 && mips32_op[i].o == o )
			return i;
	}
	return -1;
}
static int get_index_op_cop_2 ( const int operate, unsigned int o, unsigned int pos ) {
	for ( int i = 0; i < mips32_ops_count; i++ ) {
		if ( mips32_op[i].check == pos && mips32_op[i].o == o )
			if ( mips32_op[i].special == operate ) return i;
	}
	return -1;
}
static int get_index_op_first ( const int first ) {
	for ( int i = 0; i < mips32_ops_count; i++ ) {
		if ( mips32_op[i].check == FIRST )
			if ( mips32_op[i].special == first ) return i;
	}
	return -1;
}

static int get_index_op_last ( const int last, unsigned int o, unsigned int operate ) {
	for ( int i = 0; i < mips32_ops_count; i++ ) {
		if ( operate == 0 && mips32_op[i].o == o && mips32_op[i].special == last ) return INDEX_MIPS_INS_NOP;
		if ( mips32_op[i].check == BOTH || mips32_op[i].check == LAST ) {
			if ( mips32_op[i].special == last && mips32_op[i].o == o && operate != 0 && strncmp ( mips32_op[i].special_cmd, "nop", 4 ) ) {
				return i;
			}
		}
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
static char *get_name_register_fmt ( const unsigned int reg ) {
	for ( int i = 0; i < mips32_cpuf_count; i++ ) {
		if ( cpuf[i].r == reg ) { 
			return cpuf[i].name;
		}
	}
	return NULL;
}

static char *get_fmt_string ( const unsigned int fmt ) {
	for ( int i = 0; i < fmt_count; i++ ) {
		if ( fmt_struct[i].type == fmt ) return fmt_struct[i].name;
	}
	return "";
}

static unsigned int check_condition ( const unsigned int cond ) {
	for ( int i = 0; i < mips32_cond_count; i++ ) {
		if ( condition[i].type == cond ) return 1;
	}
	return 0;
}
static char *get_condition ( const unsigned int cond ) {
	for ( int i = 0; i < mips32_cond_count; i++ ) {
		if ( condition[i].type == cond ) return condition[i].name;
	}
	return "";
}

#define CONDITION              1
#define NO_CONDITION           2


static void scheme ( const int index, const int op, const unsigned int pointer, int dialog, int cond ) {
	struct mips32_registers mr;

	switch ( index > 0 ? mips32_op[index].scheme : 15 ) {
		case 1: {
				unsigned int fmt = get_info_op ( op, 21, 25 );
				unsigned int fs = get_info_op ( op, 11, 15 );
				unsigned int fd = get_info_op ( op, 6, 10 );

				if ( dialog == PRINT )
				printf ( "%s%s %s, %s", colored_string ( mips32_op[index].special_cmd, COLOR_OPERATE, 1 ),
						colored_string ( get_fmt_string ( fmt ), COLOR_OPERATE, 2 ),
						colored_string ( get_name_register_fmt ( fd ), COLOR_REGISTER, 3 ),
						colored_string ( get_name_register_fmt ( fs ), COLOR_REGISTER, 4 )
				       );

				if ( dialog == PRINT )
				printf ( "\n" );
			}
			break;
		case 2: {
				unsigned int special = get_info_op ( op, 26, 31 );
				unsigned int rs = get_info_op ( op, 21, 25 );
				unsigned int rt = get_info_op ( op, 16, 20 );
				unsigned int rd = get_info_op ( op, 11, 15 );
				unsigned int zero = get_info_op ( op, 6, 10 );
				unsigned int operate = get_info_op ( op, 0, 5 );

				if ( dialog == PRINT ) 
				printf ( "%s %s, %s, %s", colored_string ( mips32_op[index].special_cmd, COLOR_OPERATE, 1 ),
						colored_string ( get_name_register ( rd ), COLOR_REGISTER, 2 ),
						colored_string ( get_name_register ( rs ), COLOR_REGISTER, 3 ),
						colored_string ( get_name_register ( rt ), COLOR_REGISTER, 4 )
				       );


				mr.rd = rd;
				mr.rs = rs;
				mr.rt = rt;
				mips32_op[index].operate ( &mr, 0, 0 );

				if ( dialog == PRINT )
				printf ( "\n" );
				
			}
			break;
		case 3: {
				unsigned int fmt = get_info_op ( op, 21, 25 );
				unsigned int ft = get_info_op ( op, 16, 20 );
				unsigned int fs = get_info_op ( op, 11, 15 );
				unsigned int fd = get_info_op ( op, 6, 10 );

				printf ( "%s%s %s, %s, %s", colored_string ( mips32_op[index].special_cmd, COLOR_OPERATE, 1 ),
						colored_string ( get_fmt_string ( fmt ), COLOR_OPERATE, 2 ),
						colored_string ( get_name_register_fmt ( fd ), COLOR_REGISTER, 3 ),
						colored_string ( get_name_register_fmt ( fs ), COLOR_REGISTER, 4 ),
						colored_string ( get_name_register_fmt ( ft ), COLOR_REGISTER, 5 )
				       );

				printf ( "\n" );
			}
			break;
		case 4: {
				unsigned int operate = get_info_op ( op, 26, 31 );
				unsigned int rs = get_info_op ( op, 21, 25 );
				unsigned int rt = get_info_op ( op, 16, 20 );
				unsigned short immediate = get_info_op ( op, 0, 15 );

				if ( dialog == PRINT )
				printf ( "%s %s, %s, %c%s", colored_string ( mips32_op[index].special_cmd, COLOR_OPERATE, 1 ),
						colored_string ( get_name_register ( rt ), COLOR_REGISTER, 2 ),
						colored_string ( get_name_register ( rs ), COLOR_REGISTER, 3 ),
						immediate < 0x8000 ? 0 : '-',
						colored_num ( immediate < 0x8000 ? (unsigned short) immediate : (short) ( 0x0 - immediate & 0xffff ), 
							COLOR_NUMBER, 4 )
				       );

				mr.rs = rs;
				mr.rt = rt;
				mips32_op[index].operate ( &mr, immediate < 0x8000 ? immediate : (short) immediate & 0xffff, 0 );

				if ( dialog == PRINT )
				printf ( "\n" );
			}
			break;
		case 5: {
				unsigned int operate = get_info_op ( op, 26, 31 );
				unsigned int base = get_info_op ( op, 21, 25 );
				unsigned int rt = get_info_op ( op, 16, 20 );
				unsigned short offset = get_info_op ( op, 0, 15 );

				if ( dialog == PRINT )
				printf ( "%s %s, %c%s(%s)", colored_string ( mips32_op[index].special_cmd, COLOR_LOAD_SAVE, 1 ),
						colored_string ( get_name_register ( rt ), COLOR_REGISTER, 2 ),
						offset < 0x8000 ? 0 : '-',
						colored_num ( offset < 0x8000 ? (unsigned short) offset : (short) ( 0x0 - offset & 0xffff ),
						       COLOR_NUMBER, 3 ),
						colored_string ( get_name_register ( base ), COLOR_REGISTER, 4 )
				       );

				mr.base = base;
				mr.rt = rt;
				mips32_op[index].operate ( &mr, offset < 0x8000 ? offset : (short) offset, 0 );

				if ( dialog == PRINT )
				printf ( "\n" );
			}
			break;
		case 6: {
				unsigned int operate = get_info_op ( op, 26, 31 );
				unsigned int zero = get_info_op ( op, 21, 25 );
				unsigned int rt = get_info_op ( op, 16, 20 );
				unsigned short immediate = get_info_op ( op, 0, 15 );

				if ( dialog == PRINT )
				printf ( "%s %s, %s", colored_string ( mips32_op[index].special_cmd, COLOR_OPERATE, 1 ),
						colored_string ( get_name_register ( rt ), COLOR_REGISTER, 2),
						colored_num ( immediate, COLOR_NUMBER, 3 )
				       );

				mr.rt = rt;
				mips32_op[index].operate ( &mr, immediate, 0 );

				if ( dialog == PRINT )
				printf ( "\n" );
			}
			break;

		case 7: {
				unsigned int operate = get_info_op ( op, 26, 31 );
				unsigned int rs = get_info_op ( op, 21, 25 );
				unsigned int rt = get_info_op ( op, 16, 20 );
				unsigned int offset = get_info_op ( op, 0, 15 );
				offset <<= 2;

				if ( dialog == PRINT )
				printf ( "%s %s, %s, %s", colored_string ( mips32_op[index].special_cmd, COLOR_BE, 1 ),
						colored_string ( get_name_register ( rs ), COLOR_REGISTER, 2 ),
						colored_string ( get_name_register ( rt ), COLOR_REGISTER, 3 ),
						colored_num ( pointer + offset + 4, COLOR_NUMBER, 4 )
				       );

				if ( dialog == PRINT )
				printf ( "\n" );
			}
			break;
		case 8: {
				if ( dialog == PRINT )
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

				if ( dialog == PRINT )
				printf ( "%s %s, %s", colored_string ( mips32_op[index].special_cmd, COLOR_OPERATE, 1 ),
						colored_string ( get_name_register ( rs ), COLOR_REGISTER, 2 ),
						colored_string ( get_name_register ( rd ), COLOR_REGISTER, 3 )
				       );

				mr.rs = rs;
				mr.rd = rd;
				mips32_op[index].operate ( &mr, 0, 0 );

				if ( dialog == PRINT )
				printf ( "\n" );
			}
			break;
		case 10: 
			{
				unsigned short special = get_info_op ( op, 26, 31 );
				unsigned short rs = get_info_op ( op, 21, 25 );
				unsigned short operate = get_info_op ( op, 0, 5 );

				if ( dialog == PRINT )
				printf ( "%s %s", colored_string ( mips32_op[index].special_cmd, COLOR_OPERATE, 1 ),
						colored_string ( get_name_register ( rs ), COLOR_REGISTER, 2 )
				       );

				mr.rs = rs;
				mips32_op[index].operate ( &mr, 0, 0 );

				if ( dialog == PRINT )
				printf ( "\n" );
			}
			break;
		case 11:
			{
				unsigned short regimm = get_info_op ( op, 26, 31 );
				unsigned short rs = get_info_op ( op, 21, 25 );
				unsigned short operate = get_info_op ( op, 16, 20 );
				unsigned short offset = get_info_op ( op, 0, 15 );

				if ( dialog == PRINT )
				printf ( "%s %s, %s", colored_string ( mips32_op[index].special_cmd, COLOR_OPERATE, 1 ),
						colored_string ( get_name_register ( rs ), COLOR_REGISTER, 2 ),
						colored_num ( pointer + ( offset * 4 ) + 4, COLOR_NUMBER, 3 )
				       );

				if ( dialog == PRINT )
				printf ( "\n" );
			}
			break;
		case 12: {
				 unsigned short operate = get_info_op ( op, 26, 31 );
				 unsigned int offset = get_info_op ( op, 0, 25 );
				 offset <<= 2;

				 if ( dialog == PRINT )
					 printf ( "%s %s", colored_string ( mips32_op[index].special_cmd, COLOR_BE, 1 ),
							 colored_num ( offset, COLOR_FUNCTION, 2 )
						);
				 mips32_op[index].operate ( &mr, 0, offset );

				 if ( dialog == PRINT )
				 print_comment_function ( global_pointer, offset );

				 if ( dialog == PRINT )
					 printf ( "\n" );
			 }
			 break;
		case 13: { // bc
				 unsigned short operate = get_info_op ( op, 21, 25 );
				 unsigned short cc = get_info_op ( op, 18, 20 );
				 unsigned short nd = get_info_op_one_bit ( op, 17 );
				 unsigned short tf = get_info_op_one_bit ( op, 16 );
				 unsigned short offset = get_info_op ( op, 0, 15 );

				 mr.cc = cc;
				 mr.nd = nd;
				 mr.tf = tf;
				 mips32_op[index].operate ( &mr, offset, 0 );

				 if ( dialog == PRINT )
				 printf ( "\n" );
			 }
			 break;
		case 14: {
				 if ( dialog == PRINT )
				 printf ( "%s", colored_string ( mips32_op[index].special_cmd, COLOR_OPERATE, 1 ) );

				 if ( dialog == PRINT )
				 printf ( "\n" );
			 }
			 break;
		case 15: {
				 unsigned int cond = get_info_op ( op, 0, 3 );
				 unsigned int fmt = get_info_op ( op, 21, 25 );
				 unsigned int fs = get_info_op ( op, 11, 15 );
				 unsigned int ft = get_info_op ( op, 16, 20 );

				 if ( check_condition ( cond ) )
				 if ( dialog == PRINT )
				 printf ( "%s.%s.%s %s %s", colored_string ( "c", COLOR_OPERATE, 1 ),
						 colored_string ( get_condition ( cond ), COLOR_OPERATE, 2 ),
						 colored_string ( get_fmt_string ( fmt ), COLOR_OPERATE, 3 ),
						 colored_string ( get_name_register_fmt ( fs ), COLOR_REGISTER, 4 ),
						 colored_string ( get_name_register_fmt ( ft ), COLOR_REGISTER, 5 )
					);

				 if ( check_condition ( cond ) )
				 if ( dialog == PRINT )
				 printf ( "\n" );

			 }
			 break;
		case 16: {
				 unsigned int base = get_info_op ( op, 21, 25 );
				 unsigned int op = get_info_op ( op, 16, 20 );
				 unsigned int offset = get_info_op ( op, 0, 15 );

				 if ( dialog == PRINT )
				 printf ( "%s %s, %s(%s) - ; не понял что такое op", colored_string ( mips32_op[index].special_cmd, COLOR_OPERATE, 1 ),
						 colored_num ( op, COLOR_NUMBER, 2 ),
						 colored_num ( offset, COLOR_NUMBER, 3 ),
						 colored_string ( get_name_register ( base ), COLOR_REGISTER, 4 )
							 );

				 if ( dialog == PRINT )
				 printf ( "\n" );
						 

			 }
			 break;
		case 17: {
				 unsigned int fmt = get_info_op ( op, 21, 25 );
				 unsigned int zero = get_info_op ( op, 16, 20 );
				 unsigned int fs = get_info_op ( op, 11, 15 );
				 unsigned int fd = get_info_op ( op, 6, 10 );

				 if ( dialog == PRINT )
				 printf ( "%s%s %s, %s", colored_string ( mips32_op[index].special_cmd, COLOR_OPERATE, 1 ),
						 colored_string ( get_fmt_string ( fmt ), COLOR_OPERATE, 2 ),
						 colored_string ( get_name_register_fmt ( fd ), COLOR_REGISTER, 3 ),
						 colored_string ( get_name_register_fmt ( fs ), COLOR_REGISTER, 4 )
					);

				 if ( dialog == PRINT )
				 printf ( "\n" );
			 }
			 break;
		case 18: {
				 unsigned int rt = get_info_op ( op, 16, 20 );
				 unsigned int fs = get_info_op ( op, 11, 15 );

				 if ( dialog == PRINT )
				 printf ( "%s %s, %s", colored_string ( mips32_op[index].special_cmd, COLOR_OPERATE, 1 ),
						 colored_string ( get_name_register ( rt ), COLOR_REGISTER, 2 ),
						 colored_string ( get_name_register_fmt ( fs ), COLOR_REGISTER, 3 )
					);

				 if ( dialog == PRINT )
				 printf ( "\n" );
			 }
			 break;
		case 19: {
			 }
			 break;
		case 20: {
			 }
			 break;
		case 21: {
				unsigned int rt = get_info_op ( op, 16, 20 );
				unsigned int rd = get_info_op ( op, 11, 15 );
				unsigned int sa = get_info_op ( op, 6, 10 );

				if ( dialog == PRINT )
				printf ( "%s %s, %s, %s", colored_string ( mips32_op[index].special_cmd, COLOR_OPERATE, 1 ),
						colored_string ( get_name_register ( rd ), COLOR_REGISTER, 2 ),
						colored_string ( get_name_register ( rt ), COLOR_REGISTER, 3 ),
						colored_num ( sa, COLOR_NUMBER, 4 ) );

				if ( dialog == PRINT )
				printf ( "\n" );

			 }
	}
}

static unsigned int get_index_for_special_code ( const unsigned int code ) {
	for ( int i = 0; i < mips32_ops_count; i++ ) {
		if ( mips32_op[i].special == code ) return i;
	}
	return -1;
}

static void dump_bit ( const unsigned int op ) {
	unsigned int bit = 1 << 31;
	for ( int i = 0; i < 32; i++ ) {
		printf ( "%c", op & bit ? '1' : '0' );
		bit >>= 1;
	}
	printf ( " " );
}

static void test ( const unsigned int op ) {
	unsigned short regimm = get_info_op ( op, 26, 31 );
	unsigned short zero = get_info_op ( op, 21, 25 );
	unsigned short bgezal = get_info_op ( op, 16, 20 );
	unsigned short offset = get_info_op ( op, 0, 15 );
	printf ( ":%x %x %x %x:", regimm, zero, bgezal, offset );
}

static void parse_operation ( const int op, const unsigned int pointer, int dialog ) {
	//dump_bit ( op );
	//test ( op );
	int first = get_info_op ( op, 26, 31 );
	int last = get_info_op ( op, 0, 5 );

	switch ( first ) {
		case MIPS_REGIMM_CUSTOM: {
						 unsigned short operate = get_info_op ( op, 16, 20 );
						 int index = get_index_op_last ( operate, MIPS_REGIMM_CUSTOM, op );
						 if ( index == -1 ) {
						 }
						 scheme ( index, op, pointer, dialog, NO_CONDITION );
					 }
					 break;
		case MIPS_SPECIAL_CUSTOM: {
						  int index = get_index_op_last ( last, MIPS_SPECIAL_CUSTOM, op );
						  if ( index == -1 ) {
						  }
						  scheme ( index, op, pointer, dialog, NO_CONDITION );
					  }
					  break;
		case MIPS_COP1_CUSTOM: {
					       unsigned short operate = get_info_op ( op, 21, 25 );
					       int index = get_index_op_cop_2 ( operate, MIPS_COP1_CUSTOM, BOTH );
					       if ( index == -1 ) {
						       index = get_index_op_cop_2 ( operate, MIPS_COP1_CUSTOM, SECOND );
					       }
					       if ( index == -1 ) {
						       operate = get_info_op ( op, 0, 5 );
					       	       index = get_index_op_cop_2 ( operate, MIPS_COP1_CUSTOM, BOTH );
					       }
					       if ( index == -1 && get_index_op_cop1_cond ( op, MIPS_COP1_CUSTOM ) ) {
						       scheme ( -1, op, pointer, dialog, CONDITION );
						       break;
					       }

					       scheme ( index, op, pointer, dialog, NO_CONDITION );
				       }
				       break;
		case MIPS_COP2_CUSTOM: {
					       unsigned int bit_co = get_info_op_one_bit ( op, 25 );
					       if ( bit_co == 1 ) {
						       unsigned int index = get_index_for_special_code ( 0xffff );
					       	       scheme ( index, op, pointer, dialog, NO_CONDITION );
						       break;
					       }
					       unsigned short operate = get_info_op ( op, 21, 25 );
					       int index = get_index_op_cop_2 ( operate, MIPS_COP2_CUSTOM, BOTH );
					       if ( index == -1 ) {
						       index = get_index_op_cop_2 ( operate, MIPS_COP1_CUSTOM, SECOND );
					       }

					       scheme ( index, op, pointer, dialog, NO_CONDITION );
				       }
				       break;
		default: {
				 int index = get_index_op_first ( first );
#if 0
				 if ( index == -1 ) {
					 printf ( "not found index\n" );
					 exit ( EXIT_FAILURE );
				 }
#endif
				 scheme ( index, op, pointer, dialog, NO_CONDITION );
			 }
			 break;
				
	}
}
static int check_is_number ( char *val ) {
	for ( ; *val != 0; val++ ) {
		if ( *val >= '0' && *val <= '9' ) continue;
		return 0;
	}
	return 1;
}

static void exec_pfe ( char *param ) {
	if ( !check_is_hex ( param ) ) {
		printf ( "Должно быть шестнадцатеричное число.\n" );
		return;
	}
	if ( param[0] == '0' && param[1] == 'x' ) param += 2;

	unsigned int offset = get_hex_offset ( param );
	printf ( "#############################################################################\n" );

	switch_to_end_function = 1;
	global_print = 1;
	for ( int pointer = offset, i = 0; 1; i++ ) {
		unsigned int offset = pointer & 0xffff;
		offset |= sect.offset[index_section_text].addr & 0xffff0000;
		const unsigned int *operation = ( const unsigned int * ) ( global_file_buffer + offset );
		printf ( "%s: ", colored_num ( pointer, COLOR_ADDRESS, 0 ) );
		global_pointer = pointer;
		parse_operation ( xchange ( *operation ), pointer, PRINT );
		if ( switch_to_end_function == 0 ) break;
		pointer += 4;
	}
	global_print = 0;
}
static void exec_pd ( char *param ) {
	if ( !check_is_number ( param ) ) {
		printf ( "Должно быть число.\n" );
		return;
	}

	unsigned int lines = atoi ( param );

	global_print = 1;
	for ( int pointer = address, i = 0; i < lines; i++ ) {
		unsigned int offset = pointer & 0xffff;
		offset |= sect.offset[index_section_text].addr & 0xffff0000;
		const unsigned int *operation = ( const unsigned int * ) ( global_file_buffer + offset );
		printf ( "%s: ", colored_num ( pointer, COLOR_ADDRESS, 0 ) );
		global_pointer = pointer;
		parse_operation ( xchange ( *operation ), pointer, PRINT );
		pointer += 4;
	}
	global_print = 0;
}

static int check_is_hex ( char *val ) {
	if ( val[0] == '0' && val[1] == 'x' ) {
		val += 2;
		for ( ; *val != 0; val++ ) {
			if ( ( *val >= '0' && *val <= '9' ) || ( *val >= 'a' && *val <= 'f' ) || ( *val >= 'A' && *val <= 'F' ) ) continue;
			return 0;
		}
	} else {
		for ( ; *val != 0; val++ ) {
			if ( *val == 'x' ) return 0;
			if ( ( *val >= '0' && *val <= '9' ) || ( *val >= 'a' && *val <= 'f' ) || ( *val >= 'A' && *val <= 'F' ) ) continue;
			return 0;
		}
	}
	return 1;
}

static unsigned int get_hex ( unsigned char s ) {
	switch ( s ) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			return s - '0';
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
			return s - 'a' + 10;
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
			return s - 'A' + 10;
	}
}

static unsigned int get_hex_offset ( char *param ) {
	int length = strlen ( param );
	char *s = &param[length-1];
	unsigned int number = 0;
	int hex = 1;

	for ( int i = 0; i < length; i++ ) {
		number |= get_hex ( *s ) * hex;
		if ( hex == 0 ) {
			hex = 16;
		} else {
			hex *= 16;
		}
		s--;
	}
	return number;
}

static void exec_s ( char *param ) {
	if ( !check_is_hex ( param ) ) {
		printf ( "Должно быть шестнадцатеричное число.\n" );
		return;
	}
	if ( param[0] == '0' && param[1] == 'x' ) param += 2;

	unsigned int offset = get_hex_offset ( param );
	address = offset;
}

static void exec_pf ( ) {
	for ( int i = 0; i < sym.count; i++ ) {
		if ( sym.offset[i].name[0] == 0 ) continue;
		printf ( "%s: %s\n", colored_num ( sym.offset[i].addr, COLOR_ADDRESS, 1 ), colored_string ( sym.offset[i].name, COLOR_REGISTER, 2 ) );
	}
}

static void exec_pfaf_ns ( int num ) {
	int pr = 0;

	temp_buffer[0] = 0;
	for ( int i = 0; i < sym.count; i++ ) {
		if ( sym.offset[i].name[0] == 0 ) continue;
		if ( sym.offset[i].addr == num ) {
			if ( !strncmp ( sym.offset[i].name, "_MIPS_STUBS_", 13 ) ) continue;
			if ( sym.offset[i].name[0] != 0 ) {
				snprintf ( temp_buffer, 255, "%s %s", colored_num ( num, COLOR_ADDRESS, 1 ),
						colored_string ( sym.offset[i].name, COLOR_FUNCTION, 2 )
					 );
				pr = 1;
				global_pfaf_found = 1;
				return;
			}
			break;
		}
	}
	global_pfaf_found = 0;
	//if ( !pr ) printf ( "\n" );
}
static void exec_pfaf_n ( int num ) {
	int pr = 0;

	for ( int i = 0; i < sym.count; i++ ) {
		if ( sym.offset[i].name[0] == 0 ) continue;
		if ( sym.offset[i].addr == num ) {
			if ( !strncmp ( sym.offset[i].name, "_MIPS_STUBS_", 13 ) ) continue;
			if ( sym.offset[i].name[0] != 0 ) {
				printf ( "%s ", colored_num ( num, COLOR_ADDRESS, 1 ) );
				printf ( "%s\n", colored_string ( sym.offset[i].name, COLOR_FUNCTION, 2 ) );
				pr = 1;
				global_pfaf_found = 1;
				return;
			}
			break;
		}
	}
	//if ( !pr ) printf ( "\n" );
}
static void exec_pfa ( char *param ) {
	if ( !check_is_hex ( param ) ) {
		printf ( "Должно быть шестнадцатеричное число.\n" );
		return;
	}
	if ( param[0] == '0' && param[1] == 'x' ) param += 2;

	global_find_offset = get_hex_offset ( param );

	switch_pfa = 1;
	global_print = 0;
	for ( int pointer = global_entry_point, i = 0; i < global_text_size; i += 4 ) {
		unsigned int offset = pointer & 0xffff;
		offset |= sect.offset[index_section_text].addr & 0xffff0000;
		const unsigned int *operation = ( const unsigned int * ) ( global_file_buffer + offset );
		global_pointer = pointer;
		parse_operation ( xchange ( *operation ), pointer, global_dialog = NO_PRINT );
		pointer += 4;
	}
	global_print = 1;
	switch_pfa = 0;
}

static void exec_pff ( char *param ) {
	if ( !check_is_hex ( param ) ) {
		printf ( "Должно быть шестнадцатеричное число.\n" );
		return;
	}
	if ( param[0] == '0' && param[1] == 'x' ) param += 2;
	switch_pff = 1;

	global_find_offset = get_hex_offset ( param );

	global_print = 0;
	for ( int pointer = global_find_offset, i = 0; i < 0xffff; i++ ) {
		unsigned int offset = pointer & 0xffff;
		offset |= sect.offset[index_section_text].addr & 0xffff0000;
		const unsigned int *operation = ( const unsigned int * ) ( global_file_buffer + offset );
		global_pointer = pointer;
		parse_operation ( xchange ( *operation ), pointer, global_dialog = NO_PRINT );
		if ( !switch_pff ) break;
		pointer += 4;
	}
	global_print = 0;

	switch_pff = 0;
}

static void get_gp_offset ( ) {
	switch_gp_offset = 1;
	
	int lines = 100;
	for ( int pointer = address, i = 0; i < lines; i++ ) {
		unsigned int offset = pointer & 0xffff;
		offset |= sect.offset[index_section_text].addr & 0xffff0000;
		const unsigned int *operation = ( const unsigned int * ) ( global_file_buffer + offset );
		//printf ( "%s: ", colored_num ( pointer, COLOR_ADDRESS, 0 ) );
		global_pointer = pointer;
		parse_operation ( xchange ( *operation ), pointer, global_dialog = NO_PRINT );
		pointer += 4;
		if ( !switch_gp_offset ) break;
	}

	switch_gp_offset = 0;
}

static int exec_pfaf_num ( unsigned int num ) {

	global_pfaf_find_offset = num;

	global_pfaf_found = 0;

	switch_pfaf_num = 1;
	global_print = 0;
	for ( int pointer = global_pfaf_find_offset, i = 0; pointer > global_entry_point; i += 4 ) {
		unsigned int offset = pointer & 0xffff;
		offset |= sect.offset[index_section_text].addr & 0xffff0000;
		const unsigned int *operation = ( const unsigned int * ) ( global_file_buffer + offset );
		global_pfaf_num_pointer = pointer;
		parse_operation ( xchange ( *operation ), pointer, global_dialog = NO_PRINT );
		pointer -= 4;
		if ( global_pfaf_found ) {
			global_pfaf_found = 0;
			switch_pfaf_num = 0;
			return 1;
			break;
		}
	}
	switch_pfaf_num = 0;

	global_pfaf_found = 0;
	return 0;
}
static int exec_pfaf ( char *param ) {
	if ( !check_is_hex ( param ) ) {
		printf ( "Должно быть шестнадцатеричное число.\n" );
		return 0;
	}
	if ( param[0] == '0' && param[1] == 'x' ) param += 2;

	global_find_offset = get_hex_offset ( param );

	switch_pfaf = 1;
	global_print = 0;
	for ( int pointer = global_find_offset, i = 0; pointer > global_entry_point; i += 4 ) {
		unsigned int offset = pointer & 0xffff;
		offset |= sect.offset[index_section_text].addr & 0xffff0000;
		const unsigned int *operation = ( const unsigned int * ) ( global_file_buffer + offset );
		global_pointer = pointer;
		parse_operation ( xchange ( *operation ), pointer, global_dialog = NO_PRINT );
		pointer -= 4;
		if ( global_pfaf_found ) {
			global_print = 1;
			switch_pfaf = 0;
			global_pfaf_found = 0;
			return 1;
			break;
		}
	}
	global_print = 1;
	switch_pfaf = 0;

	global_pfaf_found = 0;
	return 0;
}

static void exec_pfn ( const char *val ) {
	for ( int i = 0; i < sym.count; i++ ) {
		if ( !strncmp ( sym.offset[i].name, val, strlen ( val ) + 1 ) ) {
			printf ( "%s: %s\n", colored_num ( sym.offset[i].addr, COLOR_ADDRESS, 1 ),
					colored_string ( sym.offset[i].name, COLOR_FUNCTION, 2 )
			       );
		}
	}
}

static void exec_help ( ) {
	printf ( 
			" - pd [lines] - print disassemble. отобразить дизассемблированный код. lines - количество строк.\n"
			" - pf - print functions. отобразить функции, которые есть в программе.\n"
			" - pfa [address] - print function all call. узнать с какого адреса был вызов этой функции.\n"
			" - pfaf [address] - print functions of function. узнать к какой функции принадлежит адрес.\n"
			" - pfn [name] - print function of name. узнать какой адрес у функции.\n"
			" - s [address] - seek to address. перейти на нужное смещение.\n"
			" - pff [address] - print functions in function. отобразить функции, которые выполнятся в этой функции.\n"
			" - pfe [address] - print to end function. отобразить дизассемблерный код до конца функции.\n"
	       );
}

static void parse_buf ( char *b ) {
	char cmd[255];
	char param[255];

	char *s = &cmd[0];
	int is_param = 0;

	while ( 1 ) {
		for ( int i = 0; i < 255; i++ ) {
			if ( *b == '\0' ) {
				is_param = 1;
				*s = 0;
				break;
			}
			if ( *b == '\n' ) {
				is_param = 1;
				*s = 0;
				break;
			}
			if ( *b == ' ' ) {
				*s = 0;
				b++;
				break;
			}
			*s = *b;
			b++;
			s++;
		}
		if ( is_param == 1 ) break;
		is_param = 1;
		s = &param[0];
	}

	if ( !strncmp ( cmd, "pd", 3 ) ) {
		exec_pd ( param );
		return;
	}
	if ( !strncmp ( cmd, "s", 2 ) ) {
		exec_s ( param );
		return;
	}
	if ( !strncmp ( cmd, "pf", 3 ) ) {
		exec_pf ( );
		return;
	}
	if ( !strncmp ( cmd, "pfa", 4 ) ) {
		exec_pfa ( param );
		return;
	}
	if ( !strncmp ( cmd, "pfaf", 5 ) ) {
		exec_pfaf ( param );
		return;
	}
	if ( !strncmp ( cmd, "pfn", 4 ) ) {
		exec_pfn ( param );
		return;
	}
	if( !strncmp ( cmd, "pff", 4 ) ) {
		exec_pff ( param );
		return;
	}
	if ( !strncmp ( cmd, "pfe", 4 ) ) {
		exec_pfe ( param );
		return;
	}
	if ( !strncmp ( cmd, "h", 2 ) || !strncmp ( cmd, "help", 5 ) ) {
		exec_help ( );
		return;
	}
}

static void decompile ( const char * const program_buffer, const int size_of_section_code ) {
	const int end_address = address + size_of_section_code;
	global_file_buffer = program_buffer;
	get_gp_offset ( );
	char buf[255];

	while ( 1 ) {
		printf ( "[%s]:", colored_num ( address, COLOR_ADDRESS, 0 ) );
		fgets ( buf, 255, stdin );
		parse_buf ( buf );
	}

}

static int xchange ( int num ) {
	if ( global_engian == ELFDATA2MSB ) {
		return ((num>>24)&0xff) | ((num<<8)&0xff0000) | ((num>>8)&0xff00) | ((num<<24)&0xff000000);
	} else {
		return num;
	}
}
static short change ( short num ) {
	if ( global_engian == ELFDATA2MSB ) {
		return ( ( num >> 8 ) | ( num << 8 ) );
	} else {
		return num;
	}
}

void mips32_exec ( const Elf32_Ehdr * const program_header, const char * const program_buffer, const int engian, const char * const out_file ) {
	stack = calloc ( 0, sizeof ( char ** ) );
	sym.count = 0;
	sym.offset = calloc ( 0, 0 );
	if ( !sym.offset ) {
		fprintf ( stderr, "No alloc memomy.\n" );
		exit ( EXIT_FAILURE );
	}
	sect.offset = calloc ( 0, 0 );
	if ( !sect.offset ) {
		fprintf ( stderr, "No alloc memomy.\n" );
		exit ( EXIT_FAILURE );
	}

	Elf32_Shdr *section_header = ( Elf32_Shdr * ) &program_buffer[xchange ( program_header->e_shoff ) ];

	const short sections_header_count = change ( program_header->e_shnum );
	Elf32_Shdr *section_string_table = ( Elf32_Shdr * ) &section_header[ change ( program_header->e_shstrndx ) ];
	Elf32_Shdr *section_sym = NULL;
	Elf32_Shdr *section_code = NULL;
	int size_of_section_code = 0;
	Elf32_Sym *section_sym_tab = NULL;
	Elf32_Shdr *section = section_header;
	Elf32_Shdr *str_table = NULL;

	for ( int i = 0; i < sections_header_count; i++ ) {
		const char *name = program_buffer + xchange ( section_string_table->sh_offset ) + xchange ( section_header->sh_name );

		int index = sect.count++;
		sect.offset = realloc ( sect.offset, sizeof ( struct info ) * sect.count );
		if ( !sect.offset ) {
			fprintf ( stderr, "sect.offset realloc %s:%d\n", __FILE__, __LINE__ );
			exit ( EXIT_FAILURE );
		}
		sect.offset[index].name = calloc ( strlen ( name ) + 1, 1 );
	        if ( !sect.offset[index].name ) {
		       fprintf ( stderr, "sym.offset[index].name calloc %s:%d\n", __FILE__, __LINE__ );
		       exit ( EXIT_FAILURE );
	        }
	       strncpy ( sect.offset[index].name, name, strlen ( name ) );
	       sect.offset[index].addr = xchange ( section_header->sh_offset );
	       sect.offset[index].vaddr = xchange ( section_header->sh_addr );
#if 1
		if ( xchange ( section_header->sh_type ) == SHT_SYMTAB ) {
			if ( !section_sym ) section_sym = section_header;
			printf ( "symtab:\n" );
		}
#endif
		if ( !strncmp ( name, ".text", 6 ) ) {
			index_section_text = index;
			section_code = section_header;
			size_of_section_code = xchange ( section_code->sh_size );
		}
#if 1
		if ( !strncmp ( name, ".strtab", 8 ) ) {
			if ( !str_table ) str_table = section_header;
		}
#endif
		if ( !strncmp ( name, ".dynsym", 8 ) ) {
			section_sym = section_header;
		}
		if ( !strncmp ( name, ".dynstr", 8 ) ) {
			str_table = section_header;
		}
		//printf ( "%d: sh_name: %x %s\n", i, xchange ( section_header->sh_offset ), name );
		section_header++;
	}
	printf ( "\n" );

	section_sym_tab = ( Elf32_Sym * ) ( program_buffer + xchange ( section_sym->sh_offset ) );
	const int count = xchange ( section_sym->sh_size ) / sizeof ( Elf32_Sym );
	for ( int i = 0; i < count; i++ ) {
		int ty = ELF32_ST_TYPE(section_sym_tab->st_info);
		const char *name = program_buffer + xchange ( str_table->sh_offset ) + xchange ( section_sym_tab->st_name );
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
					       sym.offset[index].addr = xchange ( section_sym_tab->st_value );
					       sym.offset[index].type = ty;
				       }
				break;
			default:
				break;
		}
		section_sym_tab++;
	}

#if 0
	if ( found_bool_sym_name ( "main", STT_FUNC ) ) {
		address = get_offset_by_name ( "main" );
	} else {
		address = get_offset_by_name ( ".text" );
	}
#endif
	address = xchange ( section_code->sh_addr );
	global_entry_point = address;
	global_text_size = xchange ( section_code->sh_size );

	decompile ( program_buffer, size_of_section_code );
}
