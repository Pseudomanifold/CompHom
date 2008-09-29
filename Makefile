CC          = g++
CCFLAGS     = -c -g
LIBS        = -L /usr/local/lib
INCLUDES    = -I /usr/local/include
LDFLAGS     = 

MYOBJECTS   = ct.o matrix.o simplex.o chain.o
BIN         = ct

$(BIN): $(MYOBJECTS) Makefile
	$(CC) $(MYOBJECTS) $(LIBS) $(LDFLAGS)  -o $(BIN)

.cpp.o:
	$(CC) $(INCLUDES) $(CCFLAGS) $<

clean:
	rm -f *.o *.core $(BIN)


