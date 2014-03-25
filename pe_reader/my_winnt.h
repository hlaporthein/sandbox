#ifndef MY_WINNT_H
#define MY_WINNT_H

#define SECTION_MACHINE 0
#define SECTION_CHARACTERISTICS 1
#define SECTION_MAGIC 2
#define SECTION_SUBSYSTEM 3
#define SECTION_DLL_CHARACTERISICS 4
#define SECTION_DATA_DIRECTORY 5
#define SECTION_SECTION_CHARACTERISTICS 6
#define SECTION_RELOC 7

// Flags for imgage's characteristics
#define IMAGE_FILE_RELOCS_STRIPPED			1
#define IMAGE_FILE_EXECUTABLE_IMAGE			2
#define IMAGE_FILE_LINE_NUMS_STRIPPED		4
#define IMAGE_FILE_LOCAL_SYMS_STRIPPED		8
#define IMAGE_FILE_AGGRESIVE_WS_TRIM 		16
#define IMAGE_FILE_LARGE_ADDRESS_AWARE		32
#define IMAGE_FILE_BYTES_REVERSED_LO		128
#define IMAGE_FILE_32BIT_MACHINE			256
#define IMAGE_FILE_DEBUG_STRIPPED			512
#define IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP	1024
#define IMAGE_FILE_NET_RUN_FROM_SWAP		2048
#define IMAGE_FILE_SYSTEM					4096
#define IMAGE_FILE_DLL						8192
#define IMAGE_FILE_UP_SYSTEM_ONLY			16384
#define IMAGE_FILE_BYTES_REVERSED_HI		32768

// Flags for DLL's characteristics
#define IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE			0x0040
#define IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY		0x0080
#define IMAGE_DLLCHARACTERISTICS_NX_COMPAT				0x0100
#define IMAGE_DLLCHARACTERISTICS_NO_ISOLATION			0x0200
#define IMAGE_DLLCHARACTERISTICS_NO_SEH					0x0400
#define IMAGE_DLLCHARACTERISTICS_NO_BIND				0x0800
#define IMAGE_DLLCHARACTERISTICS_WDM_DRIVER				0x2000
#define IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE	0x8000

// Flags for Section's characteristics
#define IMAGE_SCN_TYPE_REG					0
#define IMAGE_SCN_TYPE_DSECT				1
#define IMAGE_SCN_TYPE_NOLOAD				2
#define IMAGE_SCN_TYPE_GROUP				4
#define IMAGE_SCN_TYPE_NO_PAD				8
#define IMAGE_SCN_TYPE_COPY					16
#define IMAGE_SCN_CNT_CODE					32
#define IMAGE_SCN_CNT_INITIALIZED_DATA		64
#define IMAGE_SCN_CNT_UNINITIALIZED_DATA	128
#define IMAGE_SCN_LNK_OTHER					256
#define IMAGE_SCN_LNK_INFO					512
#define IMAGE_SCN_TYPE_OVER					1024
#define IMAGE_SCN_LNK_REMOVE				2048
#define IMAGE_SCN_LNK_COMDAT				4096
#define IMAGE_SCN_GPREL						0x8000 /*  Valid only for IA64 */
#define IMAGE_SCN_MEM_FARDATA				0x8000 /* Not in PECOFF v8 spec */
#define IMAGE_SCN_MEM_PURGEABLE				0x20000
#define IMAGE_SCN_MEM_16BIT					0x20000
#define IMAGE_SCN_MEM_LOCKED				0x40000
#define IMAGE_SCN_MEM_PRELOAD				0x80000
#define IMAGE_SCN_ALIGN_1BYTES				0x100000
#define IMAGE_SCN_ALIGN_2BYTES				0x200000
#define IMAGE_SCN_ALIGN_4BYTES				0x300000
#define IMAGE_SCN_ALIGN_8BYTES				0x400000
#define IMAGE_SCN_ALIGN_16BYTES				0x500000
#define IMAGE_SCN_ALIGN_32BYTES				0x600000
#define IMAGE_SCN_ALIGN_64BYTES				0x700000
#define IMAGE_SCN_ALIGN_128BYTES			0x800000
#define IMAGE_SCN_ALIGN_256BYTES			0x900000
#define IMAGE_SCN_ALIGN_512BYTES			0xa00000
#define IMAGE_SCN_ALIGN_1024BYTES			0xb00000
#define IMAGE_SCN_ALIGN_2048BYTES			0xc00000
#define IMAGE_SCN_ALIGN_4096BYTES			0xd00000
#define IMAGE_SCN_ALIGN_8192BYTES			0xe00000
#define IMAGE_SCN_LNK_NRELOC_OVFL			0x1000000
#define IMAGE_SCN_MEM_DISCARDABLE			0x2000000
#define IMAGE_SCN_MEM_NOT_CACHED			0x4000000
#define IMAGE_SCN_MEM_NOT_PAGED				0x8000000
#define IMAGE_SCN_MEM_SHARED				0x10000000
#define IMAGE_SCN_MEM_EXECUTE				0x20000000
#define IMAGE_SCN_MEM_READ					0x40000000
#define IMAGE_SCN_MEM_WRITE					0x80000000

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

#define IMAGE_SUBSYSTEM_UNKNOWN					0
#define IMAGE_SUBSYSTEM_NATIVE					1
#define IMAGE_SUBSYSTEM_WINDOWS_GUI				2
#define IMAGE_SUBSYSTEM_WINDOWS_CUI				3
#define IMAGE_SUBSYSTEM_POSIX_CUI				7
#define IMAGE_SUBSYSTEM_WINDOWS_CE_GUI			9
#define IMAGE_SUBSYSTEM_EFI_APPLICATION			10
#define IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER	11
#define IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER		12
#define IMAGE_SUBSYSTEM_EFI_ROM					13
#define IMAGE_SUBSYSTEM_XBOX					14

