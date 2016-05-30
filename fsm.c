/*
	fsu.c
	
   Austin Ingraham
   Based on starter code by Professor George Mobus
	Date: 4/27/16
	Descritption:
		This file contains the implementation code for simulating a Finite State Machine
*/

#include "cpu.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FETCH 0
#define DECODE 1
#define EVAL_ADDR 2
#define FETCH_OP 3
#define EXECUTE 4
#define STORE 5
#define MEMORY_SIZE 32

unsigned short memory[MEMORY_SIZE]; 

void purgeBuffer() {
   char c; 
   c = getchar();

   while(c != '\n' && c != EOF) {
      c = getchar();  
   }
}

void displayMemory(CPU_p cpu, int m) {
   int i = 0;
   printf("\nRegisters\tMemory\n==================================\n");
   for(i; i < NO_OF_REGISTERS; i++, m += 2) {
      printf("R%d: %.4X\t", i, getRegister(cpu, i));
      printf("0x%.4X: %.4X  %.4X\n", m, memory[m], memory[m + 1]);
   }
   printf("\nPC: %.4X   SW: %.4X   IR: %.4X\n", cpu->pc, 0, getIR(cpu));
}

int debug(CPU_p cpu) {
   int m = 0, input = 7, repeat = 1;;
   unsigned short reg = 0;
   char* memoryLoc = malloc(sizeof(char) * 20);
   displayMemory(cpu, m);
   while(input > 0) {      
      printf("\n1 Load, 2 Save, 3 Run, 4 Step, 5 Memory Dump, 6 Memory Edit\n> ");
      scanf(" %d", &input);
      purgeBuffer();
      switch(input) {
         case 4:
            input = 0;
            break;
         case 5: 
            printf("\tChoose memory dump starting index, (0 - %d)\n\t> ", (MEMORY_SIZE - 16));
            scanf(" %d", &m);
            if(m < 0) {
               m = 0;
            } else if (m > (MEMORY_SIZE - 16)) {
               m = MEMORY_SIZE - 16;   
            }
            displayMemory(cpu, m);

            break;
         case 6:
            repeat = 1;
            while(repeat) {
               printf("\tEnter memory location to edit,\n\t> ");
               scanf(" %s", memoryLoc);
               m = strtol(memoryLoc, NULL, 0);
               if(m < 0 || m >= MEMORY_SIZE) {
                  printf("\tIndex out of range, please enter value between 0 and %d.\n", (MEMORY_SIZE - 1));
               } else {
                  printf("\t%d/%.4X location chosen. Please enter new value:\n\t> ", m, m);
                  scanf(" %s", memoryLoc);
                  memory[m] = strtol(memoryLoc, NULL, 16);
                  printf("\tContinue editing data? y/n\n\t> ");
                  scanf(" %s", memoryLoc);
                  if(memoryLoc[0] == 'y' || memoryLoc[0] == 'Y') {} else { repeat = 0;}
               }
            }
            break;
         default:
            printf("\tInvalid choice, please enter value 1-6\n");
            input = 7;
            break;
      }
      
   }
   free(memoryLoc);
}

void initRegisters(CPU_p cpu) {
	cpu->reg_file[0] = 0;	
	cpu->reg_file[1] = 0;
	cpu->reg_file[2] = 0;
	cpu->reg_file[3] = 0;
	cpu->reg_file[4] = 0;
	cpu->reg_file[5] = 0;
	cpu->reg_file[6] = 0;
	cpu->reg_file[7] = 0;
}

