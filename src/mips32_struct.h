#ifndef __MIPS32_STRUCT__
#define __MIPS32_STRUCT__
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
 *
 * 13.
 * 	31-26 cop1
 * 	25-21 operate
 * 	20-18 cc
 * 	17    nd
 * 	16    tf
 * 	15-0  offset
 *
 * 14.
 * 	31-26 special
 * 	25-6  code
 * 	5-0   operate
 *
 * 15.
 * 	31-26 special
 * 	25-21 fmt
 * 	20-16 ft
 * 	15-11 fs
 * 	10-8  cc
 * 	7     0
 * 	6     a
 * 	5-4   fc
 * 	3-0   cond
 *
 * 16.
 * 	31-26 operate
 * 	25-21 base
 * 	20-16 op
 * 	15-0  offset
 *
 * 17.
 * 	31-26 cop1
 * 	25-21 fmt
 * 	20-16 zero
 * 	15-11 fs
 * 	10-6  fd
 * 	5-0   operate
 *
 * 18.
 * 	31-26 cop1
 * 	25-21 operate
 * 	20-16 rt
 * 	15-11 fs
 * 	10-0  zero
 *
 * 19.
 * 	31-26 cop2
 * 	25-21 operate
 * 	20-16 rt
 * 	15-11 rd
 * 	10-0  zero
 *
 * 20.
 * 	31-26 cop2
 * 	25    co == 1
 * 	24-0  cofun
 * 
 * 21.
 * 	31-26 special
 * 	25-21 zero
 * 	20-16 rt
 * 	15-11 rd
 * 	10-6  sa
 * 	5-0   operate
 *
 */
#include "mips32_registers.h"

void mips32_operate_add ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_abs_fmt ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_add_fmt ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_addi ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_addiu ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_sw ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_or ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_lui ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_lw ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_beq ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_nop ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_jalr ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_jr ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_bgezal ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_and ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_jal ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_addu ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_andi ( struct mips32_registers *mr, short static_number, int );
void mips32_cop1_operate_bc ( struct mips32_registers *mr, short static_number, int );
void mips32_cop2_operate_bc ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_beql ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_bgez ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_bgezall ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_bgezl ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_bgtz ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_bgtzl ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_blez ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_blezl ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_bltz ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_bltzal ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_bltzall ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_bltzl ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_bne ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_bnel ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_break ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_c_cond ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_cache ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_ceil_w ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_cfc1 ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_cfc2 ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_clo ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_clz ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_cop2 ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_ctc1 ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_ctc2 ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_cvt_d ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_cvt_s ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_cvt_w ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_deret ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_div ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_div_fmt ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_divu ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_eret ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_floor_w_fmt ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_j ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_lb ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_lbu ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_ldc1 ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_ldc2 ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_lh ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_lhu ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_ll ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_lwc1 ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_lwc2 ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_lwl ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_lwr ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_madd ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_maddu ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_mfc0 ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_mfc1 ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_mfc2 ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_mfhi ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_mflo ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_mov_fmt ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_movf ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_movf_fmt ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_movn ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_movn_fmt ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_movt ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_movt_fmt ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_movz ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_movz_fmt ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_msub ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_msubu ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_mtc0 ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_mtc1 ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_mtc2 ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_mthi ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_mtlo ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_mul ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_mul_fmt ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_mult ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_multu ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_neg_fmt ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_nor ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_ori ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_pref ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_round_w ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_sb ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_sc ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_sdbbp ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_sdc1 ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_sdc2 ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_sh ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_sll ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_sllv ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_slt ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_slti ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_sltiu ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_sltu ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_sqrt_fmt ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_sra ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_srav ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_srl ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_srlv ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_ssnop ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_sub ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_sub_fmt ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_subu ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_swc1 ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_swc2 ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_swl ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_swr ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_sync ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_syscall ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_teq ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_teqi ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_tge ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_tgei ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_tgeiu ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_tgeu ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_tlbp ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_tlbr ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_tlbwi ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_tlbwr ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_tlt ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_tlti ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_tltiu ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_tltu ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_tne ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_tnei ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_trunc_w ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_wait ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_xor ( struct mips32_registers *mr, short static_number, int );
void mips32_operate_xori ( struct mips32_registers *mr, short static_number, int );

