BinName := blutooth_dongle

objs += main.o bt.o

AS       = $(CROSS_COMPILE)as
AR       = $(CROSS_COMPILE)ar
CC       = $(CROSS_COMPILE)gcc
LD       = $(CROSS_COMPILE)ld
NM       = $(CROSS_COMPILE)nm
OBJCOPY  = $(CROSS_COMPILE)objcopy
OBJDUMP  = $(CROSS_COMPILE)objdump
RANLIB   = $(CROSS_COMPILE)ranlib
READELF  = $(CROSS_COMPILE)readelf
SIZE     = $(CROSS_COMPILE)size
STRINGS  = $(CROSS_COMPILE)strings
STRIP    = $(CROSS_COMPILE)strip

LDFLAGS = -lpthread -lm
LDFLAGS += -lbluetooth
#LDFLAGS += `pkg-config --libs libbluetooth`

#LDFLAGS += `pkg-config --libs libjpeg`

CFLAGS += -w -fpic -g


#CFLAGS += `pkg-config --cflags libjpeg`
#CFLAGS += -O3


$(BinName) : $(objs)
	$(CC) $(CFLAGS) -o $(BinName) $(objs) $(LDFLAGS)

clean : 
	rm *.o $(BinName)
