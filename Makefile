# Makefile for MyIRCTC (Train Ticket Booking App)
#
#   make          -> compiles every .c file and creates the program  myirctc
#   make clean    -> deletes the .o files and the program
#
# HOW IT WORKS: each .c file is compiled into a .o file (object file).
# Then all the .o files are linked together into the final program.

CC     = gcc
CFLAGS = -Wall -Wextra -g
TARGET = myirctc

OBJS = main.o utils.o date_utils.o train_list.o train_file.o \
       passenger_list.o passenger_file.o signup.o signin.o \
       reserve.o cancel.o booking.o admin.o

# on Windows the delete command is different
ifeq ($(OS),Windows_NT)
    CLEAN_CMD = -del /Q *.o myirctc.exe
else
    CLEAN_CMD = rm -f *.o $(TARGET)
endif

# link all object files into the program
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# rule: any .o file is made from the .c file of the same name
# (and it is rebuilt when train.h changes)
%.o: %.c train.h
	$(CC) $(CFLAGS) -c $<

clean:
	$(CLEAN_CMD)

.PHONY: clean

