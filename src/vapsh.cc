#ifndef __VAPSH_CC__
#define __VAPSH_CC__

#include "vapsh.h"


int main(int argc, char *argv[])
{
  HostapdCtrlIF instance = new HostapdCtrlIF(HostapdCtrlIF::UNIX);
  instance->main(argc, argv);

  delete instance;
  return(0);
}

#endif /*__VAPSH_CC__*/
