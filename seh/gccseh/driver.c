#include "gseh.h"
#include "driver.h"

PDRIVER_OBJECT p = NULL;
char *str = "Hello";

NTSTATUS STDCALL DriverEntry(PDRIVER_OBJECT driverObject, PUNICODE_STRING registryPath) {
	NTSTATUS status = STATUS_SUCCESS;



	DbgPrint("Hello!!\n");
	DbgPrint("str=%s\n", str);
	__gseh_try {
		DbgPrint("Success\n");
	} __gseh_except {
		DbgPrint("Exception catched.\n");
	} __gseh_end_except;

	p = driverObject;

	p->DriverUnload = my_unload;

//	__gseh_try {
//		DbgPrint("About to probe\n");
//		ProbeForWrite((PVOID) 0, 10, 4);
//		DbgPrint("Everything ok\n");
//	} __gseh_except {
//		DbgPrint("Exception catched.\n");
//	} __gseh_end_except;
//	DbgPrint("RegistryPath=%wZ\n", registryPath);

cleanup:
	return status;
}

VOID STDCALL my_unload(PDRIVER_OBJECT DriverObject) {
	DbgPrint("GoodBye!!\n");
	DbgPrint("str=%s\n", str);
}