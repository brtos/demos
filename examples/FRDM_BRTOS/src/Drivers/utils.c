
#include "BRTOS.h"
#include "utils.h"

uint32_t LWordSwap(uint32_t u32DataSwap)
{
    uint32_t u32Temp;
    u32Temp= (u32DataSwap & 0xFF000000) >> 24;
    u32Temp+=(u32DataSwap & 0xFF0000)   >> 8;
    u32Temp+=(u32DataSwap & 0xFF00)     << 8;
    u32Temp+=(u32DataSwap & 0xFF)       << 24;
    return(u32Temp);
}


void Print4Digits(uint16_t number, uint8_t align, CHAR8 *buff)
{
    uint8_t caracter = 0;
    uint8_t mil,cent,dez;
    uint8_t escreve_zero = FALSE;
    uint32_t i = 0;   
       
    if (number < 10000)
    {
      if (align == ZEROS_ALIGN)
      {
        escreve_zero = TRUE;
      }

      mil = (number / 1000);
      
      caracter = mil + '0';
      
      if(caracter != '0')
      {
        *(buff + i) = caracter;
        i++;
        escreve_zero = TRUE;
      }
      else
      {
        if(escreve_zero == TRUE)
        {
          *(buff + i) = caracter;
          i++;
        }
        else
        {
          if (align == SPACE_ALIGN)
          {
            *(buff + i) = ' ';
            i++;
          }
        }
      }
      
      cent = ((number - mil*1000)/100);
      caracter = cent + '0';
      if(caracter != '0')
      {
        *(buff + i) = caracter;
        i++;
        escreve_zero = TRUE;
      }
      else
      {
        if(escreve_zero == TRUE)
        {
          *(buff + i) = caracter;
          i++;
        }
        else
        {
          if (align == SPACE_ALIGN)
          {
            *(buff + i) = ' ';
            i++;
          }
        }
      }
      
      dez = ((number - 1000*mil - cent*100)/10);
      caracter = dez + '0';
      if(caracter != '0')
      {    
        *(buff + i) = caracter;
        i++;
      }
      else
      {
        if(escreve_zero == TRUE)
        {
          *(buff + i) = caracter;
          i++;
        }
        else
        {
          if (align == SPACE_ALIGN)
          {
            *(buff + i) = ' ';
            i++;
          }
        }
      }
      
      caracter = (number%10) + '0';
      *(buff + i) = caracter;
      i++;
      *(buff + i) = 0;
    }
}



void Print3Digits(uint16_t number, uint8_t align, CHAR8 *buff)
{
    uint8_t caracter = 0;
    uint8_t cent,dez;
    uint8_t escreve_zero = FALSE;    
    uint32_t i = 0;   
    
    if (number < 1000)
    {
      if (align == ZEROS_ALIGN)
      {
        escreve_zero = TRUE;
      }    
      
      cent = (number/100);
      caracter = cent + '0';
      
      if(caracter != '0')
      {
        *(buff + i) = caracter;
        i++;
        escreve_zero = TRUE;
      }
      else
      {
        if(escreve_zero == TRUE)
        {
          *(buff + i) = caracter;
          i++;
        }
        else
        {
          if (align == SPACE_ALIGN)
          {
            *(buff + i) = ' ';
            i++;
          }
        }
      }
      
      dez = ((number - cent*100)/10);
      caracter = dez + '0';

      if(caracter != '0')
      {
        *(buff + i) = caracter;
        i++;
        escreve_zero = TRUE;
      }
      else
      {
        if(escreve_zero == TRUE)
        {
          *(buff + i) = caracter;
          i++;
        }
        else
        {
          if (align == SPACE_ALIGN)
          {
            *(buff + i) = ' ';
            i++;
          }
        }
      }
          
      caracter = (number%10) + '0';
      *(buff + i) = caracter;
      i++;
      *(buff + i) = 0;      
    }
}



void Print2Digits(uint8_t number, uint8_t align, CHAR8 *buff)
{
    uint8_t caracter = 0;
    uint8_t dez;
    uint8_t escreve_zero = FALSE;
    uint32_t i = 0;    
    
    if (number < 100)
    {
      if (align == ZEROS_ALIGN)
      {
        escreve_zero = TRUE;
      }

      dez = number/10;
      caracter = dez + '0';

      if(caracter != '0')
      {
        *(buff + i) = caracter;
        i++;
        escreve_zero = TRUE;
      }
      else
      {
        if(escreve_zero == TRUE)
        {
          *(buff + i) = caracter;
          i++;
        }
        else
        {
          if (align == SPACE_ALIGN)
          {
            *(buff + i) = ' ';
            i++;
          }
        }
      }
      
      caracter = (number%10) + '0';
      *(buff + i) = caracter;
      i++;
      *(buff + i) = 0;
    }
}
