
CC = g++

DEBUG = -g

OPT = 
#-O2 
# -D_NO_ERR_CHECK \
#  -D_MT

CPPFLAGS = ${DEBUG} ${OPT} \
	-I../PGMCodec/

LDFLAGS = -Wall -Wl,--export-dynamic


# NOTE: ld requires dependency ordering to be in reverse!
LIBS = 

OBJS = Region.o malloc_image.o PGMCodec.o main.o

EXEC = Region.exe


# Optimization flags for Intel Pentium CPUs
PENTIUM_OPT = -O2 -march=pentium -mcpu=pentium -ffast-math -fomit-frame-pointer -malign-double -finline-functions -fschedule-insns 

#-fforce-mem -fforce-addr 

# Optimizing flags for AMD Athlon CPUs (http://aboutlinux.com/art_k7opt1_e.html)
ATHLON_OPT = -O2 -fomit-frame-pointer -mpentiumpro -march=pentiumpro -malign-functions=4 -fexpensive-optimizations -malign-double -fschedule-insns -finline-functions 


all:	$(EXEC)

Histogram.exe:$(OBJS) 
	 $(CC) $(DEBUG) $(OPT) $(LDFLAGS) -o $(EXEC) $(OBJS) $(LIBS)

%.o:	%.cpp
	$(CC) $(DEBUG) $(OPT) $(CPPFLAGS) -c $*.cpp

malloc_image.o: ../PGMCodec/malloc_image.c ../PGMCodec/malloc_image.h
	$(CC) $(DEBUG) $(OPT) $(CPPFLAGS) -c ../PGMCodec/malloc_image.c

PGMCodec.o: ../PGMCodec/PGMCodec.cpp ../PGMCodec/PGMCodec.h
	$(CC) $(DEBUG) $(OPT) $(CPPFLAGS) -c ../PGMCodec/PGMCodec.cpp

clean:
	rm -f *.o Histogram.exe