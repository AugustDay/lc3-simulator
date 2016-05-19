#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Prints the binary representation of this register (unsigned short). */
void displayRegisterBinary(unsigned short theR) {
    int i;
    for (i = 15; i >= 0; i--) {
       if(i == 12 || i == 9 || i == 6) {
          printf("|");  
       }
       printf("%d", (theR & (1 << i)) >> i);
    }
   printf("\n");
}

int setRegisterLocation(char* theInput, int distance) {
   int reg_loc = atoi(theInput);
   if(reg_loc > 7) {
      reg_loc = 7;  
   } else if(reg_loc < 0) {
      reg_loc = 0;  
   }
   reg_loc = reg_loc << distance;
   return reg_loc;
}

int main (int argc, char *argv[]) {
   if(argc == 2) {
      int operand = atoi(argv[1]);
      //DISPLAY
      displayRegisterBinary(operand);
      printf("0x%04X\n", ((unsigned short) operand));
   } else if(argc < 5) {
      printf("Need OPCODE, RD, RS, and Immed7\n[0-7], [0-7], [0-7], [-64-63]\n");  
   } else {
      //SET IMMED
      int immed = atoi(argv[4]);
      if(immed > 63) {
         immed = 63;
      } else if(immed < 0) {
         immed += 64;
         immed = immed | 0x0040;
      }
      unsigned short reg = immed;

      //SET RD AND RS
      reg = reg | setRegisterLocation(argv[3], 7);
      reg = reg | setRegisterLocation(argv[2], 10);
      reg = reg | setRegisterLocation(argv[1], 13);

      //DISPLAY
      displayRegisterBinary(reg);
      printf("0x%04X\n", reg);
   }
   
   return 0;
}