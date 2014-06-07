#include <ntddk.h>

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	DbgPrint("Hello, World\n");
	return STATUS_SUCCESS;
}