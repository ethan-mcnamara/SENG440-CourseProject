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
    FILE *fptr1, *fptr2;

    if (argc < 2)
    {
        printf("Please provide a file name\n");
        exit(1);
    }
    else
    {
        file_name = argv[1];
    }

    uint8x16_t Frame1[SIZEOFBLOCK][SIZEOFBLOCK][SIZEOFBLOCK];
    uint8x16_t Frame2[SIZEOFBLOCK][SIZEOFBLOCK][SIZEOFBLOCK];

    fptr1 = fopen("Image1.bmp", "rb");
    fptr2 = fopen("Image2.bmp", "rb");

    if(fptr1 == NULL || fptr2 == NULL)
    {
        printf("Error!");   
        exit(1);             
    }

    int block_row;
    for (block_row = 0; block_row < SIZEOFBLOCK; block_row++) {
        int row;
        uint8_t* cur_row1, cur_row2;
        for (row = 0; row < SIZEOFBLOCK; row++){
            int block;
            for (block = 0; block < SIZEOFBLOCK; block++) {
                fread(&cur_row1, sizeof(char)*16, 1, fptr1);
                fread(&cur_row2, sizeof(char)*16, 1, fptr2);
                Frame1[block_row][row][block] = vld1q_u8(cur_row1);
                Frame2[block_row][row][block] = vld1q_u8(cur_row2);
            }
        }
    }

    fclose(fptr1);
    fclose(fptr2);


    return 0;
}