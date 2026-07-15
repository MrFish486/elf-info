#include <stdio.h>

int get_field (int endianness, int count, ...) {
	int out = 0;
	va_list args;
	va_start(args, count);
	if (endianness == 1) { // little endian
		for (int i = 0; i < count, i ++) {
			out <<= 1;
			out += va_arg(args, int);
		}
	} else if (endianness == 2) { // big endian
		for (int i = count - 1; i >= 0; i --) {
			out <<= 1;
			out += va_arg(args, int);
		}
	}
	va_end(args);
	return out;
}

int main (int argc, char **argv){
	if (argc == 1) {
		fprintf(stderr, "No file specified.\n");
		return 1;
	} else if (argc > 2) {
		fprintf(stderr, "Too many files!\n");
		return 2;
	} else {
		unsigned char buffer[64];
		FILE* handle = fopen(argv[0], "rb");
		fread(buffer, sizeof(buffer), 1, handle);
		int endianness    = 0;
		int address_space = 0;
		// First step is to read the magic number
		if (buffer[0x00] == 0x7f && buffer[0x01] == 0x45 && buffer[0x02] == 0x4c && buffer[0x03] == 0x46) {
			printf("File seems to be valid ELF file\n");
			printf("-------------------------------\n");
		}
		printf("Magic number  : %02hx %02hx %02hx %02hx | %c%c%c%c\n", buffer[0x00], buffer[0x01], buffer[0x02], buffer[0x03], buffer[0x00], buffer[0x01], buffer[0x02], buffer[0x03]);
		// Now address space check
		if (buffer[0x04] == 0x01) {
			address_space = 32;
		} else if (buffer[0x04] == 0x02) {
			address_space = 64;
		} else {
			fprintf(stderr, "Invalid address space identifier at 0x04\n");
			return 3;
		}
		printf("Address space : %d-bit\n", address_space);
		// Now for endianness
		printf("Endianness    : ");
		if (buffer[0x05] == 0x01) {
			printf("little endian\n");
		} else if (buffer[0x06] == 0x02) {
			printf("big endian\n");
		} else {
			fprintf(stderr, "Invalid endianness at 0x05\n");
			return 4;
		}
		endianness = buffer[0x06];
		// ELF version
		printf("ELF version   : %02hx\n", buffer[0x06]);
		// ABI
		printf("OS ABI        : %02hx (", buffer[0x07]);
		switch (buffer[0x07]) {
			case 0x00:
				printf("System V");
				break;
			case 0x01:
				printf("HP-UX");
				break;
			case 0x02:
				printf("NetBSD");
				break;
			case 0x03:
				printf("Linux");
				break;
			case 0x04:
				printf("GNU Hurd");
				break;
			case 0x06:
				printf("Solaris");
				break;
			case 0x07:
				printf("AIX");
				break;
			case 0x08:
				printf("IRIX");
				break;
			case 0x09:
				printf("FreeBSD");
				break;
			case 0x0A:
				printf("Tru64");
				break;
			case 0x0B:
				printf("Novell Modesto");
				break;
			case 0x0C:
				printf("OpenBSD");
				break;
			case 0x0D:
				printf("OpenVMS");
				break;
			case 0x0E:
				printf("NonStop Kernel");
				break;
			case 0x0F:
				printf("AROS");
				break;
			case 0x10:
				printf("FenixOS");
				break;
			case 0x11:
				printf("Nuxi CloudABI");
				break;
			case 0x12:
				printf("Stratus Technologies OpenVOS");
				break;
			default:
				printf("unknown");
		}
		printf(")\n");
		// ABI Version
		printf("ABI Version   : %02hx\n", buffer[0x08]);
		// Padding from 0x09 -> 0x10
		// Type
		printf("Type: ");
		switch (get_field(endianness, 2, buffer[0x10], buffer[0x11])) {
			case 0x00:
				printf("ET_NONE (Unknown)");
				break;
			case 0x01:
				printf("ET_REL  (Relocatable)");
				break;
			case 0x02:
				printf("ET_EXEC (Executable)");
				break;
			case 0x03:
				printf("ET_DYN  (Shared object)");
				break;
			case 0x04:
				printf("ET_CORE (Core)");
				break;
			case 0xFE00:
				printf("ET_LOOS");
				break;
			case 0xFEFF:
				printf("ET_HIOS");
				break;
			case 0xFF00:
				printf("ET_LOPROC");
				break;
			case 0xFF00:
				printf("ET_HIPROC");
				break;
			default:
				printf("%02hx", get_field(endianness, 2, buffer[0x10], buffer[0x11]));
		}
		printf("\n");
	}
}