#define FIRST        0
#define LAST         1
#define BOTH         2
#define ONLY_COP1    3
#define SECOND       4

#define MIPS_NONE_SPECIAL_CUSTOM      0xfffff
#define MIPS_SPECIAL_CUSTOM           0x0
#define MIPS_SPECIAL_2_CUSTOM         0x1b
#define MIPS_REGIMM_CUSTOM            0x1
#define MIPS_COP1_CUSTOM              0x11
#define MIPS_COP2_CUSTOM              0x12
#define MIPS_COP0_CUSTOM              0x10
#define MIPS_COP2_CO_CUSTOM           0xffff
#define MIPS_INS_ABS_FMT_CUSTOM       0x5
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
#define MIPS_INS_ANDI_CUSTOM          0xc
#define MIPS_INS_BC_CUSTOM            0x8
#define MIPS_INS_BEQL_CUSTOM          0x14
#define MIPS_INS_BGEZ_CUSTOM          0x1
#define MIPS_INS_BGEZALL_CUSTOM       0x13
#define MIPS_INS_BGEZL_CUSTOM         0x3
#define MIPS_INS_BGTZ_CUSTOM          0x7
#define MIPS_INS_BGTZL_CUSTOM         0x17
#define MIPS_INS_BLEZ_CUSTOM          0x6
#define MIPS_INS_BLEZL_CUSTOM         0x16
#define MIPS_INS_BLTZ_CUSTOM          0x0
#define MIPS_INS_BLTZAL_CUSTOM        0x10
#define MIPS_INS_BLTZALL_CUSTOM       0x12
#define MIPS_INS_BLTZL_CUSTOM         0x2
#define MIPS_INS_BNE_CUSTOM           0x5
#define MIPS_INS_BNEL_CUSTOM          0x15
#define MIPS_INS_BREAK_CUSTOM         0xd
#define MIPS_INS_CACHE_CUSTOM         0x1f
#define MIPS_INS_CEIL_W_CUSTOM        0xe
#define MIPS_INS_CFC1_CUSTOM          0x2
#define MIPS_INS_CFC2_CUSTOM          0x2
#define MIPS_INS_CLO_CUSTOM           0x21
#define MIPS_INS_CLZ_CUSTOM           0x20
#define MIPS_INS_CTC1_CUSTOM          0x6
#define MIPS_INS_CTC2_CUSTOM          0x6
#define MIPS_INS_CVT_D_FMT_CUSTOM     0x21
#define MIPS_INS_CVT_S_FMT_CUSTOM     0x20
#define MIPS_INS_CVT_W_FMT_CUSTOM     0x24
#define MIPS_INS_DERET_CUSTOM         0x1f
#define MIPS_INS_DIV_CUSTOM           0x1a
#define MIPS_INS_DIV_FMT_CUSTOM       0x3
#define MIPS_INS_DIVU_CUSTOM          0x1b
#define MIPS_INS_ERET_CUSTOM          0x18
#define MIPS_INS_FLOOR_W_CUSTOM       0xf
#define MIPS_INS_J_CUSTOM             0x2
#define MIPS_INS_LB_CUSTOM            0x20
#define MIPS_INS_LBU_CUSTOM           0x24
#define MIPS_INS_LDC1_CUSTOM          0x35
#define MIPS_INS_LDC2_CUSTOM          0x36
#define MIPS_INS_LH_CUSTOM            0x21
#define MIPS_INS_LHU_CUSTOM           0x25
#define MIPS_INS_LL_CUSTOM            0x30
#define MIPS_INS_LWC1_CUSTOM          0x31
#define MIPS_INS_LWC2_CUSTOM          0x32
#define MIPS_INS_LWL_CUSTOM           0x22
#define MIPS_INS_LWR_CUSTOM           0x26
#define MIPS_INS_MADD_CUSTOM          0x0
#define MIPS_INS_MADDU_CUSTOM         0x1
#define MIPS_INS_MFC0_CUSTOM          0x0
#define MIPS_INS_MFC1_CUSTOM          0x0
#define MIPS_INS_MFC2_CUSTOM          0x0
#define MIPS_INS_MFHI_CUSTOM          0x10
#define MIPS_INS_MFLO_CUSTOM          0x12
#define MIPS_INS_MOV_FMT_CUSTOM       0x6
#define MIPS_INS_MOVF_CUSTOM          0x1
#define MIPS_INS_MOVF_FMT_CUSTOM      0x11
#define MIPS_INS_MOVN_CUSTOM          0xb
#define MIPS_INS_MOVN_FMT_CUSTOM      0x13
#define MIPS_INS_MOVT_CUSTOM          0x1
#define MIPS_INS_MOVT_FMT_CUSTOM      0x11
#define MIPS_INS_MOVZ_CUSTOM          0xa
#define MIPS_INS_MOVZ_FMT_CUSTOM      0x12
#define MIPS_INS_MSUB_CUSTOM          0x4
#define MIPS_INS_MSUBU_CUSTOM         0x5
#define MIPS_INS_MTC0_CUSTOM          0x4
#define MIPS_INS_MTC1_CUSTOM          0x4
#define MIPS_INS_MTC2_CUSTOM          0x4
#define MIPS_INS_MTHI_CUSTOM          0x11
#define MIPS_INS_MTLO_CUSTOM          0x13
#define MIPS_INS_MUL_CUSTOM           0x2
#define MIPS_INS_MUL_FMT_CUSTOM       0x2
#define MIPS_INS_MULT_CUSTOM          0x18
#define MIPS_INS_MULTU_CUSTOM         0x19
#define MIPS_INS_NEG_CUSTOM           0x7
#define MIPS_INS_NOR_CUSTOM           0x27
#define MIPS_INS_ORI_CUSTOM           0xd
#define MIPS_INS_PREF_CUSTOM          0x33
#define MIPS_INS_ROUND_W_FMT_CUSTOM   0xc
#define MIPS_INS_SB_CUSTOM            0x28
#define MIPS_INS_SC_CUSTOM            0x38
#define MIPS_INS_SDBBP_CUSTOM         0x3f
#define MIPS_INS_SDC1_CUSTOM          0x3d
#define MIPS_INS_SDC2_CUSTOM          0x3e
#define MIPS_INS_SH_CUSTOM            0x29
#define MIPS_INS_SLL_CUSTOM           0x0
#define MIPS_INS_SLLV_CUSTOM          0x4
#define MIPS_INS_SLT_CUSTOM           0x2a
#define MIPS_INS_SLTI_CUSTOM          0xa
#define MIPS_INS_SLTIU_CUSTOM         0xb
#define MIPS_INS_SLTU_CUSTOM          0x2b
#define MIPS_INS_SQRT_FMT_CUSTOM      0x4
#define MIPS_INS_SRA_CUSTOM           0x3
#define MIPS_INS_SRAV_CUSTOM          0x7
#define MIPS_INS_SRL_CUSTOM           0x2
#define MIPS_INS_SRLV_CUSTOM          0x6
#define MIPS_INS_SLLNOP_CUSTOM        0x0
#define MIPS_INS_SUB_CUSTOM           0x22
#define MIPS_INS_SUB_FMT_CUSTOM       0x1
#define MIPS_INS_SUBU_CUSTOM          0x23
#define MIPS_INS_SWC1_CUSTOM          0x39
#define MIPS_INS_SWC2_CUSTOM          0x3a
#define MIPS_INS_SWL_CUSTOM           0x2a
#define MIPS_INS_SWR_CUSTOM           0x2e
#define MIPS_INS_SYNC_CUSTOM          0xf
#define MIPS_INS_SYSCALL_CUSTOM       0xc
#define MIPS_INS_TEQ_CUSTOM           0x34
#define MIPS_INS_TEQI_CUSTOM          0xc
#define MIPS_INS_TGE_CUSTOM           0x30
#define MIPS_INS_TGEI_CUSTOM          0x8
#define MIPS_INS_TGEIU_CUSTOM         0x9
#define MIPS_INS_TGEU_CUSTOM          0x31
#define MIPS_INS_TLBP_CUSTOM          0x8
#define MIPS_INS_TLBR_CUSTOM          0x1
#define MIPS_INS_TLBWI_CUSTOM         0x2
#define MIPS_INS_TLBWR_CUSTOM         0x6
#define MIPS_INS_TLT_CUSTOM           0x32
#define MIPS_INS_TLTI_CUSTOM          0xa
#define MIPS_INS_TLTIU_CUSTOM         0xb
#define MIPS_INS_TLTU_CUSTOM          0x33
#define MIPS_INS_TNE_CUSTOM           0x36
#define MIPS_INS_TNEI_CUSTOM          0xe
#define MIPS_INS_TRUNC_W_FMT_CUSTOM   0xd
#define MIPS_INS_WAIT_CUSTOM          0x20
#define MIPS_INS_XOR_CUSTOM           0x26
#define MIPS_INS_XORI_CUSTOM          0xe
#define MIPS_INS_CONDITION_CUSTOM     0x0

