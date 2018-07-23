//  This is the main header for the library
//  it exists to simplify usage, and provide
//   a place for helper functions


#include "TopicMgr.h"
#include "JsonMQTTMsgMgr.h"
#include "MQTTWrapper.h"
#include "Publisher.h"


namespace ParticleMQTTWrapper {

void init(const char* broker, int port,const char* connectionName, void (*callback)(const char*,uint8_t*,unsigned int));

void loop();

}
