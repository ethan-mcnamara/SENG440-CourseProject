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

// Struct for determining Vector with small SAD
typedef struct Vector
{
    int8_t x;
    int8_t y;
} Vector;

// Struct for defining a 16x16-pixel block
typedef struct Block
{
  uint8_t pixel[SIZEOFBLOCK][SIZEOFBLOCK];
} Block;

// Struct for defining a 16x16-block Frame
typedef struct Frame
{
    Block block[NUMBLOCKS][NUMBLOCKS];
    uint32_t differences [NUMBLOCKS][NUMBLOCKS];
    Vector vectors [NUMBLOCKS][NUMBLOCKS];
} Frame;

// Struct for possible progression using multiple frames
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

// Method for Processing Frame
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

    uint8_t header_counter = 1;

    uint8_t rm_header [54];

    // Read the header and store it in an array (not to be used)
    for (uint8_t i = 0; i < 54; i++) {
        fread(&cur_pixel, sizeof(uint8_t), 1, fptr);
    }

    // Read the first pixel for iteration purposes
    fread(&cur_pixel, sizeof(uint8_t), 1, fptr);

    while (cur_pixel_row < SIZEOFIMAGE)
    {

        // Check for first iteration to prevent updating when cur_pixel_col == 0
        if (!first_iteration)
        {
            if (cur_pixel_col % SIZEOFBLOCK == 0)
            {
                cur_block = &cur_frame->block[cur_block_row][++cur_block_col];
            }

            if (cur_block_col == SIZEOFBLOCK)
            {
                cur_pixel_col = 0;
                cur_block_col = 0;
                cur_pixel_row++;

                if (cur_pixel_row % SIZEOFBLOCK == 0)
                {
                    cur_block_row++;
                }

                cur_block = &cur_frame->block[cur_block_row][cur_block_col];

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
    FILE *fptr;
    
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

    // Process the First Frame
    process_frame(test_frame, fptr);
    test_film->frame[0] = *test_frame;
    fclose(fptr);

    // Process the Second Frame
    fptr = fopen("test_images/Image2.bmp", "rb");
    process_frame(test_frame, fptr);
    test_film->frame[1] = *test_frame;
    fclose(fptr);

    for (uint8_t frame = 0; frame < NUMFRAMES - 1; ++frame) // every frame
    {
        for (uint8_t block_row_ref = 0; block_row_ref < NUMBLOCKS; ++block_row_ref) // every row in frame (block)
        {
            for (uint8_t block_col_ref = 0; block_col_ref < NUMBLOCKS; ++block_col_ref) // every column in frame (block)
            {
                for (uint8_t block_row_comp = max(0, block_row_ref - 3); block_row_comp < min(NUMBLOCKS, block_row_ref + 3); ++block_row_comp) // every block row in other frame
                {
                    for (uint8_t block_col_comp = max(0, block_col_ref - 3); block_col_comp < min(NUMBLOCKS, block_col_ref + 3); ++block_col_comp) // every block column in other frame
                    {
                        uint32_t temp_sad = 0;
                        for (uint8_t pixel_row = 0; pixel_row < SIZEOFBLOCK; ++pixel_row) // every row in cur_block (cur_pixel)
                        {
                            for (uint8_t pixel_col = 0; pixel_col < SIZEOFBLOCK; ++pixel_col) // every column in cur_block (cur_pixel)
                            {
                                int diff = test_film->frame[frame].block[block_row_ref][block_col_ref].pixel[pixel_row][pixel_col] - test_film->frame[frame + 1].block[block_row_comp][block_col_comp].pixel[pixel_row][pixel_col];
                                if (diff < 0)
                                {
                                    temp_sad -= diff;
                                }
                                else
                                {
                                    temp_sad += diff;
                                }
                            }   
                        }
                        /* Below Commented code is an artifact for determining shortest differences
                         *  double new_distance = sqrt((block_row_comp - block_row_ref) * (block_row_comp - block_row_ref) + (block_col_ref - block_col_comp) * (block_col_ref - block_col_comp));
                         *  double cur_distance = sqrt(test_film->frame[frame].vectors[block_row_ref][block_col_ref].x * test_film->frame[frame].vectors[block_row_ref][block_col_ref].x + test_film->frame[frame].vectors[block_row_ref][block_col_ref].y * test_film->frame[frame].vectors[block_row_ref][block_col_ref].y); */
                        if (test_film->frame[frame].differences[block_row_ref][block_col_ref] > temp_sad )//|| (new_distance < cur_distance && test_film->frame[frame].differences[block_row_ref][block_col_ref] == temp_sad))
                        {
                            // Update the differences and vectors
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
    return 0;
}