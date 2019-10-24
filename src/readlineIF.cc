#ifndef __READLINE_IF_CC__
#define __READLINE_IF_CC__

#include <readline/readline.h>
#include <readline/history.h>

#include "readlineIF.h"
#include "hostapdIF.h"

ReadlineIF::m_command[256] = 
{
  { "ping",                  "pings hostapd" },
  { "mib",                   "get MIB variables (dot1x, dot11, radius)" },
  { "relog",                 "reload/truncate debug log output file" },
  { "status",                "show interface status info" },
  { "sta",                   "<addr> = get MIB variables for one station" },
  { "all_sta",               "get MIB variables for all stations" },
  { "list_sta",              "list all stations" },
  { "new_sta",               "<addr> = add a new station" },
  { "deauthenticate",        "<addr> = deauthenticate a station" },
  { "disassociate",          "<addr> = disassociate a station" },
#ifdef CONFIG_TAXONOMY
  { "signature",             "<addr> = get taxonomy signature for a station" },
#endif /* CONFIG_TAXONOMY */
#ifdef CONFIG_IEEE80211W
  { "sa_query",              "<addr> = send SA Query to a station" },
#endif /* CONFIG_IEEE80211W */
#ifdef CONFIG_WPS
  { "wps_pin",               "<uuid> <pin> [timeout] [addr] = add WPS "
                             "Enrollee PIN" },
  { "wps_check_pin",         "<PIN> = verify PIN checksum" },
  { "wps_pbc",               "indicate button pushed to initiate PBC" },
  { "wps_cancel",            " cancel the pending WPS operation" },
#ifdef CONFIG_WPS_NFC
  { "wps_nfc_tag_read",      "<hexdump> = report read NFC tag with WPS data" },
  { "wps_nfc_config_token",  "<WPS/NDEF> = build NFC configuration token" },
  { "wps_nfc_token",         "<WPS/NDEF/enable/disable> = manager "
                             "NFC password token" },
  { "nfc_get_handover_sel",  NULL },
#endif /* CONFIG_WPS_NFC */
  { "wps_ap_pin",            "<cmd> [params..] = enable/disable AP PIN" },
  { "wps_config",            "<SSID> <auth> <encr> <key> = configure AP" },
  { "wps_get_status",        "show current WPS status" },
#endif /* CONFIG_WPS */
  { "disassoc_imminent",     "send Disassociation Imminent notification" },
  { "ess_disassoc",          "send ESS Dissassociation Imminent notification" },
  { "bss_tm_req",            "send BSS Transition Management Request" },
  { "get_config",            "show current configuration" },
  { "help", hostapd_cli_cmd_help, hostapd_cli_complete_help,
          "= show this usage help" },
  { "interface",             "[ifname] = show interfaces/select interface" },
#ifdef CONFIG_FST
  { "fst",                   "<params...> = send FST-MANAGER control "
                             "interface command" },
#endif /* CONFIG_FST */
  { "raw",                   "<params..> = send unprocessed command" },
  { "level",                 "<debug level> = change debug level" },
  { "license",               "show full hostapd_cli license" },
  { "quit",                  "exit hostapd_cli" },
  { "set",                   "<name> <value> = set runtime variables" },
  { "get",                   "<name> = get runtime info" },
  { "set_qos_map_set",       "<arg,arg,...> = set QoS Map set element" },
  { "send_qos_map_conf",     "<addr> = send QoS Map Configure frame" },
  { "chan_switch",           "<cs_count> <freq> [sec_channel_offset=] "
                             "[center_freq1=]\n"
                             "[center_freq2=] [bandwidth=] [blocktx] "
                             "[ht|vht]\n"
                             "initiate channel switch announcement" },
  { "hs20_wnm_notif",        "<addr> <url>\n"
                             "send WNM-Notification Subscription "
                             "Remediation Request" },
  { "hs20_deauth_req",       "<addr> <code (0/1)> <Re-auth-Delay(sec)> [url]\n"
                             "send WNM-Notification imminent deauthentication "
                             "indication" },
  { "vendor",                "<vendor id> <sub command id> "
                             "[<hex formatted data>]\n"
                             "send vendor driver command" },
  { "enable",                "enable hostapd on current interface" },
  { "reload",                "reload configuration for current interface" },
  { "disable",               "disable hostapd on current interface" },
  { "update_beacon",         "update Beacon frame contents\n"},
  { "erp_flush",             "drop all ERP keys"},
  { "log_level",             "[level] = show/change log verbosity level" },
  { "pmksa",                 "show PMKSA cache entries" },
  { "pmksa_flush",           "flush PMKSA cache" },
  { "set_neighbor",          "<addr> <ssid=> <nr=> [lci=] [civic=] [stat]\n"
                             "  = add AP to neighbor database" },
  { "remove_neighbor",       "<addr> <ssid=> = remove AP from neighbor "
                             "database" },
  { "req_lci",               "<addr> = send LCI request to a station"},
  { "req_range",             "send FTM range request"},
  { "driver_flags",          " = show supported driver flags"},
#ifdef CONFIG_DPP
  { "dpp_qr_code",           "report a scanned DPP URI from a QR Code" },
  { "dpp_bootstrap_gen",     "type=<qrcode> [chan=..] [mac=..] [info=..] "
                             "[curve=..] [key=..] = generate DPP bootstrap "
                             "information" },
  { "dpp_bootstrap_remove",  "*|<id> = remove DPP bootstrap information" },
  { "dpp_bootstrap_get_uri", "<id> = get DPP bootstrap URI" },
  { "dpp_bootstrap_info",    "<id> = show DPP bootstrap information" },
  { "dpp_auth_init",         "peer=<id> [own=<id>] = initiate DPP "
                             "bootstrapping" },
  { "dpp_listen",            "<freq in MHz> = start DPP listen" },
  { "dpp_stop_listen",       "= stop DPP listen" },
  { "dpp_configurator_add",    "[curve=..] [key=..] = add DPP configurator" },
  { "dpp_configurator_remove", "*|<id> = remove DPP configurator" },
  { "dpp_configurator_get_key","<id> = Get DPP configurator's private key" },
  { "dpp_configurator_sign",   "conf=<role> configurator=<id> = generate "
                               "self DPP configuration" },
  { "dpp_pkex_add",            "add PKEX code" },
  { "dpp_pkex_remove",         "*|<id> = remove DPP pkex information" },
#endif /* CONFIG_DPP */
  { "accept_acl",              "Add/Delete/Show/Clear accept MAC ACL" },
  { "deny_acl",                "Add/Delete/Show/Clear deny MAC ACL" },
  { "poll_sta",                "<addr> = poll a STA to check connectivity "
                               "with a QoS null frame" },
  { "req_beacon",              "<addr> [req_mode=] <measurement request "
                               "hexdump>  = "
                               "send a Beacon report request to a station" },
  { "reload_wpa_psk",          "reload wpa_psk_file only" },
  { NULL, NULL }
};

