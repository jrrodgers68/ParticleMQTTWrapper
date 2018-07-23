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
    String getUpTime();
    bool formatMessage(Buffer*);

};

}