const int both[] = {
	MIPS_SPECIAL_CUSTOM,
	MIPS_COP1_CUSTOM,
	MIPS_COP2_CUSTOM
};

enum {
	FMT_D = 0x11,
	FMT_S = 0x10
};

struct fmt {
	unsigned int type;
	char name[255];
} fmt_struct[] = {
	{ FMT_D, "d" },
	{ FMT_S, "s" }
};

const int fmt_count = sizeof ( fmt_struct ) / sizeof ( struct fmt );

int both_count = sizeof ( both ) / sizeof ( int );

#define INDEX_MIPS_INS_NOP       12

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
	{ MIPS_COP1_CUSTOM, MIPS_INS_ABS_FMT_CUSTOM, "abs", mips32_operate_abs_fmt, 2, 1, BOTH },
	{ MIPS_SPECIAL_CUSTOM, MIPS_INS_ADD_CUSTOM, "add", mips32_operate_add, 3, 2, BOTH },
	{ MIPS_COP1_CUSTOM, MIPS_INS_ADD_FMT_CUSTOM, "add.", mips32_operate_add_fmt, 3, 3, BOTH },
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
	{ MIPS_SPECIAL_CUSTOM, MIPS_INS_ADDU_CUSTOM, "addu", mips32_operate_addu, 3, 2, BOTH },
	{ MIPS_NONE_SPECIAL_CUSTOM, MIPS_INS_ANDI_CUSTOM, "andi", mips32_operate_andi, 3, 4, FIRST },
	{ MIPS_COP1_CUSTOM, MIPS_INS_BC_CUSTOM, "bc", mips32_cop1_operate_bc, 2, 13, SECOND },
	{ MIPS_COP2_CUSTOM, MIPS_INS_BC_CUSTOM, "bc", mips32_cop2_operate_bc, 2, 13, SECOND },
	{ MIPS_NONE_SPECIAL_CUSTOM, MIPS_INS_BEQL_CUSTOM, "beq", mips32_operate_beql, 3, 7, FIRST },
	{ MIPS_REGIMM_CUSTOM, MIPS_INS_BGEZ_CUSTOM, "bgez", mips32_operate_bgez, 2, 11, BOTH },
	{ MIPS_REGIMM_CUSTOM, MIPS_INS_BGEZALL_CUSTOM, "bgezall", mips32_operate_bgezall, 2, 11, BOTH },
	{ MIPS_REGIMM_CUSTOM, MIPS_INS_BGEZL_CUSTOM, "bgezl", mips32_operate_bgezl, 2, 11, BOTH },
	{ MIPS_NONE_SPECIAL_CUSTOM, MIPS_INS_BGTZ_CUSTOM, "bgtz", mips32_operate_bgtz, 3, 7, FIRST },
	{ MIPS_NONE_SPECIAL_CUSTOM, MIPS_INS_BGTZL_CUSTOM, "bgtzl", mips32_operate_bgtzl, 3, 7, FIRST },
	{ MIPS_NONE_SPECIAL_CUSTOM, MIPS_INS_BLEZ_CUSTOM, "blez", mips32_operate_blez, 3, 7, FIRST },
	{ MIPS_NONE_SPECIAL_CUSTOM, MIPS_INS_BLEZL_CUSTOM, "blezl", mips32_operate_blezl, 3, 7, FIRST },
	{ MIPS_REGIMM_CUSTOM, MIPS_INS_BLTZ_CUSTOM, "bltz", mips32_operate_bltz, 2, 11, BOTH },
	{ MIPS_NONE_SPECIAL_CUSTOM, MIPS_INS_BEQL_CUSTOM, "beql", mips32_operate_beql, 3, 4, FIRST },
	{ MIPS_REGIMM_CUSTOM, MIPS_INS_BLTZAL_CUSTOM, "bltzal", mips32_operate_bltzal, 2, 11, BOTH },
	{ MIPS_REGIMM_CUSTOM, MIPS_INS_BLTZALL_CUSTOM, "bltzall", mips32_operate_bltzall, 2, 11, BOTH },
	{ MIPS_REGIMM_CUSTOM, MIPS_INS_BLTZL_CUSTOM, "bltzl", mips32_operate_bltzl, 2, 11, BOTH },
	{ MIPS_NONE_SPECIAL_CUSTOM, MIPS_INS_BNE_CUSTOM, "bne", mips32_operate_bne, 3, 7, FIRST },
	{ MIPS_NONE_SPECIAL_CUSTOM, MIPS_INS_BNEL_CUSTOM, "bnel", mips32_operate_bnel, 3, 7, FIRST },
	{ MIPS_SPECIAL_CUSTOM, MIPS_INS_BREAK_CUSTOM, "break", mips32_operate_break, 0, 14, BOTH },
	{ MIPS_COP1_CUSTOM, MIPS_INS_CONDITION_CUSTOM, "c", mips32_operate_c_cond, 2, 15, ONLY_COP1 },
	{ MIPS_NONE_SPECIAL_CUSTOM, MIPS_INS_CACHE_CUSTOM, "cache", mips32_operate_cache, 3, 16, FIRST },
	{ MIPS_COP1_CUSTOM, MIPS_INS_CEIL_W_CUSTOM, "ceil.w", mips32_operate_ceil_w, 2, 17, BOTH },
	{ MIPS_COP1_CUSTOM, MIPS_INS_CFC1_CUSTOM, "cfc1", mips32_operate_cfc1, 2, 18, SECOND },
	// { MIPS_COP2_CUSTOM, MIPS_INS_CFC2_CUSTOM, "cfc2", mips32_operate_cfc2, 2, 19, SECOND },
	{ MIPS_SPECIAL_CUSTOM, MIPS_INS_CLO_CUSTOM, "clo", mips32_operate_clo, 2, 2, BOTH },
	{ MIPS_SPECIAL_CUSTOM, MIPS_INS_CLZ_CUSTOM, "clz", mips32_operate_clz, 2, 2, BOTH },
	{ MIPS_COP2_CUSTOM, MIPS_COP2_CO_CUSTOM, "cop2", mips32_operate_cop2, 1, 20, FIRST },
	{ MIPS_COP1_CUSTOM, MIPS_INS_CTC1_CUSTOM, "ctc1", mips32_operate_ctc1, 2, 18, SECOND },
	{ MIPS_SPECIAL_CUSTOM, MIPS_INS_SLL_CUSTOM, "sll", mips32_operate_sll, 3, 21, BOTH }
};

