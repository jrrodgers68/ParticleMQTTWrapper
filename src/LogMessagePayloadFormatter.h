#pragma once


namespace ParticleMQTTWrapper {

// forward declarations
class Buffer;


class LogMessagePayloadFormatter
{
public:
    LogMessagePayloadFormatter();

    Buffer* writeMessage(const char* source, const char* text);

private:
    bool formatMessage(Buffer*, const char* source, const char* text);
};
}
