#include <ntddk.h>

VOID unload(PDRIVER_OBJECT  DriverObject);
NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath);

#pragma alloc_text(INIT, DriverEntry)
#pragma alloc_text(PAGE, unload)

VOID unload(PDRIVER_OBJECT  DriverObject) {
	DbgPrint("GoodBye\n");
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
	DbgPrint("Hello, World\n");
	DriverObject->DriverUnload =  unload;

	return STATUS_SUCCESS;
}