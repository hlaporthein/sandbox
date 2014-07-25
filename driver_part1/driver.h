#ifndef DRIVER_H
#define DRIVER_H

#include <ntddk.h>
#include <gseh.h>

#define TYPE_ALIGNMENT(type) offsetof(struct {char x; type t;}, t)

VOID STDCALL my_unload(PDRIVER_OBJECT DriverObject);
NTSTATUS STDCALL unsuported_function(PDEVICE_OBJECT DeviceObject, PIRP Irp);
NTSTATUS STDCALL my_create(PDEVICE_OBJECT DeviceObject, PIRP Irp);
NTSTATUS STDCALL my_close(PDEVICE_OBJECT DeviceObject, PIRP Irp);
NTSTATUS STDCALL my_io_control(PDEVICE_OBJECT DeviceObject, PIRP Irp);

// The only difference between those functions is the access method to the data buffer
NTSTATUS STDCALL my_write_direct(PDEVICE_OBJECT DeviceObject, PIRP Irp);
NTSTATUS STDCALL my_write_buffered(PDEVICE_OBJECT DeviceObject, PIRP Irp);

NTSTATUS STDCALL my_read_direct(PDEVICE_OBJECT DeviceObject, PIRP Irp);
NTSTATUS STDCALL my_read_buffered(PDEVICE_OBJECT DeviceObject, PIRP Irp);

NTSTATUS STDCALL my_read_neither(PDEVICE_OBJECT DeviceObject, PIRP Irp);
NTSTATUS STDCALL my_write_neither(PDEVICE_OBJECT DeviceObject, PIRP Irp);

BOOLEAN isStrNullTerminated(PCHAR pString, UINT uiLength);

#if defined(__USE_DIRECT__)

#define IO_TYPE DO_DIRECT_IO
#define USE_WRITE_FUNCTION my_write_direct
#define USE_READ_FUNCTION my_read_direct

#elseif defined(__USE_BUFFERED__)

#define IO_TYPE DO_BUFFERED_IO
#define USE_WRITE_FUNCTION  my_write_buffered
#define USE_READ_FUNCTION  my_read_buffered

#else

#define IO_TYPE 0
#define USE_WRITE_FUNCTION  my_write_neither
#define USE_READ_FUNCTION  my_read_neither

#endif // __USE_DIRECT__ || __USE_BUFFERED__

#endif // DRIVER_H