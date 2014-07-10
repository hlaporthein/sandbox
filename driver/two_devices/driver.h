#ifndef DRIVER_H
#define DRIVER_H

VOID STDCALL my_unload(PDRIVER_OBJECT driverObject);
NTSTATUS STDCALL my_unsuported_function(PDEVICE_OBJECT deviceObject, PIRP Irp);
NTSTATUS STDCALL my_create(PDEVICE_OBJECT deviceObject, PIRP Irp);
NTSTATUS STDCALL my_close(PDEVICE_OBJECT deviceObject, PIRP Irp);

NTSTATUS create_device(PDRIVER_OBJECT driverObject, PCWSTR name, PCWSTR dosName);
#endif // DRIVER_H