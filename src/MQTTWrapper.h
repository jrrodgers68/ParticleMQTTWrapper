#pragma once

#include <MQTT.h>

namespace ParticleMQTTWrapper {


class MQTTWrapper
{
public:
    static MQTTWrapper* instance()
    {
        if(_instance == NULL)
        {
            _instance = new MQTTWrapper();
        }
        return _instance;
    }

    static void callback(char*,uint8_t*, unsigned int);

    MQTT&  client()  {  return *_client; }

    void init(const char* broker, int port, const char* connectionName, void (*callback)(const char*,uint8_t*,unsigned int));

    void loop();

private:
    MQTTWrapper();

    static MQTTWrapper* _instance;
    MQTT* _client = NULL;
    const char* _connectionName = NULL;

    void (*clientCallback)(const char*,uint8_t*,unsigned int);
};

}
