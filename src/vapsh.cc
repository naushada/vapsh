#ifndef __VAPSH_CC__
#define __VAPSH_CC__

#include "vapsh.h"


int main(int argc, char *argv[])
{
  /*This virtual AP Shell interact with hostapd control interface via UNIX socket.*/
  HostapdCtrlIF *instance = new HostapdCtrlIF(HostapdCtrlIF::UNIX);
  instance->main(argc, argv);

  /*reclaim the memory now.*/
  delete instance;
  /*reset to NULL*/
  instance = NULL;
  return(0);
}

#endif /*__VAPSH_CC__*/
