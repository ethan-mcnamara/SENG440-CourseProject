#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#include <arm_neon.h>

#define SIZEOFBLOCK 16
#define NUMFRAMES 2
#define NUMBLOCKS 16
#define SIZEOFIMAGE 256

/* General Barr C Compliance
*  =======================================================================
*  Barr C (1.3): Braces shall always surround the blocks of code. Moreover,
*                the braces on the left-hand side are always a line below.
*                This is evident throughout the code.
*  
*  Barr C (5.2): Whenever the width, in bits or bytes, of an integer value matters in
*                the program, one of the fixed width data types shall be used in place
*                of char, short, int, long, or long long. This is evident throughout the
*                code. Note, there is a double in the code, but it is only for testing
*                and is not an integer.
*
*  Barr C (6.3): Parameterized macros shall not be used if a function can be written
*                to accomplish the same behavior. Parameterized Macros could have been used
*                for the comp_zero and comp_max methods.
*
*  Barr C (8.1): The comma operator (,) shall not be used within variable
*                declarations. For reference, consider the File pointers in Process_Frame 
*                are on separate lines and the SAD local variables are declared separately
*
*  Barr C (8.4): With the exception of the initialization of a loop counter in the first clause of a
*                for statement and the change to the same variable in the third, no assignment
*                shall be made in any loop’s controlling expression. This is best shown in the for-loops
*                associated with SAD.
*/

/*
* Struct definition
*/

typedef struct
{
    int8_t x;
    int8_t y;

} Vector;

/*
* Function definitions
*/

// Barr C (6.3) prevented use of Macros. Also,
// the use of macros did not improve performance.
int8_t comp_zero(int8_t val_1)
{
    return (val_1 > 0) ? val_1 : 0;
}

// Barr C (6.3) prevented use of Macros
int8_t comp_max(int8_t val_1)
{
    return (val_1 > SIZEOFBLOCK) ? SIZEOFBLOCK : val_1;
}


void process_frame(uint8x16_t Frame1[NUMBLOCKS][NUMBLOCKS][SIZEOFBLOCK], 
                   uint8x16_t Frame2[NUMBLOCKS][NUMBLOCKS][SIZEOFBLOCK])
{
    FILE *fptr1;
    FILE *fptr2;
    uint8_t rm_header [54];

    fptr1 = fopen("test_images/Image1.bmp", "rb");
    fptr2 = fopen("test_images/Image2.bmp", "rb");

    // BMP header size is 54 bytes (8 bytes * 7 - 2 = 54 bytes)
    fread(&rm_header, sizeof(uint8_t) * 7 - 2, 1, fptr1);
    fread(&rm_header, sizeof(uint8_t)* 7 - 2, 1, fptr2);

    if(fptr1 == NULL || fptr2 == NULL)
    {
        printf("Error!");   
        exit(1);             
    }

    // The iteration is through block_col, and block_col is the second to last dimension.
    // The reason iteration is through block_col is because SAD iterates through pixel_row.
    // Also, int32_t is used because ARM does not support short ints or characters (assumes an entire
    // register).
    for (int32_t block_row = 0; block_row < NUMBLOCKS; block_row++) {
        for (int32_t pixel_row = 0; pixel_row < SIZEOFBLOCK; pixel_row++){
            for (int32_t block_col = 0; block_col < NUMBLOCKS; block_col++) {
                fread(&Frame1[block_row][block_col][pixel_row], sizeof(uint8_t)*16, 1, fptr1);
                fread(&Frame2[block_row][block_col][pixel_row], sizeof(uint8_t)*16, 1, fptr2);
            }
        }
    }

    fclose(fptr1);
    fclose(fptr2);
    return;
}

// This is just for testing purposes
void print_uint8 (uint8x16_t data) {
    int32_t i;
    static uint8_t p[16];

    vst1q_u8 (p, data);

    for (i = 0; i < 16; i++) {
	printf ("%02d ", p[i]);
    }
    printf ("\n");
}

