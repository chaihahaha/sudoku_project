#include<stdio.h>
#include<stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


// the dictionary to look for the units where (squares in a unit are diff) a square is placed
int units[81][3][9];

// the dictionary to look for all peers (squares with all diff constraint) of a square
int peers[81][20];

bool in(int s, int array[], int length);
void initialize_array();
int n_left(bool values[9], int* d);
int n_left_units(bool values[81][9], int u, int d, int s, int* ps);
bool eliminate(bool values[81][9], int s, int d);
bool assign(bool values[81][9], int s, int d);
void grid_values(char* grid, bool values[81][9]);
char** from_file(char* filename, char sep);
void parse_grid(char** grids, int n, bool values[81][9]);
bool search(bool values[81][9]);
bool solved(bool values[81][9]);

int main()
{
    initialize_array();
    char** grids = from_file("top95.txt",'\n');

    bool values[81][9];
    
    int n=0;
    while(*(grids+n))
    {
        parse_grid(grids,n,values);
        bool solved = search(values);
        printf("Solved? %s\n",solved?"Yes!":"No");
        for(int j=0; j<81; j++)
        {
            int d;
            n_left(values[j], &d);
            printf("%d ",d+1);
            if(j%9==8)
                printf("\n");
        }
        printf("\n");
        n++;
    }
    free(grids);
    return 0;
}
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

bool in(int s, int array[], int length)
{
    // if "s" is in "array" of "length", return true, else return false
    for(int i=0;i<length;i++)
    {
        if(s==array[i])
        {
            return true;
        }
    }
    return false;
}

