#include <ntddk.h>
#include "public.h"
#include "driver.h"

NTSTATUS STDCALL DriverEntry(PDRIVER_OBJECT driverObject, PUNICODE_STRING registryPath) {
	NTSTATUS status = STATUS_SUCCESS;

	DbgPrint("Load\n");

	driverObject->DriverUnload = my_unload;

	for (int i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++) {
		driverObject->MajorFunction[i] = my_unsuported_function;
	}
	driverObject->MajorFunction[IRP_MJ_CREATE] = my_create;
	driverObject->MajorFunction[IRP_MJ_CLOSE] = my_close;

	// Creating first device
	status = create_device(driverObject, FIRST_DEVICE_PATH, FIRST_DOSDEVICE_PATH);
	if (status != STATUS_SUCCESS) {
		DbgPrint("Cannot create device\n");
		goto cleanup;
	}

	// Creating second device
	status = create_device(driverObject, SECOND_DEVICE_PATH, SECOND_DOSDEVICE_PATH);
	if (status != STATUS_SUCCESS) {
		DbgPrint("Cannot create device\n");
		goto cleanup;
	}

cleanup:
	if (status != STATUS_SUCCESS) {
		my_unload(driverObject);
	}
	return status;
}

NTSTATUS create_device(PDRIVER_OBJECT driverObject, PCWSTR name, PCWSTR dosName) {
	NTSTATUS status = STATUS_SUCCESS;
	PDEVICE_OBJECT deviceObject = NULL;

	UNICODE_STRING deviceName;
	RtlInitUnicodeString(&deviceName, name);
	status = IoCreateDevice(driverObject,
							0,
							&deviceName,
							FILE_DEVICE_UNKNOWN,
							FILE_DEVICE_SECURE_OPEN,
							FALSE,
							&deviceObject);
	if (status != STATUS_SUCCESS) {
		DbgPrint("Cannot create device\n");
		goto cleanup;
	}
	DbgPrint("%wZ: 0x%08X\n", &deviceName, deviceObject);

	deviceObject->Flags |= DO_BUFFERED_IO;
	deviceObject->Flags &= (~DO_DEVICE_INITIALIZING);

	UNICODE_STRING dosDeviceName;
	RtlInitUnicodeString(&dosDeviceName, dosName);
	IoCreateSymbolicLink(&dosDeviceName, &deviceName);

cleanup:
	if (status != STATUS_SUCCESS) {
		if (deviceObject) {
			IoDeleteDevice(deviceObject);
		}
	}
	return status;
}

VOID STDCALL my_unload(PDRIVER_OBJECT driverObject) {
	DbgPrint("Unload\n");

	// Removing first symlink
	UNICODE_STRING firstDosDeviceName;
	RtlInitUnicodeString(&firstDosDeviceName, FIRST_DOSDEVICE_PATH);
	IoDeleteSymbolicLink(&firstDosDeviceName);
	IoDeleteSymbolicLink(&firstDosDeviceName); // Can delete many times without BSOD

	// Removing second symlink
	UNICODE_STRING secondDosDeviceName;
	RtlInitUnicodeString(&secondDosDeviceName, SECOND_DOSDEVICE_PATH);
	IoDeleteSymbolicLink(&secondDosDeviceName);

	// Get the last created device
	PDEVICE_OBJECT device = driverObject->DeviceObject;
	while (device) {
		PDEVICE_OBJECT device_to_delete = device;
		// Next device is the previous created device
		device = device->NextDevice;
		IoDeleteDevice(device_to_delete);
	}
}

NTSTATUS STDCALL my_unsuported_function(PDEVICE_OBJECT deviceObject, PIRP Irp) {
	NTSTATUS status = STATUS_NOT_SUPPORTED;

	PIO_STACK_LOCATION pIoStackIrp = IoGetCurrentIrpStackLocation(Irp);
	if (!pIoStackIrp) {
		DbgPrint("Unsuported function called without Irp location\n");
		status = STATUS_UNSUCCESSFUL;
		goto cleanup;
	}
	DbgPrint("Unsuported function: 0x%02X\n", pIoStackIrp->MajorFunction);

cleanup:
	Irp->IoStatus.Status = status;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return status;
}

NTSTATUS STDCALL my_create(PDEVICE_OBJECT deviceObject, PIRP Irp) {
	NTSTATUS status = STATUS_SUCCESS;

	DbgPrint("my_create called for: 0x%08X\n", deviceObject);
	DbgPrint("Next device for 0x%08X: 0x%08X\n", deviceObject, deviceObject->NextDevice);
	DbgPrint("Attached device for 0x%08X: 0x%08X\n", deviceObject, deviceObject->AttachedDevice);

	Irp->IoStatus.Status = status;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return status;
}

NTSTATUS STDCALL my_close(PDEVICE_OBJECT deviceObject, PIRP Irp) {
	NTSTATUS status = STATUS_SUCCESS;

	DbgPrint("my_close called for: 0x%08X\n", deviceObject);
	DbgPrint("Next device for 0x%08X: 0x%08X\n", deviceObject, deviceObject->NextDevice);

	Irp->IoStatus.Status = status;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return status;
}