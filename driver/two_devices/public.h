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

#endif // PUBLIC_H