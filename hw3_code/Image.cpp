//*****************************************************************************
//
// Image.cpp : Defines the class operations on images
//
// Author - Parag Havaldar
// Code used by students as starter code to display and modify images
//
//*****************************************************************************

#include "Image.h"
#include <math.h>

// Constructor and Desctructors
MyImage::MyImage()
{
	Data = NULL;
	Width = -1;
	Height = -1;
	ImagePath[0] = 0;
}

MyImage::~MyImage()
{
	if (Data)
		delete Data;
}


// Copy constructor
MyImage::MyImage(MyImage* otherImage)
{
	Height = otherImage->Height;
	Width = otherImage->Width;
	Data = new char[Width * Height * 3];
	strcpy(otherImage->ImagePath, ImagePath);

	for (int i = 0; i < (Height * Width * 3); i++)
	{
		Data[i] = otherImage->Data[i];
	}
}



// = operator overload
MyImage& MyImage::operator= (const MyImage& otherImage)
{
	Height = otherImage.Height;
	Width = otherImage.Width;
	Data = new char[Width * Height * 3];
	strcpy((char*)otherImage.ImagePath, ImagePath);

	for (int i = 0; i < (Height * Width * 3); i++)
	{
		Data[i] = otherImage.Data[i];
	}

	return *this;

}


// MyImage::ReadImage
// Function to read the image given a path
bool MyImage::ReadImage()
{

	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0)
	{
		fprintf(stderr, "Image or Image properties not defined");
		fprintf(stderr, "Usage is `Image.exe Imagefile w h`");
		return false;
	}

	// Create a valid output file pointer
	FILE* IN_FILE;
	IN_FILE = fopen(ImagePath, "rb");
	if (IN_FILE == NULL)
	{
		fprintf(stderr, "Error Opening File for Reading");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char* Rbuf = new char[Height * Width];
	char* Gbuf = new char[Height * Width];
	char* Bbuf = new char[Height * Width];

	for (i = 0; i < Width * Height; i++)
	{
		Rbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width * Height; i++)
	{
		Gbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width * Height; i++)
	{
		Bbuf[i] = fgetc(IN_FILE);
	}

	// Allocate Data structure and copy
	Data = new char[Width * Height * 3];
	for (i = 0; i < Height * Width; i++)
	{
		Data[3 * i] = Bbuf[i];
		Data[3 * i + 1] = Gbuf[i];
		Data[3 * i + 2] = Rbuf[i];
	}

	// Clean up and return
	delete Rbuf;
	delete Gbuf;
	delete Bbuf;
	fclose(IN_FILE);

	return true;

}



