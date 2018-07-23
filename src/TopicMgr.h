
#pragma once

#include <string>
#include <map>

namespace ParticleMQTTWrapper {


class TopicMgr
{
private:
    TopicMgr();

    static TopicMgr* _instance;

    std::map<std::string, std::string> _topicMap;

public:
    static TopicMgr* instance()
    {
        if(_instance == NULL)
        {
            _instance = new TopicMgr();
        }
        return _instance;
    }

    void init() { }

    void registerTopic(const char* name, const char* topic);
    const char* findTopic(const char* name);
};
}
