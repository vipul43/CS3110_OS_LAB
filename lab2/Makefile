.ONESHELL:
SHELL = /bin/bash

#source file directory
SRCDIR := src
#headers directory
HEADERDIR := headers
#static library directory
STLIBDIR := stlib
#dynamic library directory
SOLIBDIR := solib
#static library objects directory
STOBJDIR := stobj
#dynamic library objects directory
SOOBJDIR := soobj
#default image directory
IMAGEDIR := imgs
#test images directory
TESTDIR := test
#executable directory
EXEDIR := bin
#main object directory
MAINOBJDIR := mainobj

#executable file generated from static libraries
STEXE := $(EXEDIR)/stapp
#executable file generated using dynamic libraries
SOEXE := $(EXEDIR)/soapp
#static libraries
STLIBS := $(addprefix $(STLIBDIR)/lib, matrix.a matrixmath.a ppm.a transformations.a)
#dynamic libraries
SOLIBS := $(addprefix $(SOLIBDIR)/lib, matrix.so matrixmath.so ppm.so transformations.so)
#static library object files for archiving
STOBJS := $(addprefix $(STOBJDIR)/, ppm.o transformations.o matrixmath.o matrix.o);
#dynamic library object files for shared object creation
SOOBJS := $(addprefix $(SOOBJDIR)/, ppm.o transformations.o matrixmath.o matrix.o);
#names of libraries to be used when compiling in correct linking dependency order
LIBS := $(addprefix -l, ppm transformations matrixmath matrix)
#header files for dependency checking
HEADERS := $(patsubst $(HEADERDIR)/%,$(HEADERDIR)/%,$(wildcard $(HEADERDIR)/*.h))
#all test files
TESTFILES := $(patsubst $(TESTDIR)/%,$(TESTDIR)/%,$(wildcard $(TESTDIR)/*.*))

vpath %.c $(SRCDIR)
vpath %.h $(HEADERS)

all:	build

build:	$(STEXE)

dynamic_build: $(SOEXE)

run:	$(STEXE)
	@if [ -f $(IMAGEDIR)/tmp/userinput.ppm ]; then
	@useroutpath=`cat imgs/tmp/useroutpath.txt`
	@$< 0 2 $(IMAGEDIR)/tmp/userinput.ppm $(IMAGEDIR)/tmp/useroutput.ppm
	@if [ $${useroutpath##*.} != ppm ]; then
	@convert $(IMAGEDIR)/tmp/useroutput.ppm $$useroutpath
	@else
	@cp $(IMAGEDIR)/tmp/useroutput.ppm $$useroutpath
	@fi
	@else
	@$< 0 2 $(IMAGEDIR)/input.ppm $(IMAGEDIR)/output.ppm
	@fi

run_dynamic: $(SOEXE) $(SOLIBS)
	@if [ -f $(IMAGEDIR)/tmp/userinput.ppm ]; then
	@useroutpath=`cat imgs/tmp/useroutpath.txt`
	@LD_LIBRARY_PATH=$(SOLIBDIR) $< 0 2 $(IMAGEDIR)/tmp/userinput.ppm $(IMAGEDIR)/tmp/useroutput.ppm
	@if [ $${useroutpath##*.} != ppm ]; then
	@convert $(IMAGEDIR)/useroutput.ppm $$useroutpath
	@else
	@cp $(IMAGEDIR)/tmp/useroutput.ppm $$useroutpath
	@fi
	@else
	@LD_LIBRARY_PATH=$(SOLIBDIR) $< 0 2 $(IMAGEDIR)/input.ppm $(IMAGEDIR)/output.ppm
	@fi 

prepare:
	@echo "If you want to convert non-ppm images, make sure you have imagemagick installed." "To install, use command: sudo apt install imagemagick"
	@echo
	@read -p "Enter input ABSOLUTE file path " userinputpath
	@read -p "Enter output ABSOLUTE file path " useroutpath
	if [ -f $$userinputpath -a -d $${useroutpath%/*} -a "$${useroutpath%/*}/" != "$$useroutpath" ]; then
	@mkdir -p $(IMAGEDIR)/tmp
	@echo $$useroutpath > $(IMAGEDIR)/tmp/useroutpath.txt
	@if [ "$${userinputpath##*.}" != "ppm" ]; then
	@convert $$userinputpath $(IMAGEDIR)/tmp/userinput.ppm
	@else
	@cp $$userinputpath $(IMAGEDIR)/tmp/userinput.ppm
	@fi
	@else
	@echo "Please recheck input and output paths. Make sure they're absolute"
	@fi

tests: $(STEXE)
	@if [ -d $(TESTDIR) ]; then
	@mkdir -p $(TESTDIR)/out
	@./$(STEXE) 0 1 $(TESTFILES)
	@echo All test results generated in $(TESTDIR)/out
	@fi

test%: $(TESTDIR)/test%.ppm $(STEXE)
	@if [ -f $< ]; then
	@mkdir -p $(TESTDIR)/out
	@./$(STEXE) 0 1 $<
	@echo $@ generated in $(TESTDIR)/out
	@else
	@echo $< "doesn't exist"
	@fi

$(TESTDIR)/test%.ppm:
	@if [ -f $@ ]; then
	@echo
	@fi

T%: $(STEXE)
	@TRANSTYPE=$@
	@if [ $${TRANSTYPE:1} -lt 3 ]; then
	@if [ -f $(IMAGEDIR)/tmp/userinput.ppm ]; then
	@useroutpath=`cat imgs/tmp/useroutpath.txt`
	@./$(STEXE) $${TRANSTYPE:1} 2 $(IMAGEDIR)/tmp/userinput.ppm $(IMAGEDIR)/tmp/useroutput.ppm
	@if [ $${useroutpath##*.} != ppm ]; then
	@convert $(IMAGEDIR)/useroutput.ppm $$useroutpath
	@else
	@cp $(IMAGEDIR)/tmp/useroutput.ppm $$useroutpath
	@fi
	@else
	@./$(STEXE) $${TRANSTYPE:1} 2 $(IMAGEDIR)/input.ppm $(IMAGEDIR)/output.ppm
	@fi
	@fi

liba: $(STLIBS) $(STOBJS)

libso: $(SOLIBS) $(SOOBJS)

$(STEXE): $(MAINOBJDIR)/main.o $(STLIBS) | $(EXEDIR)
	gcc -o $@ -I $(HEADERDIR) $< -L $(STLIBDIR) $(LIBS)
$(SOEXE): $(MAINOBJDIR)/main.o $(SOLIBS) | $(EXEDIR)
	gcc -o $@ -I $(HEADERDIR) $< -L $(SOLIBDIR) $(LIBS)

$(STLIBDIR)/lib%.a: $(STOBJDIR)/%.o | $(STLIBDIR)
	ar rcs $@ $<

$(SOLIBDIR)/lib%.so: $(SOOBJDIR)/%.o | $(SOLIBDIR)
	gcc -shared -o $@ $<

$(STOBJDIR)/%.o: %.c $(HEADERS) | $(STOBJDIR)
	gcc -o $@ -c -I $(HEADERDIR) $<

$(SOOBJDIR)/%.o: %.c $(HEADERS) | $(SOOBJDIR)
	gcc -fPIC -c -I $(HEADERDIR) $< -o $@

$(MAINOBJDIR)/%.o: %.c $(HEADERS) | $(MAINOBJDIR)
	gcc -o $@ -c -I $(HEADERDIR) $<



$(MAINOBJDIR):
	mkdir -p $@
$(EXEDIR):
	mkdir -p $@
$(STOBJDIR):
	mkdir -p $@
$(SOOBJDIR):
	mkdir -p $@
$(STLIBDIR):
	mkdir -p $@
$(SOLIBDIR):
	mkdir -p $@

.PHONY: clean
clean:
	rm -rf $(STLIBDIR) $(SOLIB) $(MAINOBJDIR) $(STOBJDIR) $(SOLIBDIR) $(EXEDIR) $(SOOBJDIR) $(IMAGEDIR)/output.ppm $(IMAGEDIR)/tmp $(TESTDIR)/out

