#include "types.h"
#include "user.h"

int
main(int argc, char *argv[])
{
	uint pa1, pa2;
	uint va1 = 0xFA5;
	uint va2 = 0xFA1;
	int ret_status = v2paddr(&va1, &pa1);
	if(ret_status==0)
		printf(1, "PA1: %x\n", pa1);
	else
		printf(1, "VA1 is invalid\n");
	ret_status = v2paddr(&va2, &pa2);
	if(ret_status==0)
		printf(1, "PA2: %x\n", pa2);
	else
		printf(1, "VA2 is invalid\n");
	printf(1, "DIFFERENCE IN OFFSET B/W va1 and va2: %d\n", pa2-pa1);
	va1 = 0x7FFFFFFF;
	ret_status = v2paddr(&va1, &pa1);
	printf(1, "RETURN STATUS: %d\n", ret_status);
	va1 = 0x80001000;
	ret_status = v2paddr(&va1, &pa1);
	printf(1, "KERNEL SPACE: %x\n", pa1);
	va1 = 0xBCD;
	va2 = 0xFACE;
	ret_status = v2paddr(&va1, &pa1);
	if(ret_status==0)
		printf(1, "PHYSICAL 0xABCD: %x\n", pa1);
	else
		printf(1, "0xABCD is invalid\n");
	ret_status = v2paddr(&va2, &pa2);
	if(ret_status==0)
		printf(1, "PHYSICAL 0xABCDEF: %x\n", pa2);
	else
		printf(1, "0xFACE is invalid\n");
	exit();
}
