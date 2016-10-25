#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#include <bluetooth/rfcomm.h>

//#define TARGET_NAME "Leopard 2 A6-iKarosStudio"
#define TARGET_NAME "HC-05"

inquiry_info *BtDevice = NULL;
int BtDeviceId = 0;
int BtSock = 0;

int ScanBluetoothDevice (void )
{
	int inq_res = 0;
	char bt_addr[20] = {0} ;
	char bt_name[248] = {0} ;
	inquiry_info *DeviceList = NULL;
	int index = 0;
	int res = 0;

	/*
	  open device
	*/
	BtDeviceId = hci_get_route (NULL) ;
	if (BtDeviceId < 0) {
		perror ("open bluetooth dongle") ;
		return -1;
	}

	/*
	  open device socket
	*/
	BtSock = hci_open_dev (BtDeviceId) ;
	if (BtSock < 0) {
		perror ("open socket") ;
		return -2;
	}

	/*
	  prepare mem space for device data
	*/
	DeviceList = (inquiry_info *) malloc (255 * sizeof (inquiry_info) ) ;

	/*
	  scan bluetooth device
	*/
	inq_res = hci_inquiry (BtDeviceId, 8, 255, NULL, &DeviceList, IREQ_CACHE_FLUSH) ;
	if (inq_res < 0) {
		perror ("inquiry info") ;
		return -3;
	}

	printf ("rsp num:%d\n", inq_res) ;
	for (index = 0; index < inq_res; index++) {
		ba2str (&(DeviceList + index)->bdaddr, bt_addr) ;
		memset (bt_name, 0, sizeof (bt_name) ) ;
		res = hci_read_remote_name (BtSock, &(DeviceList + index)->bdaddr, sizeof (bt_name), bt_name, 0) ;

		if (res < 0) {
			sprintf (bt_name, "unknown") ;
		}
		printf ("[%02d] - %s - %s", index, bt_addr, bt_name) ;
		if (strcmp (bt_name, TARGET_NAME) == 0) {
			printf (" [*]\n") ;
			BtDevice = (inquiry_info *) malloc (sizeof (inquiry_info) ) ;
			memset (BtDevice, 0, sizeof (inquiry_info) ) ;
			memcpy (BtDevice, (DeviceList + index), sizeof (inquiry_info) ) ;
		} else {
			printf ("\n") ;
		}
		
	}

	printf ("\n") ;
	free (DeviceList) ;
	close (BtSock) ;

	return 0;
}


int SocketHandle = 0;
struct sockaddr_rc BtAddr = {0} ;
int ConnectBluetoothDevice (void )
{
	char bt_name[255] = {0} ;
	char bt_addr[18] = {0} ;
	int Status = 0;
	char in[4] = {0} ;

	memset (bt_name, 0, 255) ;
	ba2str (&(BtDevice)->bdaddr, bt_addr) ;
	printf ("Connect to:%s\n", bt_addr) ;

	SocketHandle = socket (AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM) ;
	if (SocketHandle < 0) {
		perror ("open socket") ;
		return -1;
	}

	BtAddr.rc_family = AF_BLUETOOTH;
	BtAddr.rc_channel = (uint8_t) 1;
    str2ba (bt_addr, &BtAddr.rc_bdaddr) ;

	/*
	  Set pin code via your bluetooth config..
	 */
	Status = connect (SocketHandle, (struct sockaddr *) &BtAddr, sizeof (BtAddr) ) ;
	if (Status < 0) {
		perror ("connect device") ;		
	} else {
		printf ("connected!\n") ; 
		//write (SocketHandle, "hello\r\n", 7) ;
		read (SocketHandle, in, 10) ;
		printf ("in:%s\n", in) ;
	}
	
	
	return 0;
}

int DisconnectBluetoothDevice (void )
{
	printf ("disconnect!\n") ;
	close (SocketHandle) ;
}

int BtSendBytes (char *Data)
{
	return write (SocketHandle, Data, sizeof (Data) ) ;
}

int BtRecvBytes (char *Buf)
{
	return read (SocketHandle, Buf, 255) ;
}
