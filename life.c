/**
 * @file life.c
 * @brief Functions of convey's game of life
 * @details 
 * Functions include below:
 * Clear array from Memory
 * Intailize array to 0
 * Update a cell's life status to next generatoin.
 * Simulate cells on the edge (hedge, torus, klein)
 * Simulate cells in the middle
 * Import patterns to matrix
 * Clear array from memoary
 * @author Tommy Pham
 * @date Fall 2020
 * @bugs None
 * @todo none
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "life.h"

/** 
 * @brief Deletes the matrix space in memory
 * @param m the matrix to be freed
 * @param row the number of rows to be freed
 * @param i index of rows.
 */
void free_matrix(unsigned char **m, int m_row){
    int i;
    for (i = 0; i < m_row; i++)
        free(m[i]);
    free(m);
}

/** 
 * @brief Creates the matrix and initializes all values to 0. Return adress of Matrix is sussesful or NULL if malloc failed.
 * @details Shift x and y coordinites normal range. Flips Y if klevin.
 * @param rows The number of rows
 * @param cols The number of columns
 * @param return M is success. NULL is calloc fail.
 * @param i index or row
 * @param j index of collum
 * @param m The array of rows in the matrix
 */
unsigned char **init_matrix(int rows, int cols)
{
    unsigned char **m;
    int i, j;
    /* allocate rows */
    m = malloc(rows * sizeof(int *));
    if(!m)
        return NULL;
    for(i = 0; i < rows; i++) {
        /* allocate cols for each row */
        m[i] = calloc(cols, sizeof(int));
        if (!m[i]) {
            for (j = 0; j < i; j++)
                free(m[j]);
            free(m);
            return NULL;
        }

    }
    return m;
}

/** 
 * @brief Adjust the relitive pattern coordinates to the matrix with a spefic edge type.
 * @param p Present Matrix - Current Generation
 * @param m_row numbers of rows in matrix
 * @param n_col numbers of collums in matrix
 * @param type type of edge - hedge, torus, klein
 * @param x relative x cordinate for pattern
 * @param y relative y cordinate for pattern
 */
void cell_in(unsigned char **p, int m_row, int n_col, char type, int x, int y){

    switch(type){
        case 'h':
            if( x >= 0 && x < n_col && y >= 0 && y < m_row )
                p[y][x] = 1;
            else{
                printf("Cell out of bound\n");
                exit(EXIT_FAILURE);
            }
            break;
        case 't':
            while(y < 0)
                y += m_row;
            while(y >= m_row)
                y -= m_row;
            while(x < 0)
                x += n_col;
            while(y >= n_col)
                x -= n_col;
            p[y][x] = 1;
            break;
        case 'k':
            while(y < 0)
                y += m_row;
            while(y >= m_row)
                y -= m_row;
            n_col*=2;
            while(x < 0)
                x += n_col;
            while(x >= n_col)
                x -= n_col;
            if(x >= 3){
                y = m_row - y - 1;
                x -= n_col/2;
            }
            p[y][x] = 1;
            break;
    }
}

/** 
 * @brief Inport a parttern from a file.
 * @details Compatible with 1.05 (Type 5) and 1.06 (Type 6). Find coordinates of the live cells in pattern and sent it to cell_in().
 * @param p Present Matrix - Current Generation
 * @param type type of edge - hedge, torus, klein
 * @param fp file containing pattern
 * @param r Row of cell to update
 * @param x initial x cordinate for pattern
 * @param y initial y cordinate for pattern
 * @param m_row numbers of rows in matrix
 * @param n_col numbers of collums in matrix
 * @param m_x x coordinate of the pattern
 * @param m_y t coordinate of the pattern
 * @param chunk buffer for to input each lines of the file
 */
void pattern_in(unsigned char **p, unsigned char type, FILE *fp, int x, int y, int m_row, int n_col){

    char chunk[128]; int m_x, m_y;

    fgets(chunk, sizeof(chunk), fp);

    switch(chunk[9]){
        case '5':
            while(fgets(chunk, sizeof(chunk), fp) != NULL){
                if(chunk[1] == 'P'){
                    if(x == 0 || y == 0)
                        sscanf(chunk,"#P%d %d",&x, &y);
                    m_x = 0; m_y = 0;
                }
                else if(chunk[0] == '.' || chunk[0] == '*'){
                    while(chunk[m_x] != '\r'){
                        if(chunk[m_x] == '*'){
                            cell_in(p, m_row, n_col, type, m_x+x, m_y+y);
                        }
                        m_x++;
                    }
                    m_x = 0;
                    m_y++;
                }
            }
            break;
        case '6':
            while(fgets(chunk, sizeof(chunk), fp) != NULL){
                sscanf(chunk,"%d %d",&m_x, &m_y);
                cell_in(p, m_row, n_col, type, m_x+x, m_y+y);
            }
            break;
        default:
            printf("File version can not found.\n");

    }

}

