#include "mips32_registers.h"
#include "mips32_reg.h"
#include <stdio.h>

extern struct mips32_operators mips32_op;

extern struct mips32_cup cpu;

static struct mips32_regs {
	unsigned int r[32];
} cpuc;

static void mips32_cpuc_set_register ( unsigned int reg, unsigned int number ) {
}

unsigned int mips32_get_gp ( ) {
	return cpuc.r[MIPS_REG_GP_CUSTOM];
}

void mips32_operate_abs_s ( struct mips32_registers *mr, unsigned int static_number ) {
}

void mips32_operate_add ( struct mips32_registers *mr, unsigned int static_number ) {
}

void mips32_operate_add_s ( struct mips32_registers *mr, unsigned int static_number ) {
}

void mips32_operate_addi ( struct mips32_registers *mr, unsigned int static_number ) {
}

void mips32_operate_addiu ( struct mips32_registers *mr, short static_number ) {
	cpuc.r[mr->rt] = cpuc.r[mr->rs] + static_number;
	printf ( "addiu: %x\n", cpuc.r[mr->rt] );
}

void mips32_operate_sw ( struct mips32_registers *mr, unsigned int static_number ) {
}

void mips32_operate_or ( struct mips32_registers *mr, unsigned int static_number ) {
}

void mips32_operate_lui ( struct mips32_registers *mr, unsigned short static_number ) {
	cpuc.r[mr->rt] = 0;
	cpuc.r[mr->rt] = ( static_number << 16 ) & 0xffff0000;
}

void mips32_operate_lw ( struct mips32_registers *mr, unsigned int static_number ) {
	cpuc.r[mr->rt] = cpuc.r[mr->base] + static_number;
	printf ( "lw::%x\n", cpuc.r[mr->rt] );
}

void mips32_operate_slti ( struct mips32_registers *mr, unsigned int static_number ) {
}

void mips32_operate_beq ( struct mips32_registers *mr, unsigned int static_number ) {
}

void mips32_operate_nop ( struct mips32_registers *mr, unsigned int static_number ) {
}
