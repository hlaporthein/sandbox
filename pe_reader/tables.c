#include <string.h>
#include <stdio.h>

#include "pe_reader.h"
#include "my_winnt.h"

typedef struct {
	int code;
	char *label;
	int section;
} record;

record hashmap[1024];
int map_counter = 0;

void set_record(int section, int code, char *label) {
	hashmap[map_counter].code = code;
	hashmap[map_counter].label = label;
	hashmap[map_counter].section = section;
	map_counter++;
}

void init_map() {
	set_record(SECTION_MACHINE, IMAGE_FILE_MACHINE_UNKNOWN	, "Machine unknown");
	set_record(SECTION_MACHINE, IMAGE_FILE_MACHINE_AM33		, "Machine i386");
	set_record(SECTION_MACHINE, IMAGE_FILE_MACHINE_AMD64	, "Machine amd64");
	set_record(SECTION_MACHINE, IMAGE_FILE_MACHINE_ARM		, "Machine arm");
	set_record(SECTION_MACHINE, IMAGE_FILE_MACHINE_EBC		, "Machine ebc");
	set_record(SECTION_MACHINE, IMAGE_FILE_MACHINE_I386		, "Machine i386");
	set_record(SECTION_MACHINE, IMAGE_FILE_MACHINE_IA64		, "Machine ia64");
	set_record(SECTION_MACHINE, IMAGE_FILE_MACHINE_M32R		, "Machine m32r");
	set_record(SECTION_MACHINE, IMAGE_FILE_MACHINE_MIPS16	, "Machine mips16");
	set_record(SECTION_MACHINE, IMAGE_FILE_MACHINE_MIPSFPU	, "Machine mipsfpu");
	set_record(SECTION_MACHINE, IMAGE_FILE_MACHINE_MIPSFPU16, "Machine mipsfpu16");
	set_record(SECTION_MACHINE, IMAGE_FILE_MACHINE_POWERPC	, "Machine powerpc");
	set_record(SECTION_MACHINE, IMAGE_FILE_MACHINE_POWERPCFP, "Machine powerpcfp");
	set_record(SECTION_MACHINE, IMAGE_FILE_MACHINE_R4000	, "Machine r4000");
	set_record(SECTION_MACHINE, IMAGE_FILE_MACHINE_SH3		, "Machine sh3");
	set_record(SECTION_MACHINE, IMAGE_FILE_MACHINE_SH3DSP	, "Machine sh3dsp");
	set_record(SECTION_MACHINE, IMAGE_FILE_MACHINE_SH4		, "Machine sh4");
	set_record(SECTION_MACHINE, IMAGE_FILE_MACHINE_SH5		, "Machine sh5");
	set_record(SECTION_MACHINE, IMAGE_FILE_MACHINE_THUMB	, "Machine thumb");
	set_record(SECTION_MACHINE, IMAGE_FILE_MACHINE_WCEMIPSV2, "Machine wcemipsv2");

	set_record(SECTION_CHARACTERISTICS, IMAGE_FILE_RELOCS_STRIPPED			, "Relocations stripped");
	set_record(SECTION_CHARACTERISTICS, IMAGE_FILE_EXECUTABLE_IMAGE			, "Executable image");
	set_record(SECTION_CHARACTERISTICS, IMAGE_FILE_LINE_NUMS_STRIPPED		, "Line numbers stripped (deprecated)");
	set_record(SECTION_CHARACTERISTICS, IMAGE_FILE_LOCAL_SYMS_STRIPPED		, "Local symbols stripped (deprecated)");
	set_record(SECTION_CHARACTERISTICS, IMAGE_FILE_AGGRESIVE_WS_TRIM 		, "Agressive working set trim (obsolete)");
	set_record(SECTION_CHARACTERISTICS, IMAGE_FILE_LARGE_ADDRESS_AWARE		, "Can handle large addresses");
	set_record(SECTION_CHARACTERISTICS, IMAGE_FILE_BYTES_REVERSED_LO		, "Little endian (deprecated)");
	set_record(SECTION_CHARACTERISTICS, IMAGE_FILE_32BIT_MACHINE			, "32bit");
	set_record(SECTION_CHARACTERISTICS, IMAGE_FILE_DEBUG_STRIPPED			, "Debug info stripped");
	set_record(SECTION_CHARACTERISTICS, IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP	, "Can run from SWAP - Removable media");
	set_record(SECTION_CHARACTERISTICS, IMAGE_FILE_NET_RUN_FROM_SWAP		, "Can run from SWAP - Network");
	set_record(SECTION_CHARACTERISTICS, IMAGE_FILE_SYSTEM					, "System file");
	set_record(SECTION_CHARACTERISTICS, IMAGE_FILE_DLL						, "DLL");
	set_record(SECTION_CHARACTERISTICS, IMAGE_FILE_UP_SYSTEM_ONLY			, "Only uniprocessor machine");
	set_record(SECTION_CHARACTERISTICS, IMAGE_FILE_BYTES_REVERSED_HI		, "Big endian (deprecated)");

	set_record(SECTION_MAGIC, IMAGE_NT_OPTIONAL_HDR32_MAGIC	, "PE32");
	set_record(SECTION_MAGIC, IMAGE_NT_OPTIONAL_HDR64_MAGIC	, "PE32+");

	set_record(SECTION_SUBSYSTEM, IMAGE_SUBSYSTEM_UNKNOWN					, "Unknown system");
	set_record(SECTION_SUBSYSTEM, IMAGE_SUBSYSTEM_NATIVE					, "Device driver/Native Windows process");
	set_record(SECTION_SUBSYSTEM, IMAGE_SUBSYSTEM_WINDOWS_GUI				, "Windows GUI");
	set_record(SECTION_SUBSYSTEM, IMAGE_SUBSYSTEM_WINDOWS_CUI				, "Windows CUI");
	set_record(SECTION_SUBSYSTEM, IMAGE_SUBSYSTEM_POSIX_CUI					, "Posix CUI");
	set_record(SECTION_SUBSYSTEM, IMAGE_SUBSYSTEM_WINDOWS_CE_GUI			, "Windows CE");
	set_record(SECTION_SUBSYSTEM, IMAGE_SUBSYSTEM_EFI_APPLICATION			, "EFI application");
	set_record(SECTION_SUBSYSTEM, IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER	, "EFI driver w/ boot service");
	set_record(SECTION_SUBSYSTEM, IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER		, "EFI driver w/ runtime service");
	set_record(SECTION_SUBSYSTEM, IMAGE_SUBSYSTEM_EFI_ROM					, "EFI ROM");
	set_record(SECTION_SUBSYSTEM, IMAGE_SUBSYSTEM_XBOX						, "XBOX");

	set_record(SECTION_DLL_CHARACTERISICS, IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE			, "Can be relocated at load time");
	set_record(SECTION_DLL_CHARACTERISICS, IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY			, "Code Integrity checks are enforced");
	set_record(SECTION_DLL_CHARACTERISICS, IMAGE_DLLCHARACTERISTICS_NX_COMPAT				, "Image is NX compatible");
	set_record(SECTION_DLL_CHARACTERISICS, IMAGE_DLLCHARACTERISTICS_NO_ISOLATION			, "Do not isolate the image");
	set_record(SECTION_DLL_CHARACTERISICS, IMAGE_DLLCHARACTERISTICS_NO_SEH					, "No SE handler called");
	set_record(SECTION_DLL_CHARACTERISICS, IMAGE_DLLCHARACTERISTICS_NO_BIND					, "Do not bind the image");
	set_record(SECTION_DLL_CHARACTERISICS, IMAGE_DLLCHARACTERISTICS_WDM_DRIVER				, "WDM driver");
	set_record(SECTION_DLL_CHARACTERISICS, IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE	, "Terminal Server aware");

	set_record(SECTION_DATA_DIRECTORY, IMAGE_DIRECTORY_ENTRY_EXPORT			, "Export Table");
	set_record(SECTION_DATA_DIRECTORY, IMAGE_DIRECTORY_ENTRY_IMPORT			, "Import Table");
	set_record(SECTION_DATA_DIRECTORY, IMAGE_DIRECTORY_ENTRY_RESOURCE		, "Resource Table");
	set_record(SECTION_DATA_DIRECTORY, IMAGE_DIRECTORY_ENTRY_EXCEPTION		, "Exception Table");
	set_record(SECTION_DATA_DIRECTORY, IMAGE_DIRECTORY_ENTRY_SECURITY		, "Certificate Table");
	set_record(SECTION_DATA_DIRECTORY, IMAGE_DIRECTORY_ENTRY_BASERELOC		, "Base Relocation Table");
	set_record(SECTION_DATA_DIRECTORY, IMAGE_DIRECTORY_ENTRY_DEBUG			, "Debug");
	set_record(SECTION_DATA_DIRECTORY, IMAGE_DIRECTORY_ENTRY_ARCHITECTURE	, "Architecture");
	set_record(SECTION_DATA_DIRECTORY, IMAGE_DIRECTORY_ENTRY_GLOBALPTR		, "Global Ptr");
	set_record(SECTION_DATA_DIRECTORY, IMAGE_DIRECTORY_ENTRY_TLS			, "TLS Table");
	set_record(SECTION_DATA_DIRECTORY, IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG	, "Load Config Table");
	set_record(SECTION_DATA_DIRECTORY, IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT	, "Bound Import");
	set_record(SECTION_DATA_DIRECTORY, IMAGE_DIRECTORY_ENTRY_IAT			, "IAT");
	set_record(SECTION_DATA_DIRECTORY, IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT	, "Delay Import Descriptor");
	set_record(SECTION_DATA_DIRECTORY, IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR	, "CLR Runtime Header");
	set_record(SECTION_DATA_DIRECTORY, IMAGE_DIRECTORY_ENTRY_RESERVED		, "Reserved, must be zero");

//	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_TYPE_REG				, "Reserved for future use");
//	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_TYPE_DSECT				, "Reserved for future use");
//	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_TYPE_NOLOAD			, "Reserved for future use");
//	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_TYPE_GROUP				, "Reserved for future use");
	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_TYPE_NO_PAD			, "Not padded (obsolete)");
