#makefile for hw5
hw5: cpu.o fsm.o scurses.o
	gcc -o hw5 cpu.o fsm.o scurses.o
   
cpu.o: cpu.c cpu.h
	gcc -c cpu.c
   
fsm.o: fsm.c
	gcc -c fsm.c

scurses.o: scurses.c
	gcc -c scurses.c