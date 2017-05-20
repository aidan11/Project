#ifndef Texture_h__
#define Texture_h__

#include <windows.h>

class Texture {
public:
	Texture(const HBITMAP& hbmp) : hBMP(hbmp) {
		glGenTextures(1, &texid);	
		GetObject(hBMP, sizeof(BMP), &BMP);	
	}

	~Texture() {
		DeleteObject(hBMP);
	}

	void bind() {
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 4);								// Pixel Storage Mode (Word Alignment / 4 Bytes) 
		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, texid);								// Bind To The Texture ID
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Min Filter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Linear Mag Filter
		glTexImage2D(GL_TEXTURE_2D, 0, 3, BMP.bmWidth, BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits); //specify the two-dimensional texture image
	}


private:
	HBITMAP hBMP;
	BITMAP BMP;
	GLuint texid;
};
#endif // Texture_h__