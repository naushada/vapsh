#ifndef __VAPSH_CC__
#define __VAPSH_CC__

/*
This file interfaces with readlineIF and hostapdIF library and provides the implementation
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
