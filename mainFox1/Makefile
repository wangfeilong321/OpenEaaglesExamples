#
include ../makedefs

PGM = mainFox1

CPPFLAGS += $(FOX_DEFINES) -I$(FOX_INC_DIR)
LDFLAGS  += $(FOX_LIBS)

EPPFLAGS += -I$(DATA_DIR) \
	   -DFONT_PATH=\"$(DATA_DIR)/fonts/\" \
	   -DTEXTURES_PATH=\"$(DATA_DIR)/textures/\"

OBJS = main.o SettingsDialog.o MainWindow.o

LDFLAGS += -lGLU -lGL
#LDFLAGS += -lXft -lXrender -lXrandr -lXi -lXinerama -lXcursor -lXfixes -lXext -lX11
#LDFLAGS += -lfontconfig
LDFLAGS += -lfontconfig
LDFLAGS += -lpthread -lrt

all: $(PGM)

edl:
	@echo "No input files to process"

$(PGM): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

clean:
	-rm -f *.o
	-rm -f $(PGM)
