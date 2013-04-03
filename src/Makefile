GXX=g++
SRCS = $(wildcard *.cpp)
OBJS = $(subst .cpp,.o,$(SRCS))
BIN  = timer.exe

CPPFLAGS =-Wall -g -Wno-deprecated
LDFLAGS  =-g

.PHONY: all clean
all: $(BIN)

$(BIN): $(OBJS)
	$(GXX) -o$(BIN) $(OBJS) $(LDFLAGS)

$(OBJS): %.o : %.cpp
	$(GXX) -c $(CPPFLAGS) $< -o $@

clean:
	-rm $(BIN)
	-rm $(OBJS)
