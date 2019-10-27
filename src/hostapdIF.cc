#ifndef __HOSTAPD_IF_CC__
#define __HOSTAPD_IF_CC__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "readlineIF.h"
#include "hostapdIF.h"

int HostapdCtrlIF::main(int argc, char *argv[])
{
  ReadlineIF *readlineIF = new ReadlineIF();
  HostapdTask *instance  = new HostapdTask(readlineIF, this);
  
  while(1)
  {  
    ACE_Reactor::instance()->handle_events();
  }

  delete readlineIF;
  return(0);
}

/*
 * @brief  This is the hook method for application to define this member function and is invoked by 
 *         ACE Framework.
 * @param  handle in which read/recv/recvfrom to be called.
 * @return 0 for success else for failure.
 */
int HostapdCtrlIF::handle_input(ACE_HANDLE handle)
{
  if(HostapdCtrlIF::UNIX == ctrlIntfType())
  {
    /*UNIX socket for IPC.*/  
  }
  else if(HostapdCtrlIF::UDP ==  ctrlIntfType())
  {
    /*UDP socket for IPC.*/
  }
  else
  {
    /*TCP socket for IPC.*/
  }

  return(0);
}

/*
 * @brief  This is the hook method of ACE Event Handler and is called by ACE Framework to retrieve the
 *         handle. The handle is nothing but it's fd - file descriptor.
 * @param  none
 * @return handle of type ACE_HANDLE
 */
ACE_HANDLE HostapdCtrlIF::get_handle(void) const
{
  return(const_cast<HostapdCtrlIF *>(this)->handle());
}

ACE_HANDLE HostapdCtrlIF::handle(void)
{
  return(m_handle);  
}

void HostapdCtrlIF::handle(ACE_HANDLE handle)
{
  m_handle = handle;  
}

HostapdCtrlIF::HostapdCtrlIF(CtrlIntfType_t ctrlIFType)
{
  ctrlIntfType(ctrlIFType);
  
  if(HostapdCtrlIF::UNIX == ctrlIFType)
  {
    m_unixAddr.set("/var/run/hostapd");
    //m_unixDgram.set(m_unixAddr);
    m_unixDgram.open(m_unixAddr);
    handle(m_unixDgram.get_handle());
    ACE_Reactor::instance()->register_handler(this, ACE_Event_Handler::READ_MASK);
  }
  else if(HostapdCtrlIF::UDP == ctrlIFType)
  {
    m_addr.set(9877);
    //m_sockDgram.set(m_addr);
    m_sockDgram.open(m_addr);
    handle(m_sockDgram.get_handle());
    ACE_Reactor::instance()->register_handler(this, ACE_Event_Handler::READ_MASK);
  }
  else
  {
    /*To be added for TCP.*/
    ;
  }
}

/*HostapdTask Class Definition...*/
HostapdTask::HostapdTask(ReadlineIF *pReadlineIF, 
                         HostapdCtrlIF *pCtrlIF)
{
  do 
  {
    readlineIF(pReadlineIF);
    if(!readlineIF())
    {
      ACE_ERROR((LM_ERROR, "Pointer to readlineIF is NULL\n"));
      break;
    }

    hostapdCtrlIF(pCtrlIF);
    if(!hostapdCtrlIF())
    {
      ACE_ERROR((LM_ERROR, "Ppointer to hostapdCtrlIF is NULL\n"));
      break;
    }
    
    //readlineIF()->hostapdTask(this);
    readlineIF()->init();
    open(0);

  }while(0);
}

HostapdTask::~HostapdTask()
{
  
}

int HostapdTask::open(void *args)
{
  /*Make this object as an Active Object.*/
  activate(THR_NEW_LWP);
  return(0);
}


int HostapdTask::svc(void)
{
  char *line = NULL;
  char *s = NULL;

  /* Loop reading and executing lines until the user quits. */
  for( ; !readlineIF()->continueStatus(); )
  {
    line = readline(readlineIF()->prompt());

    if(!line)
      break;

    /* Remove leading and trailing whitespace from the line.
       Then, if there is anything left, add it to the history list
       and execute it. */
    s = readlineIF()->stripwhite(line);

    if(*s)
    {
      //readlineIF()->add_history(s);
      add_history(s);
      readlineIF()->executeLine(s);
    }

    free (line);
  }

  exit(0);
}



#endif /*__HOSTAPD_AGENT_CC__*/