/**
 * @brief Prints the contents of the given matrix to the terminal.
 * @param matrix The matrix
 * @param m_row The number of rows
 * @param n_col The number of columns
 */
void print_matrix(unsigned char **matrix, int m_row, int n_col){
    for (int i = 0; i < m_row; i++) {
        for (int j = 0; j < n_col; j++) {
            if(matrix[i][j] == 0)
                printf("⬜");
            else
                printf("⬛");
        }
        printf("\n");
    }
    printf("\n");
}

/** 
 * @brief Evalute a cell life status and update it on the next generation.
 * @param p Present Matrix - Current Generation
 * @param f Future Matrix - Next Generation
 * @param r Row of cell to update
 * @param c Collum of cell to update
 * @param n Number of neighbors
 */
void update(unsigned char **p, unsigned char **f, int r, int c, char n){

    if(p[r][c] == 1){
        if( n == 2 || n == 3 ){
            //printf("1");
            f[r][c] = 1;}
        else{
            //printf("2");
            f[r][c] = 0;}
        }
    else{
        if( n == 3 )
            f[r][c] = 1;
        else
            f[r][c] = 0;
    }
}

/** 
 * @brief Check and update life status of cell in the middle and note edge of a given matrix.
 * @details First for loop check for left and right collum. Secound for loop check for top and bottom row.
 * @param p Present Matrix - Current Generation
 * @param f Future Matrix - Next Generation
 * @param r Max row of matrix
 * @param c Max collum of matrix
 */
void mid(unsigned char **p, unsigned char **f, int r, int c){
    r--; c--;
    int row, col, n;
    for(row = 1; row < r; row++){
        for(col = 1; col < c; col++){
            n = ( p[(row-1)][(col-1)] + p[(row-1)][col] + p[(row-1)][(col+1)] + p[row][(col+1)] + p[(row+1)][(col+1)] + p[(row+1)][(col)] + p[(row+1)][(col-1)] + p[row][(col-1)]);
            update(p, f, row, col, n);
        }
    }
}

/** 
 * @brief Hedge Edge - Check and update life status of cell on the all edge with hedge properties.
 * @param p Present Matrix - Current Generation
 * @param f Future Matrix - Next Generation
 * @param r Max row of matrix
 * @param c Max collum of matrix
 * @param i_r Index of row
 * @param i_c Index of collums
 * @param n number of cell's neighbors
 */
void hedge(unsigned char **p, unsigned char **f, int r, int c){
    
    r--;c--;
    int i_r, i_c, n;

    for(i_r = 1; i_r < r; i_r++){
        n = (p[(i_r-1)][0] + p[(i_r-1)][1] + p[i_r][1] + p[(i_r+1)][1] + p[(i_r+1)][0]);
        update(p, f, i_r, 0, n);
        n = (p[(i_r-1)][c] + p[(i_r-1)][c-1] + p[i_r][c-1] + p[(i_r+1)][c-1] + p[(i_r+1)][c]);
        update(p, f, i_r, c, n);
    }    

    for(i_c = 1; i_c < c; i_c++){
        n = ( p[0][(i_c+1)] + p[1][(i_c+1)] + p[1][i_c] + p[1][(i_c-1)] + p[0][(i_c-1)] );
        update(p, f, 0, i_c, n);
        n = ( p[r][(i_c-1)] + p[(r-1)][(i_c-1)] + p[(r-1)][i_c] + p[(r-1)][(i_c+1)] + p[r][(i_c+1)] );
        update(p, f, r, i_c, n);
    } 

    n = ( p[0][1] + p[1][1] + p[1][0] );
    update(p, f, 0, 0, n);

    n = ( p[1][c] + p[1][(c-1)] + p[0][(c-1)] );
    update(p, f, 0, c, n);

    n = ( p[r][(c-1)] + p[(r-1)][(c-1)] + p[(r-1)][c] );
    update(p, f, r, c, n);

    n = ( p[r-1][0] + p[(r-1)][1] + p[r][1] );
    update(p, f, r, 0, n);
}

/** 
 * @brief Torus - Check and update life status of cell on the all sides with torus property.
 * @details First for loop check for left and right collum. Secound for loop check for top and bottom row.
 * @param p Present Matrix - Current Generation
 * @param f Future Matrix - Next Generation
 * @param r Max row of matrix
 * @param c Max collum of matrix
 * @param i_r Index of row
 * @param i_c Index of collums
 * @param n number of cell's neighbors
 */
