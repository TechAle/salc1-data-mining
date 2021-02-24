#include <stdio.h>
#include <inttypes.h>

struct list_file_struct
{
    char name_chunk[30];
    int n_chunk;
    int len;
};

int main(int argc, char *argv[]) {

    FILE *f_input, *f_output;
    struct list_file_struct output_list;
    f_input = fopen(argv[1], "rb");
    f_output = fopen("refractor.bin", "wb");
    char* endptr;
    int max = strtoimax(argv[2], &endptr, 10);

    while(fread(&output_list ,sizeof(struct list_file_struct), 1, f_input))
    {
        if (output_list.len > max ) {
            fwrite(&output_list, sizeof(struct list_file_struct), 1, f_output);
        }
    }

    fclose(f_input);
    fclose(f_output);

}