#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define SIZEOFBLOCK 16

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
    Block block[16][16];
    uint16_t differences [16][16];
    Vector vectors [16][16];
} Frame;

typedef struct Film
{
    Frame frame[2];
} Film;


void process_frame(Frame *cur_frame, FILE *fptr)
{
    uint8_t cur_pixel;
    int cur_pixel_row = 0;
    int cur_pixel_col = 0;
    int cur_block_row = 0;
    int cur_block_col = 0;

    uint8_t first_iteration = 1;


    Block *cur_block = &cur_frame->block[0][0];

    fread(&cur_pixel, sizeof(uint8_t), 1, fptr);

    while (cur_pixel_row < 256)
    {
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

        cur_pixel_col++;
        fread(&cur_pixel, sizeof(uint8_t), 1, fptr);
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
        printf("Please provide a file name");
        exit(-1);
    }
    else
    {
        file_name = argv[1];
    }

    
    //for each block in frame
    //    find most similar (smallest SAD) other block (limit search to nearby blocks)

    fptr = fopen(file_name, "rb");
    if(fptr == NULL)
    {
        printf("Error!");   
        exit(1);             
    }
    
    
    Frame *test_frame = (Frame*)malloc(sizeof(Frame));
    printf("test above process frame\n");

    process_frame(test_frame, fptr);

    for (int i = 0; i < 16; ++i)
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
    }
    

    fclose(fptr);
    return 0;
}