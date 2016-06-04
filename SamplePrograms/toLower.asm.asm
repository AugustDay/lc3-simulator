	.ORIG	x3050
	LEA	R0, INTRO
	PUTS
	LD	R2, TERM
	LD	R4, LIM
	LD	R5, LIM2
	LD	R6, ZERO
	LD	R3, ASCII
AGAIN	TRAP	x20
	ST	R0, INPUT
	LEA	R0, PRINTIN
	PUTS
	LD	R0, INPUT
	OUT
	JSR	CHECK
	OUT
	BRnzp	AGAIN
CHECK	ADD	R1, R2, R0
	BRz	EXIT
	AND	R1, R1, R6
	ADD	R1, R0, R4
	BRnz	ERROR
	AND	R1, R1, R6
	ADD	R1, R0, R5
	BRzp	ERROR	
	ADD	R0, R0, R3
	RET

TERM	.FILL	xFFC9
INTRO	.stringz	"Enter an uppercase character"
INTRO2	.stringz	"This will output a lowercase version."
PRINTIN	.stringz	"The character you inputted was: "
PROMPTE	.stringz	"Error, value not an uppercase character."
LIM	.FILL	xFFC0
LIM2	.FILL	xFFA5
ZERO	.FILL	x0000
ASCII	.FILL	x0020
INPUT	.BLKW	1
ERROR	LEA	R0, PROMPTE
	PUTS
EXIT	HALT
	.END	