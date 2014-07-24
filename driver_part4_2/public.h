#ifndef PUBLIC_H
#define PUBLIC_H

#ifndef __USE_NTOSKRNL__ // Not inside the driver (ie: in client app)
#include <windows.h>
#include <winioctl.h>
#endif // __USE_NTOSKRNL__

#define FIRST_DEVICE_NAME "YTDevice_1"
#define FIRST_DEVICE_PATH L"\\Device\\" FIRST_DEVICE_NAME
#define FIRST_DOSDEVICE_PATH L"\\DosDevices\\" FIRST_DEVICE_NAME
#define FIRST_FILE_PATH "\\\\.\\" FIRST_DEVICE_NAME

#define SECOND_DEVICE_NAME "YTDevice_2"
#define SECOND_DEVICE_PATH L"\\Device\\" SECOND_DEVICE_NAME
#define SECOND_DOSDEVICE_PATH L"\\DosDevices\\" SECOND_DEVICE_NAME
#define SECOND_FILE_PATH "\\\\.\\" SECOND_DEVICE_NAME

#define BUFFER_SIZE 1024

// Normal IOCTL
#define MY_IOCTL_SAY_HELLO \
	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_READ_DATA | FILE_WRITE_DATA)

//Internal IOCTL (First driver)
#define MY_INTERNAL_IOCTL_HELLO \
	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_READ_DATA | FILE_WRITE_DATA)

#endif // PUBLIC_H