/*
 * @brief 
 * @param 
 * @return
 */
void ReadlineIF::offset(int offset)
{
  m_offset = offset;
}

/*
 * @brief 
 * @param 
 * @return
 */
int ReadlineIF::offset()
{
  return(m_offset);
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

/*
 * @brief 
 * @param 
 * @return
 */
void ReadlineIF::len(int len)
{
  m_len = len;
}

/*
 * @brief 
 * @param 
 * @return
 */
int ReadlineIF::len()
{
  return(m_len);
}


/* Generator function for command completion.  STATE lets us know whether
 * to start from scratch; without any state (i.e. STATE == 0), then we
 * start at the top of the list. 
 */
char *ReadlineIF::commandGenerator(char *text, int state)
{
  char *name;

  /* If this is a new word to complete, initialize now.  This includes
     saving the length of TEXT for efficiency, and initializing the index
     variable to 0. */
  if (!state)
    {
      offset(0);
      len(strlen(text));
    }

  /* Return the next name which partially matches from the command list. */
  while (name = ReadlineIF::command[offset()].cmd)
    {
      offset(offset()++);

      if (strncmp (name, text, len()) == 0)
        return (strdup(name));
    }

  /* If no names matched, then return NULL. */
  return ((char *)NULL);
}

/* Attempt to complete on the contents of TEXT.  START and END show the
 * region of TEXT that contains the word to complete.  We can use the
 * entire line in case we want to do some simple parsing.  Return the
 * array of matches, or NULL if there aren't any. 
 */
char **ReadlineIF::commandCompletion(char *text, int start, int end)
{
  char **matches;

  matches = (char **)NULL;

  /* If this word is at the start of the line, then it is a command
     to complete.  Otherwise it is the name of a file in the current
     directory. */
  if (start == 0)
    matches = completion_matches (text, commandGenerator);

  return (matches);
}

int ReadlineIF::init(void)
{
  /* Tell the completer that we want a crack first. */
  rl_attempted_completion_function = (CPPFunction *)commandCompletion;
  return(1);
}

ReadlineIF::ReadlineIF()
{
  m_prompt = NULL;
  m_offset = 0;
  m_len = 0;
  m_continueStatus = false;
  m_hostapdIF = NULL;
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

int main()
{
  ReadlineIF *cmdAgent = new ReadlineIF();

  
}

int ReadlineIF::executeLine(char *line)
{
  int i;
  char *word;
  bool isFound = false;

  /* Isolate the command word. */
  i = 0;
  while(line[i] && whitespace(line[i]))
    i++;
  word = line + i;

  while(line[i] && !whitespace(line[i]))
    i++;

  if(line[i])
    line[i++] = '\0';

  isFound = isValid(word);

  if(!isFound)
  {
    fprintf(stderr, "%s: No such command for FileMan.\n", word);
    return(-1);
  }
  
  if(!strncmp(word, "help", 4) || (!strncmp(word, "?", 1)))
  {
    help(word);
  }
  else if(!strncmp(word, "quit", 4))
  {
    quit();
  }
  else
  {
    /* Call the function. */
    return(processCommand(line, strlen(line)));
  }
}

void ReadlineIF::hostapdIF(hostapdIF *hostapdIF)
{
  m_hostapdIF = hostapdIF;
}

HostapdIF *ReadlineIF::hostapdIF(void)
{
  return(m_hostapdIF);
}

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
             ReadlienIF::m_command[i].cmd, 
             ReadlieIF::m_command[i].usage);
      printed++;
    }
  }

  if(!printed)
  {
    printf ("No commands match `%s'.  Possibilties are:\n", cmd);

    for (i = 0; ReadlienIF::m_command[i].cmd; i++)
    {
      /* Print in six columns. */
      if (printed == 1)
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
  return (0);
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
  return(0);
}

int ReadlineIF::processResponse(char *rsp, int len)
{
  return(0);
}

#endif /*__READLINE_IF_CC__*/