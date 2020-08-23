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
	mv $(PROGRAM)		/usr/local/bin
	cp auto_repmon2		/usr/local/bin
	cp auto_repmon2.service	/etc/systemd/system
	cp nextion.service	/etc/systemd/system
	killall -q -s 9 dmonitor
	systemctl stop auto_repmon
	systemctl disable auto_repmon
	systemctl daemon-reload
	systemctl enable nextion.service
	systemctl restart nextion.service

# Dependency of Header Files
$(OBJECTS)	: dmonitor.h