/*
* Main function
*/
int main(int argc, char *argv[]) 
{
    // Uncomment for Timer
    // double time_spent = 0.0;
    // clock_t begin = clock();

    // Initialize the frames
    // Make the last frame be the vectors in order to prevent loading in the for-loop
    uint8x16_t Frame1[NUMBLOCKS][NUMBLOCKS][SIZEOFBLOCK];
    uint8x16_t Frame2[NUMBLOCKS][NUMBLOCKS][SIZEOFBLOCK];
    process_frame(Frame1, Frame2);

    // Differences does not need to be initialised due to the local variable `min_sad`
    uint32_t Differences[NUMBLOCKS][NUMBLOCKS];
    Vector vectors[NUMBLOCKS][NUMBLOCKS];

    // Create Local Variables to speed-up the process
    register uint32_t temp_sad = 0;
    register uint32_t min_sad = UINT32_MAX;
    uint8_t x_displ = 0;
    uint8_t y_displ = 0;

    // Start calculating the SAD values.
    // Barr C (8.4) shows proper definitions of for-loops.
    for (int32_t block_row_ref = 0; block_row_ref < NUMBLOCKS; ++block_row_ref) // every row in frame (block)
    {
        for (int32_t block_col_ref = 0; block_col_ref < NUMBLOCKS; ++block_col_ref) // every column in frame (block)
        {
            for (int32_t block_row_comp = comp_zero(block_row_ref - 3); block_row_comp < comp_max(block_row_ref + 3); ++block_row_comp) // every block row in other frame
            {
                for (int32_t block_col_comp = comp_zero(block_col_ref - 3); block_col_comp < comp_max(block_col_ref + 3); ++block_col_comp) // every block column in other frame
                {
                    temp_sad &= 0;
                    for (int32_t pixel_row = 0; pixel_row < SIZEOFBLOCK; ++pixel_row) // every row in cur_block (cur_pixel)
                    {
                        // No longer need to load the arrays
                        uint8x16_t vector_ref = Frame1[block_row_ref][block_col_ref][pixel_row]; // declare a vector of 16 8-bit lanes
                        uint8x16_t vector_comp = Frame2[block_row_comp][block_col_comp][pixel_row]; // declare a vector of 16 8-bit lanes

                        // Perform the Absolute Differences operation:
                        uint8x16_t init_result = vabdq_u8(vector_ref, vector_comp);

                        // Store first and second halves of the result vector in two different vectors of half size
                        uint8x8_t result_high = vget_high_u8( init_result );
                        uint8x8_t result_low = vget_low_u8( init_result );

                        // Perform vector addition with the high and low vectors.
                        // This shortens number of needed calculations below.
                        // Use vaddl_u8 to prevent overflow
                        uint16x8_t final_result = vaddl_u8( result_high, result_low);

                        // Sum all elements in the result vector by reading the lanes individually
                        // A for-loop would add additional operations that are not necessary (operations require consts)
                        temp_sad += vgetq_lane_u16(final_result, 0);
                        temp_sad += vgetq_lane_u16(final_result, 1);
                        temp_sad += vgetq_lane_u16(final_result, 2);
                        temp_sad += vgetq_lane_u16(final_result, 3);
                        temp_sad += vgetq_lane_u16(final_result, 4);
                        temp_sad += vgetq_lane_u16(final_result, 5);
                        temp_sad += vgetq_lane_u16(final_result, 6);
                        temp_sad += vgetq_lane_u16(final_result, 7);
                    }

                    if (min_sad > temp_sad )
                    {
                        min_sad = temp_sad;
                        x_displ = block_col_comp - block_col_ref;
                        y_displ = block_row_ref - block_row_comp;
                    }
                }
            }
            // Assign the local values to the array (less memory handling)
            Differences[block_row_ref][block_col_ref] = min_sad;
            vectors[block_row_ref][block_col_ref].x = x_displ;
            vectors[block_row_ref][block_col_ref].y = y_displ;
            min_sad = UINT32_MAX;
        }
    }

    // Uncomment for Timer
    // clock_t end = clock();
    // time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    // printf("The elapsed time is %f seconds", time_spent);


    for (int32_t i = 0; i < NUMBLOCKS; ++i)
    {
        for (int32_t j = 0; j < NUMBLOCKS; ++j)
        {
            int temp_diff = Differences[i][j];
            int temp_x = vectors[i][j].x;
            int temp_y = vectors[i][j].y;
            printf("Block[%d][%d]: Vector: (%d, %d); Difference: %d\n", i, j, temp_x, temp_y, temp_diff);
        }
    }

    return 0;
}