//	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_TYPE_COPY				, "Reserved for future use");
	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_CNT_CODE				, "Contains executable code");
	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_CNT_INITIALIZED_DATA	, "Contains initialized data");
	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_CNT_UNINITIALIZED_DATA, "Contains uninitialized data");
//	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_LNK_OTHER				, "Reserved for future use");
	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_LNK_INFO				, "Contains comments or other info");
//	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_TYPE_OVER				, "Reserved for future use");
	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_LNK_REMOVE			, "Will not be part of the image");
	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_LNK_COMDAT			, "Contains COMDAT data");
	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_GPREL					, "Contains data referenced throught Global Pointer");
//	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_MEM_FARDATA			, "Reserved for future use");
//	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_MEM_PURGEABLE			, "Reserved for future use");
	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_MEM_16BIT				, "ARM: Contains Thumb code /Others: Reserved for future use");
//	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_MEM_LOCKED				, "Reserved for future use");
//	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_MEM_PRELOAD			, "Reserved for future use");
	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_ALIGN_1BYTES			, "Align data 1-byte boundary (object)");
	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_ALIGN_2BYTES			, "Align data 2-byte boundary (object)");
	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_ALIGN_4BYTES			, "Align data 4-byte boundary (object)");
	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_ALIGN_8BYTES			, "Align data 8-byte boundary (object)");
	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_ALIGN_16BYTES			, "Align data 16-byte boundary (object)");
	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_ALIGN_32BYTES			, "Align data 32-byte boundary (object)");
	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_ALIGN_64BYTES			, "Align data 64-byte boundary (object)");
	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_ALIGN_128BYTES		, "Align data 128-byte boundary (object)");
	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_ALIGN_256BYTES		, "Align data 256-byte boundary (object)");
	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_ALIGN_512BYTES		, "Align data 512-byte boundary (object)");
	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_ALIGN_1024BYTES		, "Align data 1024-byte boundary (object)");
	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_ALIGN_2048BYTES		, "Align data 2048-byte boundary (object)");
	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_ALIGN_4096BYTES		, "Align data 4096-byte boundary (object)");
	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_ALIGN_8192BYTES		, "Align data 8192-byte boundary (object)");
	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_LNK_NRELOC_OVFL		, "Contains extented relocation");
	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_MEM_DISCARDABLE		, "Can be discarded");
	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_MEM_NOT_CACHED		, "Cannot be cached");
	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_MEM_NOT_PAGED			, "Not pageable");
	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_MEM_SHARED			, "Can be shared in memory");
	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_MEM_EXECUTE			, "Can be executed as code");
	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_MEM_READ				, "Readable");
	set_record(SECTION_SECTION_CHARACTERISTICS, IMAGE_SCN_MEM_WRITE				, "Writable");
}

