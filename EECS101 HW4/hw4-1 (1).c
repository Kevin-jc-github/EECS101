#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ROWS		(int)480
#define COLUMNS		(int)640

void clear( unsigned char image[][COLUMNS] );
void header( int row, int col, unsigned char head[32] );

int main( int argc, char **argv )
{

	int				i, j, k, threshold[3], x, y, s;
	int max[3];  // max[0] is max of Gx, max[1] is max of Gy, max[2] is max of SGM
	FILE			*fp;
	unsigned char	image[ROWS][COLUMNS], ximage[ROWS][COLUMNS], yimage[ROWS][COLUMNS],sgmimage[ROWS][COLUMNS], head[32]; 
	char			filename[6][50], ifilename[50], ch;

	strcpy( filename[0], "image1" );
	strcpy( filename[1], "image2" );
	strcpy( filename[2], "image3" );
	header ( ROWS, COLUMNS, head );

	// choose thresholds values for each image for generate binary images
	threshold[0] = 100; 
	threshold[1] = 100; 
	threshold[2] = 100; 

	printf( "Maximum of Gx, Gy, SGM\n" );

	for ( k = 0; k < 3; k ++)
	{
		clear( ximage );
		clear( yimage );

		/* Read in the image */
		strcpy( ifilename, filename[k] );
		if (( fp = fopen( strcat(ifilename, ".raw"), "rb" )) == NULL )
		{
		  fprintf( stderr, "error: couldn't open %s\n", ifilename );
		  exit( 1 );
		}			
		for ( i = 0; i < ROWS ; i++ )
		  if ( fread( image[i], sizeof(char), COLUMNS, fp ) != COLUMNS )
		  {
			fprintf( stderr, "error: couldn't read enough stuff\n" );
			exit( 1 );
		  }
		fclose( fp );

		max[0] = 0; //maximum of Gx, initialize
		max[1] = 0; //maximum of Gy, initialize
		max[2] = 0; //maximum of SGM, initialize

		/* Compute Gx, Gy, SGM, find out the maximum and normalize*/
		int xSum, ySum, sgmSum=0, topLeft, topRight, left, right, top, bottom, bottomLeft, bottomRight;

		// Step 1: Find out the maximum value for Gx, Gy, SGM
		/*
            Gx at (i, j) =  abs(  image[i-1][j-1] + 2*image[i][j-1] + image[i+1][j-1]
                                - image[i-1][j+1] - 2*image[i][j+1] - image[i+1][j+1])
		*/
		// Write your code here. 
		/* Finding maximum for Gx */
		for (x = 1; x < COLUMNS - 1; x++) {
			for (y = 1; y < ROWS - 1; y++) {
				topLeft = image[y + 1][x - 1] * -1;
				topRight = image[y + 1][x + 1];
				left = image[y][x - 1] * -2;
				right = image[y][x + 1] * 2;
				bottomLeft = image[y - 1][x - 1] * -1;
				bottomRight = image[y - 1][x + 1];

				xSum = abs(topLeft + topRight + left + right + bottomLeft + bottomRight);

				if (xSum > max[0]) {
					max[0] = xSum;
				}

			}
		}

		/* Finding maximum for Gy */
		for (x = 1; x < COLUMNS - 1; x++) {
			for (y = 1; y < ROWS - 1; y++) {
				topLeft = image[y + 1][x - 1];
				bottomLeft = image[y - 1][x - 1] * -1;
				topRight = image[y + 1][x + 1];
				bottomRight = image[y - 1][x + 1] * -1;
				top = image[y + 1][x] * 2;
				bottom = image[y - 1][x] * -2;

				ySum = abs(topLeft + topRight + top + bottom + bottomLeft + bottomRight);

				if (ySum > max[1]) {
					max[1] = ySum;
				}

			}
		}

		/* Find maximum of SGM */
		for (x = 1; x < COLUMNS - 1; x++) {
			for (y = 1; y < ROWS - 1; y++) {
				topLeft = image[y + 1][x - 1] * -1;
				topRight = image[y + 1][x + 1];
				left = image[y][x - 1] * -2;
				right = image[y][x + 1] * 2;
				bottomLeft = image[y - 1][x - 1] * -1;
				bottomRight = image[y - 1][x + 1];

				xSum = abs(topLeft + topRight + left + right + bottomLeft + bottomRight);

				topLeft = image[y + 1][x - 1];
				bottomLeft = image[y - 1][x - 1] * -1;
				topRight = image[y + 1][x + 1];
				bottomRight = image[y - 1][x + 1] * -1;
				top = image[y + 1][x] * 2;
				bottom = image[y - 1][x] * -2;

				ySum = abs(topLeft + topRight + top + bottom + bottomLeft + bottomRight);

				sSum = pow(xSum, 2) + pow(ySum, 2);

				if (sSum > max[2]) {
					max[2] = sgmSum;
				}
			}
		}

		// Step 2: Compute Gx and normalize it to [0, 255] by converting to float data type
		/*
		 For example: for pixel (i, j)
				ximage[i][j] = (float)abs(  image[i-1][j-1] + 2*image[i][j-1] + image[i+1][j-1]
										  - image[i-1][j+1] - 2*image[i][j+1] - image[i+1][j+1]) / max[0] * 255;
	    */
		// Write your code here. 
		/* compute Gx */
		for (x = 1; x < COLUMNS-1; x++) {
			for (y = 1; y < ROWS-1; y++) {
				topLeft = image[y + 1][x - 1] * -1;
				topRight = image[y + 1][x + 1];
				left = image[y][x - 1] * -2;
				right = image[y][x + 1] * 2;
				bottomLeft = image[y - 1][x - 1] * -1;
				bottomRight = image[y - 1][x + 1];

				xSum = abs(topLeft + topRight + left + right + bottomLeft + bottomRight);

				
				
				/* Normalizing Gx */
				ximage[y][x] = (xSum * 255) / max[0];

			}
		}


		// Step 3: Compute Gy and normalize to to [0, 255] by converting to float (similar to step 2)
		// Write your code here. 
		/* compute Gy */
		for (x = 1; x < COLUMNS-1; x++) {
			for (y = 1; y < ROWS-1; y++) {
				topLeft = image[y + 1][x - 1];
				bottomLeft = image[y - 1][x - 1] * -1;
				topRight = image[y + 1][x + 1];
				bottomRight = image[y - 1][x + 1] * -1;
				top = image[y + 1][x] * 2;
				bottom = image[y - 1][x] * -2;

				ySum = abs(topLeft + topRight + top + bottom + bottomLeft + bottomRight);
				
				/* Normalizing Gy */
				yimage[y][x] = (ySum * 255) / max[1];
			}
		}

		

		/* Write Gx to a new image*/
		strcpy( ifilename, filename[k] );
		if (!( fp = fopen( strcat( ifilename, "-x.ras" ), "wb" ) ))
		{
		  fprintf( stderr, "error: could not open %s\n", ifilename );
		  exit( 1 );
		}
		fwrite( head, 4, 8, fp );

		for (i = 0; i < ROWS; i++)
			fwrite(ximage[i], sizeof(char), COLUMNS, fp);
		
	    fclose( fp );
					
		/* Write Gy to a new image */
		strcpy( ifilename, filename[k] );
		if (!( fp = fopen( strcat( ifilename, "-y.ras" ), "wb" ) ))
		{
		  fprintf( stderr, "error: could not open %s\n", ifilename );
		  exit( 1 );
		}
		fwrite( head, 4, 8, fp );

		for (i = 0; i < ROWS; i++)
			fwrite(yimage[i], sizeof(char), COLUMNS, fp);

		fclose( fp );

		// Step 4: Compute squared gradient and normalize to [0, 255] by converting to float
		// Write your code here ... you can reuse vaiable 'ximage' to store values for SGM
		/* compute SGM */
		for (x = 1; x < COLUMNS-1; x++) {
			for (y = 1; y < ROWS-1; y++) {

				topLeft = image[y + 1][x - 1] * -1;
				topRight = image[y + 1][x + 1];
				left = image[y][x - 1] * -2;
				right = image[y][x + 1] * 2;
				bottomLeft = image[y - 1][x - 1] * -1;
				bottomRight = image[y - 1][x + 1];

				xSum = abs(topLeft + topRight + left + right + bottomLeft + bottomRight);

				topLeft = image[y + 1][x - 1];
				bottomLeft = image[y - 1][x - 1] * -1;
				topRight = image[y + 1][x + 1];
				bottomRight = image[y - 1][x + 1] * -1;
				top = image[y + 1][x] * 2;
				bottom = image[y - 1][x] * -2;

				ySum = abs(topLeft + topRight + top + bottom + bottomLeft + bottomRight);

				sgmSum = xSum*xSum + ySum*ySum;
				
				/* Normalizing SGM */
				sgmimage[y][x] = (sgmSum * 255) / max[2];

			}
		}


		/* Write SGM to a new image */
		strcpy( ifilename, filename[k] );
		if (!( fp = fopen( strcat( ifilename, "-s.ras" ), "wb" ) ))
		{
		  fprintf( stderr, "error: could not open %s\n", ifilename );
		  exit( 1 );
		}
		fwrite( head, 4, 8, fp );
		for (i = 0; i < ROWS; i++)
			fwrite(sgmimage[i], sizeof(char), COLUMNS, fp);
		fclose( fp );
		
		/* Compute the binary image */ // need to check histogram and select threshold
		// Write your code here ... you can reuse variable 'ximage' to store values for binary image
		for (int x = 0; x < COLUMNS; x++) {
			for (int y = 0; y < ROWS; y++) {
				if (strcmp(filename[k], "image1") == 0) {
					if (sgmimage[y][x] > 70) {
						sgmimage[y][x] = 255;
					}
					else {
						sgmimage[y][x] = 0;
					}
				}
				else if (strcmp(filename[k], "image2") == 0) {
					if (sgmimage[y][x] > 10) {
						sgmimage[y][x] = 255;
					}
					else {
						sgmimage[y][x] = 0;
					}
				}
				else if (strcmp(filename[k], "image3") == 0) {
					if (sgmimage[y][x] > 10) {
						sgmimage[y][x] = 255;
					}
					else {
						sgmimage[y][x] = 0;
					}
				}
			}
		}

		
		/* Write the binary image to a new image */
		strcpy( ifilename, filename[k] );
		if (!( fp = fopen( strcat( ifilename, "-b.ras" ), "wb" ) ))
		{
		  fprintf( stderr, "error: could not open %s\n", ifilename );
		  exit( 1 );
		}
		fwrite( head, 4, 8, fp );
		for (i = 0; i < ROWS; i++)
			fwrite(sgmimage[i], sizeof(char), COLUMNS, fp);
		fclose( fp );

		printf( "%d %d %d\n", max[0], max[1], max[2] );

	}

	printf( "Press any key to exit: " );
	gets( &ch );
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
