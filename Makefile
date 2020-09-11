###################################################
# Makefile for compiling nextion for dmonitor
###################################################

# Program Name and object files
PROGRAM	= nextion
OBJECTS = main.o functions.o getlinkdata.o dmonitor.o getstatus.o getcmdinfo.o getusers.o getactive.o

# Redefine MACRO
CC	= gcc

# Define extention of Suffix Rules
.SUFFIXES	: .c .o

# Rule of compiling program
$(PROGRAM)	: $(OBJECTS)
	$(CC) -o $(PROGRAM) $^

# Sufix Rule
.c.o	:
	$(CC) -c $<

# Target of Delete files
.PHONY	: clean
clean	:
	$(RM)  $(PROGRAM) $(OBJECTS)

# Install files
install	:
	sudo mv $(PROGRAM)		/usr/local/bin
	sudo cp nextion.service		/etc/systemd/system
	sudo systemctl daemon-reload
	sudo killall -q -s 9 dmonitor
	sudo systemctl stop auto_repmon.service
	sudo systemctl disable auto_repmon.service
	sudo systemctl stop rpt_conn.service
	sudo systemctl disable rpt_conn.service
	sudo systemctl enable nextion.service
	sudo systemctl restart nextion.service

# Dependency of Header Files
$(OBJECTS)	: dmonitor.h

