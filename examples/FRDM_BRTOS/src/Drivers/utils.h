#define NO_ALIGN    (uint8_t)0
#define SPACE_ALIGN (uint8_t)1
#define ZEROS_ALIGN (uint8_t)2


void Print4Digits(unsigned short int number, unsigned char align, char *buff);
void Print3Digits(unsigned short int number, unsigned char align, char *buff);
void Print2Digits(unsigned char number, unsigned char align, char *buff);
char *PrintDecimal(int16_t val, CHAR8 *buff);

#define ByteSwap(A)     (A=(A<<8)+(A>>8))
unsigned long int LWordSwap(unsigned long int u32DataSwap);