int mips32_ops_count = sizeof ( mips32_op ) / sizeof ( struct mips32_operators );

#include "mips32_reg.h"

struct mips32_cpu {
	unsigned int r;
	char name[15];
} cpu[] = {
	{ MIPS_REG_ZERO_CUSTOM, "zero" },
	{ MIPS_REG_AT_CUSTOM, "at" },
	{ MIPS_REG_V0_CUSTOM, "v0" },
	{ MIPS_REG_V1_CUSTOM, "v1" },
	{ MIPS_REG_A0_CUSTOM, "a0" },
	{ MIPS_REG_A1_CUSTOM, "a1" },
	{ MIPS_REG_A2_CUSTOM, "a2" },
	{ MIPS_REG_A3_CUSTOM, "a3" },
	{ MIPS_REG_T0_CUSTOM, "t0" },
	{ MIPS_REG_T1_CUSTOM, "t1" },
	{ MIPS_REG_T2_CUSTOM, "t2" },
	{ MIPS_REG_T3_CUSTOM, "t3" },
	{ MIPS_REG_T4_CUSTOM, "t4" },
	{ MIPS_REG_T5_CUSTOM, "t5" },
	{ MIPS_REG_T6_CUSTOM, "t6" },
	{ MIPS_REG_T7_CUSTOM, "t7" },
	{ MIPS_REG_S0_CUSTOM, "s0" },
	{ MIPS_REG_S1_CUSTOM, "s1" },
	{ MIPS_REG_S2_CUSTOM, "s2" },
	{ MIPS_REG_S3_CUSTOM, "s3" },
	{ MIPS_REG_S4_CUSTOM, "s4" },
	{ MIPS_REG_S5_CUSTOM, "s5" },
	{ MIPS_REG_S6_CUSTOM, "s6" },
	{ MIPS_REG_S7_CUSTOM, "s7" },
	{ MIPS_REG_T8_CUSTOM, "t8" },
	{ MIPS_REG_T9_CUSTOM, "t9" },
	{ MIPS_REG_K0_CUSTOM, "k0" },
	{ MIPS_REG_K1_CUSTOM, "k1" },
	{ MIPS_REG_GP_CUSTOM, "gp" },
	{ MIPS_REG_SP_CUSTOM, "sp" },
	{ MIPS_REG_FP_CUSTOM, "fp" },
	{ MIPS_REG_RA_CUSTOM, "ra" }
};