void torus(unsigned char **p, unsigned char **f, int r, int c){
    
    r--; c--;
    int i_r, i_c, n;

    for(i_r = 1; i_r < r; i_r++){
        n = ( p[(i_r-1)][0] + p[(i_r-1)][1] + p[i_r][1] + p[(i_r+1)][1] + p[(i_r+1)][0] + p[(i_r+1)][c] + p[i_r][c] + p[(i_r-1)][c] );
        update(p, f, i_r, 0, n);
        n = ( p[(i_r-1)][c] + p[(i_r-1)][c-1] + p[i_r][c-1] + p[(i_r+1)][c-1] + p[(i_r+1)][c] + p[(i_r+1)][0] + p[i_r][0] + p[(i_r-1)][0] );
        update(p, f, i_r, c, n);       
        }    

    for(i_c = 1; i_c < c; i_c++){
        n =( p[r][(i_c-1)] + p[r][i_c] + p[r][(i_c+1)] + p[0][(i_c+1)] + p[1][(i_c+1)] + p[1][i_c] + p[1][(i_c-1)] + p[0][(i_c-1)] );
        update(p, f, 0, i_c, n);
        n = ( p[r][(i_c-1)] + p[(r-1)][(i_c-1)] + p[(r-1)][i_c] + p[(r-1)][(i_c+1)] + p[r][(i_c+1)] + p[0][(i_c+1)] + p[0][(i_c)] + p[0][(i_c-1)] );
        update(p, f, r, i_c, n);
    } 
    n = ( p[0][1] + p[1][1] + p[1][0] + p[1][c] + p[0][c] + p[r][c] + p[r][0] + p[r][1] );
    update(p, f, 0, 0, n); 

    n = ( p[1][c] + p[1][(c-1)] + p[0][(c-1)] + p[r][(c-1)] + p[r][c] + p[r][0] + p[0][0] + p[1][0] );
    update(p, f, 0, c, n);

    n = ( p[r][(c-1)] + p[(r-1)][(c-1)] + p[(r-1)][c] + p[(r-1)][0] + p[r][0] + p[0][0] + p[0][c] + p[0][(c-1)]);
    update(p, f, r, c, n);

    n = ( p[r-1][0] + p[(r-1)][1] + p[r][1] + p[0][1] + p[0][0] + p[0][c] + p[r][c] + p[r-1][c] );
    update(p, f, r, 0, n);
}

/** 
 * @brief Klein Bottle - Check and update life status of cell on the all sides with kelin properties.
 * @details First for loop check for left and right collum. Secound for loop check for top and bottom row.
 * @param p Present Matrix - Current Generation
 * @param f Future Matrix - Next Generation
 * @param r Max row of matrix
 * @param c Max collum of matrix
 * @param i_r Index of row
 * @param i_c Index of collums
 * @param n number of cell's neighbors
 */
void klein(unsigned char **p, unsigned char **f, int r, int c){
    
    r--; c--;
    int i_r, i_c, n;

    // Check Left and Right Collum
    for(i_r = 1; i_r < r; i_r++){
        n = ( p[(i_r-1)][0] + p[(i_r-1)][1] + p[i_r][1] + p[(i_r+1)][1] + p[(i_r+1)][0] + p[(r-i_r-1)][c] + p[(r-i_r)][c] + p[(r-i_r+1)][c] );
        update(p, f, i_r, 0, n);
        n = ( p[(i_r-1)][c] + p[(i_r-1)][c-1] + p[i_r][c-1] + p[(i_r+1)][c-1] + p[(i_r+1)][c] + p[(r-i_r-1)][0] + p[(r-i_r)][0] + p[(r-i_r+1)][0] );
        update(p, f, i_r, c, n);      
        }    

    // Check Top and Bottom Row
    for(i_c = 1; i_c < c; i_c++){
        n =( p[r][(i_c-1)] + p[r][i_c] + p[r][(i_c+1)] + p[0][(i_c+1)] + p[1][(i_c+1)] + p[1][i_c] + p[1][(i_c-1)] + p[0][(i_c-1)] );
        update(p, f, 0, i_c, n);
        n = ( p[r][(i_c-1)] + p[(r-1)][(i_c-1)] + p[(r-1)][i_c] + p[(r-1)][(i_c+1)] + p[r][(i_c+1)] + p[0][(i_c+1)] + p[0][(i_c)] + p[0][(i_c-1)] );
        update(p, f, r, i_c, n);
    } 
    n = ( p[0][1] + p[1][1] + p[1][0] + p[(r-1)][c] + p[r][c] + p[0][c] + p[r][0] + p[r][1] );
    update(p, f, 0, 0, n);

    n = ( p[1][c] + p[1][(c-1)] + p[0][(c-1)] + p[r][(c-1)] + p[r][c] + p[0][0] + p[r][0] + p[(r-1)][0] );
    update(p, f, 0, c, n);
    
    n = ( p[r][(c-1)] + p[(r-1)][(c-1)] + p[(r-1)][c] + p[1][0] + p[0][0] + p[r][0] + p[0][c] + p[0][(c-1)]);
    update(p, f, r, c, n);

    n = ( p[r-1][0] + p[(r-1)][1] + p[r][1] + p[0][1] + p[0][0] + p[r][c] + p[0][c] + p[1][c] );
    update(p, f, r, 0, n);
}