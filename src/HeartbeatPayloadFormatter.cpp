#include "application.h"
#include "HeartbeatPayloadFormatter.h"
#include "Buffer.h"
#include "BufferMgr.h"
#include <SparkJson.h>
#include <time.h>


using namespace ParticleMQTTWrapper;


HeartbeatPayloadFormatter::HeartbeatPayloadFormatter()
{

}

Buffer* HeartbeatPayloadFormatter::writeMessage()
{
    // get buffer from BufferMgr
    // format message as json
    // write json object to buffer
    Buffer* buf = BufferMgr::instance()->allocate(256);
    if(buf)
    {
        if(!formatMessage(buf))
        {
            // failed - deallocate buffer
            BufferMgr::instance()->deallocate(buf);
            buf = NULL;
        }
    }

    return buf;
}

Buffer* HeartbeatPayloadFormatter::getUpTime()
{
    Buffer* buf = BufferMgr::instance()->allocate(64);
    if(buf)
    {
        // string as:  W days, X hours, Y minutes, Z seconds
        const int ms_per_day = 86400 * 1000;
        const int ms_per_hour = 3600 * 1000;
        const int ms_per_min = 60 * 1000;

        int up_millis = millis();
        int days = up_millis / ms_per_day;
        up_millis -= (days * ms_per_day);

        int hours = up_millis / ms_per_hour;
        up_millis -= (hours * ms_per_hour);

        int minutes = up_millis / ms_per_min;
        up_millis -= (minutes * ms_per_min);

        int seconds = up_millis / 1000;
        up_millis -= (seconds * 1000);

        snprintf(buf->buffer(), buf->maxSize(), "%d days, %d hours, %d minutes, %d seconds", days, hours, minutes, seconds);
    }

    return buf;
}

bool HeartbeatPayloadFormatter::formatMessage(Buffer* p)
{
    StaticJsonBuffer<512> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    JsonObject& message = root.createNestedObject("message");

    String ts = Time.format(Time.now(), TIME_FORMAT_ISO8601_FULL);
    message["timestamp"] = ts.c_str();

    IPAddress ip = WiFi.localIP();
    char sip[16];
    snprintf(sip, 16, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
    message["ipaddress"] = sip;

    uint32_t freeMemory = System.freeMemory();
    message["free_memory"] = freeMemory;

    Buffer* upTime = getUpTime();
    message["up_time"] = upTime->buffer();

    p->size() = root.printTo(p->buffer(), p->maxSize());

    BufferMgr::instance()->deallocate(upTime);

    return p->size() > 0;
}
