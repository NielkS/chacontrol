OUT = chacontrol
CC = g++
ODIR = obj
SDIR = src
INC = -Iinc -Iext
EXT = -lwiringPi

_OBJS = chacon.o utils.o main.o
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))


$(ODIR)/%.o: $(SDIR)/%.cpp 
	$(CC) -c $(INC) -o $@ $< $(CFLAGS) 

$(OUT): $(OBJS) 
	$(CC) -o $(OUT) $(EXT) $^

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(OUT)
