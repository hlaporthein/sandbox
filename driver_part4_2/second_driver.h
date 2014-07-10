#ifndef DRIVER_H
#define DRIVER_H

#define DbgPrint(format, ...) DbgPrint("[Second_Driver] "format, ##__VA_ARGS__)

typedef struct {
	PDEVICE_OBJECT next;
} device_extension_t;

VOID STDCALL my_unload(PDRIVER_OBJECT driverObject);
NTSTATUS STDCALL my_unsuported_function(PDEVICE_OBJECT deviceObject, PIRP Irp);
NTSTATUS STDCALL my_create(PDEVICE_OBJECT deviceObject, PIRP Irp);
NTSTATUS STDCALL my_close(PDEVICE_OBJECT deviceObject, PIRP Irp);
NTSTATUS STDCALL my_ioctl(PDEVICE_OBJECT deviceObject, PIRP Irp);

void try_attach_to_other_driver(PDRIVER_OBJECT driverObject);

NTSTATUS my_ioctl_say_hello(PIRP Irp, PDEVICE_OBJECT deviceObject);

NTSTATUS say_hello(PDEVICE_OBJECT deviceObject);

NTSTATUS STDCALL my_completion_routine(PDEVICE_OBJECT deviceObject, PIRP Irp, PVOID Context);

#endif // DRIVER_H