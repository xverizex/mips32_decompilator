#ifndef __MIPS32_STRUCT__
#define __MIPS32_STRUCT__
#include <capstone/mips.h>
/*
 * scheme
 * 1.
 * 	31-26 cop1
 * 	25-21 fmt
 * 	20-16 0
 * 	15-11 fs
 * 	10-6 fd
 * 	5-0 operate
 *
 * 2.
 * 	31-26 special
 * 	25-21 rs
 * 	20-16 rt
 * 	15-11 rd
 * 	10-6  0
 * 	5-0   operate
 *
 * 3.
 * 	31-26 cop1
 * 	25-21 fmt
 * 	20-16 ft
 * 	15-11 fs
 * 	10-6  fd
 * 	5-0   operate
 *
 * 4.
 * 	31-26 operate
 * 	25-21 rs
 * 	20-16 rt
 * 	15-0  immediate
 *
 * 5.   
 * 	31-26 operate
 * 	25-21 base
 * 	20-16 rt
 * 	15-0  offset
 *
 * 6.
 * 	31-26 operate
 * 	25-21 zero
 * 	20-16 rt
 * 	15-0  immediate
 * 	
 * 7.
 * 	31-26 operate
 * 	25-21 rs
 * 	20-16 rt
 * 	15-0  offset
 *
 * 8.
 * 	31-26 special
 * 	25-21 zero
 * 	20-16 zero
 * 	15-11 zero
 * 	10-6  zero
 * 	5-0   sll
 *
 * 9.
 * 	31-26 special
 * 	25-21 rs
 * 	20-16 zero
 * 	15-11 rd
 * 	10-6  hint
 * 	5-0   operate
 *
 * 10.
 * 	31-26 special
 * 	25-21 rs
 * 	20-11 zero
 * 	10-6  hint
 * 	5-0   operate
 * 
 * 11.
 * 	31-26 regimm
 * 	25-21 rs
 * 	20-16 operate
 * 	15-0  offset
 *
 * 12.
 * 	31-26 operate
 * 	25-0  instr_index
 */
#include "mips32_registers.h"

void mips32_operate_add ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_abs_s ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_add_s ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_addi ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_addiu ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_sw ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_or ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_lui ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_lw ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_slti ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_beq ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_nop ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_jalr ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_jr ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_bgezal ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_and ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_jal ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_addu ( struct mips32_registers *mr, short static_number, int );

#define FIRST        0
#define LAST         1
#define BOTH         2

#define MIPS_NONE_SPECIAL_CUSTOM      0xfffff
#define MIPS_SPECIAL_CUSTOM           0x0
#define MIPS_REGIMM_CUSTOM            0x1
#define MIPS_COP1_CUSTOM              0x11
#define MIPS_INS_ABS_S_FMT_CUSTOM     0x5
#define MIPS_INS_ADD_CUSTOM           0x20
#define MIPS_INS_ADD_FMT_CUSTOM       0x0
#define MIPS_INS_ADDI_CUSTOM          0x8
#define MIPS_INS_ADDIU_CUSTOM         0x9
#define MIPS_INS_SW_CUSTOM            0x2b
#define MIPS_INS_OR_CUSTOM            0x25
#define MIPS_INS_LUI_CUSTOM           0xf
#define MIPS_INS_LW_CUSTOM            0x23
#define MIPS_INS_SLTI_CUSTOM          0xa
#define MIPS_INS_BEQ_CUSTOM           0x4
#define MIPS_INS_NOP_CUSTOM           0x0
#define MIPS_INS_JALR_CUSTOM          0x9
#define MIPS_INS_JR_CUSTOM            0x8
#define MIPS_INS_BAL_CUSTOM           0x1
#define MIPS_INS_BGEZAL_CUSTOM        0x11
#define MIPS_INS_AND_CUSTOM           0x24
#define MIPS_INS_JAL_CUSTOM           0x3
#define MIPS_INS_ADDU_CUSTOM          0x21

const int both[] = {
	MIPS_SPECIAL_CUSTOM,
	MIPS_COP1_CUSTOM
};

int both_count = sizeof ( both ) / sizeof ( int );

