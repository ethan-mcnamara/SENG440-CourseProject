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

    uint8_t rm_header [54];

    // Read the header and store it in an array (not to be used)
    fread(&rm_header, sizeof(uint8_t) * 7 - 2, 1, fptr);

    fread(&cur_pixel, sizeof(uint8_t), 1, fptr);

    while (cur_pixel_row < SIZEOFIMAGE)
    {

        if (!first_iteration)
        {
            if (cur_block_col == SIZEOFBLOCK)
            {
                cur_pixel_col = 0;
                cur_block_col = 0;
                cur_pixel_row++;

                if (cur_pixel_row % SIZEOFBLOCK == 0)
                {
                    printf("%d\n", cur_pixel_row);
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


    for (uint8_t frame = 0; frame < NUMFRAMES - 1; ++frame) // every frame
    {
        for (uint8_t block_row_ref = 0; block_row_ref < NUMBLOCKS; ++block_row_ref) // every row in frame (block)
        {
            for (uint8_t block_col_ref = 0; block_col_ref < NUMBLOCKS; ++block_col_ref) // every column in frame (block)
            {
                for (uint8_t block_row_comp = max(0, block_row_ref - 3); block_row_comp < min(NUMBLOCKS, block_row_ref + 3); ++block_row_comp) // every block row in other frame
                {
                    // printf("block_row_ref: %d, block_row_comp: %d\n", block_row_ref, block_row_comp);
                    for (uint8_t block_col_comp = max(0, block_col_ref - 3); block_col_comp < min(NUMBLOCKS, block_col_ref + 3); ++block_col_comp) // every block column in other frame
                    {
                        uint32_t temp_sad = 0;
                        for (uint8_t pixel_row = 0; pixel_row < SIZEOFBLOCK; ++pixel_row) // every row in cur_block (cur_pixel)
                        {
                            for (uint8_t pixel_col = 0; pixel_col < SIZEOFBLOCK; ++pixel_col) // every column in cur_block (cur_pixel)
                            {
                                int diff = test_film->frame[frame].block[block_row_ref][block_col_ref].pixel[pixel_row][pixel_col] - test_film->frame[frame + 1].block[block_row_comp][block_col_comp].pixel[pixel_row][pixel_col];
                                // printf("Block[%d][%d], difference: %d\n", block_row_ref, block_col_ref, diff);
                                if (diff < 0)
                                {
                                    temp_sad -= diff;
                                }
                                else
                                {
                                    temp_sad += diff;
                                }

                                //printf("Block[%d][%d], temp_sad: %d\n", block_row_ref, block_col_ref, temp_sad);

                            }   
                        }
                        /*if (temp_sad == 0)
                        {
                            printf("Block[%d][%d] compared to Block[%d][%d] in other frame, diff is 0\n", block_row_ref, block_col_ref, block_row_comp, block_col_comp);
                        }*/
                        //double new_distance = sqrt((block_row_comp - block_row_ref) * (block_row_comp - block_row_ref) + (block_col_ref - block_col_comp) * (block_col_ref - block_col_comp));
                        //double cur_distance = sqrt(test_film->frame[frame].vectors[block_row_ref][block_col_ref].x * test_film->frame[frame].vectors[block_row_ref][block_col_ref].x + test_film->frame[frame].vectors[block_row_ref][block_col_ref].y * test_film->frame[frame].vectors[block_row_ref][block_col_ref].y);
                        if (test_film->frame[frame].differences[block_row_ref][block_col_ref] > temp_sad )//|| (new_distance < cur_distance && test_film->frame[frame].differences[block_row_ref][block_col_ref] == temp_sad))
                        {
                            // printf("In if condition, temp_sad = %d, old value = %d\n", temp_sad, test_film->frame[frame].differences[block_row_ref][block_col_ref]);
                            test_film->frame[frame].differences[block_row_ref][block_col_ref] = temp_sad;
                            test_film->frame[frame].vectors[block_row_ref][block_col_ref].x = block_col_comp - block_col_ref;
                            test_film->frame[frame].vectors[block_row_ref][block_col_ref].y = block_row_ref - block_row_comp;
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