#include "JPEGMediaSubsession.hh"
#include "FramedSource.hh"
#include "CameraJPEGDeviceSource.hh"
#include "JPEGVideoRTPSink.hh"

JPEGMediaSubsession::JPEGMediaSubsession(UsageEnvironment& env, char const* fileName,
			    Boolean reuseFirstSource)
  : OnDemandServerMediaSubsession(env, reuseFirstSource),
    fFileSize(0) {
  fFileName = strDup(fileName);
}

JPEGMediaSubsession::~JPEGMediaSubsession() {
  delete[] (char*)fFileName;
}


FramedSource* JPEGMediaSubsession::createNewStreamSource(unsigned clientSessionId, unsigned& estBitrate)
{ 
	FramedSource* framedSource = CameraJPEGDeviceSource::createNew(envir(), clientSessionId);
  return framedSource;
}

RTPSink* JPEGMediaSubsession::createNewRTPSink(Groupsock* rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic, FramedSource* inputSource){

   return JPEGVideoRTPSink::createNew(envir(), rtpGroupsock);
}
