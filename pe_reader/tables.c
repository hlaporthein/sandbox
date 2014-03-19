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

int has_flags(int flag, int flags) {
	return flag & flags;
}