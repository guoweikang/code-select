CROSS_COMPILE=
CC := $(CROSS_COMPILE)gcc
AR := $(CROSS_COMPILE)ar
DBG := -g
CFLAGS := -Wall -fstack-protector -fPIC  -DLWM2M_CLIENT_MODE -DLWM2M_SUPPORT_JSON -DLWM2M_LITTLE_ENDIAN -DLWM2M_WITH_LOGS 

CUR_DIR := $(shell pwd)
CORE_DIR := $(CUR_DIR)/core
SDKSRC_DIR := $(CUR_DIR)/sdksrc
SDK_DIR := $(SDKSRC_DIR)/sdk
LIB_HOME := $(CUR_DIR)
PLAT_INC := $(CUR_DIR)/../libplat/platform/include
PLAT_LIB := $(CUR_DIR)/../libplat
#LINUX
CFLAGS += -DLINUX_PLAT -DHAVE_PIPE -DHAVE_SELECT -DHAVE_SIGNAL

#C216B
#CFLAGS += -DC216B_PLAT


TARG := liblwm2msdkL.a
TESTSDK :=testsdk
TARGSO := liblwm2msdkL.so

LDFLAGS_1 := -L$(PLAT_LIB)
LDFLAGS_1 += -lplatform


#LDFLAGS_2 := -L$(PLAT_LIB)
#LDFLAGS_2 += -lplatform

LDFLAGS_2 += -L$(CUR_DIR)
LDFLAGS_2 += -llwm2msdkL
LDFLAGS_2 += -lpthread
#============================================================= 

INCS := -I$(CORE_DIR) -I$(CORE_DIR)/er-coap-13 -I$(SDKSRC_DIR)/shared -I$(SDK_DIR) -I$(PLAT_INC)
SRCS = $(shell ls $(CORE_DIR)/*.c) $(shell ls $(CORE_DIR)/er-coap-13/*.c)  $(shell ls $(SDKSRC_DIR)/shared/*.c) $(shell ls $(SDK_DIR)/*.c)

OBJS := $(patsubst %.c, %.o, $(SRCS))

CFLAGS += $(INCS) 
	
all: $(TARG) $(TARGSO)
	

	
$(TARG): $(OBJS)
	$(AR) -r $@ $^


$(TARGSO): $(OBJS)
	$(CC)  -Wall -fstack-protector -fPIC -shared -o  $@ $^  $(LDFLAGS_1)

$(TESTSDK):
	$(CC) -Wall -fstack-protector -fPIC  $(SDKSRC_DIR)/testsdk/main.c $(CFLAGS)  -o $@  $(LDFLAGS_2)

	
.c.o:
	$(CC) $(CFLAGS) -c $<  -o $@

install:
	-rm -rf ./install
	-mkdir -p install/lib
	-mkdir -p install/include
	-cp -rf $(TARG) ./install/lib/
	-cp -rf $(SDKSRC_DIR)/sdk/lwm2msdk.h  ./install/include/
	-cp -rf $(TARGSO) ./install/lib/
	
.Phony: clean
clean:
	-rm -rf $(TARG) $(TARGSO) $(TESTSDK) $(OBJS) $(SDKSRC_DIR)/testsdk/main.o  ./install  *.bak

