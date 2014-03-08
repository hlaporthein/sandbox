#include <stdio.h>
#include <windows.h>

#define N 512

void manage_error(const char *message) {
	printf("Error while doing %s.\n", message);
	DWORD code = GetLastError();

	LPVOID lpMsgBuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		code,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0, NULL );
	printf("Error: Image not loaded. Error %d = %s\n", code, lpMsgBuf);
	LocalFree(lpMsgBuf);
	ExitProcess(code);
}

#define _IMAGEHLPAPI DECLSPEC_IMPORT WINAPI
typedef struct _LOADED_IMAGE {
	LPSTR ModuleName;
	HANDLE hFile;
	PUCHAR MappedAddress;
	PIMAGE_NT_HEADERS FileHeader;
	PIMAGE_SECTION_HEADER LastRvaSection;
	ULONG NumberOfSections;
	PIMAGE_SECTION_HEADER Sections;
	ULONG Characteristics;
	BOOLEAN fSystemImage;
	BOOLEAN fDOSImage;
	LIST_ENTRY Links;
	ULONG SizeOfImage;
} LOADED_IMAGE,*PLOADED_IMAGE;
PLOADED_IMAGE _IMAGEHLPAPI ImageLoad(LPSTR,LPSTR);
BOOL _IMAGEHLPAPI ImageUnload(PLOADED_IMAGE);
typedef struct {
  DWORD     Size;
  DWORD     TimeDateStamp;
  WORD      MajorVersion;
  WORD      MinorVersion;
  DWORD     GlobalFlagsClear;
  DWORD     GlobalFlagsSet;
  DWORD     CriticalSectionDefaultTimeout;
  ULONGLONG DeCommitFreeBlockThreshold;
  ULONGLONG DeCommitTotalFreeThreshold;
  ULONGLONG LockPrefixTable;
  ULONGLONG MaximumAllocationSize;
  ULONGLONG VirtualMemoryThreshold;
  ULONGLONG ProcessAffinityMask;
  DWORD     ProcessHeapFlags;
  WORD      CSDVersion;
  WORD      Reserved1;
  ULONGLONG EditList;
  ULONGLONG SecurityCookie;
  ULONGLONG SEHandlerTable;
  ULONGLONG SEHandlerCount;
} MY_IMAGE_LOAD_CONFIG_DIRECTORY64, *PMY_IMAGE_LOAD_CONFIG_DIRECTORY64;

BOOL _IMAGEHLPAPI GetImageConfigInformation(PLOADED_IMAGE,PMY_IMAGE_LOAD_CONFIG_DIRECTORY64);
BOOL _IMAGEHLPAPI SetImageConfigInformation(PLOADED_IMAGE,PMY_IMAGE_LOAD_CONFIG_DIRECTORY64);

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Usage: %s <dll_or_exe>\n", argv[0]);
		return 1;
	}

	//open the file
	char drive[N];
	char dir[N];
	char name[N];
	char ext[N];
	_splitpath(argv[1], drive, dir, name, ext);
	char file[N];
	snprintf(file, N, "%s%s", name, ext);
	char path[N];
	snprintf(path, N, "%s%s", drive, dir);

	printf("file=%s\n", file);
	printf("path=%s\n", path);

	PLOADED_IMAGE ploaded_image = ImageLoad(file, path);
	if (ploaded_image == NULL) {
		manage_error("ImageLoad");
	}
	printf("Image loaded.\n");
	MY_IMAGE_LOAD_CONFIG_DIRECTORY64 c;
	PMY_IMAGE_LOAD_CONFIG_DIRECTORY64 p = &c;
	BOOL res = GetImageConfigInformation(ploaded_image, p);
	if (res == FALSE) {
		manage_error("GetImageConfigInformation");
	}
	printf("MajorVersion=%02x\n", c.MajorVersion);
	printf("MinorVersion=%02x\n", c.MinorVersion);

	res = ImageUnload(ploaded_image);
	if (res) {
		printf("Unload successfull.\n");
	} else {
		printf("Error while unloading.\n");
	}


	return 0;
}