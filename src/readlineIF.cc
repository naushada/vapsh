#ifndef __READLINE_IF_CC__
#define __READLINE_IF_CC__

/*
This file interfaces with readline library and provides the implementation
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

#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "readlineIF.h"
#include "hostapdIF.h"

/*These are static data member*/
int ReadlineIF::m_offset = 0;
int ReadlineIF::m_len = 0;
int ReadlineIF::m_argOffset = 0;

/*This static data member stores the selected command*/
char *ReadlineIF::m_cmdName = NULL;

ReadlineIF::command ReadlineIF::m_command[256] = 
{
  /* command name */           /* command argument(s) */                 /* command description */ 
  { "ping",                    {/*argument list*/ NULL},                 "pings hostapd" },
  { "mib",                     {NULL},                 "get MIB variables (dot1x, dot11, radius)" },
  { "relog",                   {NULL},                 "reload/truncate debug log output file" },
  { "status",                  {NULL},                 "show interface status info" },
  { "sta",                     {NULL},                 "<addr> = get MIB variables for one station" },
  { "all_sta",                 {NULL},                 "get MIB variables for all stations" },
  { "list_sta",                {NULL},                 "list all stations" },
  { "new_sta",                 {NULL},                 "<addr> = add a new station" },
  { "deauthenticate",          {NULL},                 "<addr> = deauthenticate a station" },
  { "disassociate",            {NULL},                 "<addr> = disassociate a station" },
  { "signature",               {NULL},                 "<addr> = get taxonomy signature for a station" },
  { "sa_query",                {NULL},                 "<addr> = send SA Query to a station" },
  { "wps_pin",                 {NULL},                 "<uuid> <pin> [timeout] [addr] = add WPS "
                                                       "Enrollee PIN" },
  { "wps_check_pin",           {NULL},                 "<PIN> = verify PIN checksum" },
  { "wps_pbc",                 {NULL},                 "indicate button pushed to initiate PBC" },
  { "wps_cancel",              {NULL},                 " cancel the pending WPS operation" },
  { "wps_nfc_tag_read",        {NULL},                 "<hexdump> = report read NFC tag with WPS data" },
  { "wps_nfc_config_token",    {NULL},                 "<WPS/NDEF> = build NFC configuration token" },
  { "wps_nfc_token",           {NULL},                 "<WPS/NDEF/enable/disable> = manager "
                                                       "NFC password token" },
  { "nfc_get_handover_sel",    {NULL},                 NULL },
  /* command name */           /* command argument(s) */                 /* command description */ 
  { "wps_ap_pin", {"disable",
                   "random",
                   "get",
                   "set",
                   NULL},                 "<cmd> [params..] = enable/disable AP PIN" },
  { "wps_config",              {NULL},                 "<SSID> <auth> <encr> <key> = configure AP" },
  { "wps_get_status",          {NULL},                 "show current WPS status" },
  { "disassoc_imminent",       {NULL},                 "send Disassociation Imminent notification" },
  { "ess_disassoc",            {NULL},                 "send ESS Dissassociation Imminent notification" },
  { "bss_tm_req", {"disassoc_timer=", 
                   "valid_int=", 
                   "bss_term=",
                   "url=",
                   "pref=1",
                   "abridged=1",
                   NULL},                 "send BSS Transition Management Request, First Argument is the mac address in hex contiguous bytes" },
  { "get_config",              {NULL},                 "show current configuration" },
  { "help",                    {NULL},                 "= show this usage help" },
  { "interface",               {NULL},                 "[ifname] = show interfaces/select interface" },
  { "fst",                     {NULL},                 "<params...> = send FST-MANAGER control "
                                                       "interface command" },
  { "raw",                     {NULL},                 "<params..> = send unprocessed command" },
  { "level",                   {NULL},                 "<debug level> = change debug level" },
  { "license",                 {NULL},                 "show full hostapd_cli license" },
  { "quit",                    {NULL},                 "exit hostapd_cli" },
  { "set",  {"wps_version_number=", 
             "wps_testing_dummy_cred=", 
             "wps_corrupt_pkhash=", 
             "ext_mgmt_frame_handling=", 
             "ext_eapol_frame_io=",
             "dpp_config_obj_override=",
             "dpp_discovery_override=",
             "dpp_groups_override=",
             "dpp_ignore_netaccesskey_mismatch=",
             "dpp_test=",
             "mbo_assoc_disallow=",
             "dpp_configurator_params=",
             "deny_mac_file=",
             "accept_mac_file=",
             NULL},
                                                        "<name> <value> = set runtime variables" },
  /*get with arguments*/
  { "get",                     {"version", "tls_library", NULL}, "<name> = get runtime info" },
  { "set_qos_map_set",         {NULL},                 "<arg,arg,...> = set QoS Map set element" },
  { "send_qos_map_conf",       {NULL},                 "<addr> = send QoS Map Configure frame" },
  { "chan_switch",             {NULL},                 "<cs_count> <freq> [sec_channel_offset=] "
  /* command name */           /* command argument(s) */                 /* command description */ 
                                                       "[center_freq1=]\n"
                                                       "[center_freq2=] [bandwidth=] [blocktx] "
                                                       "[ht|vht]\n"
                                                       "initiate channel switch announcement" },
  { "hs20_wnm_notif",          {NULL},                 "<addr> <url>\n"
                                                       "send WNM-Notification Subscription "
                                                       "Remediation Request" },
  { "hs20_deauth_req",         {NULL},                 "<addr> <code (0/1)> <Re-auth-Delay(sec)> [url]\n"
                                                       "send WNM-Notification imminent deauthentication "
                                                       "indication" },
  { "vendor",                  {NULL},                 "<vendor id> <sub command id> "
                                                       "[<hex formatted data>]\n"
                                                       "send vendor driver command" },
  { "enable",                  {NULL},                 "enable hostapd on current interface" },
  { "reload",                  {NULL},                 "reload configuration for current interface" },
  { "disable",                 {NULL},                 "disable hostapd on current interface" },
  { "update_beacon",           {NULL},                 "update Beacon frame contents\n"},
  { "erp_flush",               {NULL},                 "drop all ERP keys"},
  { "log_level",               {NULL},                 "[level] = show/change log verbosity level" },
  { "pmksa",                   {NULL},                 "show PMKSA cache entries" },
  { "pmksa_flush",             {NULL},                 "flush PMKSA cache" },
  { "set_neighbor",            {NULL},                 "<addr> <ssid=> <nr=> [lci=] [civic=] [stat]\n"
                                                       "  = add AP to neighbor database" },
  { "remove_neighbor",         {NULL},                 "<addr> <ssid=> = remove AP from neighbor "
                                                       "database" },
  { "req_lci",                 {NULL},                 "<addr> = send LCI request to a station"},
  /* command name */           /* command argument(s) */                 /* command description */ 
  { "req_range",               {NULL},                 "send FTM range request"},
  { "driver_flags",            {NULL},                 " = show supported driver flags"},
  { "dpp_qr_code",             {NULL},                 "report a scanned DPP URI from a QR Code" },
  { "dpp_bootstrap_gen",       {NULL},                 "type=<qrcode> [chan=..] [mac=..] [info=..] "
                                                       "[curve=..] [key=..] = generate DPP bootstrap "
                                                       "information" },
  { "dpp_bootstrap_remove",    {NULL},                 "*|<id> = remove DPP bootstrap information" },
  { "dpp_bootstrap_get_uri",   {NULL},                 "<id> = get DPP bootstrap URI" },
  { "dpp_bootstrap_info",      {NULL},                 "<id> = show DPP bootstrap information" },
  { "dpp_auth_init",           {NULL},                 "peer=<id> [own=<id>] = initiate DPP "
                                                       "bootstrapping" },
  { "dpp_listen",              {NULL},                 "<freq in MHz> = start DPP listen" },
  { "dpp_stop_listen",         {NULL},                 "= stop DPP listen" },
  { "dpp_configurator_add",    {NULL},                 "[curve=..] [key=..] = add DPP configurator" },
  { "dpp_configurator_remove", {NULL},                 "*|<id> = remove DPP configurator" },
  { "dpp_configurator_get_key",{NULL},                 "<id> = Get DPP configurator's private key" },
  { "dpp_configurator_sign",   {NULL},                 "conf=<role> configurator=<id> = generate "
                                                       "self DPP configuration" },
  { "dpp_pkex_add",            {NULL},                 "add PKEX code" },
  { "dpp_pkex_remove",         {NULL},                 "*|<id> = remove DPP pkex information" },
  { "accept_acl",              {NULL},                 "Add/Delete/Show/Clear accept MAC ACL" },
  /* command name */           /* command argument(s) */                 /* command description */ 
  { "deny_acl",                {NULL},                 "Add/Delete/Show/Clear deny MAC ACL" },
  { "poll_sta",                {NULL},                 "<addr> = poll a STA to check connectivity "
                                                       "with a QoS null frame" },
  { "req_beacon",              {NULL},                 "<addr> [req_mode=] <measurement request "
                                                       "hexdump>  = "
                                                       "send a Beacon report request to a station" },
  { "reload_wpa_psk",          {NULL},                 "reload wpa_psk_file only" },
  { NULL,                      {NULL},                  NULL }
};

