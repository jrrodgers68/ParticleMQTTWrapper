
#pragma once

#include <list>

namespace ParticleMQTTWrapper {

class Buffer;

class BufferMgr
{
private:
    static BufferMgr* _instance;

    BufferMgr();

    std::list<Buffer*> _freeList;
    std::list<Buffer*> _inuseList;

public:
    static BufferMgr* instance()
    {
        if(_instance == NULL)
        {
            _instance = new BufferMgr();
        }
        return _instance;
    }

    Buffer*  allocate(unsigned long size);
    void deallocate(Buffer *);
};
}
