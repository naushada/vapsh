#ifndef __HOSTAPD_IF_H__
#define __HOSTAPD_IF_H__

#include "ace/ACE_Event_Handler.h"
#include "ace/ACE_Task.h"

class HostapdIF : public ACE_Task<ACE_MT_SYNC>
{
  public:
    virtual int open(void *= 0); 
    virtual int handle_input(ACE_HANDLE fd);
    virtual int svc(void);
    virtual int wait();
    virtual int suspend(void);
    virtual int resume(void);
    virtual int activate(long flags);
    virtual int close(u_long flags=0);
    virtual int put(ACE_Message_Block *mb, ACE_Time_Value *to=0);
    virtual ACE_HANDLE get_handle() const;

    HostapdIF(ReadlineIF *pReadlineIF);
    HostapdIF();
    virtual ~HostapdIF();
    void readlineIF(ReadlineIF *pReadlineIF);
    ReadlineIF *readlineIF(void);
    int main(int argc, char *argv[]);
    int init(char *sockPath);

  private:
    ACE_HANDLE           m_handle;
    ReadlineIF          *m_readlineIF;
    ACE_LSOCK_Dgram      m_lsockDgram;
    ACE_UNIX_Addr        m_unixAddr;
    ACE_INET_Addr        m_addr;
    ACE_LSOCK_Connector  m_connector; 
};

#endif /*__HOSTAPD_IF_H__*/
