#include <ntddk.h>

static VOID STDCALL
my_unload(PDRIVER_OBJECT DriverObject) {
	ANSI_STRING SymbolicLinkNameA;
	UNICODE_STRING SymbolicLinkNameW;

	DbgPrint("DriverUnload called\r\n");  // catch this using DBGVIEW from www.sysinternals.com

	// this does a BSOTD because the deviceobject was not instantiated.
	// IoDeleteDevice(DriverObject->DeviceObject);
}

NTSTATUS STDCALL DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	DbgPrint("Hello, World\n");
	DbgPrint("RegistryPath=%wZ\n", RegistryPath);
	// support for service stopping
	DriverObject->DriverUnload = my_unload;

	return STATUS_SUCCESS;
}