// MyImage functions defined here
bool MyImage::WriteImage()
{
	// Verify ImagePath
	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0)
	{
		fprintf(stderr, "Image or Image properties not defined");
		return false;
	}

	// Create a valid output file pointer
	FILE* OUT_FILE;
	OUT_FILE = fopen(ImagePath, "wb");
	if (OUT_FILE == NULL)
	{
		fprintf(stderr, "Error Opening File for Writing");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char* Rbuf = new char[Height * Width];
	char* Gbuf = new char[Height * Width];
	char* Bbuf = new char[Height * Width];

	for (i = 0; i < Height * Width; i++)
	{
		Bbuf[i] = Data[3 * i];
		Gbuf[i] = Data[3 * i + 1];
		Rbuf[i] = Data[3 * i + 2];
	}


	// Write data to file
	for (i = 0; i < Width * Height; i++)
	{
		fputc(Rbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width * Height; i++)
	{
		fputc(Gbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width * Height; i++)
	{
		fputc(Bbuf[i], OUT_FILE);
	}

	// Clean up and return
	delete Rbuf;
	delete Gbuf;
	delete Bbuf;
	fclose(OUT_FILE);

	return true;

}


double QuantTable[8][8] = { {16., 11., 10., 16., 24., 40., 51., 61.},
						   {12., 12., 14., 19., 26., 58., 60., 55.},
						   {14., 13., 16., 24., 40., 57., 69., 56.},
						   {14., 17., 22., 29., 51., 87., 80., 62.},
						   {18., 22., 37., 56., 68., 109., 103., 77.},
						   {24., 35., 55., 64., 81., 104., 113., 92.},
						   {49., 64., 78., 87., 103., 121., 120., 101.},
						   {72., 92., 95., 98., 112., 100., 103., 99.} };

void MyImage::convertDCTBlock(int nRow, int nCol, int numCoeffsPerBlock,
	double* DCTorigin, double* DCTcoeffs, double* DCTquots, double* DCTquants, double* IDCTvalues) {

	for (int channel = 0; channel < 3; channel++)
	{
		// Get DCT original data in doubles;
		int m, n;
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				m = i + nRow;
				n = j + nCol;
				DCTorigin[i * 8 + j] = (double)(unsigned char)Data[3 * (m * Width + n) + channel];
				//printf("DCT original data: %lf\n", DCTorigin[i * 8 + j]);
			}
		}

		// Get DCT coefficients in doubles
		double cu, cv;
		double pi = 2 * acos(0.0);
		for (int u = 0; u < 8; u++) {
			for (int v = 0; v < 8; v++) {
			
				double sum = 0;
				if (u == 0)
					cu = 1.0 / sqrt(2);
				else
					cu = 1.0;

				if (v == 0)
					cv = 1.0 / sqrt(2);
				else
					cv = 1.0;

				for (int x = 0; x < 8; x++) {
					for (int y = 0; y < 8; y++) {
						sum += 0.25 * cu * cv * DCTorigin[x * 8 + y] * cos((2.0 * ((double)x) + 1.0) * ((double)u) * pi / 16.0) * cos((2.0 * ((double)y) + 1.0) * ((double)v) * pi / 16.0);
					}
				}

				DCTcoeffs[u * 8 + v] = sum;
				//printf("DCT coefficients: %lf\n", DCTcoeffs[u * 8 + v]);
			}
		}


		// Get DCT quotients in doubles
		for (int u = 0; u < 8; u++) {
			for (int v = 0; v < 8; v++) {
				DCTquots[u * 8 + v] = DCTcoeffs[u * 8 + v] / QuantTable[u][v];
				//printf("DCT quotients: %lf\n", DCTquots[u * 8 + v]);
			}
		}

		// Get Quantized DCT in ints
		for (int u = 0; u < 8; u++) {
			for (int v = 0; v < 8; v++) {
				DCTquants[u * 8 + v] = round(DCTquots[u * 8 + v]);
				//printf("DCT quants: %d\n", DCTquants[u * 8 + v]);
			}
		}


		//int intArray[8][8] = { {-1, 1, 5, 6, 14, 15, 27, 28},
		//				{2, 4, 7, 13, 16, 26, 29, 42},
		//				{3, 8, 12, 17, 25, 30, 41, 43},
		//				{9, 11, 18, 24, 31, 40, 44, 53},
		//				{10, 19, 23, 32, 39, 45, 52, 54},
		//				{20, 22, 33, 38, 46, 51, 55, 60},
		//				{21, 34, 37, 47, 50, 56, 59, 61},
		//				{35, 36, 48, 49, 57, 58, 62, 63} };


		// Set coefficients
		int iCoeff = 0;
		for (int iDiag = 0; iDiag <= 14; iDiag++) {
			int larger, smaller, dir;
			if (iDiag > 7) {
				larger = 7;
				smaller = iDiag - 7;
			}
			else {
				larger = iDiag;
				smaller = 0;
			}
			
			dir = iDiag % 2;
			int x, y;
			if (dir == 0) {
				x = larger;
				y = smaller;
			}
			else {
				x = smaller;
				y = larger;
			}

			int numValuesInDiag = (iDiag <= 7) ? (iDiag + 1) : (15 - iDiag);
			for (int i = 0; i < numValuesInDiag; i++) {
				if (iCoeff >= numCoeffsPerBlock) {
					DCTquants[x * 8 + y] = 0;
				}
				if (dir == 0) {
					x--;
					y++;
				}
				else {
					x++;
					y--;
				}
				iCoeff++;
			}
		}

		//for (int x = 0; x < 8; x++) {
		//	for (int y = 0; y < 8; y++) {
		//		printf("%lf ", DCTquants[x * 8 + y]);
		//	}
		//	printf("\n");
		//}

		// Inverse DCT
		double DCTcoeff;
		for (int x = 0; x < 8; x++) {
			for (int y = 0; y < 8; y++) {

				double sum = 0;

				for (int u = 0; u < 8; u++) {
					for (int v = 0; v < 8; v++) {
						if (u == 0)
							cu = 1.0 / sqrt(2);
						else
							cu = 1.0;

						if (v == 0)
							cv = 1.0 / sqrt(2);
						else
							cv = 1.0;

						DCTcoeff = DCTquants[u * 8 + v] * QuantTable[u][v];
						sum += 0.25 * cu * cv * DCTcoeff  * cos((2.0 * ((double)x) + 1.0) * ((double)u) * pi / 16.0) * cos((2.0 * ((double)y) + 1.0) * ((double)v) * pi / 16.0);
					}
				}

				// Keep IDCT values in range [0.0, 255.0]
				IDCTvalues[x * 8 + y] = (sum < 0.0) ? 0.0 : ((sum > 255.0) ? 255.0 : sum);
				//printf("IDCT values: %lf\n", IDCTvalues[x * 8 + y]);
			}
		}

		// Print original and IDCT values
		//for (int x = 0; x < 8; x++) {
		//	for (int y = 0; y < 8; y++) {
		//		printf("Compare: %lf, %lf\n", DCTorigin[x * 8 + y], IDCTvalues[x * 8 + y]);
		//	}
		//}




		// Convert IDCT values to characters 
		for (int x = 0; x < 8; x++) {
			for (int y = 0; y < 8; y++) {
				m = nRow + x;
				n = nCol + y;
				Data[3 * (m * Width + n) + channel] = (char)(int)round(IDCTvalues[x * 8 + y]);
			}
		}
	}

}



