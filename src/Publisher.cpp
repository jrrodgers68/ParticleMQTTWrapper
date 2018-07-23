#include "application.h"
#include "Publisher.h"
#include "LogMessagePayloadFormatter.h"
#include "HeartbeatPayloadFormatter.h"
#include "TopicMgr.h"
#include "JsonMQTTMsgMgr.h"

using namespace ParticleMQTTWrapper;


Publisher::Publisher()
{

}

void Publisher::writeLogMessage(const char* source, const char* text)
{

    // use LogMessagePayloadFormatter to generate a buffer
    LogMessagePayloadFormatter lmpf;
    Buffer* p = lmpf.writeMessage(source, text);
    if(p)
    {
        // get topic from TopicMgr
        const char* topic = TopicMgr::instance()->findTopic("log");
        if(topic)
        {
            JsonMQTTMsgMgr::instance()->publish(topic, p);
        }
    }
}

void Publisher::sendHeartbeat()
{
    HeartbeatPayloadFormatter hbpf;
    Buffer* p = hbpf.writeMessage();
    if(p)
    {
        const char* topic = TopicMgr::instance()->findTopic("heartbeat");
        if(topic)
        {
            JsonMQTTMsgMgr::instance()->publish(topic, p);
        }
    }
}
