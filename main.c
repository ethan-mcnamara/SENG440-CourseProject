#include "arm_neon.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>

#define SIZEOFBLOCK 16
#define NUMFRAMES 2
#define NUMBLOCKS 16
#define SIZEOFIMAGE 256

int8_t max(int8_t val_1, int8_t val_2)
{
    return (val_1 < val_2) ? val_2 : val_1;
}

int8_t min(int8_t val_1, int8_t val_2)
{
    return (val_1 > val_2) ? val_2 : val_1;
}

/*
* Main function
*/
int main(int argc, char *argv[]) 
{
    char* file_name;
    FILE *fptr1;
    FILE *fptr2;

    if (argc < 2)
    {
        printf("Please provide a file name\n");
        exit(1);
    }
    else
    {
        file_name = argv[1];
    }

    uint8_t Frame1[NUMBLOCKS][NUMBLOCKS][SIZEOFBLOCK][SIZEOFBLOCK];
    uint8_t Frame2[NUMBLOCKS][NUMBLOCKS][SIZEOFBLOCK][SIZEOFBLOCK];
    uint32_t Differences[NUMBLOCKS][NUMBLOCKS];
    uint32_t x_vector[NUMBLOCKS][NUMBLOCKS];
    uint32_t y_vector[NUMBLOCKS][NUMBLOCKS];

    fptr1 = fopen("Image1.bmp", "rb");
    fptr2 = fopen("Image1.bmp", "rb");

    if(fptr1 == NULL || fptr2 == NULL)
    {
        printf("Error!");   
        exit(1);             
    }

    int block_row;
    for (block_row = 0; block_row < NUMBLOCKS; block_row++) {
        int row;
        uint8_t* cur_row1;
        uint8_t* cur_row2;
        for (row = 0; row < SIZEOFBLOCK; row++){
            int block_col;
            for (block_col = 0; block_col < NUMBLOCKS; block_col++) {
                fread(&Frame1[block_row][row][block_col], sizeof(char)*16, 1, fptr1);
                fread(&Frame2[block_row][row][block_col], sizeof(char)*16, 1, fptr2);
            }
        }
    }

    fclose(fptr1);
    fclose(fptr2);

    uint8_t frame1br;
    for (frame1br = 0; frame1br < NUMBLOCKS; frame1br++) {
        uint8_t frame1bc;
        for (frame1bc = 0; frame1bc < NUMBLOCKS; frame1bc++) {
            uint32_t max_sad = 0;
            uint32_t x_displ = 0;
            uint32_t y_displ = 0;
            uint8_t frame2br;
            for (frame2br = max(0, frame1br - 3); frame2br < min(NUMBLOCKS, frame1br + 3); ++frame2br) {
                uint8_t frame2bc;
                uint32_t temp_sad = 0;
                for (frame2bc = max(0, frame1bc - 3); frame2bc < min(NUMBLOCKS, frame1bc+ 3); ++frame2bc) {
                    uint8_t px_row;
                    for (px_row = 0; px_row < SIZEOFBLOCK; px_row++) {
                        const uint8x16_t Frame_2_Vector = vld1q_u8(Frame2[frame2br][px_row][frame2bc]);
                        const uint8x16_t Frame_1_Vector = vld1q_u8(Frame1[frame1br][px_row][frame1bc]);

                        uint8x16_t sad = vabdq_u8(Frame_2_Vector, Frame_1_Vector);
                        // Cannot use accumulate function
                        uint8_t px_i;
                        for (px_i = 0; px_i < SIZEOFBLOCK; px_i++) {
                            temp_sad += vgetq_lane_u8(sad, px_i);
                        }
                    }
                    if (max_sad > temp_sad) {
                        max_sad = temp_sad;
                        // Confirm these changes are correct with Ethan
                        x_displ = frame2bc - frame1bc;
                        y_displ = frame1br - frame2br;
                    }
                }
            }
            Differences[frame1br][frame1bc] = max_sad;
            x_vector[frame1br][frame1bc] = x_displ;
            y_vector[frame1br][frame1bc] = y_displ;
        }
    }

    uint8_t print_i;
    for (print_i = 0; print_i < NUMBLOCKS; ++print_i)
    {
        uint8_t print_j;
        for (print_j = 0; print_j < NUMBLOCKS; ++print_j)
        {
            int temp_diff = Differences[print_i][print_j];
            int temp_x = x_vector[print_i][print_j];
            int temp_y = y_vector[print_i][print_j];
            printf("Block[%d][%d]: Vector: (%d, %d); Difference: %d\n", print_i, print_j, temp_x, temp_y, temp_diff);
        }
    }

    return 0;
}