#include <arm_neon.h>
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
                fread(&cur_row1, sizeof(char)*16, 1, fptr1);
                fread(&cur_row2, sizeof(char)*16, 1, fptr2);
                Frame1[block_row][row][block_col] = &cur_row1;
                Frame2[block_row][row][block_col] = cur_row2;
            }
        }
    }

    fclose(fptr1);
    fclose(fptr2);

    uint8_t frame1br;
    for (frame1br = 0; frame1br < NUMBLOCKS; frame1br++) {
        uint8_t frame1bc;
        for (frame1bc = 0; frame1bc < NUMBLOCKS; frame1bc++) {
            uint8_t frame2br;
            for (frame2br = max(0, frame1br - 3); frame2br < min(NUMBLOCKS, frame1br + 3); ++frame2br) {
                uint8_t frame2bc;
                 for (frame2bc = max(0, frame1bc - 3); frame2bc < min(NUMBLOCKS, frame1bc+ 3); ++frame2bc) {
                    uint32_t temp_sad = 0;
                    uint8_t px_row;
                    for (px_row = 0; px_row < SIZEOFBLOCK; px_row++) {
                        const uint8x16_t Frame2Vector = {Frame2[frame2br][px_row][frame2bc][0], Frame2[frame2br][px_row][frame2bc][1],
                                                        Frame2[frame2br][px_row][frame2bc][2], Frame2[frame2br][px_row][frame2bc][3],
                                                        Frame2[frame2br][px_row][frame2bc][4], Frame2[frame2br][px_row][frame2bc][5],
                                                        Frame2[frame2br][px_row][frame2bc][6], Frame2[frame2br][px_row][frame2bc][7],
                                                        Frame2[frame2br][px_row][frame2bc][8], Frame2[frame2br][px_row][frame2bc][9],
                                                        Frame2[frame2br][px_row][frame2bc][10], Frame2[frame2br][px_row][frame2bc][11],
                                                        Frame2[frame2br][px_row][frame2bc][12], Frame2[frame2br][px_row][frame2bc][13],
                                                        Frame2[frame2br][px_row][frame2bc][14], Frame2[frame2br][px_row][frame2bc][15]}

                        const uint8x16_t Frame1Vector = {Frame1[frame1br][px_row][frame1bc][0],Frame1[frame1br][px_row][frame1bc][1],
                                                        Frame1[frame1br][px_row][frame1bc][2], Frame1[frame1br][px_row][frame1bc][3],
                                                        Frame1[frame1br][px_row][frame1bc][4], Frame1[frame1br][px_row][frame1bc][5],
                                                        Frame1[frame1br][px_row][frame1bc][6], Frame1[frame1br][px_row][frame1bc][7],
                                                        Frame1[frame1br][px_row][frame1bc][8], Frame1[frame1br][px_row][frame1bc][9],
                                                        Frame1[frame1br][px_row][frame1bc][10], Frame1[frame1br][px_row][frame1bc][11],
                                                        Frame1[frame1br][px_row][frame1bc][12], Frame1[frame1br][px_row][frame1bc][13],
                                                        Frame1[frame1br][px_row][frame1bc][14], Frame1[frame1br][px_row][frame1bc][15]}
                        uint8x16_t test = vabdq_u8(Frame1Vector, Frame2Vector);
                    }

                 }
            }
        }
    }

    return 0;
}