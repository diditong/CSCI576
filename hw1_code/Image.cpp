//*****************************************************************************
//
// Image.cpp : Defines the class operations on images
//
// Author - Parag Havaldar
// Code used by students as starter code to display and modify images
//
//*****************************************************************************

#include "Image.h"
#include <iostream>
using namespace std;

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
	if ( Data )
		delete Data;
}


// Copy constructor
MyImage::MyImage( MyImage *otherImage)
{
	Height = otherImage->Height;
	Width  = otherImage->Width;
	Data   = new char[Width*Height*3];
	strcpy(otherImage->ImagePath, ImagePath );

	for ( int i=0; i<(Height*Width*3); i++ )
	{
		Data[i]	= otherImage->Data[i];
	}


}

// = operator overload
MyImage & MyImage::operator= (const MyImage &otherImage)
{
	Height = otherImage.Height;
	Width  = otherImage.Width;
	Data   = new char[Width*Height*3];
	strcpy( (char *)otherImage.ImagePath, ImagePath );

	for ( int i=0; i<(Height*Width*3); i++ )
	{
		Data[i]	= otherImage.Data[i];
	}
	
	return *this;

}


// MyImage::ReadImage
// Function to read the image given a path
bool MyImage::ReadImage()
{
	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		fprintf(stderr, "Usage is `Image.exe Imagefile w h`");
		return false;
	}
	
	// Create a valid output file pointer
	FILE *IN_FILE;
	IN_FILE = fopen(ImagePath, "rb");
	if ( IN_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Reading");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char *Rbuf = new char[Height*Width]; 
	char *Gbuf = new char[Height*Width]; 
	char *Bbuf = new char[Height*Width]; 

	for (i = 0; i < Width*Height; i ++)
	{
		Rbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		Gbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		Bbuf[i] = fgetc(IN_FILE);
	}
	
	// Allocate Data structure and copy
	Data = new char[Width*Height*3];
	for (i = 0; i < Height*Width; i++)
	{
		Data[3*i]	= Bbuf[i];
		Data[3*i+1]	= Gbuf[i];
		Data[3*i+2]	= Rbuf[i];
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
	if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		return false;
	}
	
	// Create a valid output file pointer
	FILE *OUT_FILE;
	OUT_FILE = fopen(ImagePath, "wb");
	if ( OUT_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Writing");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char *Rbuf = new char[Height*Width]; 
	char *Gbuf = new char[Height*Width]; 
	char *Bbuf = new char[Height*Width]; 

	for (i = 0; i < Height*Width; i++)
	{
		Bbuf[i] = Data[3*i];
		Gbuf[i] = Data[3*i+1];
		Rbuf[i] = Data[3*i+2];
	}

	
	// Write data to file
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Rbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Gbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
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


// Here is where you would place your code to modify an image
// eg Filtering, Transformation, Cropping, etc.
bool MyImage::Modify(int YRatio, int URatio, int VRatio, int QRatio)
{
	int R, G, B;

	double* Rbuf = new double[Height * Width];
	double* Gbuf = new double[Height * Width];
	double* Bbuf = new double[Height * Width];
	double* Ybuf = new double[Height * Width];
	double* Ubuf = new double[Height * Width];
	double* Vbuf = new double[Height * Width];
	
	// Convert to YUV space
	for (int i = 0; i < Width * Height; i++)
	{
		R = (int)((unsigned char)Data[3 * i]);
		G = (int)((unsigned char)Data[3 * i + 1]);
		B = (int)((unsigned char)Data[3 * i + 2]);

		Rbuf[i] = ((R < 0) ? 128 - R : R);
		Gbuf[i] = (G < 0) ? 128 - G : G;
		Bbuf[i] = (B < 0) ? 128 - B : B;

		Ybuf[i] = 0.299 * Rbuf[i] + 0.587 * Gbuf[i] + 0.114 * Bbuf[i];
		Ubuf[i] = 0.596 * Rbuf[i] - 0.274 * Gbuf[i] - 0.322 * Bbuf[i];
		Vbuf[i] = 0.211 * Rbuf[i] - 0.523 * Gbuf[i] + 0.312 * Bbuf[i];
	}

	// Subsample
	int col, row;
	for (int i = 0; i < Width * Height; i++)
	{
		row = i / Width;
		col = i % Width;
		if (col % YRatio != 0) {
			Ybuf[row * Width + col] = 0;
		}
		if (col % URatio != 0) {
			Ubuf[row * Width + col] = 0;
		}
		if (col % VRatio != 0) {
			Vbuf[row * Width + col] = 0;
		}
	}

	// Upsample
	for (int i = 0; i < Width * Height; i++)
	{
		row = i / Width;
		col = i % Width;
		if (col % YRatio != 0) {
			if (YRatio * (col / YRatio + 1) < Width) {
				Ybuf[row * Width + col] = (Ybuf[row * Width + YRatio * (col / YRatio)] + Ybuf[row * Width + YRatio * (col / YRatio + 1)]) / 2;
			}
			else {
				Ybuf[row * Width + col] = Ybuf[row * Width + YRatio * (col / YRatio)];
			}
		}
		if (col % URatio != 0) {
			if (URatio * (col / URatio + 1) < Width) {
				Ubuf[row * Width + col] = (Ubuf[row * Width + URatio * (col / URatio)] + Ubuf[row * Width + URatio * (col / URatio + 1)]) / 2;
			}
			else {
				Ubuf[row * Width + col] = Ubuf[row * Width + URatio * (col / URatio)];
			}
		}
		if (col % VRatio != 0) {
			if (VRatio * (col / VRatio + 1) < Width) {
				Vbuf[row * Width + col] = (Vbuf[row * Width + VRatio * (col / VRatio)] + Vbuf[row * Width + VRatio * (col / VRatio + 1)]) / 2;
			}
			else {
				Vbuf[row * Width + col] = Vbuf[row * Width + VRatio * (col / VRatio)];
			}
		}
	}

	// Convert back to RGB space
	for (int i = 0; i < Width * Height; i++)
	{
		Rbuf[i] = 1.000 * Ybuf[i] + 0.956 * Ubuf[i] + 0.621 * Vbuf[i];
		Gbuf[i] = 1.000 * Ybuf[i] - 0.272 * Ubuf[i] - 0.647 * Vbuf[i];
		Bbuf[i] = 1.000 * Ybuf[i] - 1.106 * Ubuf[i] + 1.703 * Vbuf[i];
	}

	// Quantize RGB channels
	double interval = 256.0 / (double)QRatio;
	int nR, nG, nB, upperBound, lowerBound;
	for (int i = 0; i < Width * Height; i++) {
		nR = (int)(Rbuf[i] / interval);
		upperBound = min((int)((nR + 1) * interval), 255);
		lowerBound = max((int)(nR * interval), 0);
		Rbuf[i] = (double)lowerBound;

		nG = (int)(Gbuf[i] / interval);
		upperBound = min((int)((nG + 1) * interval), 255);
		lowerBound = max((int)(nG * interval), 0);
		Gbuf[i] = (double)lowerBound;

		nB = (int)(Bbuf[i] / interval);
		upperBound = min((int)((nB + 1) * interval), 255);
		lowerBound = max((int)(nB * interval), 0);
		Bbuf[i] = (double)lowerBound;
	}


	// convert int to char
	for (int i = 0; i < Width * Height; i++) {
		Data[3 * i] = (char)((int)Rbuf[i]);
		Data[3 * i + 1] = (char)((int)Gbuf[i]);
		Data[3 * i + 2] = (char)((int)Bbuf[i]);
	}

	delete Rbuf;
	delete Gbuf;
	delete Bbuf;
	delete Ybuf;
	delete Ubuf;
	delete Vbuf;
	return false;
}


void MyImage::compressRows(int length)
{
	char* Rbuf = new char[Height * Width];
	char* Gbuf = new char[Height * Width];
	char* Bbuf = new char[Height * Width];
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < length / 2; j++) {
			Bbuf[i * Width + j] = (char)(((int)(unsigned char)Data[3 * (i * Width + 2 * j)] + (int)(unsigned char)Data[3 * (i * Width + (2 * j + 1))]) / 2);
			Gbuf[i * Width + j] = (char)(((int)(unsigned char)Data[3 * (i * Width + 2 * j) + 1] + (int)(unsigned char)Data[3 * (i * Width + (2 * j + 1)) + 1]) / 2);
			Rbuf[i * Width + j] = (char)(((int)(unsigned char)Data[3 * (i * Width + 2 * j) + 2] + (int)(unsigned char)Data[3 * (i * Width + (2 * j + 1)) + 2]) / 2);
		}
		//fprintf(stdout, "reached here 1\n");
		for (int j = length / 2; j < length; j++) {
			Bbuf[i * Width + j] = (char)(((int)(unsigned char)Data[3 * (i * Width + 2 * (j - length / 2))] - (int)(unsigned char)Data[3 * (i * Width + (2 * (j - length / 2) + 1))]) / 2);
			Gbuf[i * Width + j] = (char)(((int)(unsigned char)Data[3 * (i * Width + 2 * (j - length / 2)) + 1] - (int)(unsigned char)Data[3 * (i * Width + (2 * (j - length / 2) + 1)) + 1]) / 2);
			Rbuf[i * Width + j] = (char)(((int)(unsigned char)Data[3 * (i * Width + 2 * (j - length / 2)) + 2] - (int)(unsigned char)Data[3 * (i * Width + (2 * (j - length / 2) + 1)) + 2]) / 2);
		}
		//fprintf(stdout, "reached here 2\n");
	}

	for (int i = 0; i < length; i++) {
		for (int j = 0; j < length; j++) {
			Data[3 * (i * Width + j)] = Bbuf[i * Width + j];
			Data[3 * (i * Width + j) + 1] = Gbuf[i * Width + j];
			Data[3 * (i * Width + j) + 2] = Rbuf[i * Width + j];
			//fprintf(stdout, "%d, %d, %d\n", Data[3 * i], Data[3 * i + 1], Data[3 * i + 2]);
		}
	}

	free(Rbuf);
	free(Gbuf);
	free(Bbuf);
}

