uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')

OPT = -O2

PREFIX?=/usr/local

AR = ar
INC = -I../include
HASHLIB = ../lib/libhash.a
HASHTEST = hash_test
%.o:%.c
	$(CC) $(OPT) $(INC) $(PROF) -c $< 
all: $(HASHTEST)
$(HASHTEST): hash_test.o
	$(CC) $(LDFLAGS) -Wall $(OPT) $(INC) $(PROF)  -o $(HASHTEST) hash_test.o $(HASHLIB)
memcheck:
	- valgrind --leak-check=yes ./hash_test -v
callgraph:
	- ./hash_test
	- gprof ./hash_test | gprof2dot.py | dot -Tsvg -o hash_test.svg
gprof:
	$(MAKE) clean
	$(MAKE) PROF="-pg" OPT="-O0"
	$(MAKE) callgraph
valgrind:
	$(MAKE) clean
	$(MAKE) PROF="-g" OPT="-O0"
	$(MAKE) memcheck
clean:
	rm -rf  hash_test *.a *.o *.gcda *.gcno *.gcov *.out *.svg *.dot
