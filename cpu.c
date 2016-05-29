/*
	cpu.c
	
	Programmer: George Mobus
   Austin Ingraham, Arthur Panlilio
	Date: 4/20/16
	Descritption:
		This file contains the implementation code for the CPU class.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

/* Constructs an ALU
   OUT: pointer to an ALU
*/
ALU_p constructALU (void) {
	ALU_p alu = (ALU_p) malloc(sizeof(ALU_s));
	return alu;
}

/* Gets the given CPU's ALU
   IN: pointer to a CPU
   OUT: this CPU's ALU
*/
ALU_p getALU(CPU_p cpu) {
   return cpu->alu;
}

/* Gets the given ALU's 'a' Register
   IN: pointer to an ALU
   OUT: Register 'a'
*/
Register getALU_A(ALU_p alu) {
   if (alu == NULL) return POINTER_ERROR;
   return alu->a;
}

/* Gets the given ALU's 'b' Register
   IN: pointer to an ALU
   OUT: Register 'b'
*/
Register getALU_B(ALU_p alu) {
   if (alu == NULL) return POINTER_ERROR;
   return alu->b;
}

Register getALU_R(ALU_p alu) {
   if (alu == NULL) return POINTER_ERROR;
   return alu->r;
}

/* Performs an ADD operation on this ALU
   IN: pointer to an ALU
   OUT: result Register's value
*/
Register alu_ADD(CPU_p cpu) {
   if (cpu == NULL) return POINTER_ERROR;
   if (cpu->alu == NULL) return POINTER_ERROR;
   cpu->alu->r = cpu->alu->a + cpu->alu->b;
   setZeroFlag(cpu);
   return cpu->alu->r;
}

/* Performs an ADI operation on this ALU
   IN: pointer to an ALU
   OUT: result Register's value
*/
Register alu_ADI(CPU_p cpu) {
   if (cpu == NULL) return POINTER_ERROR;
   if (cpu->alu == NULL) return POINTER_ERROR;
   cpu->alu->r = cpu->alu->a + cpu->alu->b;
   setZeroFlag(cpu);
   return cpu->alu->r;
}

/* Performs a NAND operation on this ALU
   IN: pointer to an ALU
   OUT: result Register's value
*/
Register alu_NAND(CPU_p cpu) {
   if (cpu == NULL) return POINTER_ERROR;
   if (cpu->alu == NULL) return POINTER_ERROR;
   cpu->alu->r = ~(cpu->alu->a & cpu->alu->b);
   setZeroFlag(cpu);
   return cpu->alu->r;
}

/* Constructs and returns a pointer to a CPU. 
   OUT: pointer to a CPU. May be null if malloc fails.
*/
CPU_p constructCPU (void) {
	CPU_p cpu = (CPU_p) malloc (sizeof (CPU_s));
	return cpu;
}

/* Initializes the fields inside a CPU.
   IN: pointer to a CPU
   OUT: fields initialized, POINTER_ERROR returned if given CPU is null
 */
int initCPU (CPU_p cpu) {
	if (cpu == NULL) return POINTER_ERROR;
	cpu->alu = constructALU();
	// initialize register file with random numbers
	cpu->pc = 0;
	cpu->ir = 0;
	cpu->sext = 0;
}

/* Gets the IR of the given CPU
   IN: pointer to a CPU
   OUT: Register ir, POINTER_ERROR returned if given CPU is null
 */
Register getIR (CPU_p cpu) {
	if (cpu == NULL) return POINTER_ERROR;
	return cpu->ir;
}

/* Gets the Immed portion's Register equivalent in the given CPU
   IN: pointer to a CPU
   OUT: Register immed7, POINTER_ERROR returned if given CPU is null
 */
Register getImmed(CPU_p cpu) {
   if (cpu == NULL) return POINTER_ERROR;
   Register immed = cpu->ir & IMMED_MASK;
   return (Byte) immed;
}

