#include <stdio.h>
#include <stdarg.h>

int get_field (int endianness, int count, ...) {
	int out = 0;
	va_list args;
	va_start(args, count);
	if (endianness == 1) { // little endian
		for (int i = 0; i < count; i ++) {
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
		printf("Type          : ");
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
		printf("Instruction Set Architecture : %02hx%02hx (", buffer[0x12], buffer[0x13]);
		switch (get_field(endianness, 2, buffer[0x12], buffer[0x13])) {
			case 0x01:
				printf("AT&T WE 32100");
			case 0x02:
				printf("SPARC");
			case 0x03:
				printf("x86");
			case 0x04:
				printf("Motorola 68000 (M68k)");
			case 0x05:
				printf("Motorola 88000 (M68k)");
			case 0x06:
				printf("Intel MCU");
			case 0x07:
				printf("Intel 80860");
			case 0x08:
				printf("MIPS");
			case 0x09:
				printf("IBM System / 370");
			case 0x0A:
				printf("MIPS RS3000 Little-endian");
			case 0x0F:
				printf("Hewlett-Packard PA-RISC");
			case 0x13:
				printf("Intel 80960");
			case 0x14:
				printf("PowerPC");
			case 0x15:
				printf("PowerPC 64-bit");
			case 0x16:
				printf("S390 / S390x");
			case 0x17:
				printf("IBM SPU / SPC");
			case 0x24:
				printf("NEC V800");
			case 0x25:
				printf("Fujitsu FR20");
			case 0x26:
				printf("TRW RH-32");
			case 0x27:
				printf("Motorola RCE");
			case 0x28:
				printf("Arm");
			case 0x29:
				printf("Digital Alpha");
			case 0x2A:
				printf("SuperH");
			case 0x2B:
				printf("SPARC Version 9");
			case 0x2C:
				printf("Siemens TriCore embedded processor");
			case 0x2D:
				printf("Argonaut RISC Core");
			case 0x2E:
				printf("Hitachi H8 / 300");
			case 0x2F:
				printf("Hitachi H8 / 300H");
			case 0x30:
				printf("Hitachi H8S");
			case 0x31:
				printf("Hitachi H8 / 500");
			case 0x32:
				printf("IA-64");
			case 0x33:
				printf("Stanford MIPS-X");
			case 0x34:
				printf("Motorola ColdFire");
			case 0x35:
				printf("Motorola M68HC12");
			case 0x36:
				printf("Fujitsu MMA Multimedia Accelerator");
			case 0x37:
				printf("Siemens PCP");
			case 0x38:
				printf("Sony nCPU embedded RISC processor");
			case 0x39:
				printf("Denso NDR1 microprocessor");
			case 0x3A:
				printf("Motorola Star*Core processor");
			case 0x3B:
				printf("Toyota ME16 processor");
			case 0x3C:
				printf("STMicroelectronics ST100 processor");
			case 0x3D:
				printf("Advanced Logic Corp. TinyJ embedded processor family");
			case 0x3E:
				printf("AMD x86-64");
			case 0x3F:
				printf("Sony DSP Processor");
			case 0x40:
				printf("Digital Equipment Corp. PDP-10");
			case 0x41:
				printf("Digital Equipment Corp. PDP-11");
			case 0x42:
				printf("Siemens FX66 microcontroller");
			case 0x43:
				printf("STMicroelectronics ST9+ 8 / 16-bit microcontroller");
			case 0x44:
				printf("STMicroelectronics ST7 8-bit microcontroller");
			case 0x45:
				printf("Motorola MC68HC16 Microcontroller");
			case 0x46:
				printf("Motorola MC68HC11 Microcontroller");
			case 0x47:
				printf("Motorola MC68HC08 Microcontroller");
			case 0x48:
				printf("Motorola MC68HC05 Microcontroller");
			case 0x49:
				printf("Silicon Graphics SVx");
			case 0x4A:
				printf("STMicroelectronics ST19 8-bit microcontroller");
			case 0x4B:
				printf("Digital VAX");
			case 0x4C:
				printf("Axis Communications 32-bit embedded processor");
			case 0x4D:
				printf("Infineon Technologies 32-bit embedded processor");
			case 0x4E:
				printf("Element 14 64-bit DSP Processor");
			case 0x4F:
				printf("LSI Logic 16-bit DSP Processor");
			case 0x8C:
				pritnf("TMS320C6000 Family");
			case 0xAF:
				printf("MCST Elbrus e2k");
			case 0xB7:
				printf("Arm 64-bits");
			case 0xDC:
				printf("Zilog Z80");
			case 0xF3:
				printf("RISC-V");
			case 0xF7:
				printf("Berkeley Packet Filter");
			case 0x101:
				printf("WDC 65C816");
			case 0x102:
				pritnf("LoongArch");

			default:
				printf("Reserved / no particular set");
		}
		printf(")\n");
	}
}