char* map(int section, int code) {
	for (int i = 0; i < map_counter; i++) {
		if (hashmap[i].section == section && hashmap[i].code == code) {
			return hashmap[i].label;
		}
	}
	return "Label to be defined.";
}

char* get_flags(char *buffer, size_t size, int section, int flags) {
	snprintf(buffer, size, "");
	for (int i = 0; i < map_counter; i++) {
		if (hashmap[i].section == section) {
			if (hashmap[i].code & flags) {
				char buf[1024];
				snprintf(buf, 1024, " <%s>", hashmap[i].label);
				strcat(buffer, buf);
			}
		}
	}
	if (strcmp(buffer, "") == 0) {
		snprintf(buffer, size, "No flag");
	}
	return buffer;
}

char* list_flags(char *buffer, size_t size, int section, int flags) {
	snprintf(buffer, size, "\n");
	for (int i = 0; i < map_counter; i++) {
		if (hashmap[i].section == section) {
			char buf[1024];
			char *value = "Off";
			if (hashmap[i].code & flags) {
				value = "On";
			}
			snprintf(buf, 1024, "\t%s: %s\n", hashmap[i].label, value);
			strlcat(buffer, buf, BUFFER_SIZE);
		}
	}
	return buffer;
}

int has_flags(int flag, int flags) {
	return flag & flags;
}