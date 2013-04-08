GXX=g++
SRCS = $(wildcard *.cc)
OBJS = $(subst .cc,.o,$(SRCS))
BIN  = timer.exe

CPPFLAGS =-Wall -g -Wno-deprecated
LDFLAGS  =-g

.PHONY: all clean
all: $(BIN)

$(BIN): $(OBJS)
	$(GXX) -o$(BIN) $(OBJS) $(LDFLAGS)

$(OBJS): %.o : %.cc
	$(GXX) -c $(CPPFLAGS) $< -o $@

clean:
	-rm $(BIN)
	-rm $(OBJS)
