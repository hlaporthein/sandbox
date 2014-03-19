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
}

char* map(int section, int code) {
	for (int i = 0; i < map_counter; i++) {
		if (hashmap[i].section == section && hashmap[i].code == code) {
			return hashmap[i].label;
		}
	}
	return "Label to be defined.";
}