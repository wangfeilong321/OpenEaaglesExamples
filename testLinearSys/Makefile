#
include ../makedefs

PGM = testLinearSys
OBJS = main.o

LDFLAGS += -loelinearsystem -loebase

all: $(PGM)

$(PGM): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

edl:

clean:
	-rm -f *.o
	-rm -f $(PGM)
