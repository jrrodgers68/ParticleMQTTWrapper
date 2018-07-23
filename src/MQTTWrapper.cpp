#include "application.h"
#include "MQTTWrapper.h"
#include "HeartbeatMgr.h"

using namespace ParticleMQTTWrapper;


MQTTWrapper* MQTTWrapper::_instance = NULL;


MQTTWrapper::MQTTWrapper() : clientCallback(NULL)
{
}

void MQTTWrapper::callback(char* topic, uint8_t* payload, unsigned int length)
{
    if(MQTTWrapper::instance()->clientCallback)
    {
        MQTTWrapper::instance()->clientCallback(topic, payload, length);
    }
}

void MQTTWrapper::init(const char* broker,
                       int         port,
                       const char* connectionName,
                       void (*callback)(const char*,uint8_t*,unsigned int))
{
    _client = new MQTT((char*)broker, port, MQTTWrapper::callback);
    _connectionName = connectionName;
    clientCallback = callback;

    if(_client)
    {
        _client->connect(connectionName);
    }
}

void MQTTWrapper::loop()
{
    if(_client)
    {
        if(!_client->isConnected())
        {
            _client->connect(_connectionName);
        }

        _client->loop();
    }
}
