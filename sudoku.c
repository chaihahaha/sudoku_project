#include<stdio.h>
#include<stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define MAX_GRIDS 100
char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}
//char digits[9] = "123456789";

// row coordinate of a grid indicated by a char from 'A' to 'I'
char rows[9] = "ABCDEFGHI";

// column coordinate of a grid indicated by a char from '1' to '9'
char cols[9] = "123456789";

// all the row-column char pair of the grids on the board
char squares[81][2]; // 81 

// all the row-column char pair of the units with all different constraint on the board
char unitlist[27][9][2]; // 27x9

// the dictionary look for the units a char pair is in 
char units[81][3][9][2];

// the dictionary looking for all peers (grids with all diff constraint) of a char pair
char peers[81][20][2];

void print_pair(char pair[2])
{
    printf("%c%c ",pair[0],pair[1]);
}

char hash(char pair[2])
{
    // return the hash of a char pair
    // assuming pair[0] is a char from 'A' to 'I'
    // pair[1] is a char from '1' to '9'
    char row=pair[0];
    char col=pair[1];
    return 9*(row-65)+col-49;
}
bool in(char pair[2], char array[][2], int length)
{
    // if char pair "pair" (coordinate of a grid) is in a char-pair array "array" of length "length", return true, else return false
    for(int i=0;i<length;i++)
    {
        if(hash(pair)==hash(array[i]))
        {
            return true;
        }
    }
    return false;
}

void initialize_array()
{
    // initialize all the global variables
    for(int i=0;i<81;i++)
    {
        squares[i][0]=rows[i/9];
        squares[i][1]=cols[i%9];
    }
    for(int i=0;i<9;i++)
    {
        for(int j=0;j<9;j++)
        {
            unitlist[i][j][0]=rows[j];
            unitlist[i][j][1]=cols[i];
        }
    }
    for(int i=9;i<18;i++)
    {
        for(int j=0;j<9;j++)
        {
            unitlist[i][j][0]=rows[i-9];
            unitlist[i][j][1]=cols[j];
        }
    }
    for(int i=18;i<27;i++)
    {
        for(int j=0;j<9;j++)
        {
            unitlist[i][j][0]=rows[(i-18)/3*3+j/3];
            unitlist[i][j][1]=cols[(i-18)%3*3+j%3];
        }
    }
    printf("squares initialized\n");
    //for(int i=0;i<81;i++)
    //{
    //    print_pair(squares[i]);
    //}
    //printf("\n");

    printf("unit list initialized\n");
    //for(int i=0;i<27;i++)
    //{
    //    for(int j=0;j<9;j++)
    //    {
    //        print_pair(unitlist[i][j]);
    //    }
    //    printf("\n");
    //}
    //printf("\n");
    
    for(int i=0; i<81; i++)
    {
        int count=0;
        for(int j=0; j<27; j++)
        {
            if(in(squares[i], unitlist[j],9))
            {
                for(int k=0; k<9; k++)
                {
                    units[i][count][k][0]=unitlist[j][k][0];
                    units[i][count][k][1]=unitlist[j][k][1];
                }
                count++;
            }
            if(count>2)
            {
                break;
            }
        }
    }
    printf("units initialized\n");
    //for(int i=0; i<81; i++)
    //{
    //    print_pair(squares[i]);
    //    printf(": \n[");
    //    for(int j=0; j<3; j++)
    //    {
    //        for(int k=0; k<9; k++)
    //        {
    //            print_pair(units[i][j][k]);
    //        }
    //        printf("\n");
    //    }
    //    printf("]\n");
    //}

    for(int i=0; i<81; i++)
    {
        bool hasht[81]={false};
        for(int j=0; j<3; j++)
        {
            for(int k=0; k<9; k++)
            {
                if(hash(squares[i])!=hash(units[i][j][k]))
                {
                    hasht[hash(units[i][j][k])]=true;
                }
            }
        }
        int count = 0;
        for(int j=0; j<81; j++)
        {
            if(hasht[j])
            {
                peers[i][count][0]=squares[j][0];
                peers[i][count][1]=squares[j][1];
                count++;
            }
            if(count>20)
            {
                break;
            }
        }
    }

    printf("peers initialized\n");
    //for(int i=0; i<81; i++)
    //{
    //    print_pair(squares[i]);
    //    printf(": \n[");
    //    for(int k=0; k<20; k++)
    //    {
    //        print_pair(peers[i][k]);
    //    }
    //    printf("\n");
    //    printf("]\n");
    //}
    
}
void grid_values(char* grid, bool values[81][9])
{
    for(int j=0; j<81; j++)
    {
        if(grid[j]=='0' || grid[j]=='.')
        {
            for(int k=0; k<9; k++)
            {
                values[j][k] = true;
            }
        }
        else if(grid[j]>=49 || grid[j]<=57)
        {
            for(int k=0; k<9; k++)
            {
                if(k==grid[j]-49)
                {
                    values[j][k] = true;
                }
                else
                    values[j][k] = false;
            }
        }
    }
}
void from_file(char* filename, char sep, bool values[MAX_GRIDS][81][9], int* n)
{
    FILE* f;
    f = fopen(filename, "r");
    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* buffer = malloc(length);
    if(buffer)
    {
        fread(buffer, 1, length, f);
    }
    fclose(f);

    *n = 0;
    if(buffer)
    {
        char** grids = str_split(buffer, sep);
        if(grids)
        {
            int i;
            for(i=0; *(grids + i); i++)
            {
                char* grid = *(grids + i);
                grid_values(grid, values[*n]);
                free(*(grids + i));
                (*n)++;
            }
            printf("\n");
            free(grids);
        }
    }

}
int main()
{
    initialize_array();
    bool values[MAX_GRIDS][81][9];
    int n;
    from_file("top95.txt",'\n',values,&n);
    printf("%d\n", n);
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<81; j++)
        {
            for(int k=0; k<9; k++)
            {
                printf("%d ",values[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }
    return 0;
}
