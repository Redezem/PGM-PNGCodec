
CC = g++

DEBUG = -g

OPT = 
#-O2 
# -D_NO_ERR_CHECK \
#  -D_M


# NOTE: ld requires dependency ordering to be in reverse!
LIBS = -lpng -lz -lm 

OBJS = main.o PNGCodec.o

EXEC = pngInverter


# Optimization flags for Intel Pentium CPUs
PENTIUM_OPT = -O2 -march=pentium -mcpu=pentium -ffast-math -fomit-frame-pointer -malign-double -finline-functions -fschedule-insns 

#-fforce-mem -fforce-addr 

# Optimizing flags for AMD Athlon CPUs (http://aboutlinux.com/art_k7opt1_e.html)
ATHLON_OPT = -O2 -fomit-frame-pointer -mpentiumpro -march=pentiumpro -malign-functions=4 -fexpensive-optimizations -malign-double -fschedule-insns -finline-functions 


all:	$(EXEC)

pngInverter:$(OBJS) 
	 $(CC) $(DEBUG) $(OPT) $(LDFLAGS) -o $(EXEC) $(OBJS) $(LIBS)

%.o:	%.cpp
	$(CC) $(DEBUG) $(OPT) $(CPPFLAGS) -c $*.cpp

clean:
	rm -f *.o *~ ./core* ./gmon.out pngInverter

