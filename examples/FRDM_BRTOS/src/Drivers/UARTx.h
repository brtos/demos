#include "xhw_types.h"


xtBoolean UARTPutString(unsigned long ulBase, char *string);
xtBoolean UARTPutChar(unsigned long ulBase, char data);
unsigned char UARTGetChar(unsigned long ulBase, unsigned char *data,int timeout);
void Init_UART0(void *parameters);
void Init_UART1(void *parameters);
void Init_UART2(void *parameters);