void initMemory() {
	memory[0] = 0x6812; //LDI R2, 0x12
	memory[1] = 0x8D00; //LD R3, R2, 0 
	memory[2] = 0x2901; //ADI R2, R2, 1
	memory[3] = 0x9100; //LD R4, R2, 0
	memory[4] = 0x0E00; //ADD R3, R4
	memory[5] = 0xC002; //BRZ OUT
	memory[6] = 0x2901; //ADI R2, R2, 1
	memory[7] = 0xA980; //ST R2, R3, 0
	memory[8] = 0xE000; //OUT: HALT
	memory[9] = 0xE000;
	memory[10] = 0xE000;
	memory[11] = 0xE000;
	memory[12] = 0xE000;
	memory[13] = 0xE000;
	memory[14] = 0xE000;
	memory[15] = 0xE000;
	memory[16] = 0xE000;
	memory[17] = 0xE000;
	memory[18] = 0x0002; //OPND1
	memory[19] = 0xFFFE; //OPND2
	memory[20] = 0xE000; //result
	memory[21] = 0xE000;
	memory[22] = 0xE000; 
	memory[23] = 0xE000;
	memory[24] = 0xE000;
	memory[25] = 0xE000;
	memory[26] = 0xE000;
	memory[27] = 0xE000;
	memory[28] = 0xE000;
	memory[29] = 0xE000;
	memory[30] = 0xE000;
	memory[31] = 0xE000;
}

