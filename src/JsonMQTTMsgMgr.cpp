#include "application.h"
#include "JsonMQTTMsgMgr.h"
#include <MQTT.h>
#include "Buffer.h"
#include "BufferMgr.h"

using namespace ParticleMQTTWrapper;

JsonMQTTMsgMgr* JsonMQTTMsgMgr::_instance = NULL;

void qoscallback(unsigned int messageid)
{
    JsonMQTTMsgMgr::instance()->messageAck(messageid);
}

void JsonMQTTMsgMgr::Message::reset()
{
    messageId = 0;
    duplicate = false;
    topic     = NULL;
    BufferMgr::instance()->deallocate(payload);
    payload    = NULL;
}


JsonMQTTMsgMgr::JsonMQTTMsgMgr()
{
}

void JsonMQTTMsgMgr::init(MQTT& mqttClient, int default_message_count)
{
    _client = &mqttClient;
    _client->addQosCallback(qoscallback);

    for(int i = 0; i < default_message_count; ++i)
    {
        Message* msg = new Message();
        _freeList.push_back(msg);
    }
}

void JsonMQTTMsgMgr::publish(const char* topic, Buffer* payload)
{
    Message* msg = findNextFreeMessage();
    if(!msg)
    {
        // fixme: how to long/alert on this???
        return;
    }

    msg->topic = topic;  // store the ptr, we don't own it!
    msg->payload = payload;
    publish(msg);
}

void JsonMQTTMsgMgr::messageAck(uint16_t messageId)
{
    std::list<Message*>::iterator it = _outstandingList.begin();
    while(it != _outstandingList.end())
    {
        Message* p = *it;
        if(p->messageId == messageId)
        {
            p->reset();
            resendMessages();
            return;
        }

        it++;
    }
}

JsonMQTTMsgMgr::Message* JsonMQTTMsgMgr::findNextFreeMessage()
{
    if(_freeList.size() == 0)
        return NULL;

    Message* p = _freeList.front();
    _freeList.pop_front();
    return p;
}

void JsonMQTTMsgMgr::resendMessages()
{
    if(_outstandingList.size() == 0)
        return;

    Message* p = _outstandingList.front();
    _outstandingList.pop_front();
    while(p)
    {
        if(p->isValid() == false)
        {
            // all done!
            p->reset();
            _freeList.push_back(p);
            return;
        }
        else
        {
            p->duplicate = true;
            publish(p);
            p = NULL;
            if(_outstandingList.size() > 0)
            {
                p = _outstandingList.front();
                _outstandingList.pop_front();
            }
        }
    }
}

void JsonMQTTMsgMgr::publish(JsonMQTTMsgMgr::Message* msg)
{
    bool result = _client->publish(msg->topic, (const uint8_t*)msg->payload->buffer(),
                                   msg->payload->size(), MQTT::QOS1, msg->duplicate,
                                   &msg->messageId);
    if(result)
    {
        _outstandingList.push_back(msg);
    }
    else
    {
        // fixme : alert somehow??
        msg->reset();
        _freeList.push_back(msg);
    }
}
