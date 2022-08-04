#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>

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

    
    //for each block in frame
    //    find most similar (smallest SAD) other block (limit search to nearby blocks)

    fptr = fopen("test_images/Image1.bmp", "rb");
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
    fptr = fopen("test_images/Image2.bmp", "rb");
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


    for (int i = 0; i < NUMFRAMES - 1; ++i) // every frame
    {
        for (int j = 0; j < NUMBLOCKS; ++j) // every row in frame (block)
        {
            for (int k = 0; k < NUMBLOCKS; ++k) // every column in frame (block)
            {
                for (int w = 0; w < NUMBLOCKS; ++w) // every block row in other frame
                {
                    for (int f = 0; f < NUMBLOCKS; ++f) // every block column in other frame
                    {
                        uint32_t temp_sad = 0;
                        for (int s = 0; s < SIZEOFBLOCK; ++s) // every row in cur_block (cur_pixel)
                        {
                            for (int r = 0; r < SIZEOFBLOCK; ++r) // every column in cur_block (cur_pixel)
                            {
                                int diff = test_film->frame[i].block[j][k].pixel[s][r] - test_film->frame[i + 1].block[w][f].pixel[s][r];
                                // printf("Block[%d][%d], difference: %d\n", j, k, diff);
                                if (diff < 0)
                                {
                                    temp_sad -= diff;
                                }
                                else
                                {
                                    temp_sad += diff;
                                }

                                //printf("Block[%d][%d], temp_sad: %d\n", j, k, temp_sad);

                            }   
                        }
                        /*if (temp_sad == 0)
                        {
                            printf("Block[%d][%d] compared to Block[%d][%d] in other frame, diff is 0\n", j, k, w, f);
                        }*/
                        if (test_film->frame[i].differences[j][k] > temp_sad)
                        {
                            // printf("In if condition, temp_sad = %d, old value = %d\n", temp_sad, test_film->frame[i].differences[j][k]);
                            test_film->frame[i].differences[j][k] = temp_sad;
                            test_film->frame[i].vectors[j][k].x = w - j;
                            test_film->frame[i].vectors[j][k].y = k - f;
                        }
                    }
                }
            }
        }
    }
    
    for (int i = 0; i < NUMBLOCKS; ++i)
    {
        for (int j = 0; j < NUMBLOCKS; ++j)
        {
            int temp_diff = test_film->frame[0].differences[i][j];
            int temp_x = test_film->frame[0].vectors[i][j].x;
            int temp_y = test_film->frame[0].vectors[i][j].y;
            printf("Block[%d][%d]: Vector: (%d, %d); Difference: %d\n", i, j, temp_x, temp_y, temp_diff);
        }
    }

    fclose(fptr);
    return 0;
}