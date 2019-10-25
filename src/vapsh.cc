#ifndef __VAPSH_CC__
#define __VAPSH_CC__

#include "vapsh.h"


int main(int argc, char *argv[])
{
  HostapdCtrlIF instance = new HostapdCtrlIF();
  instance->createSockIF(HostapdCtrlIF::UNIX);
  instance->main(argc, argv);

  return(0);
}

#endif /*__VAPSH_CC__*/
