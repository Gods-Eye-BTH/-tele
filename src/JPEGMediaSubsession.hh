#include "OnDemandServerMediaSubsession.hh"
#include "FramedSource.hh"
#include "RTPSink.hh"

class JPEGMediaSubsession: public OnDemandServerMediaSubsession {
public:
	JPEGMediaSubsession(UsageEnvironment& env, char const* fileName,
			    Boolean reuseFirstSource);
	~JPEGMediaSubsession();
protected:
	virtual FramedSource* JPEGMediaSubsession::createNewStreamSource(unsigned clientSessionId, unsigned& estBitrate);
	virtual RTPSink* JPEGMediaSubsession::createNewRTPSink(Groupsock* rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic, FramedSource* inputSource);
private:
  char const* fFileName;
  u_int64_t fFileSize; // if known
};