void MyImage::compressColumns(int length)
{
	char* Rbuf = new char[Height * Width];
	char* Gbuf = new char[Height * Width];
	char* Bbuf = new char[Height * Width];
	for (int j = 0; j < length; j++) {
		for (int i = 0; i < length / 2; i++) {
			Bbuf[i * Width + j] = (char)(((int)(unsigned char)Data[3 * ((2 * i) * Width + j)] + (int)(unsigned char)Data[3 * ((2 * i + 1) * Width + j)]) / 2);
			Gbuf[i * Width + j] = (char)(((int)(unsigned char)Data[3 * ((2 * i) * Width + j) + 1] + (int)(unsigned char)Data[3 * ((2 * i + 1) * Width + j) + 1]) / 2);
			Rbuf[i * Width + j] = (char)(((int)(unsigned char)Data[3 * ((2 * i) * Width + j) + 2] + (int)(unsigned char)Data[3 * ((2 * i + 1) * Width + j) + 2]) / 2);
		}
		for (int i = length / 2; i < length; i++) {
			Bbuf[i * Width + j] = (char)(((int)(unsigned char)Data[3 * ((2 * (i - length / 2)) * Width + j)] - (int)(unsigned char)Data[3 * ((2 * (i - length / 2) + 1) * Width + j)]) / 2);
			Gbuf[i * Width + j] = (char)(((int)(unsigned char)Data[3 * ((2 * (i - length / 2)) * Width + j) + 1] - (int)(unsigned char)Data[3 * ((2 * (i - length / 2) + 1) * Width + j) + 1]) / 2);
			Rbuf[i * Width + j] = (char)(((int)(unsigned char)Data[3 * ((2 * (i - length / 2)) * Width + j) + 2] - (int)(unsigned char)Data[3 * ((2 * (i - length / 2) + 1) * Width + j) + 2]) / 2);
		}
	}

	for (int i = 0; i < length; i++) {
		for (int j = 0; j < length; j++) {
			Data[3 * (i * Width + j)] = Bbuf[i * Width + j];
			Data[3 * (i * Width + j) + 1] = Gbuf[i * Width + j];
			Data[3 * (i * Width + j) + 2] = Rbuf[i * Width + j];
			//fprintf(stdout, "%d, %d, %d\n", Data[3 * i], Data[3 * i + 1], Data[3 * i + 2]);
		}
	}

	free(Rbuf);
	free(Gbuf);
	free(Bbuf);
}