// Here is where you would place your code to modify an image
// eg Filtering, Transformation, Cropping, etc.
bool MyImage::convertDCT(int numCoeffs)
{
	fprintf(stdout, "**************************************DCT**************************************\n");

	// Allocate heap memory for arrays
	double* DCTorigin = new double[8 * 8];
	double* DCTcoeffs = new double[8 * 8];
	double* DCTquots = new double[8 * 8];
	double* DCTquants = new double[8 * 8];
	double* IDCTvalues = new double[8 * 8];

	// Calculate number of coefficients per block
	int numCoeffsPerBlock;
	if (numCoeffs >= 262144) {
		fprintf(stdout, "DCT Returning the original image... (number of coefficients is greater than or equal to 262144)\n", numCoeffs);
		return true;
	}
	else if (numCoeffs <= 4096) {
		fprintf(stdout, "Error: %d is not a valid number for DCT (too small), correcting to 4096\n", numCoeffs);
		numCoeffs = 4096;
		numCoeffsPerBlock = 1;
	}
	else {
		numCoeffsPerBlock = numCoeffs / 4096;
	}
	
	fprintf(stdout, "Compressing Image using DCT... (number of coefficients is %d)\n", numCoeffsPerBlock * 4096);
	fprintf(stdout, "	Please wait. Should complete in 10 seconds.\n");
	// Apply DCT on each block
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			convertDCTBlock(i*8, j*8, numCoeffsPerBlock, DCTorigin, DCTcoeffs, DCTquots, DCTquants, IDCTvalues);
		}
	}

	delete[] DCTorigin;
	delete[] DCTcoeffs;
	delete[] DCTquots;
	delete[] DCTquants;
	delete[] IDCTvalues;

	return true;
}



