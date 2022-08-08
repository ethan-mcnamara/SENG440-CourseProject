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
            uint8_t frame2br;
            for (frame2br = max(0, frame1br - 3); frame2br < min(NUMBLOCKS, frame1br + 3); ++frame2br) {
                uint8_t frame2bc;
                 for (frame2bc = max(0, frame1bc - 3); frame2bc < min(NUMBLOCKS, frame1bc+ 3); ++frame2bc) {
                    uint32_t temp_sad = 0;
                    uint8_t px_row;
                    for (px_row = 0; px_row < SIZEOFBLOCK; px_row++) {

                        uint8_t* Frame_2_px = Frame2[frame2br][px_row][frame2bc];
                        const uint8x16_t Frame_2_Vector = {Frame_2_px[0], Frame_2_px[1],
                                                            Frame_2_px[2], Frame_2_px[3],
                                                            Frame_2_px[4], Frame_2_px[5],
                                                            Frame_2_px[6], Frame_2_px[7],
                                                            Frame_2_px[8], Frame_2_px[9],
                                                            Frame_2_px[10], Frame_2_px[11],
                                                            Frame_2_px[12], Frame_2_px[13],
                                                            Frame_2_px[14], Frame_2_px[15]};

                        uint8_t* Frame_1_px = Frame1[frame1br][px_row][frame1bc];
                        const uint8x16_t Frame_1_Vector = {Frame_1_px[0],Frame_1_px[1],
                                                            Frame_1_px[2], Frame_1_px[3],
                                                            Frame_1_px[4], Frame_1_px[5],
                                                            Frame_1_px[6], Frame_1_px[7],
                                                            Frame_1_px[8], Frame_1_px[9],
                                                            Frame_1_px[10], Frame_1_px[11],
                                                            Frame_1_px[12], Frame_1_px[13],
                                                            Frame_1_px[14], Frame_1_px[15]};

                        uint8x16_t sad = vabdq_u8(Frame_2_Vector, Frame_1_Vector);
                        
                        uint8_t px_i;
                        for (px_i = 0; px_i < SIZEOFBLOCK; px_i++) {
                            temp_sad += sad[px_i];
                        } 
                    }

                 }
            }
        }
    }

    return 0;
}