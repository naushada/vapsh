#ifndef __HOSTAPD_IF_CC__
#define __HOSTAPD_IF_CC__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "readlineIF.h"
#include "hostapdIF.h"

void HostapdTask::readlineIF(ReadlineIF *readlineIF)
{
  m_readlineIF = readlineIF;  
}

ReadlineIF *HostapdTask::readlineIF(void)
{
  return(m_readlineIF);
}

void HostapdCtrlIF::ctrlIntfType(CtrlIntfType_t ctrlIFType)
{
  m_ctrlIntfType = ctrlIFType;
}

HostapdCtrlIF::CtrlIntfType_t HostapdCtrlIF::ctrlIntfType(void)
{
  return(m_ctrlIntfType);
}

int HostapdCtrlIF::main(int argc, char *argv[])
{
  ReadlineIF *readlineIF = new ReadlineIF();

  if(argc <= 1)
  {
    readlineIF->prompt("(#): ");
  }
  else
  {
    readlineIF->prompt(argv[1]);
  }

  HostapdTask *instance  = new HostapdTask(ACE_Thread_Manager::instance(),
                                           readlineIF, 
                                           this);
  /*! Time Out Value of 1sec.*/
  ACE_Time_Value to(1,0);
  while(1)
  {  
    if(-1 == ACE_Reactor::instance()->handle_events(to))
    {
      ACE_ERROR((LM_ERROR, "handle_events failed\n"));
      break;
    }
  }

  delete readlineIF;
  return(0);
}

int HostapdCtrlIF::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask mask)
{
 
  if(HostapdCtrlIF::UNIX == ctrlIntfType()) 
  {
    unlink(HOSTAPD_UNIX_SOCK_PATH);
  }

  ACE_DEBUG((LM_DEBUG, "handle_close Hook Method is called\n"));
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
  ACE_DEBUG((LM_DEBUG, "The Value of handle is %d\n", m_handle));
}

HostapdCtrlIF::~HostapdCtrlIF()
{
  if(ctrlIntfType() == HostapdCtrlIF::UNIX)
  {
    ACE_Reactor::instance()->remove_handler(this, ACE_Event_Handler::READ_MASK);
  }
  else if(ctrlIntfType() == HostapdCtrlIF::UDP)
  {
    ACE_Reactor::instance()->remove_handler(this, ACE_Event_Handler::READ_MASK);
    ACE_DEBUG((LM_DEBUG, "de-registering the handler\n"));
  }
  else
  {
    ACE_DEBUG((LM_DEBUG, "To be updated for TCP\n"));
  }
}

HostapdCtrlIF::HostapdCtrlIF(HostapdCtrlIF::CtrlIntfType_t ctrlIFType)
{
  do 
  {
    ctrlIntfType(ctrlIFType);
  
    if(HostapdCtrlIF::UNIX == ctrlIFType)
    {
      m_unixAddr.set(HOSTAPD_UNIX_SOCK_PATH);

      if(-1 == m_unixDgram.open(m_unixAddr))
      {
        ACE_ERROR((LM_ERROR,"Unix Socket Creation Failed\n"));
        perror("Open Failed:");
        break;
      }

      handle(m_unixDgram.get_handle());
      /*Note: Right after registering handler, ACE Framework calls get_handle 
              to retrieve the handle. The handle is nothing but a fd 
              (File Descriptor).*/
      ACE_Reactor::instance()->register_handler(this, ACE_Event_Handler::READ_MASK);
    }
    else if(HostapdCtrlIF::UDP == ctrlIFType)
    {
      ACE_INT32 len = strlen(HOSTAPD_LO_IP);
      m_addr.set_port_number(HOSTAPD_UDP_PORT);
      m_addr.set_address(HOSTAPD_LO_IP, len); 

      if(-1 == m_sockDgram.open(m_addr))
      {
        ACE_ERROR((LM_ERROR,
                  "UDP socket creation failed\n"));
        perror("Failed to create:");
        break;
      }  

      handle(m_sockDgram.get_handle());
      /*Note: Right after registering handler, ACE Framework calls get_handle 
              to retrieve the handle. The handle is nothing but a fd 
              (File Descriptor).*/
      ACE_Reactor::instance()->register_handler(this, ACE_Event_Handler::READ_MASK);
    }
    else
    {
      ACE_TRACE(("HostapdCtrlIF::TCP\n"));
      /*To be added for TCP.*/
      ;
    }

  }while(0);
}

/*HostapdTask Class Definition...*/
void HostapdTask::hostapdCtrlIF(HostapdCtrlIF *hostapdCtrlIF)
{
  m_hostapdCtrlIF = hostapdCtrlIF;
}

HostapdCtrlIF *HostapdTask::hostapdCtrlIF(void)
{
  return(m_hostapdCtrlIF);
}

HostapdTask::HostapdTask()
{
  m_readlineIF = NULL;
  m_hostapdCtrlIF = NULL;
}

HostapdTask::HostapdTask(ACE_Thread_Manager *thrMgr,
                         ReadlineIF *pReadlineIF, 
                         HostapdCtrlIF *pCtrlIF) 
            : ACE_Task<ACE_MT_SYNCH>(thrMgr)
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
      add_history(s);
      readlineIF()->executeLine(s);
    }

    free (line);
  }

  delete hostapdCtrlIF();
  exit(0);
}



#endif /*__HOSTAPD_AGENT_CC__*/
