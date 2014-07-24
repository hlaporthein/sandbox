#include <ntddk.h>

void DriverUnload(PDRIVER_OBJECT pDriverObject) {
	DbgPrint("Driver unloading\n");
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
	DriverObject->DriverUnload = DriverUnload;
	DbgPrint("Hello, World\n");
	return STATUS_SUCCESS;
}