void MyImage::decompressRows(int length)
{
	char* Rbuf = new char[Height * Width];
	char* Gbuf = new char[Height * Width];
	char* Bbuf = new char[Height * Width];

	for (int i = 0; i < length; i++) {
		for (int j = 0; j < length / 2; j++) {
			Bbuf[i * Width + 2 * j] = (char)((int)(unsigned char)Data[3 * (i * Width + j)] + (int)Data[3 * (i * Width + j + length / 2)]);
			Bbuf[i * Width + 2 * j + 1] = (char)((int)(unsigned char)Data[3 * (i * Width + j)] - (int)Data[3 * (i * Width + j + length / 2)]);
			Gbuf[i * Width + 2 * j] = (char)((int)(unsigned char)Data[3 * (i * Width + j) + 1] + (int)Data[3 * (i * Width + j + length / 2) + 1]);
			Gbuf[i * Width + 2 * j + 1] = (char)((int)(unsigned char)Data[3 * (i * Width + j) + 1] - (int)Data[3 * (i * Width + j + length / 2) + 1]);
			Rbuf[i * Width + 2 * j] = (char)((int)(unsigned char)Data[3 * (i * Width + j) + 2] + (int)Data[3 * (i * Width + j + length / 2) + 2]);
			Rbuf[i * Width + 2 * j + 1] = (char)((int)(unsigned char)Data[3 * (i * Width + j) + 2] - (int)Data[3 * (i * Width + j + length / 2) + 2]);
		}
	}

	for (int i = 0; i < length; i++) {
		for (int j = 0; j < length; j++) {
			Data[3 * (i * Width + j)] = Bbuf[i * Width + j];
			Data[3 * (i * Width + j) + 1] = Gbuf[i * Width + j];
			Data[3 * (i * Width + j) + 2] = Rbuf[i * Width + j];
			//fprintf(stdout, "%d, %d, %d\n", Data[3 * i], Data[3 * i + 1], Data[3 * i + 2]);
		}
	}

	free(Rbuf);
	free(Gbuf);
	free(Bbuf);
}

