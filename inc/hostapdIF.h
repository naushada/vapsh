#ifndef __HOSTAPD_IF_H__
#define __HOSTAPD_IF_H__

#include <ace/Event_Handler.h>
#include <ace/Task.h>

class HostapdCtrlIF : public ACE_EVENT_Handler
{
  public:
    virtual int handle_input(ACE_HANDLE handle = ACE_INVALID_HANDLE);
    virtual ACE_HANDLE get_handle(void) const;
    HostapdCtrlIF(int ctrlIFType);
    HostapdCtrlIF();
    virtual ~HostapdCtrlIF();
    enum CtrlIntfType_t {UNIX, UDP, TCP};
    void handle(ACE_HANDLE handle);
    ACE_HANDLE handle(void);
    void ctrlIntfType(CtrlIntfType_t ctrlIFType);
    CtrlIntfType_t ctrlIntfType(void);
    int main(int argc, char *argv[]);
  
  private:
    ACE_HANDLE          m_handle;
    ACE_UNIX_Addr       m_unixAddr;
    ACE_INET_Addr       m_addr;
    ACE_SOCK_Dgram      m_sockDgram;
    ACE_LSOCK_Dgram     m_unixDgram;
    ACE_LSOCK_Connector m_conector;
    CtrlIntfType_t      m_ctrlIntfType;
    HostapdTask         *m_hostapdTask;
    
};

class HostapdTask : public ACE_Task<ACE_MT_SYNC>
{
  public:
    virtual int open(void *= 0); 
    virtual int svc(void);
    virtual int wait();
    virtual int suspend(void);
    virtual int resume(void);
    virtual int activate(long flags);
    virtual int close(u_long flags=0);
    virtual int put(ACE_Message_Block *mb, ACE_Time_Value *to=0);

    HostapdTask(ReadlineIF *pReadlineIF, HostapdCtrlIF *pCtrlIF);
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