char *ReadlineIF::cmdName(void)
{
  return(m_cmdName);
}

void ReadlineIF::cmdName(char *cmdName)
{
  if(cmdName)
  {
    m_cmdName = strdup(cmdName);
  }
}

/*
 * @brief 
 * @param 
 * @return
 */
void ReadlineIF::prompt(char *prompt)
{
  m_prompt = strdup(prompt);
}

/*
 * @brief 
 * @param 
 * @return
 */
char *ReadlineIF::prompt()
{
  return(m_prompt);
}

char *commandArgListGenerator(const char *text, int state)
{
  /* If this is a new word to complete, initialize now.  This includes
     saving the length of TEXT for efficiency, and initializing the index
     variable to 0. */
  if (!state)
  {
    ReadlineIF::m_argOffset = 0;
  }

  int &idx = ReadlineIF::m_offset;
  int &inner = ReadlineIF::m_argOffset;

  if(NULL == ReadlineIF::m_command[idx].argv[inner])
    return (char *)NULL;

  return(strdup(ReadlineIF::m_command[idx].argv[inner++]));
}


char *commandArgGenerator(const char *text, int state)
{
  const char *name;
  /* If this is a new word to complete, initialize now.  This includes
     saving the length of TEXT for efficiency, and initializing the index
     variable to 0. */
  if (!state)
  {
    ReadlineIF::m_argOffset = 0;
  }

  int &idx = ReadlineIF::m_offset;
  int &inner = ReadlineIF::m_argOffset;
  /* Return the next name which partially matches from the command list. */
  while(NULL != (name = ReadlineIF::m_command[idx].argv[inner]))
  {
    inner += 1;
    if(strncmp (name, text, ACE_OS::strlen(text)) == 0)
    {
      return(strdup(name));
    }
  }

  /*Reset to default either for next command or command argument(s).*/
  ReadlineIF::m_argOffset = 0;

  /* If no names matched, then return NULL. */
  return((char *)NULL);
}

