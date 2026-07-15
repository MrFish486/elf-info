#include <stdio.h>

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
	}
}

