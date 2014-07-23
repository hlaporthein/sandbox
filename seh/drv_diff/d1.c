#include <ntddk.h>

void DriverUnload(PDRIVER_OBJECT pDriverObject) {
	DbgPrint("Driver unloading\n");
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
	DriverObject->DriverUnload = DriverUnload;
	__try {
		DbgPrint("Hello, World\n");
		ProbeForWrite((PVOID) 0, 10, 4);
	} __except(EXCEPTION_EXECUTE_HANDLER) {
		DbgPrint("Error\n");
	}
	return STATUS_SUCCESS;
}