#ifndef BmpReader_h__
#define BmpReader_h__

#include <windows.h>		// Header File For Windows
#include <GL/GL.h>

class BmpReader {
public:
	static HBITMAP loadBMP(const char *filename) {
		HBITMAP hBMP;														// Handle Of The Bitmap
		hBMP=(HBITMAP)LoadImageA(GetModuleHandle(NULL), filename, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE );
		return hBMP;
	}
};

#endif // BmpReader_h__