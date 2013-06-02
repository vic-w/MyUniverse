#include "stdafx.h"
#include "image.h"
#include "cv.h"
#include "highgui.h"

bool LoadTexture(const char *tex_name, unsigned int &texID)
{
	IplImage *image = cvLoadImage(tex_name);

	if(!image)
	{
		AfxMessageBox("¶ÁÈ¡Í¼Ïñ´íÎó£¡", NULL, MB_OK);
		return false;
	}

	int sizeX,sizeY;
	sizeX = image->width;
	sizeY = image->height;

	float texAspectRatio = (float)sizeX / (float)sizeY;

	// Generate a texture with the associative texture ID stored in the array
	glGenTextures(1, &texID);

	// This sets the alignment requirements for the start of each pixel row in memory.
	// glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

	// Bind the texture to the texture arrays index and init the texture
	glBindTexture(GL_TEXTURE_2D, texID);

	//Assign the mip map levels and texture info
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	// Build Mipmaps (builds different versions of the picture for distances - looks better)

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, sizeX, sizeY, GL_BGR_EXT, GL_UNSIGNED_BYTE, image->imageData);

	//glEnable(GL_TEXTURE_2D);
	// Now we need to free the image data that we loaded since openGL stored it as a texture
    cvReleaseImage(&image);
	
	return true;
}
