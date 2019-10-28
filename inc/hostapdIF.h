#ifndef __HOSTAPD_IF_H__
#define __HOSTAPD_IF_H__

#include <ace/Reactor.h>
#include <ace/Basic_Types.h>
#include <ace/Event_Handler.h>
#include <ace/Task.h>
#include <ace/INET_Addr.h>
#include <ace/UNIX_Addr.h>
#include <ace/SOCK_Dgram.h>
#include <ace/LSOCK_Dgram.h>
#include <ace/Task_T.h>
#include <ace/UNIX_Addr.h>

#include "readlineIF.h"

#define HOSTAPD_UNIX_SOCK_PATH "/var/run/hostapd"
#define HOSTAPD_UDP_PORT 9877
#define HOSTAPD_LO_IP "127.0.0.1"

/*Forward Declaration...*/
class HostapdTask;

class HostapdCtrlIF : public ACE_Event_Handler
{
  public:
    virtual int handle_input(ACE_HANDLE handle = ACE_INVALID_HANDLE);
    virtual int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask mask);
    virtual ACE_HANDLE get_handle(void) const;
    HostapdCtrlIF();
    virtual ~HostapdCtrlIF();
    enum CtrlIntfType_t {UNIX, UDP, TCP};
    HostapdCtrlIF(CtrlIntfType_t ctrlIFType);
    void handle(ACE_HANDLE handle);
    ACE_HANDLE handle(void);
    void ctrlIntfType(CtrlIntfType_t ctrlIFType);
    CtrlIntfType_t ctrlIntfType(void);
    int main(int argc, char *argv[]);
  
  private:
    ACE_HANDLE       m_handle;
    ACE_UNIX_Addr    m_unixAddr;
    ACE_INET_Addr    m_addr;
    ACE_SOCK_Dgram   m_sockDgram;
    ACE_LSOCK_Dgram  m_unixDgram;
    CtrlIntfType_t   m_ctrlIntfType;
    HostapdTask      *m_hostapdTask;
    
};

class HostapdTask : public ACE_Task<ACE_MT_SYNCH>
{
  public:
    virtual int open(void *args=0); 
    virtual int svc(void);
    //virtual int wait();
    //virtual int suspend(void);
    //virtual int resume(void);
    //virtual int activate(long flags);
    //virtual int close(u_long flags=0);
    //virtual int put(ACE_Message_Block *mb, ACE_Time_Value *to=0);

    HostapdTask(ACE_Thread_Manager *thrMgr,
                ReadlineIF *pReadlineIF, 
                HostapdCtrlIF *pCtrlIF);

    HostapdTask();
    virtual ~HostapdTask();
    void readlineIF(ReadlineIF *pReadlineIF);
    ReadlineIF *readlineIF(void);
    void hostapdCtrlIF(HostapdCtrlIF *ctrlIF);
    HostapdCtrlIF *hostapdCtrlIF(void);

  private:
    ReadlineIF    *m_readlineIF;
    HostapdCtrlIF *m_hostapdCtrlIF;
    
};

#endif /*__HOSTAPD_IF_H__*/
