#ifndef __HOSTAPD_IF_CC__
#define __HOSTAPD_IF_CC__

#include "readlineIF.h"
#include "hostapdIF.h"

int HostapdIF::init(char *path)
{
  /*hostapd accepts request on port 9877 for control interface message.*/
  m_addr.set(9877);
  /*Creating Unix Socket*/
  m_unixAddr.set("/var/run/hostapd");

  /**/
  m_lsockDgram.set(m_unixAddr);
  m_lsockDgram.open();

  /**/
  m_sockDgram.set(m_addr);
  m_sockDgram.open();

  ACE_Reactor::instance()->register_event(this, ACE_ 
}

HostapdIF::HostapdIF(ReadlineIF *pReadlineIF)
{
  do 
  {
    readlineIF(pReadlineIF);
    if(!readlineIF())
    {
      ACE_ERRORO((LM_ERROR, "Pointer to readlineIF is NULL\n"));
      break;
    }

    readlineIF()->hostapdIF(this);
    readlineIF()->init();
    open();

  }while(0);

}

int HostapdIF::open(void)
{
  /*Make this object as an Active Object.*/
  activate(THR_NEW_LWP, 1);
  return(0);
}


int HostapdIF::svc(void)
{
  /*readline completer is bind successfully.*/

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
      readlineIF()->add_history(s);
      readlineIF()->executeLine(s);
    }

    free (line);
  }

  exit (0);
}

int HostapdIF::main(int argc, char *argv[])
{

  HostapdIF *instance = new HostapdIF(new ReadlineIF());
  

  return(1);
}

ACE_HANDLE HostapdIF::get_handle() const
{
  return(m_handle);
}

#endif /*__HOSTAPD_AGENT_CC__*/
