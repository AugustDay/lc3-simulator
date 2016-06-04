/*
	fsu.c
	
   Austin Ingraham
   Arthur Panlilio
   Based on starter code by Professor George Mobus
	Date: 6/3/16
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
#define HALTINSTRUCTION 6
#define MEMORY_SIZE 3000

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
   char c = 'N';
   printf("\nRegisters\tMemory\n==================================\n");
   for(i; i < NO_OF_REGISTERS; i++, m += 2) {
      printf("R%d: %.4X\t", i, getRegister(cpu, i));
      printf("0x%.4X: %.4X  %.4X\n", m, memory[m], memory[m + 1]);
   }
   if (cpu->sw == 1) { 
      c = 'P'; 
   } else if (cpu->sw == 2) {
      c = 'Z';      
   } 
   printf("\nPC: %.4X   SW: %c   IR: %.4X\n", cpu->pc, c, getIR(cpu));
}

FILE* openFile(char filename[]) {
    FILE* file = fopen(filename, "r");

    if (!file) {
        printf("\tCould not open filename \"%s\"\n\n", filename);
        return getInputFile();
    } else {
        return file;
    }
}

FILE* getInputFile() {
    int i = 0, size = 50; //max allowed filename
    char ch, filename[size];  

    printf("\tEnter the name of the file to load:\n\t> ");
    scanf("%c", &ch);
    while (ch != '\n' && i < size - 1) { //builds string from chars entered
        filename[i] = ch;
        ch = getchar();
        i++;
    }    
    filename[i] = '\0';
    return openFile(filename);
}

int loadData() {
   int i = 0;
   char line[10];
   FILE* file = getInputFile();
   printf("\tFound the file, loading data... ");
   while (fgets(line, sizeof(line), file) && i < MEMORY_SIZE) {
      memory[i] = strtol(line, NULL, 16);
      i++;
   }
   printf("done.\n");
   return 1;  
}

int saveData(int m) {
	FILE* outFile = fopen("output.hex", "w");
	int i = 0;
	if (!outFile) {
    	printf("\tERROR OPENING FILE\n");
		return FILE_ERROR;
    }
    for(i; i <= m; i++) {
	   fprintf(outFile, "%.4X\n", memory[i]); 		   
    }
	fclose(outFile);
	return 1;
}

int debug(CPU_p cpu) {
   int m = cpu->pc, input = 8, repeat = 1;;
   unsigned short reg = 0;
   char* memoryLoc = malloc(sizeof(char) * 20);
   displayMemory(cpu, m);
   while(input > 0) {      
      if (m > (MEMORY_SIZE - 16)) {
         m = MEMORY_SIZE - 16;   
      }
      printf("\n1 Load, 2 Save, 3 Run, 4 Step, 5 Memory Dump, 6 Memory Edit, 7 Reset\n> ");
      scanf(" %d", &input);
      purgeBuffer();
      switch(input) {
         case 1: //LOAD OBJECT FILE
            loadData();   
            displayMemory(cpu, m);
            break;
		 case 2:
			printf("\tChoose terminating index of memory to output, (0 - %d)\n\t> ", (MEMORY_SIZE - 1));
			scanf(" %d", &m);
            if(m < 0) {
               m = 0;
            } else if (m > (MEMORY_SIZE - 1)) {
               m = MEMORY_SIZE - 1;   
            }
			printf("\tSaving memory into output.hex ... ");
			saveData(m);
			printf("done.\n");
			break;
         case 4: //STEP
            input = 0;
            break;
         case 5: //MEMORY DUMP
            printf("\tChoose memory dump starting index, (0 - %d)\n\t> ", (MEMORY_SIZE - 16));
            scanf(" %d", &m);
            if(m < 0) {
               m = 0;
            } else if (m > (MEMORY_SIZE - 16)) {
               m = MEMORY_SIZE - 16;   
            }
            displayMemory(cpu, m);

            break;
         case 6: //MEMORY EDIT
            repeat = 1;
            while(repeat) {
               printf("\tEnter memory location to edit,\n\t> ");
               scanf(" %s", memoryLoc);
               m = strtol(memoryLoc, NULL, 0);
               if(m < 0 || m >= MEMORY_SIZE) {
                  printf("\tIndex out of range, please enter value between 0 and %d.\n",
                         (MEMORY_SIZE - 1));
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
         case 7: //RESET
            printf("\tResetting simulator, y to confirm,\n\t> ");
            scanf(" %s", memoryLoc);
            if(memoryLoc[0] == 'y' || memoryLoc[0] == 'Y') {
               printf("\tResetting... ");
               initMemory();
               initRegisters(cpu);
               resetCPU(cpu);
               printf("done.\n");
               displayMemory(cpu, m);
            } 
            break;
         default:
            printf("\tInvalid choice, please enter value 1-7\n");
            input = 8;
            break;
      }
      
   }
   free(memoryLoc);
}

void initRegisters(CPU_p cpu) {
   int i = 0;
   for(i; i < NO_OF_REGISTERS; i++) {
	   cpu->reg_file[i] = 0;	
   }
}

void initMemory() {
   int i = 0;
   for(i; i < MEMORY_SIZE; i++) {
	   memory[i] = 0x0000;	
   }
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
                   case BR: //TODO condense
                      setSext(cpu, OFFSET9_SIGN);
                      cpu->mar = cpu->pc + getSext(cpu);
                      printf("mar = %d, sext = %d", cpu->mar, getSext(cpu));
                      break;
                   case LDI:
                      setSext(cpu, OFFSET9_SIGN);
                      cpu->mar = cpu->pc + getSext(cpu);
                      printf("mar = %d", cpu->mar);
				          break;
                   case LD:
                      setSext(cpu, OFFSET9_SIGN);
                      cpu->mar = cpu->pc + getSext(cpu);
                      printf("mar = %d", cpu->mar); 
                      break;
                   case LDR:
                      setSext(cpu, OFFSET6_SIGN);
                      cpu->mar = sr1 + getSext(cpu); //adds the base register with offset6
                      printf("mar = %d", cpu->mar);
                      break;
                   case LEA:
                      setSext(cpu, OFFSET9_SIGN);
                      cpu->mar = cpu->pc + getSext(cpu);
                      printf("mar = %d", cpu->mar);
                   case STI:
                      setSext(cpu, OFFSET9_SIGN);
                      cpu->mar = cpu->pc + getSext(cpu);
                      printf("mar = %d", cpu->mar);
                      break;
                   case ST:
                      setSext(cpu, OFFSET9_SIGN);
                      cpu->mar = cpu->pc + getSext(cpu);
                      printf("mar = %d", cpu->mar); 
                      break;
                   case STR:
                      setSext(cpu, OFFSET6_SIGN);
                      cpu->mar = sr1 + getSext(cpu); //adds the base register with offset6
                      printf("mar = %d", cpu->mar);
                      break;
                   case JSR:
                      setSext(cpu, OFFSET11_SIGN);
                      sr2 = cpu->pc; //temp variable
                      break;
                   case TRAP:
                      printf("TRAP");
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
                      cpu->mdr = memory[cpu->mar];  
                      printf("mdr = memory[%d] = 0x%.4X", cpu->mar, memory[cpu->mar]);      
                      break;
                   case LDI: 
                     cpu->mdr = memory[cpu->mdr]; //not sure if this is where it goes.
                     cpu->mdr = memory[cpu->mar];
                     cpu->mar = cpu->mdr;
                     printf("mdr = 0x%.4X", cpu->mdr); 
                     break;
                   case LDR:
                     cpu->mdr = memory[cpu->mdr];
                     printf("mdr = memory[%d] = 0x%.4X", cpu->mdr, memory[cpu->mdr]); 
                     break;
                   case NOT:
                     cpu->alu->a = getRegister(cpu, sr1); // get first operand
                     printf("ALU_A = %d", getALU_A(cpu->alu));
                     break;
                   case STI:
                     cpu->mdr = memory[cpu->mar]; //not sure if this is where it goes.
                     cpu->mar = cpu->mdr;
                     cpu->mdr = getRegister(cpu, dr);
                     printf("mdr = 0x%.4X", cpu->mdr); 
                     break;
                   case ST: 
                     cpu->mdr = getRegister(cpu, dr); //dr is the register value to put into memory
                     printf("mdr = 0x%.4X", cpu->mdr); 
                     break;
                   case STR:
                     cpu->mdr = getRegister(cpu, dr);
                     printf("mdr = 0x%.4X", cpu->mdr); 
                     break;
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
                        printf("AND 0x%X, 0x%X", getALU_A(cpu->alu), getALU_B(cpu->alu));
                     }
                     cpu->alu->r = (getALU_A(cpu->alu) & getALU_B(cpu->alu)); // ANDS the two registers.
                     break;
                   case BR:
                     if(takeBranch(cpu, cpu->ir)) {
                        cpu->pc = cpu->mar;
                     }                      
                     break;
                   case JMP:
					 printf("JMP");
                     cpu->pc = getRegister(cpu, sr1);
                     break;
                   case JSR:
					 printf("JSR");
                     if(cpu->ir & BIT11_MASK) {
                        cpu->pc = cpu->pc + cpu->sext;                        
                     } else {
                        cpu->pc = getRegister(cpu, sr1);
                     }
                     printf("PC = %d", cpu->pc);
                     break;
                   case LDI: 
				     printf("LDI");
                     cpu->mdr = memory[cpu->mar];
                     break;
                   case LD: 
					 printf("LD");
					 break;
                   case LDR:
				     printf("LDR");
                     cpu->mdr = memory[cpu->mar];
                     printf("mdr = 0x%.4X", cpu->mdr); 
                   case NOT:
				     printf("NOT");
                     cpu->alu->r = ~(getALU_A(cpu->alu));
                     break;
                   case STI:
				     printf("STI");
                     memory[cpu->mar] = cpu->mdr;
                     printf("memory[0x%.4X] = 0x%.4X", cpu->mar, cpu->mdr); 
                     break;
                   case ST:
				     printf("ST");
                     memory[cpu->mar] = cpu->mdr;
                     printf("memory[0x%.4X] = 0x%.4X", cpu->mar, cpu->mdr); 
                     break;
                   case STR:
				     printf("STR");
                     memory[cpu->mar] = cpu->mdr;
                     printf("memory[0x%.4X] = 0x%.4X", cpu->mar, cpu->mdr); 
                     break;
                   case TRAP:				 
                     setSext(cpu, TRAPVECTOR_SIGN);
                     int trapVector = cpu->sext;
                     setRegister(cpu, cpu->pc, 7); //in actual LC3, this is for RET to read.
                     switch (trapVector) {
                        case GETC:
                           trapGetc(cpu);
                           printf("GETC");
                           break;
                        case OUT:
						    printf("OUT");
                           trapOut(cpu);
                           break;
                        case PUTS:
						    printf("PUTS");
                           trapPuts(cpu, memory);                       
                           break;
                        case HALT:
                           trapHalt(cpu);
                           break;
                      }
                      break;
                }
                if (cpu->sext == HALT) {
                   state = HALTINSTRUCTION;
                   break;
                }
                state = STORE;
                break;
            case STORE:
				    printf("\nSTORE: ");
                switch (opcode) {
                   case ADD: 
                      setRegister(cpu, getALU_R(cpu->alu), dr); 
                      setSW(cpu, getALU_R(cpu->alu));
                      printf("reg_file[%d] = 0x%.4X", dr, getALU_R(cpu->alu)); 
                      break;
				   case AND:
                      setRegister(cpu, getALU_R(cpu->alu), dr);
                      setSW(cpu, getALU_R(cpu->alu));
                      printf("reg_file[%d] = 0x%.4X", dr, getALU_R(cpu->alu));
                      break;
                   case JSR:
                      cpu->reg_file[7] = sr2;
                      printf("R7 = 0x%.4X", cpu->reg_file[7]);
                      break;
                   case LDI: //TODO LDI, LD, and LDR are all same here, condense.
                      cpu->reg_file[dr] = cpu->mdr;
                      setSW(cpu, cpu->mdr);
                      printf("reg_file[%d] = 0x%.4X", dr, cpu->mdr);
                      break;
                   case LD:
                      cpu->reg_file[dr] = cpu->mdr;
                      setSW(cpu, cpu->mdr);
                      printf("reg_file[%d] = 0x%.4X", dr, cpu->mdr);
                      break;
				       case LDR:
					       cpu->reg_file[dr] = cpu->mdr;
                      setSW(cpu, cpu->mdr);
					       printf("reg_file[%d] = 0x%.4X", dr, cpu->mdr);
                      break;
                   case LEA: /* Need setSW() */ 
                      cpu->reg_file[dr] = cpu->mar;
                      setSW(cpu, cpu->mdr);
					       printf("reg_file[%d] = 0x%.4X", dr, cpu->mar);
                      break;
				       case NOT:
					       setRegister(cpu, getALU_R(cpu->alu), dr);
                      setSW(cpu, getALU_R(cpu->alu));
					       printf("reg_file[%d] = 0x%.4X", dr, getALU_R(cpu->alu)); 
                      break;
                   case ST: break;
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
		if (state == HALTINSTRUCTION) {
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