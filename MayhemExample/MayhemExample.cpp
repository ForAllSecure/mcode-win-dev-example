#include "MayhemExample.h"

using namespace std;

static void doit(uint8_t* data, size_t size) {
    if (size >= 3) {
		if (data[0] == 'b') {
			if (data[1] == 'u') {
				if (data[2] == 'g') {
					abort();
				}
			}
		}
    }
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        fprintf(stderr, "Running: %s\n", argv[i]);
        FILE* f = fopen(argv[i], "r");
        assert(f);
        fseek(f, 0, SEEK_END);
        size_t size = ftell(f);
        fseek(f, 0, SEEK_SET);
        unsigned char* data = (unsigned char*)malloc(size);
        size_t n_read = fread(data, 1, size, f);
        fclose(f);
        assert(n_read == size);
        doit(data, size);
        free(data);
        fprintf(stderr, "Done:    %s: (%zd bytes)\n", argv[i], n_read);
    }
}
