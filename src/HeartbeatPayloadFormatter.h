#pragma once


namespace ParticleMQTTWrapper {

// forward declarations
class Buffer;


class HeartbeatPayloadFormatter
{
public:
    HeartbeatPayloadFormatter();

    Buffer* writeMessage();

private:
    Buffer* getUpTime();
    bool formatMessage(Buffer*);

};

}
