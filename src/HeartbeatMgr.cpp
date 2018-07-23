#include "application.h"
#include "HeartbeatMgr.h"
#include "Publisher.h"

using namespace ParticleMQTTWrapper;

HeartbeatMgr* HeartbeatMgr::_instance = NULL;

HeartbeatMgr::HeartbeatMgr() : _lastHeartbeatTime(0)
{

}

void HeartbeatMgr::loop()
{
    // check if its time to send another heartbeat - every 15 seconds
    int now = Time.now();
    if((now-_lastHeartbeatTime) >= 15)
    {
        Publisher p;
        p.sendHeartbeat();
        _lastHeartbeatTime = now;
    }
}
