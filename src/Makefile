GXX=g++
SRC_PATH=.:time_wheel:hash_map
SRCS=$(foreach dir,$(subst :, ,$(SRC_PATH)),$(wildcard $(dir)/*.cc))
#SRCS=$(wildcard *.cc)
OBJS=$(subst .cc,.o,$(SRCS))
BIN=timer.exe
LIB=libtimer.a
RM=@rm -rf

CPPFLAGS =-Wall -g -Wno-deprecated -I.
LDFLAGS  =-g

.PHONY: all clean
all: $(OBJS)
	ar -r $(LIB) $(OBJS)

bin: $(BIN)	

$(BIN): $(OBJS)
	$(GXX) -o$(BIN) $(OBJS) $(LDFLAGS)

$(OBJS): %.o : %.cc
	$(GXX) -c $(CPPFLAGS) $< -o $@

clean:
	$(RM) $(BIN)
	$(RM) $(OBJS)
	$(RM) $(LIB)
