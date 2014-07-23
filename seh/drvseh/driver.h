#ifndef DRIVER_H
#define DRIVER_H

#include <ntddk.h>

VOID STDCALL my_unload(PDRIVER_OBJECT DriverObject);
NTSTATUS STDCALL unsuported_function(PDEVICE_OBJECT DeviceObject, PIRP Irp);

#endif // DRIVER_H