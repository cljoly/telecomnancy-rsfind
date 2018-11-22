#todo : permettre l'execution des tests dans le makefile

#Compiler and Linker
CC          := gcc

#The Target Binary Program
TARGET      := rsfind

#The Directories, Source, Objects, Binary
SRCDIR      := src
BUILDDIR    := obj
TARGETDIR   := .
SRCEXT      := c
OBJEXT      := o

#Flags and Libraries
CFLAGS      := -Wall -Werror -Wextra -Wformat -g
LIB         := -lmagic

#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
SOURCES     := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS     := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))

#Default Make
all: directories $(TARGET)

#test
test: all test.sh
	bash test.sh $(TARGETDIR)/$(TARGET)

#Remake
remake: cleaner all

#Make the Directories
directories:
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(BUILDDIR)

#Clean only Objecst
clean:
	@$(RM) -rf $(BUILDDIR)

#Full Clean, Objects and Binaries
cleaner: clean
	@$(RM) -rf $(TARGETDIR)

#Link
$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGETDIR)/$(TARGET) $^ $(LIB)

#Compile
$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

#Non-File Targets
.PHONY: all remake clean cleaner directories
