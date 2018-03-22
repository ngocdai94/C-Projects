all: mdatA mdatB

OBJS = main.o
INSTR_OBJS = sectionsA.o sectionsB.o
MDAT_OBJS = $(INSTR_OBJS:.o=.mdat.o)
MDAT_SRC = $(INSTR_OBJS:.o=.mdat.c)
MDAT_PP = $(INSTR_OBJS:.o=.i)

MDAT_DIR = .
SUDS_DIR = .

LIBS = -L$(MDAT_DIR) -lmdat -lpthread
INCLUDE = -I. -I$(MDAT_DIR)

CPP = gcc -E
CPPFLAGS =
CC = gcc
CFLAGS = -O0 -Wall -g
CXX = g++
CXXFLAGS = -O0 -Wall -g
LD = g++
LDFLAGS =

mdatA: $(OBJS) sectionsA.mdat.o checkerA.o
	$(LD) $(LDFLAGS) -o $@ $(OBJS) sectionsA.mdat.o checkerA.o $(LIBS)

mdatB: $(OBJS) sectionsB.mdat.o checkerB.o
	$(LD) $(LDFLAGS) -o $@ $(OBJS) sectionsB.mdat.o checkerB.o $(LIBS)

$(OBJS): %.o : %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDE)

$(MDAT_OBJS): %.o : %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)

$(MDAT_SRC): %.mdat.c : %.i
	$(SUDS_DIR)/suds -use_func_list suds_file_list.txt -suffix mdat $<

$(MDAT_PP): %.i : %.c $(HDRS)
	$(CPP) $(CPPFLAGS) $< > $@ $(INCLUDE)

checkerA.o: checker.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDE)

checkerB.o: checker.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -DCHECK_LIMIT -c $< -o $@ $(INCLUDE)

.DELETE_ON_ERROR: $(MDAT_SRC)

clean: 
	rm -f mdatA mdatB
	rm -f $(OBJS) $(MDAT_OBJS) checkerA.o checkerB.o
	rm -f $(MDAT_SRC) $(MDAT_PP)
