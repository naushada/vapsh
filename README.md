# vapsh
this is the virtual Hostapd Shell for control interface to interface with hostapd.
___
## Pre-requisite
- Install ACE Library [Click here for ACE](http://download.dre.vanderbilt.edu/)
- Install readline Library [Clicke here for readline](https://ftp.gnu.org/gnu/readline/)
- Install pthread Library
## The Hostapd Control interface
 - Hostapd opens UNIX socket with name specified in ctrl_interface and default is **/var/run/hostapd**, 
 - Hostapd appends name of interface on which hostapd is running. **/var/run/hostapd/wlan0**
 ## vapsh usages 
 - by default it usages **/var/run/hostapds/wlan0** for interfacing with hostapd.
 - It can be run with first command line argument which is the prompt for vapsh.
 - By default vapsh prompt is (Peace)# 
 - vapsh supports the command name completion by pressing tha **tab** button
 - vapsh supports the command argument list commpletion by pressing **tab** button
## Example
 - (Peace)# ping - Press enter
 - The Output will be PONG from Hostapd

## How to Compile
 - cd <vapsh>
 - ./autogen.sh
 - ./configure

