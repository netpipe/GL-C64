PROJECT := grafdemo

C_SOURCES := $(wildcard *.c)

ASM_SOURCES := $(wildcard *.s)

EXECUTABLE = $(PROJECT).prg

DISKIMAGE = $(PROJECT).d64

CC = cl65

CCOPTS =

DEBUGOPTS = 

EMULATOR_APP = /usr/bin/x64

DISKIMAGE_APP = /usr/bin/c1541


.PHONY: all
compile:
	$(CC) $(CCOPTS) $(C_SOURCES) $(ASM_SOURCES) -o $(EXECUTABLE)

image: compile
	$(DISKIMAGE_APP) -format $(PROJECT),1 d64 $(DISKIMAGE)
	$(DISKIMAGE_APP) $(DISKIMAGE) -write $(EXECUTABLE) $(EXECUTABLE)

clean:
	rm *.o $(EXECUTABLE) $(DISKIMAGE)

emulator: image
	$(EMULATOR_APP) -autostart "$(DISKIMAGE):$(EXECUTABLE)"
