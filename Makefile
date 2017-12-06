# GBS Shell Makefile

CC = gcc
CFLAGS  = -Wall -g
OBJ = gbsh.o execute_command.o functions.o Matrix_Sum_Max.o 

all: gbsh 

gbsh: $(OBJ) 
	$(CC) $(CFLAGS) -o gbsh $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $<
