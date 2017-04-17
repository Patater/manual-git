SUBDIRS:= $(shell ls)

#---------------------------------------------------------------------------------
all: chapters
#---------------------------------------------------------------------------------
	@rm -fr bin
	@mkdir -p bin
	@find . -name "*.nds" -exec cp -fv {} bin \;

chapters:
	@for i in $(SUBDIRS); do if test -e $$i/Makefile ; then $(MAKE) -C $$i || { exit 1;} fi; done;

#---------------------------------------------------------------------------------
clean:
#---------------------------------------------------------------------------------
	@rm -fr bin
	@rm -f *.bz2
	@for i in $(SUBDIRS); do if test -e $$i/Makefile ; then $(MAKE)  -C $$i clean || { exit 1;} fi; done;
