#ifndef __MIPS32_REGISTERS__
#define __MIPS32_REGISTERS__
struct mips32_registers {
	unsigned int rs;
	unsigned int rt;
	unsigned int rd;
	unsigned int fs;
	unsigned int fd;
	unsigned int cc;
	unsigned int nd;
	unsigned int tf;
	unsigned int base;
};
#endif
