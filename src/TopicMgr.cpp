#include "application.h"
#include "TopicMgr.h"

using namespace ParticleMQTTWrapper;

TopicMgr* TopicMgr::_instance = NULL;


TopicMgr::TopicMgr()
{
    // register known topics:
    //     device/<device_id>/log
    //     device/<device_id>/config
    String id = System.deviceID();
    char buf[128];
    memset(buf, 0, 128);

    String base("device/");
    String log = base + id + "/log";
    String config = base + id + "/config";
    String heartbeat = base + id + "/heartbeat";

    log.toCharArray(buf, 128);
    registerTopic("log", buf);

    config.toCharArray(buf, 128);
    registerTopic("config", buf);

    heartbeat.toCharArray(buf, 128);
    registerTopic("heartbeat", buf);
}

void TopicMgr::registerTopic(const char* name, const char* topic)
{
    _topicMap[name] = topic;
}

const char* TopicMgr::findTopic(const char* name)
{
    const char* retval = NULL;

    std::map<std::string,std::string>::iterator it = _topicMap.find(name);
    if(it != _topicMap.end())
    {
        std::string& topic = it->second;
        retval = topic.c_str();
    }

    return retval;
}
