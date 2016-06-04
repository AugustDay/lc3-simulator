;  mul_div.asm - a data entry program and operations
;
; George Mobus
;
; This program does what the original data entry program did, it gets two two-digit numbers in 
; decimal, and then prompts the user for an operation code. 0 = multiply, 1 = divide. We can 
; assume that the answer in multiplication will not overflow and that the second number input
; at the prompt will always be smaller than the first (always division by a smaller positive
; number.
;
; The program demonstrates the use of subroutines, including a two-deep nesting of one subroutine
; calling another subroutine.
;
		.orig		x3000
;
		JSR		GETDATA
		ST		R1,FIRST
		AND		R5,R5,#0
		JSR		GETDATA
		ST		R1,SECOND
		LEA		R0,OP_PROMPT
		PUTS
		GETC				; single character assumed either 1 or 2
		OUT
		ST		R0, CHOOSE
		LD		R0,LF
		OUT
		LD		R0,CR
		OUT
		LD		R5,FIRST
		LD		R1,SECOND
		LD		R0, CHOOSE
		ADD		R0,R0,R6	; subtract x30 for digit
		BRp		CALL_DIV
		JSR		MULT
		AND		R0,R0,#0
		ADD		R0,R3,R0
		JSR		MULT_ANS
		BR		STOPFORREAL
CALL_DIV	JSR		DIVIDE
		LD		R6,THIRTY
		LD		R0,CHOOSE
		ADD		R0,R0,R6
		BRp		DIV_ANS
MULT_ANS	LD		R0,ANSWER
		ST		R3,FIRST
		LD		R5,NTHOUSAND
		ADD		R0,R0,R5
		OUT
		BRn		BIGNUMBER
BWAH		LD		R6,HUNDRED
		ST		R6,SECOND
		JSR		DIVIDE
		LD		R0,ANSWER
		OUT 	
		LD		R0,REMAINDER
		LD		R6,THIRTY
		ADD		R0,R0,R6
		BRz		SKIP
		ST		R0,FIRST
		LD		R6,TEN
		ST		R6,SECOND
		JSR		DIVIDE
		BRnzp		SKIPDONE
BIGNUMBER	LD		R6,THOUSAND
		ST		R6,SECOND
		JSR		DIVIDE
		LD		R0,ANSWER
		OUT
		ST		R5,FIRST
		BRnzp		BWAH
SKIP		LD		R0,REMAINDER
		ST		R0,ANSWER
SKIPDONE	LD		R0,ANSWER	
		OUT
		LD		R0,REMAINDER
		OUT
		BRnzp		STOPFORREAL
DIV_ANS		LEA		R0,ANSWERPROMPT
		PUTS
		LD		R0,ANSWER
		OUT
		LEA		R0,REMAINDERPROMPT
		PUTS
		LD		R0,REMAINDER
		OUT


		
STOPFORREAL	HALT
FIRST		.BLKW		1
SECOND		.BLKW		1
CHOOSE		.BLKW		1
HUNDRETH	.BLKW		1
TENTH		.BLKW		1
ONE		.BLKW		1
NTHOUSAND	.FILL		xFC18
THOUSAND	.FILL		x03E8
HUNDRED		.FILL		x0064
OP_PROMPT	.STRINGZ		"0) Multiply, 1) Divide: "
ANSWER		.BLKW		1
ANSWERPROMPT	.STRINGZ		"ANSWER:"
REMAINDERPROMPT	.STRINGZ		"  REMAINDER:"
REMAINDER	.BLKW		1

;
; Subroutine to get a two digit number
;
GETDATA		ST		R7,TEMP		; save return address
		AND		R1,R1,#0
		AND		R2,R2,#0
		LEA		R0,PROMPT
		LD		R6,THIRTY
		PUTS	
		GETC
		OUT
		ADD		R1,R0,#0	; move R0 to R1
		ADD		R1,R1,R6	; subtract x30 from R1 to get digit
		LD		R5,TEN
		JSR		MULT		; multiply R1 by 10
		GETC
		OUT
		ADD		R0,R0,R6	; subtract x30 from next character
		ADD		R1,R0,R1	; add two registers to get decimal value
		LD		R0,CR		; output a carriage return
		OUT		
		LD		R0,LF
		OUT
		LD		R7,TEMP
		RET				; value returned in R1

TEN		.FILL		#10
THIRTY		.FILL		xFFD0		; -48 or neg x30
FORTYONE	.FILL		xFFDB
FORTYEIGHT	.FILL		x0030
PROMPT		.STRINGZ	"Enter a two digit number: "
CR		.FILL		#13		; carriage return
LF		.FILL		#10		; line feed
;
;
; Subroutine to multiply R5 * R1
;
MULT		AND		R3,R3,#0	; R3 will be the result
AGAIN		ADD		R3,R3,R1
		ADD		R5,R5,#-1
		BRp		AGAIN
		AND		R1,R1,#0
		ADD		R1,R3,#0
		ST		R3,ANSWER
		RET				;answer in R1
		
;
; Subroutine to divide FIRST by SECOND
; R5 is FIRST, R1 is SECOND /note that these could have been obtained from memory also.
; This algorithm assumes the divisor will always be smaller than the dividend to make life easy!
;
DIVIDE		ST		R7,TEMP
		LD		R1,SECOND
		LD		R5,FIRST
		NOT		R2,R1		; assume R1 is divisor
		ADD		R2,R2,x0001	; negates R1
		AND		R3,R3,#0	; clear R3 for answer
		AND		R4,R4,#0	; clear R4 for remainder
		
SUB_AGAIN	ADD		R5,R5,R2	; subtract divisor 
		BRzp		INC_ANSWER
		BR		FIX_REMAIN

INC_ANSWER	ADD		R3,R3,#1
		AND		R4,R4,#0
		ADD		R4,R5,#0	; move current remainder to R4
		BRp		SUB_AGAIN
		BRnzp		FIX_REMAIN
		
		
		
FIX_REMAIN	AND		R5,R5,#0
		ADD		R5,R4,#0			
		ST		R5,REMAINDER
		LD		R6,FORTYEIGHT
		ADD		R3,R3,R6
		ADD		R5,R5,R6
		ST		R5,REMAINDER
		ST		R3,ANSWER
		LD		R7,TEMP
		RET
;


TEMP		.BLKW		1		; use to keep subroutine return address
		.END

