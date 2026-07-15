#include <stdio.h>
#include <stdarg.h>

int get_field (int endianness, int count, ...) {
	int out = 0;
	va_list args;
	va_start(args, count);
	if (endianness == 2) { // big endian
		for (int i = 0; i < count; i ++) {
			out <<= 4;
			out += va_arg(args, int);
		}
	} else if (endianness == 1) { // little endian
		for (int i = 0; i < count; i ++) {
			out += va_arg(args, int) << (i * 4);
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
		printf("Magic number                  : %02hx %02hx %02hx %02hx | %c%c%c%c\n", buffer[0x00], buffer[0x01], buffer[0x02], buffer[0x03], buffer[0x00], buffer[0x01], buffer[0x02], buffer[0x03]);
		// Now address space check
		if (buffer[0x04] == 0x01) {
			address_space = 32;
		} else if (buffer[0x04] == 0x02) {
			address_space = 64;
		} else {
			fprintf(stderr, "Invalid address space identifier at 0x04\n");
			return 3;
		}
		printf("Address space                 : %d-bit\n", address_space);
		// Now for endianness
		printf("Endianness                    : ");
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
		printf("ELF version                   : %02hx\n", buffer[0x06]);
		// ABI
		printf("OS ABI                        : %02hx (", buffer[0x07]);
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
		printf("ABI Version                   : %02hx\n", buffer[0x08]);
		// Padding from 0x09 -> 0x10
		// Type
		printf("Type                          : ");
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
			case 0xFFFF:
				printf("ET_HIPROC");
				break;
			default:
				printf("%02hx", get_field(endianness, 2, buffer[0x10], buffer[0x11]));
		}
		printf("\n");
		// Target architecture... :(
		printf("Instruction Set Architecture  : %02hx%02hx (", buffer[0x12], buffer[0x13]);
		switch (get_field(endianness, 2, buffer[0x12], buffer[0x13])) {
			case 0x01:
				printf("AT&T WE 32100");
				break;
			case 0x02:
				printf("SPARC");
				break;
			case 0x03:
				printf("x86");
				break;
			case 0x04:
				printf("Motorola 68000 (M68k)");
				break;
			case 0x05:
				printf("Motorola 88000 (M68k)");
				break;
			case 0x06:
				printf("Intel MCU");
				break;
			case 0x07:
				printf("Intel 80860");
				break;
			case 0x08:
				printf("MIPS");
				break;
			case 0x09:
				printf("IBM System / 370");
				break;
			case 0x0A:
				printf("MIPS RS3000 Little-endian");
				break;
			case 0x0F:
				printf("Hewlett-Packard PA-RISC");
				break;
			case 0x13:
				printf("Intel 80960");
				break;
			case 0x14:
				printf("PowerPC");
				break;
			case 0x15:
				printf("PowerPC 64-bit");
				break;
			case 0x16:
				printf("S390 / S390x");
				break;
			case 0x17:
				printf("IBM SPU / SPC");
				break;
			case 0x24:
				printf("NEC V800");
				break;
			case 0x25:
				printf("Fujitsu FR20");
				break;
			case 0x26:
				printf("TRW RH-32");
				break;
			case 0x27:
				printf("Motorola RCE");
				break;
			case 0x28:
				printf("Arm");
				break;
			case 0x29:
				printf("Digital Alpha");
				break;
			case 0x2A:
				printf("SuperH");
				break;
			case 0x2B:
				printf("SPARC Version 9");
				break;
			case 0x2C:
				printf("Siemens TriCore embedded processor");
				break;
			case 0x2D:
				printf("Argonaut RISC Core");
				break;
			case 0x2E:
				printf("Hitachi H8 / 300");
				break;
			case 0x2F:
				printf("Hitachi H8 / 300H");
				break;
			case 0x30:
				printf("Hitachi H8S");
				break;
			case 0x31:
				printf("Hitachi H8 / 500");
				break;
			case 0x32:
				printf("IA-64");
				break;
			case 0x33:
				printf("Stanford MIPS-X");
				break;
			case 0x34:
				printf("Motorola ColdFire");
				break;
			case 0x35:
				printf("Motorola M68HC12");
				break;
			case 0x36:
				printf("Fujitsu MMA Multimedia Accelerator");
				break;
			case 0x37:
				printf("Siemens PCP");
				break;
			case 0x38:
				printf("Sony nCPU embedded RISC processor");
				break;
			case 0x39:
				printf("Denso NDR1 microprocessor");
				break;
			case 0x3A:
				printf("Motorola Star*Core processor");
				break;
			case 0x3B:
				printf("Toyota ME16 processor");
				break;
			case 0x3C:
				printf("STMicroelectronics ST100 processor");
				break;
			case 0x3D:
				printf("Advanced Logic Corp. TinyJ embedded processor family");
				break;
			case 0x3E:
				printf("AMD x86-64");
				break;
			case 0x3F:
				printf("Sony DSP Processor");
				break;
			case 0x40:
				printf("Digital Equipment Corp. PDP-10");
				break;
			case 0x41:
				printf("Digital Equipment Corp. PDP-11");
				break;
			case 0x42:
				printf("Siemens FX66 microcontroller");
				break;
			case 0x43:
				printf("STMicroelectronics ST9+ 8 / 16-bit microcontroller");
				break;
			case 0x44:
				printf("STMicroelectronics ST7 8-bit microcontroller");
				break;
			case 0x45:
				printf("Motorola MC68HC16 Microcontroller");
				break;
			case 0x46:
				printf("Motorola MC68HC11 Microcontroller");
				break;
			case 0x47:
				printf("Motorola MC68HC08 Microcontroller");
				break;
			case 0x48:
				printf("Motorola MC68HC05 Microcontroller");
				break;
			case 0x49:
				printf("Silicon Graphics SVx");
				break;
			case 0x4A:
				printf("STMicroelectronics ST19 8-bit microcontroller");
				break;
			case 0x4B:
				printf("Digital VAX");
				break;
			case 0x4C:
				printf("Axis Communications 32-bit embedded processor");
				break;
			case 0x4D:
				printf("Infineon Technologies 32-bit embedded processor");
				break;
			case 0x4E:
				printf("Element 14 64-bit DSP Processor");
				break;
			case 0x4F:
				printf("LSI Logic 16-bit DSP Processor");
				break;
			case 0x8C:
				printf("TMS320C6000 Family");
				break;
			case 0xAF:
				printf("MCST Elbrus e2k");
				break;
			case 0xB7:
				printf("Arm 64-bits");
				break;
			case 0xDC:
				printf("Zilog Z80");
				break;
			case 0xF3:
				printf("RISC-V");
				break;
			case 0xF7:
				printf("Berkeley Packet Filter");
				break;
			default:
				printf("Reserved / no particular set");
		}
		printf(")\n");
		// Version Detail
		printf("Version detail                : %02hx%02hx%02hx%02hx\n", buffer[0x14], buffer[0x15], buffer[0x16], buffer[0x17]);
		printf("-------------------------------\n");
		// Program entry
		printf("Program entry point           : ");
		if (address_space == 32) {
			printf("%02hx%02hx%02hx%02hx", buffer[0x18], buffer[0x19], buffer[0x1A], buffer[0x1B]);
		} else if (address_space == 64) {
			printf("%02hx%02hx%02hx%02hx %02hx%02hx%02hx%02hx", buffer[0x18], buffer[0x19], buffer[0x1A], buffer[0x1B], buffer[0x1C], buffer[0x1D], buffer[0x1E], buffer[0x1F]);
		}
		printf("\n");
		// Program header table
		printf("Program header table          : ");
		if (address_space == 32) {
			printf("%02hx%02hx%02hx%02hx", buffer[0x1C], buffer[0x1D], buffer[0x1E], buffer[0x1F]);
		} else if (address_space == 64) {
			printf("%02hx%02hx%02hx%02hx %02hx%02hx%02hx%02hx", buffer[0x20], buffer[0x21], buffer[0x22], buffer[0x23], buffer[0x24], buffer[0x25], buffer[0x26], buffer[0x27]);
		}
		printf("\n");
		// Section header table
		printf("Section header table          : ");
		if (address_space == 32) {
			printf("%02hx%02hx%02hx%02hx", buffer[0x20], buffer[0x21], buffer[0x22], buffer[0x23]);
		} else if (address_space == 64) {
			printf("%02hx%02hx%02hx%02hx %02hx%02hx%02hx%02hx", buffer[0x28], buffer[0x29], buffer[0x2A], buffer[0x2B], buffer[0x2C], buffer[0x2D], buffer[0x2E], buffer[0x2F]);
		}	
	}
}

