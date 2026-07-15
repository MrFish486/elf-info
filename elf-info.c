#include <stdio.h>

int main (int argc, char *argv){
	if (argc == 1) {
		fprintf(stderr, "No file specified.");
		return 1;
	} else if (argc > 2) {
		fprintf(stderr, "Too many files!");
		return 2;
	}
}

