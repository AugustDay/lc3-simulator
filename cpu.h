/*
    cpu.h
	
    Programer: George Mobus
    Austin Ingraham, Arthur Panlilio
	 Date: 4/20/16
	 Description:
	   This header file contains the definitions for the CPU object.
*/

#include <stdio.h>

#ifndef _CPU_H_
#define _CPU_H_

#define POINTER_ERROR -1
#define INDEX_OUT_OF_BOUNDS -1

#define NO_OF_REGISTERS 8

#define OPCODE_MASK 0xF000
#define DR_MASK 0x0E00
#define SR1_MASK 0x01C0
#define SR2_MASK 0x0003

#define IMMED5_MASK 0x001F
#define OFFSET6_MASK 0x003F
#define OFFSET9_MASK 0x01FF
#define OFFSET11_MASK 0x07FF
#define BASER_MASK 0x01C0
#define TRAPVECT8_MASK 0x00FF
#define BIT5_MASK 0x0020

#define RD_MASK 0x1C00
#define RS_MASK 0x0380
#define IMMED_MASK 0x007F
#define SIGN_MASK 0x0040  
#define NEG_SIGN_EXTEND 0xFFC0 

/* Old OPCODES */

// #define LDI 30
#define BNZ 60
// #define HALT 70

/* New OPCODES, in order they appear on Page 119 of textbook */
#define ADD 1
     // ADD(I) <- flag bit (index 5) determines operation 
#define AND 5
     // AND(I) <- flag bit determines operation
#define BR 0
#define JMP 12
     // RET is the same command, basically (see Page 529 of textbook)
#define JSR 4
#define LEA 14
#define LD 4
#define LDR 6
#define LDI 10
#define ST 3
#define STR 7
#define STI 11
#define NOT 9
#define TRAP 15

/* Sign-Extension codes */
#define IMMED5_SIGN 0
#define IMMED5_SIGN_MASK 0x0010
#define IMMED5_SIGN_EXTEND 0xFFF0

#define OFFSET6_SIGN 1
#define OFFSET6_SIGN_MASK 0x0020
#define OFFSET6_SIGN_EXTEND 0xFFE0

#define OFFSET9_SIGN 2
#define OFFSET9_SIGN_MASK 0x0100
#define OFFSET9_SIGN_EXTEND 0xFF00

#define OFFSET11_SIGN 3
#define OFFSET11_SIGN_MASK 0x0400
#define OFFSET11_SIGN_EXTEND 0xFC00

#define TRAPVECTOR_SIGN 4
#define TRAPVECTOR_SIGN_MASK 0x0080
#define TRAPVECTOR_SIGN_EXTEND 0xFFC0

#define GETC 32
#define OUT	 33
#define PUTS 34
#define HALT 37


typedef unsigned short Register;
typedef unsigned char Byte;

typedef struct alu_s {
	Register  a, b, r;
} ALU_s;

typedef ALU_s * ALU_p;

typedef struct cpu_s {
	Register reg_file[NO_OF_REGISTERS];
	ALU_p alu;
	Register ir, pc, sw; 	// IR, PC, and SW (status word)
	Register mar, mdr;
   Register sext;
   Byte zero;
} CPU_s;

typedef CPU_s * CPU_p;

//PROTOTYPES
//ALU
ALU_p constructALU(void);
ALU_p getALU(CPU_p);
Register getALU_A(ALU_p);
Register getALU_B(ALU_p);
Register getALU_R(ALU_p);
Register alu_ADD(CPU_p);
Register alu_ADI(CPU_p);
Register alu_NAND(CPU_p);

//CPU
CPU_p constructCPU(void);
int initCPU (CPU_p);
int resetCPU(CPU_p);
int setIR (CPU_p, char*);
int setSext(CPU_p, int);
int setRegister(CPU_p, unsigned int, int);
Byte setZeroFlag(CPU_p);
Register getIR(CPU_p);
Register getSext(CPU_p);
Register getRegister(CPU_p, int);
Register getImmed(CPU_p);
Byte getOPCODE(CPU_p);
Byte getDR (CPU_p);
Byte getSR1(CPU_p);
Byte getSR2(CPU_p);
Byte getImmed5(CPU_p);
Byte getOffset6(CPU_p);
Byte getOffset9(CPU_p);
Byte getOffset11(CPU_p);
Byte getBaseR(CPU_p);
Byte getTrapvect8(CPU_p);
Byte getBit5(CPU_p);

//Utility functions
void displayRegisterBinary(Register);
void displayByteBinary(Byte);
void purgeBuffer();

//Debug and simulator initialization
int debug(CPU_p);
void displayMemory(CPU_p, int);
void initRegisters(CPU_p);
void initMemory();
FILE* openFile(char[]);
FILE* getInputFile();
//int loadData(FILE*);

#endif
