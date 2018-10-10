#ifndef JPEG_DEVICE_SOURCE_HH
#define JPEG_DEVICE_SOURCE_HH

//#ifndef _JPEG_VIDEO_SOURCE_HH
#include "JPEGVideoSource.hh"
//#endif

#include "JpegFrameParser.hh"

//#define JPEG_HEADER_SIZE 623
//#define JPEG_HEADER_SIZE 623
#define JPEG_HEADER_SIZE 0x299+12

//extern char InputFileName[100];

class CameraJPEGDeviceSource : public JPEGVideoSource {
public:
	static CameraJPEGDeviceSource* createNew(UsageEnvironment& env, unsigned clientSessionId);
	static CameraJPEGDeviceSource* createNew(UsageEnvironment& env, unsigned clientSessionId,
		unsigned timePerFrame);
	// "timePerFrame" is in microseconds

public:
	static EventTriggerId eventTriggerId;
	static CameraJPEGDeviceSource* ourDevice;

protected:
	CameraJPEGDeviceSource(UsageEnvironment& env,
		FILE* fid, unsigned timePerFrame);
	virtual ~CameraJPEGDeviceSource();

private:
	virtual void doGetNextFrame();
	virtual u_int8_t type();
	virtual u_int8_t qFactor();
	virtual u_int8_t width();
	virtual u_int8_t height();
	virtual u_int8_t const* quantizationTables(u_int8_t& precision,
		u_int16_t& length);

private:
	UsageEnvironment& fEnv;
	static void deliverFrame0(void* clientData);
	void deliverFrame();

	static void newFrameHandler(CameraJPEGDeviceSource* source, int mask);
	void newFrameHandler1();
	void deliverFrameToClient();
	void startCapture();
	void setParamsFromHeader();

private:

	FILE* fFid;
	JpegFrameParser *fJpegFrameParser;
	unsigned fTimePerFrame;
	struct timeval fLastCaptureTime;
	u_int8_t fType, fLastQFactor, fLastWidth, fLastHeight;
	Boolean fNeedAFrame;
	unsigned char fJPEGHeader[JPEG_HEADER_SIZE];

};

#endif
