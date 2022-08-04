#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <arm_neon.h>

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


    for (uint8_t frame = 0; frame < NUMFRAMES - 1; ++frame) // every frame
    {
        for (uint8_t block_row_ref = 0; block_row_ref < NUMBLOCKS; ++block_row_ref) // every row in frame (block)
        {
            for (uint8_t block_col_ref = 0; block_col_ref < NUMBLOCKS; ++block_col_ref) // every column in frame (block)
            {
                for (uint8_t block_row_comp = max(0, block_row_ref - 3); block_row_comp < min(NUMBLOCKS, block_row_ref + 3); ++block_row_comp) // every block row in other frame
                {
                    printf("block_row_ref: %d, block_row_comp: %d\n", block_row_ref, block_row_comp);
                    for (uint8_t block_col_comp = max(0, block_col_ref - 3); block_col_comp < min(NUMBLOCKS, block_col_ref + 3); ++block_col_comp) // every block column in other frame
                    {
                        uint32_t temp_sad = 0;
                        for (uint8_t pixel_row = 0; pixel_row < SIZEOFBLOCK; ++pixel_row) // every row in cur_block (cur_pixel)
                        {
                            uint8x16_t vector_ref; // declare a vector of 16 8-bit lanes
                            uint8x16_t vector_comp; // declare a vector of 16 8-bit lanes
                            printf("After declaration, before intialization\n");
                            uint8_t ref_test_array [16];
                            uint8_t comp_test_array [16];
                            for (int i = 0; i < 16; ++i)
                            {
                                ref_test_array[i] = test_film->frame[frame].block[block_row_ref][block_col_ref].pixel[pixel_row][i];
                                comp_test_array[i] = test_film->frame[frame + 1].block[block_row_comp][block_col_comp].pixel[pixel_row][i];
                            }
                            vector_ref = vld1q_u8(ref_test_array); // load the array from memory into a vector
                            printf("After first initalization\n");
                            vector_comp = vld1q_u8(comp_test_array); // load the array from memory into a vector
                            printf("After second initliazation\n");

                            // Perform the Absolute Differences operation:
                            uint8x16_t result;
                            result = vabdq_u8(vector_ref, vector_comp);

                            uint8x8_t result_low;
                            uint8x8_t result_high;
                            result_low = vget_low_u8(result);
                            result_high = vget_high_u8(result);

                            uint8x8_t small_result;
                            small_result = vadd_u8(result_low, result_high);

                            uint8_t temp_storage [8];

                            vst1_u8(temp_storage, small_result);

                            temp_sad = temp_sad + temp_storage[0] + temp_storage[1] + temp_storage[2] + temp_storage[3] + temp_storage[4] + temp_storage[5] + temp_storage[6] + temp_storage[7];
 
                        }
                        if (test_film->frame[frame].differences[block_row_ref][block_col_ref] > temp_sad )
                        {
                            // printf("In if condition, temp_sad = %d, old value = %d\n", temp_sad, test_film->frame[frame].differences[block_row_ref][block_col_ref]);
                            test_film->frame[frame].differences[block_row_ref][block_col_ref] = temp_sad;
                            test_film->frame[frame].vectors[block_row_ref][block_col_ref].x = block_row_comp - block_row_ref;
                            test_film->frame[frame].vectors[block_row_ref][block_col_ref].y = block_col_ref - block_col_comp;
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

    printf("max of 0, -3: %d\n", max(0, -3));
    printf("min of 0, 3: %d\n", min(0, 3));

    fclose(fptr);
    return 0;
}