#include <ntddk.h>

#ifdef __MINGW32__
#else
#define STDCALL
#endif // _MINGW

void STDCALL DriverUnload(PDRIVER_OBJECT pDriverObject) {
	DbgPrint("Driver unloading\n");
}

NTSTATUS STDCALL DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
	DriverObject->DriverUnload = DriverUnload;
	DbgPrint("Hello, World\n");
	return STATUS_SUCCESS;
}