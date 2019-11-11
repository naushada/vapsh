#ifndef __HOSTAPD_IF_H__
#define __HOSTAPD_IF_H__

/*
This file interfaces with ACE Framework library and provides the implementation
for command/response to/from hostapd.
Copyright (C) <2019>  <Mohammed Naushad Ahmed>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <ace/Reactor.h>
#include <ace/Basic_Types.h>
#include <ace/Event_Handler.h>
#include <ace/Task.h>
#include <ace/INET_Addr.h>
#include <ace/UNIX_Addr.h>
#include <ace/SOCK_Dgram.h>
#include <ace/LSOCK_CODgram.h>
#include <ace/LSOCK_Dgram.h>
#include <ace/Task_T.h>
#include <ace/UNIX_Addr.h>

#include "readlineIF.h"

#define HOSTAPD_UNIX_SOCK_PATH "/var/run/hostapd/wlan0"
#define HOSTAPD_UNIX_LOCAL_SOCK_PATH "/tmp/vapsh"
#define HOSTAPD_DEST_UDP_PORT 9877
#define HOSTAPD_DEST_IP "10.100.16.1"
#define HOSTAPD_LOCAL_UDP_PORT 59877
#define HOSTAPD_LOCAL_IP "127.0.0.1"

/*Forward Declaration...*/
class HostapdTask;

class HostapdCtrlIF : public ACE_Event_Handler
{
  public:
    virtual int handle_input(ACE_HANDLE handle = ACE_INVALID_HANDLE);
    virtual int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask mask);
    virtual ACE_HANDLE get_handle(void) const;

    // Called when timer expires (TV stores the
    // current time and ARG is the argument given
    // when the handler was originally scheduled).
    virtual int handle_timeout(const ACE_Time_Value &tv,const void *arg = 0);

    // = Timer-based event handler methods
    // Register a handler to expire at time DELTA.
    // When DELTA expires the handle_timeout()
    // method will be called with the current time
    // and ARG as parameters.  If INTERVAL is > 0
    // then the handler is reinvoked periodically
    // at that INTERVAL.  DELTA is interpreted
    // "relative" to the current time of day.
    virtual void schedule_timer (ACE_Event_Handler *,
				 const void *arg,
				 const ACE_Time_Value &delta,
				 const ACE_Time_Value &interval =ACE_Timer_Queue::zero);
    // Locate and cancel timer.
    virtual void cancel_timer (ACE_Event_Handler *);
  
    /*Default constructor.*/
    HostapdCtrlIF();
    virtual ~HostapdCtrlIF();
    enum CtrlIntfType_t {UNIX, UDP, TCP};

    /*Parametrize constructor*/
    HostapdCtrlIF(CtrlIntfType_t ctrlIFType);
    void handle(ACE_HANDLE handle);
    ACE_HANDLE handle(void);
    void ctrlIntfType(CtrlIntfType_t ctrlIFType);
    CtrlIntfType_t ctrlIntfType(void);
    int main(int argc, char *argv[]);
    int transmit(char *command);
  
  private:
    ACE_HANDLE  m_handle;
    ACE_UNIX_Addr  m_unixAddr;
    /*Local Address*/
    ACE_INET_Addr  m_addr;
    ACE_SOCK_Dgram  m_sockDgram;
    ACE_LSOCK_Dgram  m_unixDgram;
    /*Connection Oriented Datagram*/
    ACE_LSOCK_CODgram  m_unixCODgram;
    CtrlIntfType_t  m_ctrlIntfType;
    HostapdTask  *m_hostapdTask;
    
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
