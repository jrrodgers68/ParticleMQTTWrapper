
#pragma once

class MQTT;
#include <list>


namespace ParticleMQTTWrapper {

class Buffer;

class JsonMQTTMsgMgr
{
private:
    JsonMQTTMsgMgr();

    static JsonMQTTMsgMgr* _instance;

    MQTT* _client = NULL;

    // keeps an internal list of messages
    // needs a message formatter - should be a provided object

    class Message
    {
    public:
        uint16_t             messageId;
        Buffer*              payload;
        const char*          topic;
        bool                 duplicate;

        Message()  {   reset();  }

        void reset();

        bool isValid()
        {
            return messageId != 0;
        }
    };

    std::list<Message*> _freeList;
    std::list<Message*> _outstandingList;

    Message* findNextFreeMessage();
    void resendMessages();
    void publish(Message*);

public:
    static JsonMQTTMsgMgr* instance()
    {
        if(_instance == NULL)
        {
            _instance = new JsonMQTTMsgMgr();
        }
        return _instance;
    }

    void init(MQTT& mqttClient, int default_message_count = 5);
    void publish(const char* topic, Buffer* payload);
    void messageAck(uint16_t messageId);

};

}
