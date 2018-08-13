CROSS_COMPILE=
CC := $(CROSS_COMPILE)gcc
AR := $(CROSS_COMPILE)ar
DBG := -g
CFLAGS := -Wall -fstack-protector -fPIC

CUR_DIR := $(shell pwd)
SRC_DIR := $(CUR_DIR)/src


LIB_HOME := $(CUR_DIR)

PLAT_DIR := $(CUR_DIR)/platform
PLAT_INC := $(CUR_DIR)/platform/include

LINUX_PLAT := $(PLAT_DIR)/linux
C216B_PLAT := $(PLAT_DIR)/c216b

#LINUX
CFLAGS += -DLINUX_PLAT -DHAVE_PIPE -DHAVE_SELECT -DHAVE_SIGNAL
PLAT_SRCS := $(shell ls $(LINUX_PLAT)/*.c)
LDFLAGS := -lpthread
LDFLAGS += -lm

#C216B
#CFLAGS += -DC216B_PLAT
#PLAT_SRCS := $(shell ls $(C216B_PLAT)/*.c)

TARG := libplatform.a
TARGSO := libotamqtt.so

#============================================================= 

INCS :=  -I$(PLAT_INC)

SRCS = $(PLAT_SRCS)

OBJS := $(patsubst %.c, %.o, $(SRCS))

	
all: $(TARG) $(TARGSO)
	

	
$(TARG): $(OBJS)
	$(AR) -r $@ $^
	#cp $(TARG) $(LIB_HOME)

$(TARGSO): $(OBJS)
	$(CC)  -Wall -fstack-protector -fPIC -shared -o  $@ $^	 $(LDFLAGS)


	
.c.o:
	$(CC) $(CFLAGS) -c $< $(INCS) -o $@

install:
	-rm -rf ./install
	-mkdir -p install/lib
	-mkdir -p install/include
	-cp -rf $(TARG) ./install/lib/
	-cp -rf $(TARGSO) ./install/lib/
	-cp -rf $(PLAT_INC)/*.h ./install/include/
	
.Phony: clean
clean:
	-rm -rf $(TARG) $(TARGSO)  $(OBJS)  ./install  *.bak

