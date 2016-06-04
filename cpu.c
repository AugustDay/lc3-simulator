/*
	cpu.c
	
	Programmer: George Mobus
   Austin Ingraham, Arthur Panlilio
	Date: 6/3/16
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
	resetCPU(cpu); //TODO verify accuracy!
}

/* Gets the IR of the given CPU
   IN: pointer to a CPU
   OUT: Register ir, POINTER_ERROR returned if given CPU is null
 */
Register getIR (CPU_p cpu) {
	if (cpu == NULL) return POINTER_ERROR;
	return cpu->ir;
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
Byte getSR1(CPU_p cpu) {
	if (cpu == NULL) return POINTER_ERROR;
	Register temp = cpu->ir & SR1_MASK;
	temp = temp >> 6;
	return (Byte) temp;
}

/* Gets SR2 of a given CPU from the current ir
   IN: pointer to a CPU
   OUT: Byte Sr2, POINTER_ERROR returned if given CPU is null
*/
Byte getSR2(CPU_p cpu) {
	if (cpu == NULL) return POINTER_ERROR;
	Register temp = cpu->ir & SR2_MASK;
	return (Byte) temp;
}

/* Gets Immed5 of a given CPU from the current ir
   IN: pointer to a CPU
   OUT: Byte Immed5, POINTER_ERROR returned if given CPU is null
*/
Register getImmed5(CPU_p cpu) {
	if (cpu == NULL) return POINTER_ERROR;
	Register temp = cpu->ir & IMMED5_MASK;
	return temp;
}
 
/* Gets Offset6 of a given CPU from the current ir
   IN: pointer to a CPU
   OUT: Byte offset6, POINTER_ERROR returned if given CPU is null
*/
Register getOffset6(CPU_p cpu) {
	if (cpu == NULL) return POINTER_ERROR;
	Register temp = cpu->ir & OFFSET6_MASK;
	return temp;
}

/* Gets PCoffset9 of a given CPU from the current ir
   IN: pointer to a CPU
   OUT: Byte PCoffset9, POINTER_ERROR returned if given CPU is null
*/
Register getOffset9(CPU_p cpu) {
	if (cpu == NULL) return POINTER_ERROR;
	Register temp = cpu->ir & OFFSET9_MASK;
	return temp;
}

/* Gets PCoffset11 of a given CPU from the current ir
   IN: pointer to a CPU
   OUT: Byte PCoffset11, POINTER_ERROR returned if given CPU is null
*/
Register getOffset11(CPU_p cpu) {
	if (cpu == NULL) return POINTER_ERROR;
	Register temp = cpu->ir & OFFSET11_MASK; 
	return temp;
} //TODO remove redundent functions (could go in getsext()

/* Gets BaseR of a given CPU from the current ir
   IN: pointer to a CPU
   OUT: Byte BaseR, POINTER_ERROR returned if given CPU is null
*/
Byte getBaseR(CPU_p cpu) {
	if (cpu == NULL) return POINTER_ERROR;
	Register temp = cpu->ir & BASER_MASK;
	temp = temp >> 6;
	return (Byte) temp;
}

/* Gets Trapvect8 of a given CPU from the current ir
   IN: pointer to a CPU
   OUT: Byte Trapvect8, POINTER_ERROR returned if given CPU is null
*/
Byte getTrapvect8(CPU_p cpu) {
	if (cpu == NULL) return POINTER_ERROR;
	Register temp = cpu->ir & TRAPVECT8_MASK;
	return (Byte) temp;	
}

/* Gets bit[5] of the ir from the given CPU
   IN: pointer to a CPU
   OUT: 1 or 0, POINTER_ERROR returned if given CPU is null
*/
Byte getBit5(CPU_p cpu) {
	if (cpu == NULL) return POINTER_ERROR;
	Register temp = cpu->ir & BIT5_MASK;
   temp = temp >> 5;
	return (Byte) temp;	
}

int setSW(CPU_p cpu, int s) {
   if (cpu == NULL) return POINTER_ERROR;
   if (s >= 0x8000) { cpu->sw = N_MASK; }
   if (s == 0) { cpu->sw = Z_MASK; }
   if (s > 0 && s < 0x8000) { cpu->sw = P_MASK; }
   return 1;
}

/* Sets a given CPU's ir Register
   IN: pointer to a CPU, char input to set
   OUT: 1 if set operation is successful, -1 if POINTER_ERROR
 */
int setIR(CPU_p cpu, char* input) {
   if (cpu == NULL) return POINTER_ERROR;
   cpu->ir = strtol(input, NULL, 16);
   return 1;
}

/* Sets a given CPU's sign-extended (sext) Register
   IN: pointer to a CPU, value code for what offset to set
   OUT: 1 if set operation is successful, -1 if POINTER_ERROR
 */
int setSext(CPU_p cpu, int signLocation) {
   if (cpu == NULL) return POINTER_ERROR;
   Register immed;
   switch (signLocation) {
      case IMMED5_SIGN:
         immed = getImmed5(cpu);
         if (immed & IMMED5_SIGN_MASK) {
            cpu->sext = immed | IMMED5_SIGN_EXTEND;
         } else {
            cpu->sext = immed;  
         }
         break;
      case OFFSET6_SIGN:
         immed = getOffset6(cpu);
         if (immed & OFFSET6_SIGN_MASK) {
            cpu->sext = immed | OFFSET6_SIGN_EXTEND;
         } else {
            cpu->sext = immed;  
         }
         break;
      case OFFSET9_SIGN:
         immed = getOffset9(cpu);
         displayRegisterBinary(immed);
         if (immed & OFFSET9_SIGN_MASK) {
            printf("extended.\n");
            cpu->sext = immed | OFFSET9_SIGN_EXTEND;
         } else {
            cpu->sext = immed;  
         }
         break;
      case OFFSET11_SIGN:
         immed = getOffset11(cpu);
         if (immed & OFFSET11_SIGN_MASK) {
            cpu->sext = immed | OFFSET11_SIGN_EXTEND;
         } else {
            cpu->sext = immed;  
         }
         break;
	  case TRAPVECTOR_SIGN:
		 immed = getTrapvect8(cpu);
		 if (immed & TRAPVECTOR_SIGN_MASK) {
			cpu->sext = immed | TRAPVECTOR_SIGN_EXTEND;
	     } else {
			cpu->sext = immed;
		 }
   }
   
	return 1;
}

void trapGetc(CPU_p cpu) {
	char c;
	c = getch();
	int value = (int) c;
	setRegister(cpu, value, ZEROREGISTER);
}

void trapOut(CPU_p cpu) {
	char c = (char)cpu->reg_file[ZEROREGISTER];
	printf("				> %c", c);

}

void trapPuts(CPU_p cpu, unsigned short *memory) {
	int address;
	address = cpu->reg_file[ZEROREGISTER];
	printf("				> ");
	int i = 0;
	while (memory[address] != 0) {
		char c = (char) memory[address];
		printf("%c", c);
		address++;
		if (i >= NEWLINE) {
			printf("\n					  ");
			i = 0;
		}
		i++;
	}
}

void trapHalt(CPU_p cpu) {
	printf("INSTRUCTION HALTED\n");
}
	
int takeBranch(CPU_p cpu, Register r) {
   if (cpu == NULL) return POINTER_ERROR;
   if (r & N_BR_MASK) {
      if (cpu->sw & N_MASK) {
         return 1;  
      }
   }
   if (r & Z_BR_MASK) {
      if (cpu->sw & Z_MASK) {
         return 1;  
      }
   }
   if (r & P_BR_MASK) {
      if (cpu->sw & P_MASK) {
         return 1;  
      }
   }
   return 0;
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

int resetCPU(CPU_p cpu) {
   if (cpu == NULL) return POINTER_ERROR;
   cpu->pc = 0;
   cpu->sw = 0;
   cpu->ir = 0;
   cpu->mar = 0;
   cpu->mdr = 0;
   cpu->sext = 0;
   cpu->sw = Z_MASK;
   cpu->alu->a = 0;
   cpu->alu->b = 0;
   cpu->alu->r = 0;
   return 1;
}

/* Prints the binary representation of this register (unsigned short). */
void displayRegisterBinary(Register theR) {
    int i;
    for (i = 15; i >= 0; i--) {       
       printf("%d", (theR & (1 << i)) >> i);
       if(i % 4 == 0 && i > 1) {
          printf("|");  
       }
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