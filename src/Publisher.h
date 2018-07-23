#pragma once


namespace ParticleMQTTWrapper {


class Publisher
{
public:
    Publisher();

    static void writeLogMessage(const char* source, const char* text);
    static void sendHeartbeat();
};
}
