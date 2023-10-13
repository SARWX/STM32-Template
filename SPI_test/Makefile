TEMPLATEROOT = ../

# compilation flags for gdb

CFLAGS  += -O0 -g
ASFLAGS += -g 

# object files

OBJS=  $(STARTUP) spi_test.o
OBJS+= stm32f10x_gpio.o stm32f10x_rcc.o stm32f10x_usart.o stm32f10x_spi.o spi_functions.o

# include common make file

include $(TEMPLATEROOT)/Makefile.common


