#include <stdio.h>

struct list_file_struct
{
    char name_chunk[30];
    int n_chunk;
    int len;
};

int main(int argc, char *argv[]) {

    FILE *f;
    struct list_file_struct output_list;
    f = fopen(argv[1], "rb");

    while(fread(&output_list ,sizeof(struct list_file_struct), 1, f))
    {
        printf("%s %d %d\n", output_list.name_chunk, output_list.n_chunk, output_list.len);
    }

    fclose(f);

}