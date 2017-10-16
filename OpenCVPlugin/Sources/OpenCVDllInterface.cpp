#include "OpenCVDllInterface.h"



using namespace cv;
using namespace std;



OpenCVDllInterface::OpenCVDllInterface()
	: m_captureDevice()
	, m_textureHandle(0)
	, m_frameBuffer(NULL)
	, m_frameBufferSize(0)
{
	m_captureDevice.open(0);
}

OpenCVDllInterface::~OpenCVDllInterface()
{
	if (m_frameBuffer)
	{
		free(m_frameBuffer);
		m_frameBuffer = NULL;
		m_frameBufferSize = 0;
	}
}

void OpenCVDllInterface::getFrameBufferInfo(FrameInfo& bufferInfo)
{
	if (m_captureDevice.isOpened())
	{
		bufferInfo.width = int(m_captureDevice.get(CAP_PROP_FRAME_WIDTH));
		bufferInfo.height = int(m_captureDevice.get(CAP_PROP_FRAME_HEIGHT));

		cv::Mat frame;
		m_captureDevice >> frame;
		if (frame.empty())
		{
			bufferInfo.sizeInBytes = 0;
		}
		else
		{
			bufferInfo.sizeInBytes = unsigned int(frame.total() * 3);
		}
	}
}


void OpenCVDllInterface::setTextureHandle(void* handle)
{
	m_textureHandle = (GLuint)(size_t)handle;
}

void OpenCVDllInterface::updateFrameDataOGL(int eventID)
{
	if (m_captureDevice.isOpened())
	{
		cv::Mat originalFrame;
		m_captureDevice >> originalFrame;

		if (!originalFrame.empty())
		{
			cv::Mat frame;

			flip(originalFrame, frame, 0);

			size_t currentFrameSize = frame.total() * 3;

			if (m_frameBufferSize < currentFrameSize)
			{
				m_frameBufferSize = currentFrameSize;
				m_frameBuffer = (uchar*)realloc(m_frameBuffer, m_frameBufferSize);
			}

			Mat continuousRGB(frame.size(), CV_8UC4, m_frameBuffer);
			cvtColor(frame, continuousRGB, CV_BGR2RGB, 3);

			glBindTexture(GL_TEXTURE_2D, m_textureHandle);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// Set texture clamping method
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

			//set length of one complete row in data (doesn't need to equal image.cols)
			glPixelStorei(GL_UNPACK_ROW_LENGTH, (int)(frame.step / frame.elemSize()));

			glTexSubImage2D(GL_TEXTURE_2D,
				0,
				0,
				0,
				continuousRGB.size().width,
				continuousRGB.size().height,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				continuousRGB.data);
		}
	}
}
