/*
    cpu.h
	
    Programer: George Mobus
    Austin Ingraham
	Date: 4/20/16
	Description:
		This header file contains the definitions for the CPU object.
*/

#ifndef _CPU_H_
#define _CPU_H_

#define POINTER_ERROR -1
#define INDEX_OUT_OF_BOUNDS -1

#define NO_OF_REGISTERS 8

#define OPCODE_MASK 0xE000
#define RD_MASK 0x1C00
#define RS_MASK 0x0380
#define IMMED_MASK 0x007F
#define SIGN_MASK 0x0040  
#define NEG_SIGN_EXTEND 0xFFC0 

#define HALT 7
#define ADD 0
#define ADI 1 
#define NAND 2
#define LDI 3
#define LD 4
#define ST 5
#define BNZ 6

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

// prototypes
ALU_p constructALU(void);
ALU_p getALU(CPU_p);
Register getALU_A(ALU_p);
Register getALU_B(ALU_p);
Register getALU_R(ALU_p);
Register alu_ADD(CPU_p);
Register alu_ADI(CPU_p);
Register alu_NAND(CPU_p);

CPU_p constructCPU(void);
int initCPU (CPU_p);

int setIR (CPU_p, char*);
int setSext(CPU_p);
int setRegisters(CPU_p, char*, char*);
int setRegister(CPU_p, unsigned int, int);
Byte setZeroFlag (CPU_p);
Register getIR (CPU_p);
Register getSext(CPU_p);
Register getRegister(CPU_p, int);
Register getImmed(CPU_p);
Byte getOPCODE (CPU_p);
Byte getRD (CPU_p);
Byte getRS (CPU_p);

void displayRegisterBinary(Register);
void displayByteBinary(Byte);

#endif