#include "drvseh.h"
#include "driver.h"

PDRIVER_OBJECT p = NULL;

NTSTATUS STDCALL DriverEntry(PDRIVER_OBJECT driverObject, PUNICODE_STRING registryPath) {
	NTSTATUS status = STATUS_SUCCESS;



	DbgPrint("Hello!!\n");
	__drv_try {
		DbgPrint("Success\n");
	} __drv_except {
		DbgPrint("Exception catched.\n");
	} __drv_end_except;

	p = driverObject;

	p->DriverUnload = my_unload;

	__drv_try {
		DbgPrint("About to probe\n");
		ProbeForWrite((PVOID) 0, 10, 4);
		DbgPrint("Everything ok\n");
	} __drv_except {
		DbgPrint("Exception catched.\n");
	} __drv_end_except;
	DbgPrint("RegistryPath=%wZ\n", registryPath);

cleanup:
	return status;
}

VOID STDCALL my_unload(PDRIVER_OBJECT DriverObject) {
	DbgPrint("GoodBye!!\n");
}