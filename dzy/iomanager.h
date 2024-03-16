#pragma  once

#include <atomic>
#include <memory>
#include <functional>
#include <vector>
#include "schedule.h"
#include "timer.h"




namespace dzy {

class IOManager : public Schedule , public TimerManager {
public:
    typedef std::shared_ptr<IOManager> ptr;

    IOManager(int num,bool use_caller = true, std::string name = "Schedule_Name");
    ~IOManager();

    static IOManager* GetThis();
    int* getPipe(){return m_pipeFd;}
    
    enum Event{
        NONE = 0x00,
        READ = 0x01,
        WRITE = 0x04
    };
    struct fdContext {
        struct EventContext {
            Schedule* schedule = Schedule::GetThis();
            Fiber::ptr fiber;
            std::function<void()> cb;
            void reset();
        };
        void trigger(Event event);
        EventContext& getEventCtx(Event event);
        int fd = 0;
        Event event = Event::NONE;
        EventContext  read;
        EventContext  write;
    };

    int addEvent(int fd,Event event,std::function<void()> cb = nullptr);
    bool delEvent(int fd,Event event);
    bool cancleEvent(int fd,Event event);
    bool cancleAll(int fd);
    void resizeFdCtx(size_t);
protected:
    void tickle() override;
    void idle() override;
    bool stopping() override;
    void timerInsertAtFront() override;
private:
    int m_epfd = 0;
    int m_pipeFd[2];
    std::vector<fdContext*> m_fdCtx;
    std::atomic<uint64_t>  m_pendCount {0};




};

}