void MyImage::compressRows(double* DWTorigin, int length) {
	
	double* compressedRow = new double[length];
	double* currRow = new double[length];
	
	for (int channel = 0; channel < 3; channel++) {

		// Compress each row in DWTorigin
		for (int iRow = 0; iRow < length; iRow++) {

			// Get current row with index iRow
			for (int iCol = 0; iCol < length; iCol++) {
				currRow[iCol] = DWTorigin[3 * (iRow * Width + iCol) + channel];
			}

			// Compress the current row and save the data to compressedRow
			for (int iCol = 0; iCol < length; iCol += 2) {
				compressedRow[iCol / 2] = (currRow[iCol] + currRow[iCol + 1]) / 2;
				compressedRow[iCol / 2 + length / 2] = (currRow[iCol] - currRow[iCol + 1]) / 2;
			}

			// Copy the data in compressedRow to DWTorigin
			for (int iCol = 0; iCol < length; iCol++) {
				DWTorigin[3 * (iRow * Width + iCol) + channel] = compressedRow[iCol];
			}

		}
	}

	delete[] compressedRow;
	delete[] currRow;

}

void MyImage::compressColumns(double* DWTorigin, int length) {

	double* compressedCol = new double[length];
	double* currCol = new double[length];

	for (int channel = 0; channel < 3; channel++) {

		// Compress each column in DWTorigin
		for (int iCol = 0; iCol < length; iCol++) {

			// Get current column with index iCol
			for (int iRow = 0; iRow < length; iRow++) {
				currCol[iRow] = DWTorigin[3 * (iRow * Width + iCol) + channel];
			}

			// Compress the current column and save the data to compressedCol
			for (int iRow = 0; iRow < length; iRow += 2) {
				compressedCol[iRow / 2] = (currCol[iRow] + currCol[iRow + 1]) / 2;
				compressedCol[iRow / 2 + length / 2] = (currCol[iRow] - currCol[iRow + 1]) / 2;
			}

			// Copy the data in compressedCol to DWTorigin
			for (int iRow = 0; iRow < length; iRow++) {
				DWTorigin[3 * (iRow * Width + iCol) + channel] = compressedCol[iRow];
			}

		}
	}

	delete[] compressedCol;
	delete[] currCol;

}

void MyImage::decompressColumns(double* DWTorigin, int length) {
	double* decompressedCol = new double[length];
	double* currCol = new double[length];

	for (int channel = 0; channel < 3; channel++) {

		// Decompress each column in DWTorigin
		for (int iCol = 0; iCol < length; iCol++) {

			// Get current column with index iCol
			for (int iRow = 0; iRow < length; iRow++) {
				currCol[iRow] = DWTorigin[3 * (iRow * Width + iCol) + channel];
			}

			// Decompress the current column and save the data to decompressedCol
			for (int iRow = 0; iRow < length / 2; iRow++) {
				decompressedCol[2 * iRow] = currCol[iRow] + currCol[iRow + length / 2];
				decompressedCol[2 * iRow + 1] = currCol[iRow] - currCol[iRow + length / 2];
			}

			// Copy the data in decompressedCol to DWTorigin
			for (int iRow = 0; iRow < length; iRow++) {
				DWTorigin[3 * (iRow * Width + iCol) + channel] = decompressedCol[iRow];
			}

		}
	}

	delete[] decompressedCol;
	delete[] currCol;
	
}

void MyImage::decompressRows(double* DWTorigin, int length) {
	double* decompressedRow = new double[length];
	double* currRow = new double[length];
	
	for (int channel = 0; channel < 3; channel++) {

		// Decompress each row in DWTorigin
		for (int iRow = 0; iRow < length; iRow++) {

			// Get current row with index iCol
			for (int iCol = 0; iCol < length; iCol++) {
				currRow[iCol] = DWTorigin[3 * (iRow * Width + iCol) + channel];
			}

			// Decompress the current row and save the data to decompressedRow
			for (int iCol = 0; iCol < length / 2; iCol++) {
				decompressedRow[2 * iCol] = currRow[iCol] + currRow[iCol + length / 2];
				decompressedRow[2 * iCol + 1] = currRow[iCol] - currRow[iCol + length / 2];
			}

			// Copy the data in decompressedRow to DWTorigin
			for (int iCol = 0; iCol < length; iCol++) {
				DWTorigin[3 * (iRow * Width + iCol) + channel] = decompressedRow[iCol];
			}
		}
	}

	delete[] decompressedRow;
	delete[] currRow;

}


