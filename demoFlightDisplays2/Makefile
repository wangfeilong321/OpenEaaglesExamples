#
include ../makedefs

PGM = demoFlightDisplays2

OBJS =  \
	main.o \
	Pfd.o \
	SpdLines.o \
	TestPfd.o

LDFLAGS += -loeglut -loeinstruments -loegraphics -loebase
LDFLAGS += -lftgl -lfreetype -lglut -lGLU -lGL -lX11 -lpthread
#LDFLAGS += -lXi -lXmu
LDFLAGS += -lrt

all: $(PGM)

edl:
	cpp configs/test.epp >test.edl $(EPPFLAGS)

$(PGM): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

clean:
	-rm -f *.o
	-rm -f $(PGM)
