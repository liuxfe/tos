#include <cnix/kernel.h>

void cstartup(void){
	short *p =(short*)__p2v(0xb8000 + 80*2*10);
	*p++ = 0x2700 | 'C';
	*p++ = 0x2700 | 'N';
	*p++ = 0x2700 | 'I';
	*p++ = 0x2700 | 'X';
	*p++ = 0x2700 | ' ';
	*p++ = 0x2700 | '!';
	*p++ = 0x2700 | ' ';
	for(;;);
}
