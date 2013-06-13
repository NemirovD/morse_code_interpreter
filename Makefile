OBJECTFILES = mCI.obj Data.obj


all: $(OBJECTFILES)
	cl /FemCI  /EHsc $(OBJECTFILES)
	DEL *.obj
	
run:
	mCI dumpdata.txt
