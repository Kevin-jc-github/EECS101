#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define ROWS	480
#define COLUMNS	640
#define PI 3.14159265358979323846

#define sqr(x)	((x)*(x))




void clear( unsigned char image[][COLUMNS] );
void header( int row, int col, unsigned char head[32] );

int main( int argc, char** argv )
{
	int				i = 0;
	int 			j = 0;
	int				sgmmax = 0;
	unsigned char	temp[ROWS][COLUMNS];
	// localmax: number in the three bucket of voting array corrsponding to three local maxima
	// index[3][2]: used for store rho and theta
	int				dedx, dedy, sgm;
	int 			localmax[3] = {0, 0, 0};
	int 			index[3][2] = { 0, 0, 0, 0, 0, 0 };
	// voting; voting array
	int				sgm_threshold, hough_threshold, pairs[3][2], maxima[3];
	FILE			*fp;
	unsigned char	image[ROWS][COLUMNS], simage[ROWS][COLUMNS], head[32];
	char			filename[50], ifilename[50];
	int				rho, theta,voting[180][400],voting_new[180][400];
	
	clear(simage);
	strcpy ( filename, "image.raw");
	memset ( voting, 0, sizeof(int) * 180 * 400 );  //180 * 400 is the suggested size of voting arrayx
	header(ROWS, COLUMNS, head);
	 

	/* Read in the image */
	if (!( fp = fopen(filename, "rb" ) ))
	{
		fprintf( stderr, "error: couldn't open %s\n", argv[1]);
		exit(1);
	}

	for ( i = 0 ; i < ROWS ; i++ ){
		if (!( COLUMNS == fread( image[i], sizeof(char), COLUMNS, fp ) ))
		{
			fprintf( stderr, "error: couldn't read %s\n", argv[1] );
			exit(1);
		}
	}
	fclose(fp);
	
	sgmmax = 0;
    /* Compute SGM */
	for (i = 0; i < ROWS ; i++){
		for (j = 0; j < COLUMNS; j++)
		{
			if (i == 0 || i == ROWS - 1 || j == 0 || j == COLUMNS - 1){
            	temp[i][j] = 0;
				continue;
        	}
			else{
				dedx =
					-image[i - 1][j - 1] + image[i - 1][j + 1] +
					-2 * image[i][j - 1] + 2 * image[i][j + 1] +
					-image[i + 1][j - 1] + image[i + 1][j + 1];

				dedy =
					-image[i - 1][j - 1] - 2 * image[i - 1][j] - image[i - 1][j + 1] +
					image[i + 1][j - 1] + 2 * image[i + 1][j] + image[i + 1][j + 1];

				temp[i][j] = sqr(dedx) + sqr(dedy);
			
				if (temp[i][j] > sgmmax) {
					sgmmax = temp[i][j];
				}
			}
		}
	}
	/*normalize*/
	for (i = 0; i < ROWS; i++) {
		for (j = 0; j < COLUMNS; j++) {
			if (i == 0 || i == ROWS-1 || j == 0 || j == COLUMNS-1){
				simage[i][j] = 0;
			}
			else{
				simage[i][j] = ((float)temp[i][j]) / sgmmax * 255;
			}
		}
	}

	/* Save SGM to an image */
	strcpy(filename, "image");
	if (!(fp = fopen(strcat(filename, "-sgm.ras"), "wb")))
	{
		fprintf(stderr, "error: could not open %s\n", filename);
		exit(1);
	}
	fwrite(head, 4, 8, fp);
	for (i = 0; i < ROWS; i++){
		fwrite(simage[i], sizeof(char), COLUMNS, fp);
	}
	fclose(fp);

	
	sgm_threshold = 100;
	/* Compute the binary image */
	for (i = 0; i < ROWS ; i++) {
		for (j = 0; j < COLUMNS; j++) {
			simage[i][j] = ((float) temp[i][j])/sgmmax * 255;
			if (simage[i][j] >= sgm_threshold) {
				simage[i][j] = 255;
			}
			else {
				simage[i][j] = 0;
			}
		}
	}


	/* Save the thresholded SGM to an image */
	strcpy(filename, "image");
	if (!(fp = fopen(strcat(filename, "-binary.ras"), "wb")))
	{
		fprintf(stderr, "error: could not open %s\n", filename);
		exit(1);
	}
	fwrite(head, 4, 8, fp);
	for (i = 0; i < ROWS; i++){
		fwrite(simage[i], sizeof(char), COLUMNS, fp);
	}
	fclose(fp);
	

	/* Reconstruct an image from the voting array */
	for (i = 0; i < 180; i++) {
		for (j = 0; j < 400; j++) {
			voting[i][j] = 0;
			voting_new[i][j] = 0;
		}
	}

	for (theta = 0; theta < 180; theta++) {
		for (i = 0; i < ROWS; i++) {
			for (j = 0; j < COLUMNS; j++) {
				if (simage[i][j] == 255) {
					/*rho = 400 * (((i * cos(theta * PI / 180) - j * sin(theta*PI / 180)) - minA) / (maxA - minA));
					voting[theta][rho] += 1;*/
					rho = i * cos(theta * PI / 180) - j * sin(theta * PI / 180);
					voting[theta][rho + 200] += 1;
				}
			}
		}
	}

	/* Hough thresholds are 170 to 480 */
	for (theta = 0; theta < 180; theta++) {
		for (rho = 0; rho < 400; rho++) {
			if (voting[theta][rho] > 170 && voting[theta][rho] < 400) {
				voting_new[theta][rho] = voting[theta][rho];
			}
		}
	}

	maxima[0] = 0 ;
	maxima[1] = 0 ;
	maxima[2] = 0;
	/* Finding three local maximas */
	for (theta = 0; theta < 180; theta++) {
		for (rho = 0; rho < 400; rho++) {
			if (voting_new[theta][rho] > maxima[0]) {
				maxima[0] = voting_new[theta][rho];
			}
		}
	}
	for (theta = 0; theta < 180; theta++) {
		for (rho = 0; rho < 400; rho++) {
			if (voting_new[theta][rho] > maxima[1]) {
				if (voting_new[theta][rho] == maxima[0]) {
					continue;
				}
				else maxima[1] = voting_new[theta][rho];
			}
		}
	}
	for (theta = 0; theta < 180; theta++) {
		for (rho = 0; rho < 400; rho++) {
			if (voting_new[theta][rho] > maxima[2]) {
				if (voting_new[theta][rho] == maxima[0] || voting_new[theta][rho] == maxima[1]) {
					continue;
				}
				else {
					maxima[2] = voting_new[theta][rho];
				}				
			}
		}
	}

	for (theta = 0; theta < 180; theta++) {
		for (rho = 0; rho < 400; rho++) {
			if (voting[theta][rho] == maxima[0]) {
				pairs[0][0] = theta;
				pairs[0][1] = rho - 200;
			}
			if (voting[theta][rho] == maxima[1]) {
				pairs[1][0] = theta;
				pairs[1][1] = rho-200;
			}
			if (voting[theta][rho] == maxima[2]) {
				pairs[2][0] = theta;
				pairs[2][1] = rho - 200;
			}
		}
	}

	/* Three pairs of rhos and thetas */
	for (i = 0; i < 3; i++) {
		printf("theta: %d rho: %d voting: %d\n", pairs[i][0], pairs[i][1], maxima[i]);
	}

	/* Sketch lines from pairs */
	for (i = 0; i < ROWS; i++) {
		for (j = 0; j < COLUMNS; j++) {
			for (int x = 0; x < 3; x++) {
				if (abs(j * sin(pairs[x][0] * PI / 180) - i * cos(pairs[x][0] * PI / 180) + pairs[x][1]) == 0) {
					simage[i][j] = 255;
				}
			}
		}
	}



	/* Write the reconstructed figure to an image */
	strcpy(filename, "image");
	header(ROWS, COLUMNS, head);
	if (!(fp = fopen(strcat(filename, "-reconstructed_image.ras"), "wb")))
	{
		fprintf(stderr, "error: could not open %s\n", filename);
		exit(1);
	}
	fwrite(head, 4, 8, fp);
	for (i = 0; i < ROWS; i++)
		fwrite(simage[i], sizeof(char), COLUMNS, fp);
	fclose(fp);

	

	printf("Press any key to exit: ");
	//gets(&ch);

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

