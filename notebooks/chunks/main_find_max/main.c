#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <dirent.h>
#include "minecraft.h"

struct list_file_struct
{
    char name_chunk[30];
    int n_chunk;
    int len;
};

// Usage: ./max.out (n_time) (start)
#define LIMIT_ADD 8000

int main(int argc, char *argv[])
{
    // Reading input file
    FILE *input_file,
         *f;
    // Header
    RegionFileHeader header;
    unsigned int offset,
            length;
    ChunkDataHeader header_chunk;
    struct list_file_struct output_list;
    char prefix[30];

    // Reading binary backup
    f = fopen("pos.bin", "rb");

    int max = 0;
    int ret = 0;
    int n_time = -1;
    int count = 0;
    char* endptr;
    // If we have a value that say our max (so we have 2 argouments)
    if (argc == 3 ) {
        max = strtoimax(argv[2], &endptr, 10);
    }
    char str[30],
         max_name[30];
    // If we have a value that say where we have to start (so we have 1 argoument)
    if (argc > 1)
        n_time = strtoimax(argv[1], &endptr, 10);
    strcpy(max_name, str);

    // Open directory
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    // If everything is ok
    if (d) {
        // Until we have a file
        while ((dir = readdir(d)) != NULL) {
            // If it has .mca
            strcpy(str, dir->d_name);
            if(strstr(str, ".mca")) {
                // Reading the header
                input_file = fopen(str, "r");
                fread(&header, sizeof(RegionFileHeader), 1, input_file);

                /* for each chunk exists */
                for (int i = 0; i < CHUNKS_PER_REGION; i++) {
                    // Taking offset for checking if the chunk exist
                    offset = (65536 * header.locations[i].offset[0]) + (256 * header.locations[i].offset[1]) +
                             header.locations[i].offset[2];
                    if (offset != 0) {
                        /* read the chunk header */
                        fseek(input_file, offset * 4096, SEEK_SET);
                        fread(&header_chunk, sizeof(ChunkDataHeader), 1, input_file);
                        // Taking the length of the chunk
                        length = (16777216 * header_chunk.length[0]) + (65536 * header_chunk.length[1]) +
                                 (256 * header_chunk.length[2]) + header_chunk.length[3];
                        // If lenght > max, replace
                        if (length > LIMIT_ADD) {
                            output_list.len = length;
                            output_list.n_chunk = i;
                            strcpy(output_list.name_chunk, str);
                            f = fopen("chunk_list.bin", "ab");
                            fwrite(&output_list, sizeof(struct list_file_struct), 1, f);
                            fclose(f);
                            printf("output: %s %d %d\n", str, i, length );
                        }

                    }
                }

                fclose(input_file);

                strcpy(prefix, "done_");
                strcat(prefix, str);
                rename(str, prefix);
            }
        }

    }
}
