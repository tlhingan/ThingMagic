#ifndef SERIAL_COMMS_SEEN
#define SERIAL_COMMS_SEEN

int SerialSetup();
int SerialWrite(char* stringToSend);
int ClosePort();

#endif