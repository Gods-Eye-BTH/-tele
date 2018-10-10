#include "liveMedia.hh"
#include "GroupsockHelper.hh"
#include <iostream>
#include "BasicUsageEnvironment.hh"
#include "CameraJPEGDeviceSource.hh"
#include "JPEGMediaSubsession.hh"
#include "CameraThread.hh"

#include <thread>


//#include "MJPEGVideoSource.hh"

UsageEnvironment* env;
char* progName;
int fps = 30;

void play(TaskScheduler* scheduler); // forward

void usage() {
  *env << "Usage: " << progName << " <frames-per-second>\n";
  system("pause");
  exit(1);
}

int main(int argc, char** argv) {
	// Begin by setting up our usage environment:
	TaskScheduler* scheduler = BasicTaskScheduler::createNew();
	env = BasicUsageEnvironment::createNew(*scheduler);

	//  OutPacketBuffer::numPacketsLimit = 100;
	  // Allow for up to 100 RTP packets per JPEG frame

  progName = argv[0];
  if (argc != 2)
  {
	  usage();
  }
  if (sscanf(argv[1], "%d", &fps) != 1 || fps <= 0) 
  {
    usage();
  }
//  strcpy(InputFileName, argv[2]);

  play(scheduler);

  return 0;
}

void afterPlaying(void* clientData); // forward

// A structure to hold the state of the current session.
// It is used in the "afterPlaying()" function to clean up the session.
struct sessionState_t {
  FramedSource* source;
  RTPSink* sink;
  RTCPInstance* rtcpInstance;
  Groupsock* rtpGroupsock;
  Groupsock* rtcpGroupsock;
  RTSPServer* rtspServer;
} sessionState;

static void announceStream(RTSPServer* rtspServer, ServerMediaSession* sms,
			   char const* streamName, char const* inputFileName) {
  char* url = rtspServer->rtspURL(sms);
  UsageEnvironment& env = rtspServer->envir();
  env << "\n\"" << streamName << "\" stream, from the file \""
      << inputFileName << "\"\n";
  env << "Play this stream using the URL \"" << url << "\"\n";
  delete[] url;
}

void play(TaskScheduler* scheduler) {
  // Open the camera:
  unsigned timePerFrame = 1000000/fps; // microseconds
  OutPacketBuffer::maxSize=200000;

  Boolean reuseFirstSource = True;

  UserAuthenticationDatabase* authDB = NULL;
  // Create the RTSP server:
  RTSPServer* rtspServer = RTSPServer::createNew(*env, 554, authDB);
  if (rtspServer == NULL) {
    *env << "Failed to create RTSP server: " << env->getResultMsg() << "\n";
    exit(1);
  }

  sessionState.source
	  = CameraJPEGDeviceSource::createNew(*env, timePerFrame);
//  sessionState.source
//	  = MJPEGVideoSource::createNew(*env, 0, 1);
  if (sessionState.source == NULL) {
    *env << "Unable to open camera: "
	 << env->getResultMsg() << "\n";
    exit(1);
  }
  
  char const* descriptionString
    = "Session streamed by \"testOnDemandRTSPServer\"";
  {
    char const* streamName = "jpeg";
    char const* inputFileName = "cvideo-JPEG-1.jpeg";
    ServerMediaSession* sms
      = ServerMediaSession::createNew(*env, streamName, streamName,
				      descriptionString);
	JPEGMediaSubsession *jpeg_subsession = new JPEGMediaSubsession(*env, inputFileName, reuseFirstSource);

	sms->addSubsession(jpeg_subsession);
//    sms->addSubsession(
//		       ::createNew(*env, inputFileName, reuseFirstSource, 5.0f));
    rtspServer->addServerMediaSession(sms);

    announceStream(rtspServer, sms, streamName, inputFileName);
  }
  

  std::thread t1(cam_thread_main, scheduler, fps);
  
  
  env->taskScheduler().doEventLoop();
  //t1.join();
}


void afterPlaying(void* /*clientData*/) {
  *env << "...done streaming\n";

  // End by closing the media:
/*  Medium::close(sessionState.rtspServer);
  Medium::close(sessionState.sink);
  delete sessionState.rtpGroupsock;
  Medium::close(sessionState.source);
  Medium::close(sessionState.rtcpInstance);
  delete sessionState.rtcpGroupsock;
*/
  // We're done:
  exit(0);
}
