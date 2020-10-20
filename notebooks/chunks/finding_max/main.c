/* @(#) mca2nbt:mca2nbt.c v1.00 (2014-05-05) / Hubert Tournier */

/* @(#) mca2nbt:mca2nbt.c v1.00 (2014-05-05) / Hubert Tournier */

#include <stdio.h>
#include <inttypes.h>
#include "minecraft.h"

int main(int argc, char *argv[])
{
        FILE 			*input_file;
        RegionFileHeader	header;
        int max;
        max = strtoimax("1",&max,10);

        // Reading the header
        input_file = fopen(argv[1], "r");
        fread(&header, sizeof(RegionFileHeader), 1, input_file);


        /* for each chunk exists */
        for (int i = 0; i < CHUNKS_PER_REGION; i++)
        {

            unsigned int	offset,
                    length;
            ChunkDataHeader	header_chunk;

            offset = (65536 * header.locations[i].offset[0]) + (256 * header.locations[i].offset[1]) + header.locations[i].offset[2];
            if (offset != 0)
            {
                /* read the chunk header */
                /*************************/
                fseek(input_file, offset * 4096, SEEK_SET);
                fread(&header_chunk, sizeof(ChunkDataHeader), 1, input_file);
                length = (16777216 * header_chunk.length[0]) + (65536 * header_chunk.length[1]) + (256 * header_chunk.length[2]) + header_chunk.length[3];

                /* write the chunk as an individual deflated NBT file */
                    // Questa porzione di codice
                if (length > max)
                    max = length;

            }
        }
}
