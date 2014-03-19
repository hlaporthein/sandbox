#ifndef MY_WINNT_H
#define MY_WINNT_H

#define SECTION_MACHINE 0

// Possible values for IMAGE_FILE_HEADER.Machine
#define IMAGE_FILE_MACHINE_UNKNOWN		0x0000
#define IMAGE_FILE_MACHINE_AM33			0x01d3 /* Matsushita AM33 */
#define IMAGE_FILE_MACHINE_AMD64		0x8664 /* x64 */
#define IMAGE_FILE_MACHINE_ARM			0x01c0 /* ARM little endian */
#define IMAGE_FILE_MACHINE_EBC			0x0ebc /* EFI byte code */
#define IMAGE_FILE_MACHINE_I386			0x014c /* Intel 386 or later processors and compatible processors */
#define IMAGE_FILE_MACHINE_IA64			0x0200 /* Intel Itanium processor family */
#define IMAGE_FILE_MACHINE_M32R			0x9041 /* Mitsubishi M32R little endian */
#define IMAGE_FILE_MACHINE_MIPS16		0x0266 /* MIPS16 */
#define IMAGE_FILE_MACHINE_MIPSFPU		0x0366 /* MIPS with FPU */
#define IMAGE_FILE_MACHINE_MIPSFPU16	0x0466 /* MIPS16 with FPU */
#define IMAGE_FILE_MACHINE_POWERPC		0x01f0 /* Power PC little endian */
#define IMAGE_FILE_MACHINE_POWERPCFP	0x01f1 /* Power PC with floating point support */
#define IMAGE_FILE_MACHINE_R4000		0x0166 /* MIPS little endian */
#define IMAGE_FILE_MACHINE_SH3			0x01a2 /* Hitachi SH3 */
#define IMAGE_FILE_MACHINE_SH3DSP		0x01a3 /* Hitachi SH3 DSP */
#define IMAGE_FILE_MACHINE_SH4			0x01a6 /* Hitachi SH4 */
#define IMAGE_FILE_MACHINE_SH5			0x01a8 /* Hitachi SH5 */
#define IMAGE_FILE_MACHINE_THUMB		0x01c2 /* Thumb */
#define IMAGE_FILE_MACHINE_WCEMIPSV2	0x0169 /* MIPS little-endian WCE v2 */

#define IMAGE_DOS_SIGNATURE				0x5A4D
#define IMAGE_OS2_SIGNATURE				0x454E
#define IMAGE_OS2_SIGNATURE_LE			0x454C
#define IMAGE_VXD_SIGNATURE				0x454C
#define IMAGE_NT_SIGNATURE				0x00004550
#define IMAGE_NT_OPTIONAL_HDR32_MAGIC	0x10b
#define IMAGE_NT_OPTIONAL_HDR64_MAGIC	0x20b

#define BUFFER_SIZE 1<<16
#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES 16

//typedef struct { unsigned char a[2]; } WORD;
//typedef struct { unsigned char a[4]; } DWORD;
//typedef struct { unsigned char a[4]; } LONG;
//typedef struct { unsigned char a[1]; } BYTE;
//typedef struct { unsigned char a[8]; } ULONGLONG,*PULONGLONG;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned int LONG;
typedef unsigned char BYTE;
typedef unsigned long long ULONGLONG,*PULONGLONG;

typedef struct _IMAGE_DOS_HEADER {    // DOS .EXE header
	WORD e_magic;                     // Magic number
	WORD e_cblp;                      // Bytes on last page of file
	WORD e_cp;                        // Pages in file
	WORD e_crlc;                      // Relocations
	WORD e_cparhdr;                   // Size of header in paragraphs
	WORD e_minalloc;                  // Minimum extra paragraphs needed
	WORD e_maxalloc;                  // Maximum extra paragraphs needed
	WORD e_ss;                        // Initial (relative) SS value
	WORD e_sp;                        // Initial SP value
	WORD e_csum;                      // Checksum
	WORD e_ip;                        // Initial IP value
	WORD e_cs;                        // Initial (relative) CS value
	WORD e_lfarlc;                    // File address of relocation table
	WORD e_ovno;                      // Overlay number
	WORD e_res[4];                    // Reserved words
	WORD e_oemid;                     // OEM identifier (for e_oeminfo)
	WORD e_oeminfo;                   // OEM information; e_oemid specific
	WORD e_res2[10];                  // Reserved words
	LONG e_lfanew;                    // File address of new exe header
} IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;

