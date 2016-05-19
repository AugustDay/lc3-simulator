#makefile for hw5
hw5: cpu.o fsm.o
	gcc -o hw5 cpu.o fsm.o 
   
cpu.o: cpu.c cpu.h
	gcc -c cpu.c
   
fsm.o: fsm.c
	gcc -c fsm.c