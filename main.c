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

void process_frame(FILE *fptr)
{
    int iterator;
    uint8_t* cur_row;
    uint8x16_t Frame[SIZEOFBLOCK];

    for (iterator = 0; iterator < SIZEOFBLOCK; iterator++) {
        fread(&cur_row, sizeof(char)*16, 1, fptr);
        uint8x16_t v = {0,};
        uint8x16_t temp = {0, };
        Frame[iterator] = vaddq_u8(v, temp);
    }

    return;
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

    fptr = fopen("Image1.bmp", "rb");
    if(fptr == NULL)
    {
        printf("Error!");   
        exit(1);             
    }

    process_frame(fptr);
    fclose(fptr);

    return 0;
}