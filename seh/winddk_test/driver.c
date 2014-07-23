#include <ntddk.h>

VOID my_unload(PDRIVER_OBJECT DriverObject);

NTSTATUS DriverEntry(PDRIVER_OBJECT driverObject, PUNICODE_STRING registryPath) {
	NTSTATUS status = STATUS_SUCCESS;

	driverObject->DriverUnload = my_unload;

	DbgPrint("Hello!!\n");

cleanup:
	return status;
}

VOID my_unload(PDRIVER_OBJECT DriverObject) {
	DbgPrint("GoodBye!!\n");
}