struct mips32_operators {
	unsigned int o;
	unsigned int special;
	char special_cmd[255];
	void (*operate) ( struct mips32_registers * mr, short static_number, int second_number );
	unsigned int count;
	unsigned int scheme;
	unsigned int check;
} mips32_op[] = {
	{ 0, MIPS_INS_INVALID, "invalid", 0, 0, 0, 0 },
	{ MIPS_COP1_CUSTOM, MIPS_INS_ABS_S_FMT_CUSTOM, "abs.s", mips32_operate_abs_s, 2, 1, BOTH },
	{ MIPS_SPECIAL_CUSTOM, MIPS_INS_ADD_CUSTOM, "add", mips32_operate_add, 3, 2, BOTH },
	{ MIPS_COP1_CUSTOM, MIPS_INS_ADD_FMT_CUSTOM, "add.s", mips32_operate_add_s, 3, 3, BOTH },
	{ MIPS_NONE_SPECIAL_CUSTOM, MIPS_INS_ADDI_CUSTOM, "addi", mips32_operate_addi, 3, 4, FIRST },
	{ MIPS_NONE_SPECIAL_CUSTOM, MIPS_INS_ADDIU_CUSTOM, "addiu", mips32_operate_addiu, 3, 4, FIRST },
	{ MIPS_NONE_SPECIAL_CUSTOM, MIPS_INS_SW_CUSTOM, "sw", mips32_operate_sw, 3, 5, FIRST },
	{ MIPS_SPECIAL_CUSTOM, MIPS_INS_OR_CUSTOM, "or", mips32_operate_or, 3, 2, BOTH },
	{ MIPS_NONE_SPECIAL_CUSTOM, MIPS_INS_LUI_CUSTOM, "lui", mips32_operate_lui, 2, 6, FIRST },
	{ MIPS_NONE_SPECIAL_CUSTOM, MIPS_INS_LW_CUSTOM, "lw", mips32_operate_lw, 3, 5, FIRST },
	{ MIPS_NONE_SPECIAL_CUSTOM, MIPS_INS_SLTI_CUSTOM, "slti", mips32_operate_slti, 3, 4, FIRST },
	{ MIPS_NONE_SPECIAL_CUSTOM, MIPS_INS_BEQ_CUSTOM, "beq", mips32_operate_beq, 3, 7, FIRST },
	{ MIPS_SPECIAL_CUSTOM, MIPS_INS_NOP_CUSTOM, "nop", mips32_operate_nop, 0, 8, BOTH },
	{ MIPS_SPECIAL_CUSTOM, MIPS_INS_JALR_CUSTOM, "jalr", mips32_operate_jalr, 1, 9, BOTH },
	{ MIPS_SPECIAL_CUSTOM, MIPS_INS_JR_CUSTOM, "jr", mips32_operate_jr, 1, 10, BOTH },
	{ MIPS_REGIMM_CUSTOM, MIPS_INS_BGEZAL_CUSTOM, "bgezal", mips32_operate_bgezal, 2, 11, BOTH },
	{ MIPS_SPECIAL_CUSTOM, MIPS_INS_AND_CUSTOM, "and", mips32_operate_and, 3, 2, BOTH },
	{ MIPS_NONE_SPECIAL_CUSTOM, MIPS_INS_JAL_CUSTOM, "jal", mips32_operate_jal, 1, 12, FIRST },
	{ MIPS_SPECIAL_CUSTOM, MIPS_INS_ADDU_CUSTOM, "addu", mips32_operate_addu, 3, 2, BOTH }
};

int mips32_ops_count = sizeof ( mips32_op ) / sizeof ( struct mips32_operators );

#include "mips32_reg.h"

struct mips32_cpu {
	unsigned int r;
	char name[15];
} cpu[] = {
	{ MIPS_REG_ZERO_CUSTOM, "$zero" },
	{ MIPS_REG_AT_CUSTOM, "$at" },
	{ MIPS_REG_V0_CUSTOM, "$v0" },
	{ MIPS_REG_V1_CUSTOM, "$v1" },
	{ MIPS_REG_A0_CUSTOM, "$a0" },
	{ MIPS_REG_A1_CUSTOM, "$a1" },
	{ MIPS_REG_A2_CUSTOM, "$a2" },
	{ MIPS_REG_A3_CUSTOM, "$a3" },
	{ MIPS_REG_T0_CUSTOM, "$t0" },
	{ MIPS_REG_T1_CUSTOM, "$t1" },
	{ MIPS_REG_T2_CUSTOM, "$t2" },
	{ MIPS_REG_T3_CUSTOM, "$t3" },
	{ MIPS_REG_T4_CUSTOM, "$t4" },
	{ MIPS_REG_T5_CUSTOM, "$t5" },
	{ MIPS_REG_T6_CUSTOM, "$t6" },
	{ MIPS_REG_T7_CUSTOM, "$t7" },
	{ MIPS_REG_S0_CUSTOM, "$s0" },
	{ MIPS_REG_S1_CUSTOM, "$s1" },
	{ MIPS_REG_S2_CUSTOM, "$s2" },
	{ MIPS_REG_S3_CUSTOM, "$s3" },
	{ MIPS_REG_S4_CUSTOM, "$s4" },
	{ MIPS_REG_S5_CUSTOM, "$s5" },
	{ MIPS_REG_S6_CUSTOM, "$s6" },
	{ MIPS_REG_S7_CUSTOM, "$s7" },
	{ MIPS_REG_T8_CUSTOM, "$t8" },
	{ MIPS_REG_T9_CUSTOM, "$t9" },
	{ MIPS_REG_K0_CUSTOM, "$k0" },
	{ MIPS_REG_K1_CUSTOM, "$k1" },
	{ MIPS_REG_GP_CUSTOM, "$gp" },
	{ MIPS_REG_SP_CUSTOM, "$sp" },
	{ MIPS_REG_FP_CUSTOM, "$fp" },
	{ MIPS_REG_RA_CUSTOM, "$ra" }
};

const int mips32_cpu_count = sizeof ( cpu ) / sizeof ( struct mips32_cpu );
#endif
