#pragma once

namespace ParticleMQTTWrapper {


class HeartbeatMgr
{
private:
    static HeartbeatMgr* _instance;

    HeartbeatMgr();

    int _lastHeartbeatTime;

public:
    static HeartbeatMgr* instance()
    {
        if(_instance == NULL)
        {
            _instance = new HeartbeatMgr();
        }
        return _instance;
    }

    void init() { } 

    void loop();
};
}