void MyImage::decompressColumns(int length)
{
	char* Rbuf = new char[Height * Width];
	char* Gbuf = new char[Height * Width];
	char* Bbuf = new char[Height * Width];

	for (int j = 0; j < length; j++) {
		for (int i = 0; i < length / 2; i++) {
			Bbuf[2 * i * Width + j] = (char)((int)(unsigned char)Data[3 * (i * Width + j)] + (int)Data[3 * ((i + length / 2) * Width + j)]);
			Bbuf[(2 * i + 1) * Width + j] = (char)((int)(unsigned char)Data[3 * (i * Width + j)] - (int)Data[3 * ((i + length / 2) * Width + j)]);
			Gbuf[2 * i * Width + j] = (char)((int)(unsigned char)Data[3 * (i * Width + j) + 1] + (int)Data[3 * ((i + length / 2) * Width + j) + 1]);
			Gbuf[(2 * i + 1) * Width + j] = (char)((int)(unsigned char)Data[3 * (i * Width + j) + 1] - (int)Data[3 * ((i + length / 2) * Width + j) + 1]);
			Rbuf[2 * i * Width + j] = (char)((int)(unsigned char)Data[3 * (i * Width + j) + 2] + (int)Data[3 * ((i + length / 2) * Width + j) + 2]);
			Rbuf[(2 * i + 1) * Width + j] = (char)((int)(unsigned char)Data[3 * (i * Width + j) + 2] - (int)Data[3 * ((i + length / 2) * Width + j) + 2]);
		}
	}

	for (int i = 0; i < length; i++) {
		for (int j = 0; j < length; j++) {
			Data[3 * (i * Width + j)] = Bbuf[i * Width + j];
			Data[3 * (i * Width + j) + 1] = Gbuf[i * Width + j];
			Data[3 * (i * Width + j) + 2] = Rbuf[i * Width + j];
			//fprintf(stdout, "%d, %d, %d\n", Data[3 * i], Data[3 * i + 1], Data[3 * i + 2]);
		}
	}

	free(Rbuf);
	free(Gbuf);
	free(Bbuf);
}

void MyImage::encodeDWT()
{
	for (int i = 512; i != 1; i /= 2) {
		compressRows(i);
		compressColumns(i);
	}
}

void MyImage::decodeDWT()
{
	for (int i = 2; i <= 512; i *= 2) {
		decompressColumns(i);
		decompressRows(i);
	}
}

void MyImage::setCoeffs()
{
	int squareLength = sqrt(numCoeffs);
	for (int i = 0; i < Height; i++) {
		for (int j = 0; j < Width; j++) {
			if (!(i < squareLength && j < squareLength)) {
				Data[3 * (i * Width + j)] = 0;
				Data[3 * (i * Width + j) + 1] = 0;
				Data[3 * (i * Width + j) + 2] = 0;
			}
		}
	}
}
