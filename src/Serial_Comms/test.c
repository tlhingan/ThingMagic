#include <stdio.h>
#include <unistd.h>
#include "Serial_Comms.h"


int
main(void){
	int count = 0,
	count2 = 0;
	int numWritten = 0;
	//byte myArray[6] = {};
	
	SerialSetup();
	while(count < 10){
		char hexstring[] = {0x42, 0xff, 0xff, 0xff, 0xff, 0x0d};
		//numWritten = SerialWrite(hexstring);
		for(count2 = 0; count2 < 6; count++){
			numWritten = SerialWrite(&hexstring[count2]);
			printf("Count is %d and numWritten is %d and we wrote %hx\n", count, numWritten, hexstring[count2]);
		}
		
		//Channel (BFLR, RGB Intensity)
		// 0x42 0xff 0xff 0xff 0xff 0x0d
		sleep(2);
		printf("Count is %d and numWritten is %d\n", count, numWritten);
		count++;
	}
	ClosePort();
}
