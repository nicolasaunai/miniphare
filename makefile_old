# Simple makefile

PGM = mphare
CPP = mpicxx

#DSRC = $(HOME)/codes/miniphare
DSRC = src

SOURCES1 = $(DSRC)/main.cpp  
HEADERS1 = 
INC1 = 

SOURCES2 = $(DSRC)/pusher/pusher.cpp 
HEADERS2 = $(DSRC)/pusher/pusher.h 

INC2 = -I$(DSRC)/pusher 


SOURCES3 = $(DSRC)/Field/Field1D.cpp \
           $(DSRC)/Field/Field2D.cpp \
           $(DSRC)/Field/Field3D.cpp

HEADERS3 = $(DSRC)/Field/Field.h  \
           $(DSRC)/Field/Field1D.h \
           $(DSRC)/Field/Field2D.h \
           $(DSRC)/Field/Field3D.h \
           $(DSRC)/tools/tools.h

INC3 = -I$(DSRC)/Field -I$(DSRC)/tools 


# We substitute .cpp by .o to deduce the object file names
OBJS1=$(subst .cpp,.o,$(SOURCES1))
OBJS2=$(subst .cpp,.o,$(SOURCES2))
OBJS3=$(subst .cpp,.o,$(SOURCES3))

OBJS=$(OBJS1) $(OBJS2) $(OBJS3)

$(info OBJS="$(OBJS)")

INC=$(INC1) $(INC2) $(INC3)

ifeq ($(config),debug)
        CFLAGS = -Wall -pedantic -ansi -g
        DOBJ = objs_dbg

else ifeq ($(config),analyze)
        CFLAGS = -Wall -pedantic -ansi -g  --analyze
        DOBJ = objs_dbg

else ifeq ($(config),release)
        CFLAGS = -Wall -pedantic -ansi -O3
        DOBJ = objs
endif

LDFLAGS = -lm

# $^ designe la liste des dependances
# $@ designe le nom de la cible
# $< designe le nom de la premiere dependance
# $* designe le nom du fichier sans suffixe

#%.o: %.cpp 
#	$(CPP) -o $@ -c $< $(CFLAGS) $(INC)

all: $(PGM)

$(PGM): $(OBJS)
	$(CPP) -o $@ $^ $(LDFLAGS) 

$(OBJS1): $(SOURCES1) $(HEADERS1)
	$(CPP) -o $@ -c $*.cpp $(CFLAGS) $(INC1)

$(OBJS2): $(SOURCES2) $(HEADERS2)
	$(CPP) -o $@ -c $*.cpp $(CFLAGS) $(INC2)
	
$(OBJS3): $(SOURCES3) $(HEADERS3)
	$(CPP) -o $@ -c $*.cpp $(CFLAGS) $(INC3)

clean:
	rm -f $(OBJS) $(PGM)


