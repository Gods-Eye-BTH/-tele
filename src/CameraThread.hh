#ifndef _CAMERA_THREAD_HH_INCLUDED
#define _CAMERA_THREAD_HH_INCLUDED

#include "BasicUsageEnvironment.hh"
#include <mutex>

extern std::mutex lock_jpegbuffer;
extern char CamThreadFrameBuffer[256 * 1024];
extern int CamThreadFrameBufferSize;
void cam_thread_main(TaskScheduler* env, int fps);


#endif /* _CAMERA_THREAD_HH_INCLUDED */
