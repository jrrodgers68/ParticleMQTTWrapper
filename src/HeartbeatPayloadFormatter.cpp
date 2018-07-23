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
    Buffer* buf = BufferMgr::instance()->allocate(64);
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

String HeartbeatPayloadFormatter::getUpTime()
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

    String sDays(days);
    String sHours(hours);
    String sMins(minutes);
    String sSecs(seconds);

    String result = sDays + " days, " + sHours + " hours, " + sMins + " minutes, " + sSecs + " seconds";
    return result;
}

bool HeartbeatPayloadFormatter::formatMessage(Buffer* p)
{
    StaticJsonBuffer<512> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    JsonObject& message = root.createNestedObject("message");

    String ts = Time.format(Time.now(), TIME_FORMAT_ISO8601_FULL);
    message["timestamp"] = ts.c_str();

    IPAddress ip = WiFi.localIP();
    String sip = ip.toString();
    message["ipaddress"] = sip.c_str();

    uint32_t freeMemory = System.freeMemory();
    message["free_memory"] = freeMemory;

    String upTime = getUpTime();
    message["up_time"] = upTime.c_str();

    p->size() = root.printTo(p->buffer(), p->maxSize());
    return p->size() > 0;
}
