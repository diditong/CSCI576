//*****************************************************************************
//
// Image.cpp : Defines the class operations on images
//
// Author - Parag Havaldar
// Code used by students as starter code to display and modify images
//
//*****************************************************************************

#include "Image.h"

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

double MyImage::getCmax(double a, double b, double c)
{
	double max = (a < b) ? b : a;
	return ((max < c) ? c : max);
}

double MyImage::getCmin(double a, double b, double c)
{
	double max = (a > b) ? b : a;
	return ((max > c) ? c : max);
}

hsv MyImage::rgb2hsv(rgb RGB)
{
	hsv HSV;
	double cmin, cmax, delta;

	cmin = getCmin(RGB.r, RGB.g, RGB.b);
	cmax = getCmax(RGB.r, RGB.g, RGB.b);

	delta = cmax - cmin;

	if (delta < 0.00001) {
		HSV.h = 0.0;
	}
	else if (cmax == RGB.r) {
		HSV.h = 60.0 * (RGB.g - RGB.b) / delta;
	}
	else if (cmax == RGB.g) {
		HSV.h = 60.0 * ((RGB.b - RGB.r) / delta + 2.0);
	}
	else if (cmax == RGB.b) {
		HSV.h = 60.0 * ((RGB.r - RGB.g) / delta + 4.0);
	}

	if (HSV.h < 0.0) {
		HSV.h += 360.0;
	}
	HSV.s = (cmax < 0.00001) ? 0.0 : delta / cmax;
	HSV.v = cmax;
	
	//printf("HSV.h: %lf, HSV.s: %lf, HSV.v: %lf\n", HSV.h, HSV.s, HSV.v);
	return HSV;
}

rgb MyImage::hsv2rgb(hsv HSV)
{
	rgb RGB;
	double X, Y, Z, f;

	(HSV.h == 360.) ? (HSV.h = 0.) : (HSV.h /= 60.);
	
	f = HSV.h - floor(HSV.h);
	X = HSV.v * (1.0 - HSV.s);
	Y = HSV.v * (1.0 - HSV.s * f);
	Z = HSV.v * (1.0 - HSV.s * (1.0 - f));

	if (0. <= HSV.h && HSV.h < 1.)
		RGB = { HSV.v, Z, X };
	else if (1. <= HSV.h && HSV.h < 2.)
		RGB = { Y, HSV.v, X };
	else if (2. <= HSV.h && HSV.h < 3.)
		RGB = { X, HSV.v, Z };
	else if (3. <= HSV.h && HSV.h < 4.)
		RGB = { X, Y, HSV.v };
	else if (4. <= HSV.h && HSV.h < 5.)
		RGB = { Z, X, HSV.v };
	else if (5. <= HSV.h && HSV.h < 6.)
		RGB = { HSV.v, X, Y };
	else
		RGB = { 0., 0., 0. };

	return RGB;
}

// Here is where you would place your code to modify an image
// eg Filtering, Transformation, Cropping, etc.
bool MyImage::Modify(int lowerBound, int upperBound)
{
	double B, G, R;
	rgb RGB;
	hsv HSV;
	
	for (int i = 0; i < Width * Height; i++)
	{	
		B = (double)((unsigned char)Data[3 * i]);
		G = (double)((unsigned char)Data[3 * i + 1]);
		R = (double)((unsigned char)Data[3 * i + 2]);
		RGB = { R, G, B };
		HSV = rgb2hsv(RGB);
		
		//fprintf(stdout, "HSV.h: %lf, HSV.s: %lf, HSV.v: %lf\n", HSV.h, HSV.s, HSV.v);

		if ((HSV.h >= (double)lowerBound) && (HSV.h <= (double)upperBound))
		{
			RGB = hsv2rgb(HSV);
		}
		else 
		{
			RGB = hsv2rgb({ (double)0, (double)0, HSV.v });	
		}

		//fprintf(stdout, "RGB.r: %lf, RGB.g: %lf, RGB.b: %lf\n", RGB.r, RGB.g, RGB.b);

		Data[3 * i] = (unsigned char)(RGB.b);
		Data[3 * i + 1] = (unsigned char)(RGB.g);
		Data[3 * i + 2] = (unsigned char)(RGB.r);
	}

	
	return false;
}