int controller (CPU_p cpu) {
    if (cpu == NULL) return POINTER_ERROR;
    if (cpu->alu == NULL) return POINTER_ERROR;
	 unsigned int state = FETCH;
	 Register branchAddress;
	 Byte opcode = 0, dr = 0, sr1 = 0, sr2 = 0, bit5 = 0;
	
	 initMemory();
	 initRegisters(cpu);
   
    
    for(;;) {   // efficient endless loop
        switch (state) {
            case FETCH: 
                debug(cpu);
                printf("===========\nFETCH: ");
                cpu->mar = cpu->pc++;  
                cpu->ir = memory[cpu->mar];                
                printf("Contents of IR = 0x%04X\n", getIR(cpu));
                state = DECODE;
                break;
            case DECODE:
                printf("DECODE: ");
                opcode = getOPCODE(cpu);
                dr = getDR(cpu);
                sr1 = getSR1(cpu);
              
                printf("op = %d, dr = %d, sr1 = %d", opcode, dr, sr1);
                state = EVAL_ADDR;
                break;
            case EVAL_ADDR:
              	 printf("\nEVAL_ADDRESS: ");
                switch (opcode) {
                   case ADD: break;
                   case LDI: break;
                   case LD:
					  setSext(cpu, OFFSET9_SIGN);
                      cpu->mar = cpu->pc + getSext(cpu);
                      printf("mar = %d", cpu->mar); 
                      break;
                   case ST:
					  setSext(cpu, OFFSET9_SIGN);
                      cpu->mar = cpu->pc + getSext(cpu);
                      printf("mar = %d", cpu->mar); 
                      break;
                   case BNZ:
                     /* branchAddress = cpu->pc + getSext(cpu);
                      printf("branchAddress = %d", branchAddress); */
                      break;
                   case HALT: break;
                }
              
                state = FETCH_OP;
                break;
            case FETCH_OP:
				   printf("\nFETCH_OP: ");
				   switch (opcode) {
                   case ADD:
                        bit5 = getBit5(cpu);
                        if(bit5) { //ADD(I)
                           setSext(cpu, IMMED5_SIGN);
                           cpu->alu->a = getRegister(cpu, sr1);	// get first operand
                           cpu->alu->b = getSext(cpu); // get second operand 
                        } else { //Standard Add
                           sr2 = getSR2(cpu);
                           cpu->alu->a = getRegister(cpu, sr1);	// get first operand
                           cpu->alu->b = getRegister(cpu, sr2);	// get second operand
                        }                      
                      printf("ALU_A = %d, ALU_B = %d", getALU_A(cpu->alu), getALU_B(cpu->alu)); 
                      break; 
				   case AND:
						bit5 = getBit5(cpu);
						if(bit5) { //AND(I)
							setSext(cpu, IMMED5_SIGN);
							cpu->alu->a = getRegister(cpu, sr1); // get first operand
							cpu->alu->b = getSext(cpu); // get second operand
						} else { //Standard Add
							sr2 = getSR2(cpu);
							cpu->alu->a = getRegister(cpu, sr1); // get first operand
							cpu->alu->b = getRegister(cpu, sr2); // get second operand
						}
						printf("ALU_A = %d, ALU_B = %d", getALU_A(cpu->alu), getALU_B(cpu->alu)); 
						break;
                   case LD: 
						cpu->mdr = memory[cpu->mdr];  
						printf("mdr = memory[%d] = 0x%.4X", cpu->mdr, memory[cpu->mdr]);                    
						break;
                   case LDI: 
                     /* cpu->mdr = getSext(cpu);
                      printf("mdr = 0x%.4X", cpu->mdr); */
                      break;
				   case NOT:
						cpu->alu->a = getRegister(cpu, sr1); // get first operand
						printf("ALU_A = %d", getALU_A(cpu->alu));
                   case ST: 
                      cpu->mdr = getRegister(cpu, sr1);
                      printf("mdr = 0x%.4X", cpu->mdr); 
                      break;
                   case BNZ: break;
                   case HALT: break;
                }

                state = EXECUTE;
                break;
            case EXECUTE:
				    printf("\nEXECUTE: ");
                switch (opcode) {
						 case ADD: 
                      if(bit5) {
                         printf("ADD(I) 0x%X, 0x%X", getALU_A(cpu->alu), getALU_B(cpu->alu));
                      } else {
							    printf("ADD 0x%X, 0x%X", getALU_A(cpu->alu), getALU_B(cpu->alu));
                      }
						    alu_ADD(cpu);
							 break;
					case AND:
						if(bit5) {
							printf("AND(I) 0x%X, 0x%X", getALU_A(cpu->alu), getALU_B(cpu->alu));						
						} else {
							printf("ADD 0x%X, 0x%X", getALU_A(cpu->alu), getALU_B(cpu->alu));
						}
						cpu->alu->r = (getALU_A(cpu->alu) & getALU_B(cpu->alu)); // ANDS the two registers.
						setZeroFlag(cpu);
						break;
                   case LDI: break;
                   case LD: 
						cpu->mdr = memory[cpu->mar];
                      printf("mdr = 0x%.4X", cpu->mdr); 
						break;
				   case NOT:
						cpu->alu->r = ~(getALU_A(cpu->alu));
						break;
                   case ST:
                      memory[cpu->mar] = cpu->mdr;
                      printf("memory[0x%.4X] = 0x%.4X", cpu->mar, cpu->mdr); 
                      break;
                   case BNZ:                     
                     /* if (cpu->zero) {                         
                         cpu->pc = branchAddress;
                         printf("Branch taken.");
                      } else {
                         printf("Branch not taken.");  
                      } */
                      break;
                   case HALT: 
                      printf("HALT");
                      break;
                }
              
                state = STORE;
                break;
            case STORE:
				    printf("\nSTORE: ");
                switch (opcode) {
                   case ADD: 
                      setRegister(cpu, getALU_R(cpu->alu), dr); 
                      printf("reg_file[%d] = 0x%.4X", dr, getALU_R(cpu->alu)); 
                      break;
				   case AND:
					  setRegister(cpu, getALU_R(cpu->alu), dr);
					  printf("reg_file[%d] = 0x%.4X", dr, getALU_R(cpu->alu));
					  break;
                   case LDI:
                   case LD:
                      cpu->reg_file[dr] = cpu->mdr;
                      printf("reg_file[%d] = 0x%.4X", dr, cpu->mdr);
                      break;
				   case NOT:
					  setRegister(cpu, getALU_R(cpu->alu), dr);
					  printf("reg_file[%d] = 0x%.4X", dr, getALU_R(cpu->alu)); 
                   case ST: break;
                   case BNZ: break;
                   case HALT: break;
                }			
                printf("\n===========\n");
					 
                //zero-out everything                          
                dr = 0;
                sr1 = 0;
                sr2 = 0;
                opcode = 0;
                cpu->alu->a = 0;
                cpu->alu->b = 0;
                state = FETCH;              
                break;
        }
    }
    displayMemory(cpu, 0);
    printf("===========\n");
	 return 1;
}


/* Driver for FSM */
int main (int argc, char *argv[]) {
   
   //PREPARE CPU 
   CPU_p cpu = constructCPU();
	initCPU(cpu);

  
   if(controller(cpu) == 1) {
		printf("Operations completed successfully.\n");	
	} else {
		printf("Something went wrong.\n");
	} 
   
   return 0;
}