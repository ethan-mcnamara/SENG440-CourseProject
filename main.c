#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <math.h>
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

void process_frame(uint8_t Frame1[NUMBLOCKS][NUMBLOCKS][SIZEOFBLOCK][SIZEOFBLOCK], 
                   uint8_t Frame2[NUMBLOCKS][NUMBLOCKS][SIZEOFBLOCK][SIZEOFBLOCK])
{
    FILE *fptr1;
    FILE *fptr2;
    uint8_t rm_header1[54];
    uint8_t rm_header2[54];
    printf("after declaration of pointers\n");

    fptr1 = fopen("test_images/Image1.bmp", "rb");
    fptr2 = fopen("test_images/Image2.bmp", "rb");

    printf("after opening file pointers\n");

    // BMP header size is 54 bytes (8 bytes * 7 - 2 = 54 bytes)
    fread(&rm_header1, sizeof(uint8_t) * 7 - 2, 1, fptr1);
    fread(&rm_header2, sizeof(uint8_t)* 7 - 2, 1, fptr2);

    printf("after reading file headers\n");

    if(fptr1 == NULL || fptr2 == NULL)
    {
        printf("Error!");   
        exit(1);             
    }

    for (uint8_t block_row = 0; block_row < NUMBLOCKS; block_row++) {
        for (uint8_t pixel_row = 0; pixel_row < SIZEOFBLOCK; pixel_row++){
            for (uint8_t block_col = 0; block_col < NUMBLOCKS; block_col++) {
                printf("before writing to either frame\n");
                fread(&Frame1[block_row][block_col][pixel_row], sizeof(uint8_t)*16, 1, fptr1);
                fread(&Frame2[block_row][block_col][pixel_row], sizeof(uint8_t)*16, 1, fptr2);
                printf("after writing to both frames\n");
            }
        }
    }

    fclose(fptr1);
    fclose(fptr2);
    return;
}


/*
* Main function
*/
int main(int argc, char *argv[]) 
{
    printf("First line of main function\n");
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

    printf("Before declaration of frames\n");
    uint8_t Frame1[NUMBLOCKS][NUMBLOCKS][SIZEOFBLOCK][SIZEOFBLOCK];
    uint8_t Frame2[NUMBLOCKS][NUMBLOCKS][SIZEOFBLOCK][SIZEOFBLOCK];

    printf("before process_frame\n");

    process_frame(Frame1, Frame2);

    printf("after process_frame\n");

    uint32_t Differences[NUMBLOCKS][NUMBLOCKS] = { 0 };
    Vector vectors [NUMBLOCKS][NUMBLOCKS];


    //for each block in frame
    //    find most similar (smallest SAD) other block (limit search to nearby blocks)

    // fptr = fopen("test_images/Image1.bmp", "rb");
    // if(fptr == NULL)
    // {
    //     printf("Error!");   
    //     exit(1);             
    // }
    
    // Initialize the frame arrays
    process_frame(Frame1, Frame2);
    
    // Frame *test_frame = (Frame*)malloc(sizeof(Frame));
    // Film *test_film = (Film*) malloc(sizeof(Film));

    // process_frame(test_frame, fptr);

    // test_film->frame[0] = *test_frame;
    // fclose(fptr);
    // fptr = fopen("test_images/Image2.bmp", "rb");
    // process_frame(test_frame, fptr);
    // test_film->frame[1] = *test_frame;

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
                        uint8x16_t vector_ref; // declare a vector of 16 8-bit lanes
                        uint8x16_t vector_comp; // declare a vector of 16 8-bit lanes
                        vector_ref = vld1q_u8(Frame1[block_row_ref][block_col_ref][pixel_row]); // load the array from memory into a vector
                        vector_comp = vld1q_u8(Frame2[block_row_comp][block_row_ref][pixel_row]); // load the array from memory into a vector
                        
                        // Perform the Absolute Differences operation:
                        uint8x16_t init_result;
                        init_result = vabdq_u8(vector_ref, vector_comp);

                        // Store first and second halves of the result vector in two different vectors of half size
                        uint8x8_t result_high = vget_high_u8( init_result );
                        uint8x8_t result_low = vget_low_u8( init_result );

                        // Perform vector addition with the high and low vectors
                        uint16x8_t final_result = vaddl_u8( result_high, result_low);

                        // Sum all elements in the result vector by reading the lanes individually
                        temp_sad += vgetq_lane_u16(final_result, 0);
                        temp_sad += vgetq_lane_u16(final_result, 1);
                        temp_sad += vgetq_lane_u16(final_result, 2);
                        temp_sad += vgetq_lane_u16(final_result, 3);
                        temp_sad += vgetq_lane_u16(final_result, 4);
                        temp_sad += vgetq_lane_u16(final_result, 5);
                        temp_sad += vgetq_lane_u16(final_result, 6);
                        temp_sad += vgetq_lane_u16(final_result, 7);

                        printf("after calculating the temp_sad value\n");

                    }
                    if (Differences[block_row_ref][block_col_ref] > temp_sad )
                    {
                        printf("In if condition\n");
                        Differences[block_row_ref][block_col_ref] = temp_sad;
                        vectors[block_row_ref][block_col_ref].x = block_col_comp - block_col_ref;
                        vectors[block_row_ref][block_col_ref].y = block_row_ref - block_row_comp;
                    }
                }
            }
        }
    }

    
    for (int i = 0; i < NUMBLOCKS; ++i)
    {
        for (int j = 0; j < NUMBLOCKS; ++j)
        {
            int temp_diff = Differences[i][j];
            int temp_x = vectors[i][j].x;
            int temp_y = vectors[i][j].y;
            printf("Block[%d][%d]: Vector: (%d, %d); Difference: %d\n", i, j, temp_x, temp_y, temp_diff);
        }
    }

    return 0;
}