Register getSext(CPU_p cpu) {
   if (cpu == NULL) return POINTER_ERROR;
   return cpu->sext;
}

/* Gets the OPCODE of a given CPU from the current ir
   IN: pointer to a CPU
   OUT: Byte OPCODE, POINTER_ERROR returned if given CPU is null
 */
Byte getOPCODE (CPU_p cpu) {
	if (cpu == NULL) return POINTER_ERROR;
	Register temp = cpu->ir & OPCODE_MASK;
	temp = temp >> 12;
	return (Byte) temp;
}

/* Gets a register from the given index
   IN: pointer to a CPU, index of register to get
   OUT: a register
*/
Register getRegister(CPU_p cpu, int index) {
   if (cpu == NULL) return POINTER_ERROR;
   if (index >= NO_OF_REGISTERS || index < 0) return INDEX_OUT_OF_BOUNDS;
   return cpu->reg_file[index];
}

/* Gets Rd of a given CPU from the current ir
   IN: pointer to a CPU
   OUT: Byte Rd, POINTER_ERROR returned if given CPU is null
 */
Byte getRD (CPU_p cpu) {
	if (cpu == NULL) return POINTER_ERROR;
	Register temp = cpu->ir & RD_MASK;
	temp = temp >> 10;
	return (Byte) temp;
}

/* Gets Rs of a given CPU from the current ir
   IN: pointer to a CPU
   OUT: Byte Rd, POINTER_ERROR returned if given CPU is null
 */
Byte getRS (CPU_p cpu) {
	if (cpu == NULL) return POINTER_ERROR;
	Register temp = cpu->ir & RS_MASK;
	temp = temp >> 7;
	return (Byte) temp;
}

/* Gets DR of a given CPU from the current ir
   IN: pointer to a CPU
   OUT: Byte Dr, POINTER_ERROR returned if given CPU is null
*/   
Byte getDR (CPU_p cpu) {
	if (cpu == NULL) return POINTER_ERROR;
	Register temp = cpu->ir & DR_MASK;
	temp = temp >> 9;
	return (Byte) temp;
}

/* Gets SR1 of a given CPU from the current ir
   IN: pointer to a CPU
   OUT: Byte Sr1, POINTER_ERROR returned if given CPU is null
*/
Byte getSR1 (CPU_p cpu) {
	if (cpu == NULL) return POINTER_ERROR;
	Register temp = cpu->ir & SR1_MASK;
	temp = temp >> 6;
	return (Byte) temp;
}

/* Gets SR2 of a given CPU from the current ir
   IN: pointer to a CPU
   OUT: Byte Sr2, POINTER_ERROR returned if given CPU is null
*/
Byte getSR2 (CPU_p cpu) {
	if (cpu == NULL) return POINTER_ERROR;
	Register temp = cpu->ir & SR2_MASK;
	return (Byte) temp;
}

/* Gets Immed5 of a given CPU from the current ir
   IN: pointer to a CPU
   OUT: Byte Immed5, POINTER_ERROR returned if given CPU is null
*/
Byte getImmed5 (CPU_p cpu) {
	if (cpu == NULL) return POINTER_ERROR;
	Register temp = cpu->ir & IMMED5_MASK;
	return (Byte) temp;
}
 
/* Gets PCoffset9 of a given CPU from the current ir
   IN: pointer to a CPU
   OUT: Byte PCoffset9, POINTER_ERROR returned if given CPU is null
*/
Byte getPCoffset9 (CPU_p cpu) {
	if (cpu == NULL) return POINTER_ERROR;
	Register temp = cpu->ir & PCOFFSET9_MASK;
	return (Byte) temp;
}
 
/* Gets Offset6 of a given CPU from the current ir
   IN: pointer to a CPU
   OUT: Byte offset6, POINTER_ERROR returned if given CPU is null
*/
Byte getOffset6 (CPU_p cpu) {
	if (cpu == NULL) return POINTER_ERROR;
	Register temp = cpu->ir & OFFSET6_MASK;
	return (Byte) temp;
}

