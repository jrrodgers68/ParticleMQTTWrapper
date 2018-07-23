#include "application.h"
#include "ParticleMQTTWrapper.h"
#include "HeartbeatMgr.h"


void ParticleMQTTWrapper::init(const char* broker,
                                int port,
                                const char* connectionName,
                                void (*callback)(const char*,uint8_t*,unsigned int))
{
    ParticleMQTTWrapper::MQTTWrapper::instance()->init(broker, port, connectionName, callback);

    ParticleMQTTWrapper::TopicMgr::instance()->init();
    ParticleMQTTWrapper::JsonMQTTMsgMgr::instance()->init(ParticleMQTTWrapper::MQTTWrapper::instance()->client());

    ParticleMQTTWrapper::HeartbeatMgr::instance()->init();
}

void ParticleMQTTWrapper::loop()
{
    ParticleMQTTWrapper::MQTTWrapper::instance()->loop();
    ParticleMQTTWrapper::HeartbeatMgr::instance()->loop();
}
