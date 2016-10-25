#include <stdio.h>
#include <stdlib.h>

int main (void )
{
	int res = 0;
	int err = 0;

	printf ("---bluetooth dongle---\n") ;
	
	ScanBluetoothDevice () ;
	ConnectBluetoothDevice () ;
	DisconnectBluetoothDevice () ;
	return 0;
}