struct mips32_cpu cpuf[] = {
	{ MIPS_REG_F0_CUSTOM, "f0" },
	{ MIPS_REG_F1_CUSTOM, "f1" },
	{ MIPS_REG_F2_CUSTOM, "f2" },
	{ MIPS_REG_F3_CUSTOM, "f3" },
	{ MIPS_REG_F4_CUSTOM, "f4" },
	{ MIPS_REG_F5_CUSTOM, "f5" },
	{ MIPS_REG_F6_CUSTOM, "f6" },
	{ MIPS_REG_F7_CUSTOM, "f7" },
	{ MIPS_REG_F8_CUSTOM, "f8" },
	{ MIPS_REG_F9_CUSTOM, "f9" },
	{ MIPS_REG_F10_CUSTOM, "f10" },
	{ MIPS_REG_F11_CUSTOM, "f11" },
	{ MIPS_REG_F12_CUSTOM, "f12" },
	{ MIPS_REG_F13_CUSTOM, "f13" },
	{ MIPS_REG_F14_CUSTOM, "f14" },
	{ MIPS_REG_F15_CUSTOM, "f15" },
	{ MIPS_REG_F16_CUSTOM, "f16" },
	{ MIPS_REG_F17_CUSTOM, "f17" },
	{ MIPS_REG_F18_CUSTOM, "f18" },
	{ MIPS_REG_F19_CUSTOM, "f19" },
	{ MIPS_REG_F20_CUSTOM, "f20" },
	{ MIPS_REG_F21_CUSTOM, "f21" },
	{ MIPS_REG_F22_CUSTOM, "f22" },
	{ MIPS_REG_F23_CUSTOM, "f23" },
	{ MIPS_REG_F24_CUSTOM, "f24" },
	{ MIPS_REG_F25_CUSTOM, "f25" },
	{ MIPS_REG_F26_CUSTOM, "f26" },
	{ MIPS_REG_F27_CUSTOM, "f27" },
	{ MIPS_REG_F28_CUSTOM, "f28" },
	{ MIPS_REG_F29_CUSTOM, "f29" },
	{ MIPS_REG_F30_CUSTOM, "f30" },
	{ MIPS_REG_F31_CUSTOM, "f31" }
};

const int mips32_cpu_count = sizeof ( cpu ) / sizeof ( struct mips32_cpu );
const int mips32_cpuf_count = sizeof ( cpuf ) / sizeof ( struct mips32_cpu );

struct condition {
	unsigned int type;
	char name[255];
} condition[] = {
	{ MIPS_CONDITION_LT_CUSTOM, "lt" },
	{ MIPS_CONDITION_EQ_CUSTOM, "eq" },
	{ MIPS_CONDITION_LE_CUSTOM, "le" }
};

const int mips32_cond_count = sizeof ( condition ) / sizeof ( struct condition );
#endif
