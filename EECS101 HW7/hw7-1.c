#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define ROWS	240
#define COLUMNS	240
#define PI 3.14159265358979323846

void clear( unsigned char image[][COLUMNS] );
void header( int row, int col, unsigned char head[32] );
float dotProduct(float num1[3], float num2[3]);

int main( int argc, char** argv )
{
	int				i, j, k,num,z0;
	FILE*			fp;
	unsigned char	image[ROWS][COLUMNS], head[32];
	char			filename[9][50], ch,*outputfile;
	float			V[3] = { 0, 0, 1 };
	float			H[3];
	float	 		N[3]; 
	float			p, q, a, m, z, sVal, s[3], L, Ls, Ll, alpha, Vmagnitude;
	int				x, y, r;


	strcpy(filename[0], "image_a.ras");
	strcpy(filename[1], "image_b.ras");
	strcpy(filename[2], "image_c.ras");
	strcpy(filename[3], "image_d.ras");
	strcpy(filename[4], "image_e.ras");
	strcpy(filename[5], "image_f.ras");
	strcpy(filename[6], "image_g.ras");
	strcpy(filename[7], "image_f.ras");
	strcpy(filename[8], "image_i.ras");
	header ( ROWS, COLUMNS, head );
	
	for ( k = 0; k < 9; k ++ )
	{
		
		for (i = 0; i < ROWS; i++) {
			for (j = 0; j < COLUMNS; j++) {
				image[i][j] = 0;
			}
		}

		if (k == 0) {
			s[0] = 0;
			s[1] = 0;
			s[2] = 1;
			r = 50;
			a = 0.5;
			m = 1;
		}
		else if (k == 1) {
			s[0] = 1 / sqrt(3);
			s[1] = 1 / sqrt(3);
			s[2] = 1 / sqrt(3);
			r = 50;
			a = 0.5;
			m = 1;
		}
		else if (k == 2) {
			s[0] = 1;
			s[1] = 0;
			s[2] = 0;
			r = 50;
			a = 0.5;
			m = 1;
		}
		else if (k == 3) {
			s[0] = 0;
			s[1] = 0;
			s[2] = 1;
			r = 10;
			a = 0.5;
			m = 1;
		}
		else if (k == 4) {
			s[0] = 0;
			s[1] = 0;
			s[2] = 1;
			r = 100;
			a = 0.5;
			m = 1;
		}
		else if (k == 5) {
			s[0] = 0;
			s[1] = 0;
			s[2] = 1;
			r = 50;
			a = 0.1;
			m = 1;
		}
		else if (k == 6) {
			s[0] = 0;
			s[1] = 0;
			s[2] = 1;
			r = 50;
			a = 1;
			m = 1;
		}
		else if (k == 7) {
			s[0] = 0;
			s[1] = 0;
			s[2] = 1;
			r = 50;
			a = 0.5;
			m = 0.1;
		}
		else {
			s[0] = 0;
			s[1] = 0;
			s[2] = 1;
			r = 50;
			a = 0.5;
			m = 10000;
		}

		/* Draw circle */
		x = COLUMNS / 2;
		y = ROWS / 2;
		z0 = 0;
		Vmagnitude = 0;

		for (i = 0; i < ROWS; i++) {
			for (j = 0; j < COLUMNS; j++) {
				if (r * r - ((j - x) * (j - x) + (i - y) * (i - y)) > 0) {
					z = z0 + sqrt(r * r - ((j - x) * (j - x) + (i - y) * (i - y)));

					/* Calculating for p and q */
					p = -(j - x) / (z - z0);
					q = (i - y) / (z - z0);

					/* Calculating for normal vector */
					N[0] = -p / sqrt(p * p + q * q + 1);
					N[1] = -q / sqrt(p * p + q * q + 1);
					N[2] = 1 / sqrt(p * p + q * q + 1);

					/* Calcuating for Lambertian component */
					Ll = (N[0] * s[0]) + (N[1] * s[1]) + (N[2] * s[2]);

					if (Ll >= 0) {
						/* Calculating for specular component */
						Vmagnitude = sqrt((V[0] + s[0]) * (V[0] + s[0]) + (V[1] + s[1]) * (V[1] + s[1]) + (V[2] + s[2]) * (V[2] + s[2]));
						H[0] = (V[0] + s[0]) / Vmagnitude;
						H[1] = (V[1] + s[1]) / Vmagnitude;
						H[2] = (V[2] + s[2]) / Vmagnitude;
						alpha = acos((N[0] * H[0]) + (N[1] * H[1]) + (N[2] * H[2]));
						Ls = exp(-(alpha / m) * (alpha / m));

						/* Calculating for irradiance */
						L = a * Ll + (1 - a) * Ls;

						/* Setting image pixel value to calculated irradiance value */
						image[i][j] = L * 255;
					}
				}
			}
		}

		if (!( fp = fopen( filename[k], "wb" ) ))
		{
		  fprintf( stderr, "error: could not open %s\n", filename);
		  exit( 1 );
		}
		fwrite( head, 4, 8, fp );
		for ( i = 0 ; i < ROWS ; i++ )
		  fwrite( image[i], sizeof(char), COLUMNS, fp );
		fclose( fp );
	}

	printf("Press any key to exit: ");
	gets ( &ch );
	return 0;
}


void clear( unsigned char image[][COLUMNS] )
{
	int	i,j;
	for ( i = 0 ; i < ROWS ; i++ )
		for ( j = 0 ; j < COLUMNS ; j++ ) image[i][j] = 0;
}

void header( int row, int col, unsigned char head[32] )
{
	int *p = (int *)head;
	char *ch;
	int num = row * col;

	/* Choose little-endian or big-endian header depending on the machine. Don't modify this */
	/* Little-endian for PC */
	
	*p = 0x956aa659;
	*(p + 3) = 0x08000000;
	*(p + 5) = 0x01000000;
	*(p + 6) = 0x0;
	*(p + 7) = 0xf8000000;

	ch = (char*)&col;
	head[7] = *ch;
	ch ++; 
	head[6] = *ch;
	ch ++;
	head[5] = *ch;
	ch ++;
	head[4] = *ch;

	ch = (char*)&row;
	head[11] = *ch;
	ch ++; 
	head[10] = *ch;
	ch ++;
	head[9] = *ch;
	ch ++;
	head[8] = *ch;
	
	ch = (char*)&num;
	head[19] = *ch;
	ch ++; 
	head[18] = *ch;
	ch ++;
	head[17] = *ch;
	ch ++;
	head[16] = *ch;
	

	/* Big-endian for unix */
	/*
	*p = 0x59a66a95;
	*(p + 1) = col;
	*(p + 2) = row;
	*(p + 3) = 0x8;
	*(p + 4) = num;
	*(p + 5) = 0x1;
	*(p + 6) = 0x0;
	*(p + 7) = 0xf8;
*/
}