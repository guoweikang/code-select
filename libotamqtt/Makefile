

LIB_STATIC = libotamqtt.a
LIB_SHARED = libotamqtt.so

OTA_CUR_DIR := $(shell pwd)
OTA_SRC_DIR := $(OTA_CUR_DIR)/src
                                                                                                                                               
OTA_SDKSRC_DIR := $(OTA_SRC_DIR)/sdksrc
OTA_MQTT_DIR := $(OTA_SRC_DIR)/mqtt
OTA_SM_DIR := $(OTA_SRC_DIR)/sm
PLAT_INC := $(OTA_CUR_DIR)/../libplat/platform/include
PLAT_LIB := $(OTA_CUR_DIR)/../libplat

TESTSDK :=testsdk
#=============================================================

OBJS := $(patsubst %.c,%.o, $(wildcard $(OTA_SDKSRC_DIR)/src/*.c ))
OBJS += $(patsubst %.c,%.o, $(wildcard $(OTA_MQTT_DIR)/src/*.c ))
OBJS += $(patsubst %.c,%.o, $(wildcard $(OTA_SM_DIR)/src/*.c ))
#=============================================================
CFLAGS += -g

CFLAGS := -Wall -fstack-protector -fPIC 
CFLAGS += -I$(OTA_MQTT_DIR)/include/ 
CFLAGS += -I$(OTA_SM_DIR)/include/  
CFLAGS += -I$(OTA_SDKSRC_DIR)/include/ 
CFLAGS += -I$(PLAT_INC)
#=============================================================
CFLAGS += -DLINUX_PLAT -DHAVE_PIPE -DHAVE_SELECT -DHAVE_SIGNAL
#=============================================================

LDFLAGS += -L$(PLAT_LIB)

LDFLAGS += -lplatform
LDFLAGS += -lm


LDFLAGS2 := -L$(OTA_CUR_DIR)
LDFLAGS2 +=  -lotamqtt
LDFLAGS2 += -Wl,-rpath=$(OTA_CUR_DIR)

#=============================================================

all: $(LIB_STATIC) $(LIB_SHARED)

$(LIB_STATIC) : $(OBJS)
	$(AR) rcs $(LIB_STATIC) $(OBJS)

$(LIB_SHARED): $(OBJS)
	$(CC)  -Wall -fstack-protector -fPIC -shared -o  $@ $^  $(LDFLAGS)
	
$(TESTSDK):
	$(CC) -Wall $(CFLAGS) $(OTA_SRC_DIR)/sample/test_main.c    -o $@  $(LDFLAGS2)
	
.c.o:
	$(CC) $(CFLAGS) -c $<  -o $@

.Phony: clean
clean:
	-rm -rf *.a *.o *.so $(LIB_SHARED) $(LIB_STATIC) $(OBJS) $(TESTSDK) 
