cpprepos=/usr/local

COMPER          = g++
LINKER          = g++
COMMINC         = -I. -I$(cpprepos)/log4cplus-1.1.3-rc2/include/
CMPFLAG         =  -g -O0 -Wall -c $(COMMINC)
LNKFLAG     	= -L $(cpprepos)/log4cplus-1.1.3-rc2/lib/ -Wl,-rpath=$(cpprepos)/log4cplus-1.1.3-rc2/lib/
SYTMLIB         = -llog4cplus

EXE1 = test_normal
EXE2 = test_multi_log
OBJ1 =test_normal.o
OBJ2 =test_multi_log.o

all:$(EXE1) $(EXE2)

BINDIR=../

cleanobj:
	rm -rf $(OBJ1)
	rm -rf $(OBJ2)
clean:
	rm -rf $(OBJ1)
	rm -rf $(OBJ2)
	rm -rf $(EXE1)
	rm -rf $(EXE2)

%.o:%.cpp
	$(COMPER) $(CMPFLAG) $<

$(EXE1):$(OBJ1)
	$(LINKER) $(LNKFLAG) $(SYTMLIB) -o $@ $?
	mv $@ $(BINDIR)
$(EXE2):$(OBJ2)
	$(LINKER) $(LNKFLAG) $(SYTMLIB) -o $@ $?
	mv $@ $(BINDIR)

