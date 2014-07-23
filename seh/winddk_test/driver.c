#include <ntddk.h>

VOID my_unload(PDRIVER_OBJECT DriverObject);

NTSTATUS DriverEntry(PDRIVER_OBJECT driverObject, PUNICODE_STRING registryPath) {
	NTSTATUS status = STATUS_SUCCESS;

	driverObject->DriverUnload = my_unload;

	DbgPrint("Hello!!\n");

	__try {
		DbgPrint("inside __try\n");
	} __except(EXCEPTION_EXECUTE_HANDLER) {
		DbgPrint("Error\n");
	}

	__try {
		DbgPrint("About to probe\n");
		ProbeForWrite((PVOID) 0, 10, 4);
		DbgPrint("Everything ok\n");
	} __except(EXCEPTION_EXECUTE_HANDLER) {
		DbgPrint("Error\n");
	}

cleanup:
	return status;
}

VOID my_unload(PDRIVER_OBJECT DriverObject) {
	DbgPrint("GoodBye!!\n");
}