/* Gets BaseR of a given CPU from the current ir
   IN: pointer to a CPU
   OUT: Byte BaseR, POINTER_ERROR returned if given CPU is null
*/
Byte getBaseR (CPU_p cpu) {
	if (cpu == NULL) return POINTER_ERROR;
	Register temp = cpu->ir & BASER_MASK;
	temp = temp >> 6;
	return (Byte) temp;
}

/* Gets Trapvect8 of a given CPU from the current ir
   IN: pointer to a CPU
   OUT: Byte Trapvect8, POINTER_ERROR returned if given CPU is null
*/
Byte getTrapvect8 (CPU_p cpu) {
	if (cpu == NULL) return POINTER_ERROR;
	Register temp = cpu->ir & TRAPVECT8_MASK;
	return (Byte) temp;	
}


/* Sets a given CPU's ir Register
   IN: pointer to a CPU, char input to set
   OUT: 1 if set operation is successful, -1 if POINTER_ERROR
 */
int setIR (CPU_p cpu, char* input) {
   if (cpu == NULL) return POINTER_ERROR;
   cpu->ir = strtol(input, NULL, 16);
   return 1;
}

/* Sets a given CPU's sign-extended (sext) Register
   IN: pointer to a CPU
   OUT: 1 if set operation is successful, -1 if POINTER_ERROR
 */
int setSext(CPU_p cpu) {
   if (cpu == NULL) return POINTER_ERROR;
   Register immed = getImmed(cpu);
   if (immed & SIGN_MASK) {
      cpu->sext = immed | NEG_SIGN_EXTEND;
   } else {
      cpu->sext = immed;  
   }
	return 1;
}

/* Sets a given CPU's Registers based on cpu->ir index
   IN: pointer to a CPU, char input to set
   OUT: 1 if set operation is successful, -1 if POINTER_ERROR
 */
int setRegisters(CPU_p cpu, char* Rd_input, char* Rs_input) {
   if (cpu == NULL) return POINTER_ERROR;
   cpu->reg_file[getRD(cpu)] = strtol(Rd_input, NULL, 16);
   cpu->reg_file[getRS(cpu)] = strtol(Rs_input, NULL, 16);
   return 1; 
}

/* Sets a given CPU's Register at given index
   IN: pointer to a CPU, unsigned integer input to set, index of register
   OUT: 1 if set operation is successful, -1 if POINTER_ERROR
 */
int setRegister(CPU_p cpu, unsigned int value, int index) {
   if (cpu == NULL) return POINTER_ERROR;
   cpu->reg_file[index] = value;
   return 1; 
}

Byte setZeroFlag (CPU_p cpu) {
	if (cpu == NULL) return POINTER_ERROR;
	if (cpu->alu->r == 0) cpu->zero = 1;
	else cpu->zero = 0;
	return cpu->zero;
}

/* Prints the binary representation of this register (unsigned short). */
void displayRegisterBinary(Register theR) {
    int i;
    for (i = 15; i >= 0; i--) {
       if(i == 12 || i == 9 || i == 6) {
          printf("|");  
       }
       printf("%d", (theR & (1 << i)) >> i);
    }
   printf("\n");
}

/* Prints the binary representation of this char. */
void displayByteBinary(Byte theB) {
    int i;
    for (i = 7; i >= 0; i--) {
        printf("%d", (theB & (1 << i)) >> i);
    }
   printf("\n");
}

/* Driver for CPU 
int main (int argc, char *argv[]) {
	if (argc < 5) {
		printf ("Invalid input to program\n");
		exit (0);
	}
   
   //PREPARE CPU
   CPU_p cpu = constructCPU();
	initCPU(cpu);
   setIR(cpu, argv[2]);
   
   displayRegisterBinary(cpu->ir);
   
   //PERFORM OPERATIONS
   setRegisters(cpu, argv[3], argv[4]);
   operate(cpu);
   return 0;
} */

