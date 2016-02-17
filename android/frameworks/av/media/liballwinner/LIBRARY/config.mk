
################################################################################
## configurations.
################################################################################

## conpile tool chain for linux makefile.
# arm-linux-gnueabihf- or arm-none-linux-gnueabi- tool chain
OPTION_CC_GNUEABIHF = 1
OPTION_CC_GNUEABI   = 2

## option for os config.
OPTION_OS_ANDROID = 1
OPTION_OS_LINUX   = 2

## option for os version config.
OPTION_OS_VERSION_ANDROID_4_2 = 1
OPTION_OS_VERSION_ANDROID_4_4 = 2
OPTION_OS_VERSION_ANDROID_5_0 = 3

## option for momory driver config.
OPTION_MEMORY_DRIVER_SUNXIMEM = 1
OPTION_MEMORY_DRIVER_ION      = 2


## option for product config.
OPTION_PRODUCT_PAD      = 1
OPTION_PRODUCT_TVBOX    = 2
OPTION_PRODUCT_OTT_CMCC = 3
OPTION_PRODUCT_IPTV     = 4
OPTION_PRODUCT_DVB      = 5

## option for chip config.
OPTION_CHIP_1623 = 1
OPTION_CHIP_1625 = 2
OPTION_CHIP_1633 = 3
OPTION_CHIP_1651 = 4
OPTION_CHIP_1650 = 5
OPTION_CHIP_1661 = 6
OPTION_CHIP_1667 = 7
OPTION_CHIP_1639 = 8
OPTION_CHIP_1673 = 9
OPTION_CHIP_1680 = 10
OPTION_CHIP_1681 = 11
OPTION_CHIP_1689 = 12

## option for dram interface.
OPTION_DRAM_INTERFACE_DDR1_16BITS = 1
OPTION_DRAM_INTERFACE_DDR1_32BITS = 2
OPTION_DRAM_INTERFACE_DDR2_16BITS = 3
OPTION_DRAM_INTERFACE_DDR2_32BITS = 4
OPTION_DRAM_INTERFACE_DDR3_16BITS = 5
OPTION_DRAM_INTERFACE_DDR3_32BITS = 6
OPTION_DRAM_INTERFACE_DDR3_64BITS = 7

## option for debug level.
OPTION_LOG_LEVEL_CLOSE   = 0
OPTION_LOG_LEVEL_ERROR   = 1
OPTION_LOG_LEVEL_WARNING = 2
OPTION_LOG_LEVEL_DEFAULT = 3
OPTION_LOG_LEVEL_DETAIL  = 4

## option for cmcc
OPTION_CMCC_NO  = 0
OPTION_CMCC_YES = 1

## option for ve ipc
OPTION_VE_IPC_DISABLE  = 1
OPTION_VE_IPC_ENABLE = 2

## configuration.
CONFIG_CC = $(OPTION_CC_GNUEABI)
CONFIG_OS = $(OPTION_OS_ANDROID)
CONFIG_OS_VERSION = $(OPTION_OS_VERSION_ANDROID_5_0)
CONFIG_MEMORY_DRIVER = $(OPTION_MEMORY_DRIVER_ION)
CONFIG_PRODUCT = $(OPTION_PRODUCT_PAD)

ifneq ($(filter astar,$(TARGET_BOARD_PLATFORM)),)

       CONFIG_CHIP = $(OPTION_CHIP_1667)
       CONFIG_DRAM_INTERFACE = $(OPTION_DRAM_INTERFACE_DDR3_16BITS)
      
       LOCAL_CFLAGS += -DCHIP_1667

else

ifneq ($(filter kylin,$(TARGET_BOARD_PLATFORM)),)

       CONFIG_CHIP = $(OPTION_CHIP_1639)
       CONFIG_DRAM_INTERFACE = $(OPTION_DRAM_INTERFACE_DDR3_64BITS)
      
  LOCAL_CFLAGS += -DCHIP_1639
  
else

ifneq ($(filter octopus,$(TARGET_BOARD_PLATFORM)),)

       CONFIG_CHIP = $(OPTION_CHIP_1673)
       CONFIG_DRAM_INTERFACE = $(OPTION_DRAM_INTERFACE_DDR3_32BITS)
       
  LOCAL_CFLAGS += -DCHIP_1673

else

ifneq ($(filter tulip,$(TARGET_BOARD_PLATFORM)),)

       CONFIG_CHIP = $(OPTION_CHIP_1689)
       CONFIG_DRAM_INTERFACE = $(OPTION_DRAM_INTERFACE_DDR3_32BITS)
 
  LOCAL_CFLAGS += -DCHIP_1689
else
       $(warning $(TARGET_BOARD_PLATFORM))
endif
endif
endif
endif

CONFIG_CMCC = $(OPTION_CMCC_NO)
CONFIG_VE_IPC = $(OPTION_VE_IPC_DISABLE)
ifdef TARGET_BUSINESS_PLATFORM  
    ifeq (cmccwasu , $(TARGET_BUSINESS_PLATFORM))
        CONFIG_CMCC = $(OPTION_CMCC_YES)
    endif
endif
##############################define global val#################################

LIB_AW_PATH := $(TOP)/frameworks/av/media/liballwinner
LAW_CFLAGS :=

SW_DEINTERLACE_FLAGS = $(shell test -d $(LIB_AW_PATH)/LIBRARY/PLAYER/sw-deinterlace;echo $$?)
ifeq ($(SW_DEINTERLACE_FLAGS), 0)
USE_SW_DEINTERLACE := yes
LAW_CFLAGS += -DUSE_SW_DEINTERLACE
endif

USE_NEW_DISPLAY := 0

ifeq ($(CONFIG_CHIP),$(OPTION_CHIP_1667))
USE_NEW_DISPLAY := 1
endif

# on A83-pad-5.0
ifeq ($(CONFIG_CHIP),$(OPTION_CHIP_1673))
    ifeq ($(CONFIG_PRODUCT),$(OPTION_PRODUCT_PAD))
        ifeq ($(CONFIG_OS_VERSION),$(OPTION_OS_VERSION_ANDROID_5_0))
        USE_NEW_DISPLAY := 1
        endif
    endif
endif

# on A80-pad-5.0
ifeq ($(CONFIG_CHIP),$(OPTION_CHIP_1639))
    ifeq ($(CONFIG_PRODUCT),$(OPTION_PRODUCT_PAD))
        ifeq ($(CONFIG_OS_VERSION),$(OPTION_OS_VERSION_ANDROID_5_0))
        USE_NEW_DISPLAY := 1
        endif
    endif
endif

# on A64-pad-5.0
ifeq ($(CONFIG_CHIP),$(OPTION_CHIP_1689))
    ifeq ($(CONFIG_PRODUCT),$(OPTION_PRODUCT_PAD))
        ifeq ($(CONFIG_OS_VERSION),$(OPTION_OS_VERSION_ANDROID_5_0))
        USE_NEW_DISPLAY := 1
        endif
    endif
endif

CMCC := no
ifeq ($(CONFIG_CMCC), $(OPTION_CMCC_YES))
CMCC := yes
endif

###################################end define####################################

