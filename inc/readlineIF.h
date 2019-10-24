#ifndef __READLINE_IF_H__
#define __READLINE_IF_H__

#include "hostapdIF.h"

class ReadlineIF
{
  struct command
  {
    const char *cmd;
    const char *usage;  
  };

  command m_command[256];

  public:
    ReadlineIF();
    virtual ~ReadlineIF();
    int init(void);
    char **commandCompletion(char *text, int start, int end);
    char *commandGenerator(char *text, int state);
    void offset(int offset);
    int offset();
    void propmt(char *prompt);
    char *prompt();
    void len(int len);
    int len();
    int executeLine(char *line);
    int processCommand(char *cmd, int len);
    int processResponse(char *rsp, int len);
    char *stripwhite(char *string);
    bool isValid(char *cmd);
    void hostapdIF(HostapdIF *hostapdIF);
    ReadlineIF *hostapdIF(void);
    void help(char *cmd);
    void quit(void);
    bool continueStatus(void);
    void continueStatus(bool status);
 
  private:
    char      *m_prompt;
    int       m_offset;
    int       m_len;
    bool      m_continueStatus;
    HostapdIF *m_hostapdIF;
};

#endif /*__READLINE_IF_H__*/
