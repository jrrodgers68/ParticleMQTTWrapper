#include "application.h"
#include "LogMessagePayloadFormatter.h"
#include "Buffer.h"
#include "BufferMgr.h"
#include <SparkJson.h>
#include <time.h>


using namespace ParticleMQTTWrapper;


LogMessagePayloadFormatter::LogMessagePayloadFormatter()
{

}

Buffer* LogMessagePayloadFormatter::writeMessage(const char* source, const char* text)
{
    // get buffer from BufferMgr
    // format message as json
    // write json object to buffer
    Buffer* buf = BufferMgr::instance()->allocate(256);
    if(buf)
    {
        if(!formatMessage(buf, source, text))
        {
            // failed - deallocate buffer
            BufferMgr::instance()->deallocate(buf);
            buf = NULL;
        }
    }

    return buf;
}

bool LogMessagePayloadFormatter::formatMessage(Buffer* p, const char* source, const char* text)
{
    StaticJsonBuffer<1024> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    JsonObject& message = root.createNestedObject("message");

    char buf[32];
    memset(buf, 0, 32);
    String ts = Time.format(Time.now(), TIME_FORMAT_ISO8601_FULL);
    ts.toCharArray(buf, 32);
    message["timestamp"] = buf;

    message["source"] = source;
    message["message"] = text;

    p->size() = root.printTo(p->buffer(), p->maxSize());
    return p->size() > 0;
}
