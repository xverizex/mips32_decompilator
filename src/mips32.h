#ifndef __MIPS32__
#define __MIPS32__
#include <elf.h>
void mips32_exec ( const Elf32_Ehdr * const program_header, const char * const program_buffer, const int engian, const char * const out_file );
#endif
