#
include ../makedefs

PGM = testNavigation
OBJS =  main.o

LDFLAGS += -loebase

all: $(PGM)

$(PGM): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

edl:

clean:
	-rm -f *.o
	-rm -f $(PGM)
