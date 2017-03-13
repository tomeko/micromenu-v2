#
#            DMBS Build System
#     Released into the public domain.
#
#  dean [at] fourwalledcubicle [dot] com
#        www.fourwalledcubicle.com
#

# Run "make help" for target help.

MICRO_MENU_PATH = ..

MCU          = atmega328p
ARCH         = AVR8
F_CPU        = 16000000
OPTIMIZATION = s
TARGET       = Template
SRC          = Example.c $(MICRO_MENU_PATH)/MicroMenu.c $(MICRO_MENU_PATH)/usart.c
CC_FLAGS     = -I $(MICRO_MENU_PATH) 
LD_FLAGS     = -Wl,-u,vfprintf -lprintf_min 
AVRDUDE_PORT = ./com8
AVRDUDE_PROGRAMMER	= arduino

# Default target
all:

# Include DMBS build script makefiles
DMBS_PATH   ?= DMBS
include $(DMBS_PATH)/core.mk
include $(DMBS_PATH)/gcc.mk
include $(DMBS_PATH)/cppcheck.mk
include $(DMBS_PATH)/doxygen.mk
include $(DMBS_PATH)/dfu.mk
include $(DMBS_PATH)/hid.mk
include $(DMBS_PATH)/avrdude.mk
include $(DMBS_PATH)/atprogram.mk