#define IMAGE_DIRECTORY_ENTRY_EXPORT			0
#define IMAGE_DIRECTORY_ENTRY_IMPORT			1
#define IMAGE_DIRECTORY_ENTRY_RESOURCE			2
#define IMAGE_DIRECTORY_ENTRY_EXCEPTION			3
#define IMAGE_DIRECTORY_ENTRY_SECURITY			4
#define IMAGE_DIRECTORY_ENTRY_BASERELOC			5
#define IMAGE_DIRECTORY_ENTRY_DEBUG				6
//#define IMAGE_DIRECTORY_ENTRY_COPYRIGHT		7
#define IMAGE_DIRECTORY_ENTRY_ARCHITECTURE		7
#define IMAGE_DIRECTORY_ENTRY_GLOBALPTR			8
#define IMAGE_DIRECTORY_ENTRY_TLS				9
#define IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG		10
#define IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT		11
#define IMAGE_DIRECTORY_ENTRY_IAT				12
#define IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT		13
#define IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR	14
#define IMAGE_DIRECTORY_ENTRY_RESERVED			15

#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES 16
#define IMAGE_SIZEOF_SHORT_NAME 8

#define IMAGE_ORDINAL_FLAG32 0x80000000
#define IMAGE_ORDINAL_FLAG64 0x8000000000000000ULL

#define IMAGE_REL_BASED_ABSOLUTE		0
#define IMAGE_REL_BASED_HIGH			1
#define IMAGE_REL_BASED_LOW				2
#define IMAGE_REL_BASED_HIGHLOW			3
#define IMAGE_REL_BASED_HIGHADJ			4
#define IMAGE_REL_BASED_MIPS_JMPADDR	5

//typedef struct { unsigned char a[2]; } WORD;
//typedef struct { unsigned char a[4]; } DWORD;
//typedef struct { unsigned char a[4]; } LONG;
//typedef struct { unsigned char a[1]; } BYTE;
//typedef struct { unsigned char a[8]; } ULONGLONG,*PULONGLONG;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned int LONG;
typedef unsigned char BYTE;
typedef unsigned long long ULONGLONG, *PULONGLONG;

typedef DWORD rva_t;

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

typedef struct _IMAGE_DATA_DIRECTORY {
	DWORD VirtualAddress;
	DWORD Size;
} IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;

typedef struct _IMAGE_OPTIONAL_HEADER32 {
	OPTIONAL_HEADER_STANDARD standard;
	OPTIONAL_HEADER_WINSPEC32 winspec;
	IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER32, *PIMAGE_OPTIONAL_HEADER32;

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


typedef struct _IMAGE_SECTION_HEADER {
	BYTE Name[IMAGE_SIZEOF_SHORT_NAME];
	union {
		DWORD PhysicalAddress;
		DWORD VirtualSize;
	} Misc;
	DWORD VirtualAddress;
	DWORD SizeOfRawData;
	DWORD PointerToRawData;
	DWORD PointerToRelocations;
	DWORD PointerToLinenumbers;
	WORD NumberOfRelocations;
	WORD NumberOfLinenumbers;
	DWORD Characteristics;
} IMAGE_SECTION_HEADER, *PIMAGE_SECTION_HEADER;

typedef struct _IMAGE_IMPORT_DESCRIPTOR {
	union {
		DWORD Characteristics;
		DWORD OriginalFirstThunk;
	} DUMMYUNIONNAME;
	DWORD TimeDateStamp;
	DWORD ForwarderChain;
	DWORD Name;
	DWORD FirstThunk;
} IMAGE_IMPORT_DESCRIPTOR, *PIMAGE_IMPORT_DESCRIPTOR;

typedef struct _IMAGE_EXPORT_DIRECTORY {
	DWORD	Characteristics;
	DWORD	TimeDateStamp;
	WORD	MajorVersion;
	WORD	MinorVersion;
	DWORD	Name;
	DWORD	Base;
	DWORD	NumberOfFunctions;
	DWORD	NumberOfNames;
	DWORD	AddressOfFunctions;
	DWORD	AddressOfNames;
	DWORD	AddressOfNameOrdinals;
} IMAGE_EXPORT_DIRECTORY, *PIMAGE_EXPORT_DIRECTORY;

typedef struct _IMAGE_THUNK_DATA32 {
	union {
		DWORD ForwarderString;
		DWORD Function;
		DWORD Ordinal;
		DWORD AddressOfData;
	} u1;
} IMAGE_THUNK_DATA32, *PIMAGE_THUNK_DATA32;

typedef struct _IMAGE_BASE_RELOCATION {
	DWORD VirtualAddress;
	DWORD SizeOfBlock;
} IMAGE_BASE_RELOCATION, *PIMAGE_BASE_RELOCATION;

typedef struct _IMAGE_IMPORT_BY_NAME {
	WORD Hint;
	BYTE Name[1];
} IMAGE_IMPORT_BY_NAME, *PIMAGE_IMPORT_BY_NAME;

typedef struct {
	int is_32bit;
	FILE *fd;
	rva_t idata_rva;
	rva_t edata_rva;
	rva_t reloc_rva;
	rva_t iat_rva;
	IMAGE_DOS_HEADER dos_header;
	IMAGE_NT_HEADERS header;
	PIMAGE_SECTION_HEADER section_table;
	IMAGE_EXPORT_DIRECTORY export_directory_table;
	PIMAGE_THUNK_DATA32 export_address_table;
	DWORD *export_name_pointer_table;
	WORD *export_ordinal_table;
} PE_FILE;

#endif // MY_WINNT_H