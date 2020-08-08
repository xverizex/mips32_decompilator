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
 */

struct mips32_registers {
	unsigned int *rs;
	unsigned int *rt;
	unsigned int *rd;
	unsigned int *fs;
	unsigned int *fd;
	unsigned int *base;
} mr;

void mips32_operate_add ( struct mips32_registers *mr, unsigned int static_number );
void mips32_operate_abs_s ( struct mips32_registers *mr, unsigned int static_number );
void mips32_operate_add_s ( struct mips32_registers *mr, unsigned int static_number );
void mips32_operate_addi ( struct mips32_registers *mr, unsigned int static_number );
void mips32_operate_addiu ( struct mips32_registers *mr, unsigned int static_number );
void mips32_operate_sw ( struct mips32_registers *mr, unsigned int static_number );
void mips32_operate_or ( struct mips32_registers *mr, unsigned int static_number );
void mips32_operate_lui ( struct mips32_registers *mr, unsigned int static_number );
void mips32_operate_lw ( struct mips32_registers *mr, unsigned int static_number );

#define FIRST        0
#define LAST         1
#define BOTH         2

#define MIPS_SPECIAL_CUSTOM           0x0
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

const int both[] = {
	MIPS_SPECIAL_CUSTOM,
	MIPS_COP1_CUSTOM
};

int both_count = sizeof ( both ) / sizeof ( int );

struct mips32_operators {
	unsigned int special;
	char special_cmd[255];
	void (*operate) ( struct mips32_registers * mr, unsigned int static_number );
	unsigned int count;
	unsigned int scheme;
	unsigned int check;
} mips32_op[] = {
	{ MIPS_INS_INVALID, "invalid", 0, 0, 0, 0 },
	{ MIPS_INS_ABS_S_FMT_CUSTOM, "abs.s", mips32_operate_abs_s, 2, 1, BOTH },
	{ MIPS_INS_ADD_CUSTOM, "add", mips32_operate_add, 3, 2, BOTH },
	{ MIPS_INS_ADD_FMT_CUSTOM, "add.s", mips32_operate_add_s, 3, 3, BOTH },
	{ MIPS_INS_ADDI_CUSTOM, "addi", mips32_operate_addi, 3, 4, FIRST },
	{ MIPS_INS_ADDIU_CUSTOM, "addiu", mips32_operate_addiu, 3, 4, FIRST },
	{ MIPS_INS_SW_CUSTOM, "sw", mips32_operate_sw, 3, 5, FIRST },
	{ MIPS_INS_OR_CUSTOM, "or", mips32_operate_or, 3, 2, BOTH },
	{ MIPS_INS_LUI_CUSTOM, "lui", mips32_operate_lui, 2, 6, FIRST },
	{ MIPS_INS_LW_CUSTOM, "lw", mips32_operate_lw, 3, 5, FIRST }
};

int mips32_ops_count = sizeof ( mips32_op ) / sizeof ( struct mips32_operators );

#define MIPS_REG_ZERO_CUSTOM         0
#define MIPS_REG_AT_CUSTOM           1
#define MIPS_REG_V0_CUSTOM           2
#define MIPS_REG_V1_CUSTOM           3
#define MIPS_REG_A0_CUSTOM           4
#define MIPS_REG_A1_CUSTOM           5
#define MIPS_REG_A2_CUSTOM           6
#define MIPS_REG_A3_CUSTOM           7
#define MIPS_REG_T0_CUSTOM           8
#define MIPS_REG_T1_CUSTOM           9
#define MIPS_REG_T2_CUSTOM          10
#define MIPS_REG_T3_CUSTOM          11
#define MIPS_REG_T4_CUSTOM          12
#define MIPS_REG_T5_CUSTOM          13
#define MIPS_REG_T6_CUSTOM          14
#define MIPS_REG_T7_CUSTOM          15
#define MIPS_REG_S0_CUSTOM          16
#define MIPS_REG_S1_CUSTOM          17
#define MIPS_REG_S2_CUSTOM          18
#define MIPS_REG_S3_CUSTOM          19
#define MIPS_REG_S4_CUSTOM          20
#define MIPS_REG_S5_CUSTOM          21
#define MIPS_REG_S6_CUSTOM          22
#define MIPS_REG_S7_CUSTOM          23
#define MIPS_REG_T8_CUSTOM          24
#define MIPS_REG_T9_CUSTOM          25
#define MIPS_REG_K0_CUSTOM          26
#define MIPS_REG_K1_CUSTOM          27
#define MIPS_REG_GP_CUSTOM          28
#define MIPS_REG_SP_CUSTOM          29
#define MIPS_REG_FP_CUSTOM          30
#define MIPS_REG_RA_CUSTOM          31

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