void initialize_array()
{
    // all the squares of the units with all different constraint on the board
    int unitlist[27][9]; // 27x9
    // initialize all the global variables
    for(int i=0;i<9;i++)
    {
        for(int j=0;j<9;j++)
        {
            unitlist[i][j]=j*9+i;
        }
    }
    for(int i=9;i<18;i++)
    {
        for(int j=0;j<9;j++)
        {
            unitlist[i][j]=(i-9)*9+j;
        }
    }
    for(int i=18;i<27;i++)
    {
        for(int j=0;j<9;j++)
        {
            unitlist[i][j]=((i-18)/3*3+j/3)*9+(i-18)%3*3+j%3;
        }
    }

    //printf("unit list initialized\n");
    //for(int i=0;i<27;i++)
    //{
    //    for(int j=0;j<9;j++)
    //    {
    //        printf("%d ",unitlist[i][j]);
    //    }
    //    printf("\n");
    //}
    //printf("\n");
    
    for(int i=0; i<81; i++)
    {
        int count=0;
        for(int j=0; j<27; j++)
        {
            if(in(i, unitlist[j],9))
            {
                for(int k=0; k<9; k++)
                {
                    units[i][count][k]=unitlist[j][k];
                }
                count++;
            }
            if(count>2)
            {
                break;
            }
        }
    }
    //printf("units initialized\n");
    //for(int i=0; i<81; i++)
    //{
    //    print_pair(squares[i]);
    //    printf(": \n[");
    //    for(int j=0; j<3; j++)
    //    {
    //        for(int k=0; k<9; k++)
    //        {
    //            printf("%d ",units[i][j][k]);
    //        }
    //        printf("\n");
    //    }
    //    printf("]\n");
    //}

    for(int i=0; i<81; i++)
    {
        bool hasht[81]={0};
        for(int j=0; j<3; j++)
        {
            for(int k=0; k<9; k++)
            {
                if(i!=units[i][j][k])
                {
                    hasht[units[i][j][k]]=true;
                }
            }
        }
        int count = 0;
        for(int j=0; j<81; j++)
        {
            if(hasht[j])
            {
                peers[i][count]=j;
                count++;
            }
            if(count>20)
            {
                break;
            }
        }
    }

    //printf("peers initialized\n");
    //for(int i=0; i<81; i++)
    //{
    //    print_pair(squares[i]);
    //    printf(": \n[");
    //    for(int k=0; k<20; k++)
    //    {
    //        printf("%d ",peers[i][k]);
    //    }
    //    printf("\n");
    //    printf("]\n");
    //}
    
}
int n_left(bool values[9], int* d)
{
    // count the nbr of possible values left in domain "values", if only one left, indicate it by digit "d"
    int count=0;
    for(int i=0; i<9; i++)
    {
        if(values[i])
        {
            *d = i;
            count++;
        }
    }
    return count;
}
int n_left_units(bool values[81][9], int u, int d, int s, int* ps)
{
    // count nbr of squares of a unit "u" of "s" where "d" can be placed, if only one left, indicate the square by "ps"
    int count = 0;
    for(int i=0; i<9; i++)
    {
        // if "d" can be place in one square "i" of the "u"th unit of "s", i.e. can be placed in "s1"
        int s1 = units[s][u][i];
        if(values[s1][d])
        {
            // record the square
            *ps = s1;
            count++;
        }
    }
    return count;
}
bool eliminate(bool values[81][9], int s, int d)
{
    // eliminate "d" from the domain of "s", if domain is empty, contradict and return false, else return true
    if(!values[s][d])
        return true;
    values[s][d] = false;
    int d2;
    int n = n_left(values[s],&d2);
    if(n==0)
    {
        return false;
    }
    if(n==1)
    {
        int count = 0;
        for(int s2=0; s2<20; s2++)
        {
            if(!eliminate(values, peers[s][s2], d2))
            {
                count++;
            }
        }
        if(count>0)
        {
            return false;
        }
    }
    for(int u=0; u<3; u++)
    {
        int ps;
        int n=n_left_units(values, u, d, s, &ps);
        if(n==0)
        {
            return false;
        }
        if(n==1)
        {
            if(!assign(values, ps, d))
            {
                return false;
            }
        }
        
    }
    return true;

}
bool assign(bool values[81][9], int s, int d)
{
    // assign "d" to the domain of "s" and eliminate all the other values, if contradict return false, else return true
    int count = 0;
    for(int d2=0; d2<9; d2++)
    {
        if(d2!=d && values[s][d2])
        {
            if(!eliminate(values, s, d2))
            {
                count++;
            }
        }
    }
    if(count>0)
    {
        return false;
    }
    return true;

}
void grid_values(char* grid, bool values[81][9])
{
    // assign values to the domain of squares according to a grid string
    for(int j=0; j<81; j++)
    {
            for(int k=0; k<9; k++)
            {
                values[j][k] = true;
            }
    }
    int idx=0, count=0;
    while(grid[idx]!='\0' && count<81)
    {
        if(grid[idx]>=49 && grid[idx]<=57)
        {
            if(!assign(values, count, grid[idx]-49))
            {
                printf("assigning failed!\n");
            }
            count++;
        }
        if(grid[idx]=='0' || grid[idx]=='.')
        {
            count++;
        }
        idx++;
    }
}
char** from_file(char* filename, char sep)
{
    // read from a files many grids, split it and return the array of strings
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
    char** grids;
    if(buffer)
    {
        grids = str_split(buffer, sep);
    }

    return grids;
}
void parse_grid(char** grids, int n, bool values[81][9])
{
    // parse the grids string and assign values grid by grid
    char* grid = *(grids+n);
    grid_values(grid, values);
    free(*(grids+n));
}
bool solved(bool values[81][9])
{
    // decide whether a grid is solved by scanning the values of domains of its squares
    int n, d2;
    for(int s=0; s<81; s++)
    {
        n = n_left(values[s], &d2);
        if(n>1)
        {
            return false;
        }
    }
    return true;
}
bool search(bool values[81][9])
{
    // depth first search trying to assign possible values to squares with help of constraint propagation
    if(solved(values))
        return true;
    int min=10, s=0;
    for(int i=0; i<81; i++)
    {
        int d2;
        int n=n_left(values[i], &d2);
        if(n<min && n>=2)
        {
            min=n;
            s=i;
        }
    }
    for(int d=0; d<9; d++)
    {
        if(values[s][d])
        {
            // back up values by deep copying
            bool values_cp[81][9];
            for(int i=0; i<81; i++)
            {
                for(int j=0; j<9; j++)
                {
                    values_cp[i][j]= values[i][j];
                }
            }
            if(assign(values_cp, s, d))
            {
                // assigning is successful
                bool solved = search(values_cp);
                if(solved)
                {
                    // if solved copy back
                    for(int i=0; i<81; i++)
                    {
                        for(int j=0; j<9; j++)
                        {
                            values[i][j]= values_cp[i][j];
                        }
                    }
                    return true;
                }
            }
        }
    }
    return false;
}