/* Generator function for command completion.  STATE lets us know whether
 * to start from scratch; without any state (i.e. STATE == 0), then we
 * start at the top of the list. 
 */
char *commandGenerator(const char *text, int state)
{
  const char *name;
  /* If this is a new word to complete, initialize now.  This includes
     saving the length of TEXT for efficiency, and initializing the index
     variable to 0. */
  if (!state)
  {
    ReadlineIF::m_offset = 0;
    ReadlineIF::m_len = strlen(text);
  }

  int &idx = ReadlineIF::m_offset;
  /* Return the next name which partially matches from the command list. */
  while(NULL != (name = ReadlineIF::m_command[idx].cmd))
  {
    idx += 1;
    if(strncmp (name, text, ReadlineIF::m_len) == 0)
    {
      return(strdup(name));
    }
  }

  /*Reset to default either for next command or command argument(s).*/
  ReadlineIF::m_offset = 0;

  /* If no names matched, then return NULL. */
  return((char *)NULL);
}

/* Attempt to complete on the contents of TEXT.  START and END show the
 * region of TEXT that contains the word to complete.  We can use the
 * entire line in case we want to do some simple parsing.  Return the
 * array of matches, or NULL if there aren't any. 
 */
char **commandCompletion(const char *text, int start, int end)
{
  char **matches;
  matches = (char **)NULL;

  /* If this word is at the start of the line, then it is a command
     to complete.  Otherwise it is the name of a file in the current
     directory. */
  if(start == 0)
  {
    rl_attempted_completion_over = 1; 
    matches = rl_completion_matches(text, commandGenerator);
  }
  else
  {
    /*user has hit the space bar*/
    if(start == end)
    {
      int idx = 0;
      /*remember it into its context - this is the command whose argument(s) to be listed.*/
      ReadlineIF::cmdName(rl_line_buffer);

      /*Return the entire arguments - list must have last element as NULL.*/
      for(idx = 0; ReadlineIF::m_command[idx].cmd; idx++)
      {
        if(!strncmp(rl_line_buffer, 
                    ReadlineIF::m_command[idx].cmd, 
                    ACE_OS::strlen(ReadlineIF::m_command[idx].cmd)))
        {
          /*remember this offset and will be used while looping through command arguments.*/
          ReadlineIF::m_offset = idx;
          rl_attempted_completion_over = 1; 
          matches = rl_completion_matches(text, commandArgListGenerator);
          /*break the while loop.*/
          break; 
        } 
      }
    }
    else
    {
      /*user has entered the initials of argument*/
      rl_attempted_completion_over = 1; 
      matches = rl_completion_matches(text, commandArgGenerator);
    }  
  }

  return(matches);
}

