/**
 * @file life.h
 * @author Tommy Pham
 * @date Fall 2020
 * @brief Header file for Conway Game of Life
 */
#ifndef LIFE_H_
#define LIFE_H_

void free_matrix(unsigned char **matrix, int m_row);
void malloc_failed(unsigned char **a, int size);
unsigned char **init_matrix(int m_row, int n_col);
void cell_in(unsigned char **p, int m_row, int n_col, char type, int x, int y);
void pattern_in(unsigned char **p, unsigned char type, FILE *fp, int x, int y, int m_row, int n_col);
void print_matrix(unsigned char **matrix, int m_row, int n_col);
void update(unsigned char **p, unsigned char **f, int r, int c, char n);
void mid(unsigned char **p, unsigned char **f, int r, int c);
void hedge(unsigned char **p, unsigned char **f, int r, int c);
void torus(unsigned char **p, unsigned char **f, int r, int c);
void klein(unsigned char **p, unsigned char **f, int r, int c);

#endif
