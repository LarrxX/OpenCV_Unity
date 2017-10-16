#pragma once

#include "IUnityGraphics.h"

#include "opencv2/opencv.hpp"

#include <windows.h>
#include <GL/GL.h>


class OpenCVDllInterface
{
public:
	struct FrameInfo
	{
		unsigned int sizeInBytes;
		int width;
		int height;
	};

public:
	OpenCVDllInterface();
	~OpenCVDllInterface();

	void getFrameBufferInfo( FrameInfo& bufferInfo );
	void setTextureHandle(void* handle);

	void updateFrameDataOGL( int eventID );

private:
	cv::VideoCapture m_captureDevice;
	GLuint m_textureHandle;
	uchar* m_frameBuffer;
	size_t m_frameBufferSize;
};