int ReadlineIF::init(void)
{

  rl_attempted_completion_over = 1; 
  /* Tell the completer that we want a crack first. */
  rl_attempted_completion_function = commandCompletion;
  return(0);
}

ReadlineIF::~ReadlineIF()
{
  delete m_prompt;
  /*set to default - NULL*/
  m_prompt = NULL;
}

ReadlineIF::ReadlineIF()
{
  m_prompt         = NULL;
  m_continueStatus = false;
  //m_hostapdTask    = NULL;
}

/* Look up NAME as the name of a command, and return a pointer to that
   command.  Return a NULL pointer if NAME isn't a command name. */

bool ReadlineIF::isValid(char *cmd)
{
  int i;
  bool result = false;
  do
  {
    for(i = 0; ReadlineIF::m_command[i].cmd; i++)
    {
      if(cmd && strcmp(cmd, ReadlineIF::m_command[i].cmd) == 0)
      {
        result = true;
        break;
      }
    }
  }while(0);

  return(result);
}

/* Strip whitespace from the start and end of STRING.  Return a pointer
   into STRING. */
char *ReadlineIF::stripwhite (char *string)
{
  char *s, *t; 

  for (s = string; whitespace (*s); s++)
    ;   
    
  if (*s == 0)
    return (s);

  t = s + strlen (s) - 1;
  while (t > s && whitespace (*t))
    t--;
  *++t = '\0';

  return s;
}

int ReadlineIF::executeLine(char *req)
{
  int i;
  char *cmd = NULL;
  char *cmdArg = NULL;
  bool isFound = false;
  char *line = strdup(req);
  int status = 1;

  /* Isolate the command word. */
  i = 0;
  while(line[i] && whitespace(line[i]))
    i++;
  cmd = line + i;

  while(line[i] && !whitespace(line[i]))
    i++;

  if(line[i])
    cmd[i++] = '\0';

  cmdArg = line + i;
  (void)cmdArg;

  isFound = isValid(cmd);

  do 
  {
    if(!isFound)
    {
      ACE_ERROR((LM_ERROR, "%s: No such command.\n", cmd));
      help(cmd);
      /*bypass the following statement.*/
      break;
    }
  
    if(!strncmp(cmd, "help", 4) || (!strncmp(cmd, "?", 1)))
    {
      help(cmd);
    }
    else if(!strncmp(cmd, "quit", 4))
    {
      quit();
    }
    else
    {
      /* Call the function. */
      //processCommand(origLine, strlen(origLine));
      status = 0;
    }

  }while(0);

  free(line);
  return(status);
}

#if 0
void ReadlineIF::hostapdTask(HostapdTask *hostapdTask)
{
  m_hostapdTask = hostapdTask;
}

HostapdTask *ReadlineIF::hostapdTask(void)
{
  return(m_hostapdTask);
}

#endif 

/* Print out help for cmd, or for all of the commands if cmd is
 * not present. 
 */
void ReadlineIF::help(char *cmd)
{
  int i;
  int printed = 0;

  for (i = 0; ReadlineIF::m_command[i].cmd; i++)
  {
    if(!*cmd || (!strcmp(cmd, ReadlineIF::m_command[i].cmd)))
    {
      printf("%s\t\t%s.\n", 
             ReadlineIF::m_command[i].cmd, 
             ReadlineIF::m_command[i].usage);
      printed++;
    }
  }

  if(!printed)
  {
    printf ("No commands match `%s'.  Possibilties are:\n", cmd);

    for (i = 0; ReadlineIF::m_command[i].cmd; i++)
    {
      /* Print in six columns. */
      if (printed == 10)
      {
        printed = 0;
        printf ("\n");
      }

      printf ("%s\t", ReadlineIF::m_command[i].cmd);
      printed++;
    }

    if(printed)
      printf ("\n");
  }

}

void ReadlineIF::quit(void)
{
  continueStatus(true);
}

void ReadlineIF::continueStatus(bool status)
{
  m_continueStatus = status;
}

bool ReadlineIF::continueStatus(void)
{
  return(m_continueStatus);
}

int ReadlineIF::processCommand(char *cmd, int len)
{
  ACE_DEBUG((LM_DEBUG, "The Command is %s\n", cmd));
  return(0);
}

int ReadlineIF::processResponse(char *rsp, int len)
{
  return(0);
}

#endif /*__READLINE_IF_CC__*/
