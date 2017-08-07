#include "vsprintf.h"

unsigned char outbuf[1024];
unsigned char inbuf[1024];

int printf(const char* fmt, ...)
{
	va_list args;
	int i;
	
	/*�����ת��Ϊ�ַ���*/
	va_start(args,fmt);
	vsprintf((char*)outbuf, fmt, args);
	va_end();
	
	/*��ӡ�ַ���������*/
	for(i = 0; i < strlen((char*)outbuf); i++)
	{
		putc(outbuf[i]);
	}
	
	return i ;
}

int scanf(const char* fmt, ...)
{
	unsigned char ch;
	int i = 0;
	va_list args;
	
	/*��ȡ�����ַ���*/
	while(1)
	{
		ch = getc();
		if( (0x0d == ch)||(0x0a == ch) )
		{
			inbuf[i]='\n';
			break;
		}
		else
		{
			inbuf[i++] = ch;
		}
	}
	
	/*��ʽת��*/
	va_start(args, fmt);
	vsscanf((char*)inbuf, fmt, args);
	va_end(args);
	
	return i;
}