void MyImage::setDWTCoeffs(double* DWTorigin, int numCoeffs) {
	int length = (int)sqrt(numCoeffs);
	for (int i = 0; i < Height; i++) {
		for (int j = 0; j < Width; j++) {
			if (i >= length || j >= length) {
				DWTorigin[3 * (i * Width + j)] = 0.0;
				DWTorigin[3 * (i * Width + j) + 1] = 0.0;
				DWTorigin[3 * (i * Width + j) + 2] = 0.0;
			}
		}
	}
}


bool MyImage::convertDWT(int numCoeffs)
{
	fprintf(stdout, "**************************************DWT**************************************\n");

	//printf("%lf, %lf\n", ceil(log((double)262144) / log(4.)), floor(log((double)262144) / log(4.)));
	//printf("%d\n", (int)pow(4.0, floor(log((double)numCoeffs) / log(4.))));

	// Check number of coefficients
	if (numCoeffs >= 262144) {
		fprintf(stdout, "DWT Returning the original image... (number of coefficients is greater than or equal to 262144)\n", numCoeffs);
		return true;
	}
	else if (numCoeffs <= 0) {
		fprintf(stdout, "Error: %d is not a valid number for DWT (non-positive number)\n", numCoeffs);
		numCoeffs = 0;
	}
	else if (ceil(log((double)numCoeffs) / log(4.)) != floor(log((double)numCoeffs) / log(4.))) {
		int correctedNum = (int)pow(4.0, floor(log((double)numCoeffs) / log(4.)));
		fprintf(stdout, "Error: %d is not a valid number for DWT (not a power of 4), correcting to the nearest power of 4 - %d\n", numCoeffs, correctedNum);
		numCoeffs = correctedNum;
	}

	fprintf(stdout, "Compressing Image using DWT... (number of coefficients is %d)\n", numCoeffs);
	fprintf(stdout, "	Please wait. Should complete in 1 seconds.\n");

	// Allocate heap memory for arrays
	double* DWTorigin = new double[Height * Width * 3];

	// Get original data in doubles
	for (int i = 0; i < Height; i++) {
		for (int j = 0; j < Width; j++) {
			DWTorigin[3 * (i * Width + j)] = (double)(unsigned char)Data[3 * (i * Width + j)];
			DWTorigin[3 * (i * Width + j) + 1] = (double)(unsigned char)Data[3 * (i * Width + j) + 1];
			DWTorigin[3 * (i * Width + j) + 2] = (double)(unsigned char)Data[3 * (i * Width + j) + 2];
		}
	}
	
	// Compress rows and columns
	for (int i = 512; i != 1; i /= 2) {
		compressRows(DWTorigin, i);
		compressColumns(DWTorigin, i);
	}

	setDWTCoeffs(DWTorigin, numCoeffs);

	for (int i = 2; i != 1024; i *= 2) {
		decompressColumns(DWTorigin, i);
		decompressRows(DWTorigin, i);
	}


	// Check if assertions are correct
	for (int i = 0; i < 512; i++) {
		for (int j = 0; j < 512; j++) {
			if (DWTorigin[3 * (i * Width + j)] > 255.0 || DWTorigin[3 * (i * Width + j)] < 0.0) {
				printf("DWTorigin values exceeded!\n");
			}
		}
	}

	// Read DWTorigin into Data
	for (int i = 0; i < Height; i++) {
		for (int j = 0; j < Width; j++) {
			Data[3 * (i * Width + j)] = (char)(int)DWTorigin[3 * (i * Width + j)];
			Data[3 * (i * Width + j) + 1] = (char)(int)DWTorigin[3 * (i * Width + j) + 1];
			Data[3 * (i * Width + j) + 2] = (char)(int)DWTorigin[3 * (i * Width + j) + 2];
		}
	}

	return true;
}