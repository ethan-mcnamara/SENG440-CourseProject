#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include "arm_neon.h"

#define SIZEOFBLOCK 16
#define NUMFRAMES 2
#define NUMBLOCKS 16
#define SIZEOFIMAGE 256

/*
* Struct definitions
*/

typedef struct Vector
{
    int8_t x;
    int8_t y;
} Vector;

typedef struct Block
{
  uint8_t pixel[SIZEOFBLOCK][SIZEOFBLOCK];
} Block;

typedef struct Frame
{
    Block block[NUMBLOCKS][NUMBLOCKS];
    uint32_t differences [NUMBLOCKS][NUMBLOCKS];
    Vector vectors [NUMBLOCKS][NUMBLOCKS];
} Frame;

typedef struct Film
{
    Frame frame[NUMFRAMES];
} Film;

int8_t max(int8_t val_1, int8_t val_2)
{
    return (val_1 < val_2) ? val_2 : val_1;
}

int8_t min(int8_t val_1, int8_t val_2)
{
    return (val_1 > val_2) ? val_2 : val_1;
}

void process_frame(Frame *cur_frame, FILE *fptr)
{
    uint8_t cur_pixel;
    int cur_pixel_row = 0;
    int cur_pixel_col = 0;
    int cur_block_row = 0;
    int cur_block_col = 0;

    uint8_t first_iteration = 1;

    for (int i = 0; i < NUMBLOCKS; ++i)
    {
        for (int j = 0; j < NUMBLOCKS; ++j)
        {
            cur_frame->differences[i][j] = UINT32_MAX;
        }
    }

    Block *cur_block = &cur_frame->block[0][0];

    fread(&cur_pixel, sizeof(uint8_t), 1, fptr);

    uint8_t header_counter = 0;

    while (cur_pixel_row < SIZEOFIMAGE - 2)
    {
        // Skip over the jpg image header
        if (header_counter < 6)
        {
            header_counter++;
            continue;
        }

        if (!first_iteration)
        {
            if (cur_block_col == SIZEOFBLOCK)
            {
                cur_pixel_col = 0;
                cur_block_col = 0;
                cur_pixel_row++;

                if (cur_pixel_row % SIZEOFBLOCK ==0)
                {
                    cur_block_row++;
                }

                cur_block = &cur_frame->block[cur_block_row][cur_block_col];

            }
            if (cur_pixel_col % SIZEOFBLOCK == 0)
            {
                cur_block = &cur_frame->block[cur_block_row][++cur_block_col];
            }
        }
        else
        {
            first_iteration = 0;
        }

        cur_block->pixel[cur_pixel_row % SIZEOFBLOCK][cur_pixel_col % SIZEOFBLOCK] = cur_pixel;
        //printf("%d\n", cur_pixel);

        cur_pixel_col++;

        fread(&cur_pixel, sizeof(uint8_t), 1, fptr) != 1;
    }
}


/*
* Main function
*/
int main(int argc, char *argv[]) 
{
    char* file_name;
    FILE *fptr;

    if (argc < 2)
    {
        printf("Please provide a file name\n");
        exit(-1);
    }
    else
    {
        file_name = argv[1];
    }

    fptr = fopen("Image1.bmp", "rb");
    if(fptr == NULL)
    {
        printf("Error!");   
        exit(1);             
    }
    
    
    Frame *test_frame = (Frame*)malloc(sizeof(Frame));
    Film *test_film = (Film*) malloc(sizeof(Film));

    process_frame(test_frame, fptr);

    test_film->frame[0] = *test_frame;
    fclose(fptr);
    fptr = fopen("Image2.bmp", "rb");
    process_frame(test_frame, fptr);
    test_film->frame[1] = *test_frame;
    /*for (int i = 0; i < 16; ++i)
    {
        for (int j = 0; j < 16; ++j)
        {
            for (int k = 0; k < 16; ++k)
            {
                for (int t = 0; t < 16; ++t)
                {
                    printf("Block[%d][%d], Pixel[%d][%d] = '%d'\n", i, j, k, t, test_frame->block[i][j].pixel[k][t]);
                }
            }
        }
    }*/
    
    printf("max of 0, -3: %d\n", max(0, -3));
    printf("min of 0, 3: %d\n", min(0, 3));

    fclose(fptr);
    return 0;
}