typedef struct _IMAGE_FILE_HEADER {
	WORD  Machine;
	WORD  NumberOfSections;
	DWORD TimeDateStamp;
	DWORD PointerToSymbolTable;
	DWORD NumberOfSymbols;
	WORD  SizeOfOptionalHeader;
	WORD  Characteristics;
} IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;

typedef struct _IMAGE_DATA_DIRECTORY {
	DWORD VirtualAddress;
	DWORD Size;
} IMAGE_DATA_DIRECTORY,*PIMAGE_DATA_DIRECTORY;

typedef struct _OPTIONAL_HEADER_STANDARD {
	WORD                 Magic;
	BYTE                 MajorLinkerVersion;
	BYTE                 MinorLinkerVersion;
	DWORD                SizeOfCode;
	DWORD                SizeOfInitializedData;
	DWORD                SizeOfUninitializedData;
	DWORD                AddressOfEntryPoint;
	DWORD                BaseOfCode;
} OPTIONAL_HEADER_STANDARD, *POPTIONAL_HEADER_STANDARD;

typedef struct _OPTIONAL_HEADER_WINSPEC32 {
	DWORD                BaseOfData;
	DWORD                ImageBase;
	DWORD                SectionAlignment;
	DWORD                FileAlignment;
	WORD                 MajorOperatingSystemVersion;
	WORD                 MinorOperatingSystemVersion;
	WORD                 MajorImageVersion;
	WORD                 MinorImageVersion;
	WORD                 MajorSubsystemVersion;
	WORD                 MinorSubsystemVersion;
	DWORD                Win32VersionValue;
	DWORD                SizeOfImage;
	DWORD                SizeOfHeaders;
	DWORD                CheckSum;
	WORD                 Subsystem;
	WORD                 DllCharacteristics;
	DWORD                SizeOfStackReserve;
	DWORD                SizeOfStackCommit;
	DWORD                SizeOfHeapReserve;
	DWORD                SizeOfHeapCommit;
	DWORD                LoaderFlags;
	DWORD                NumberOfRvaAndSizes;
} OPTIONAL_HEADER_WINSPEC32, *POPTIONAL_HEADER_WINSPEC32;

typedef struct _IMAGE_OPTIONAL_HEADER32 {
	OPTIONAL_HEADER_STANDARD standard;
	OPTIONAL_HEADER_WINSPEC32 winspec;
	IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER32, *PIMAGE_OPTIONAL_HEADER32;

typedef struct _OPTIONAL_HEADER_WINSPEC64 {
	ULONGLONG   ImageBase;
	DWORD       SectionAlignment;
	DWORD       FileAlignment;
	WORD        MajorOperatingSystemVersion;
	WORD        MinorOperatingSystemVersion;
	WORD        MajorImageVersion;
	WORD        MinorImageVersion;
	WORD        MajorSubsystemVersion;
	WORD        MinorSubsystemVersion;
	DWORD       Win32VersionValue;
	DWORD       SizeOfImage;
	DWORD       SizeOfHeaders;
	DWORD       CheckSum;
	WORD        Subsystem;
	WORD        DllCharacteristics;
	ULONGLONG   SizeOfStackReserve;
	ULONGLONG   SizeOfStackCommit;
	ULONGLONG   SizeOfHeapReserve;
	ULONGLONG   SizeOfHeapCommit;
	DWORD       LoaderFlags;
	DWORD       NumberOfRvaAndSizes;
} OPTIONAL_HEADER_WINSPEC64, *POPTIONAL_HEADER_WINSPEC64;

typedef struct _IMAGE_OPTIONAL_HEADER64 {
	OPTIONAL_HEADER_STANDARD standard;
	OPTIONAL_HEADER_WINSPEC64 winspec;
	IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER64, *PIMAGE_OPTIONAL_HEADER64;

typedef union _IMAGE_OPTIONAL_HEADER {
	IMAGE_OPTIONAL_HEADER32 oh32;
	IMAGE_OPTIONAL_HEADER64 oh64;
} IMAGE_OPTIONAL_HEADER, *PIMAGE_OPTIONAL_HEADER;

typedef struct _IMAGE_NT_HEADERS {
	DWORD                 Signature;
	IMAGE_FILE_HEADER     FileHeader;
	IMAGE_OPTIONAL_HEADER OptionalHeader;
} IMAGE_NT_HEADERS, *PIMAGE_NT_HEADERS;

#endif // MY_WINNT_H