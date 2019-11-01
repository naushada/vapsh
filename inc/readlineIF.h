#ifndef __READLINE_IF_H__
#define __READLINE_IF_H__

#include <readline/readline.h>
#include <readline/history.h>

//#include "hostapdIF.h"

class ReadlineIF
{
  public:
    struct command
    {
      const char *cmd;
      const char *argv[16];
      const char *usage;  
    };

    static command m_command[256];

    static int m_offset;
    static int m_len;
    static char *m_cmdName;
    static int m_argOffset;

  public:
    ReadlineIF();
    virtual ~ReadlineIF();
    int init(void);

    friend char **commandCompletion(const char *text, int start, int end);
    friend char *commandGenerator(const char *text, int state);
    friend char *commandArgGenerator(const char *text, int state);
    friend char *commandArgListGenerator(const char *text, int state);

    void prompt(char *prompt);
    char *prompt(void);
    int executeLine(char *line);
    int processCommand(char *cmd, int len);
    int processResponse(char *rsp, int len);
    char *stripwhite(char *string);
    bool isValid(char *cmd);
    //void hostapdTask(HostapdTask *hostapdTask);
    //HostapdTask *hostapdTask(void);
    void help(char *cmd);
    void quit(void);
    bool continueStatus(void);
    void continueStatus(bool status);
    static void cmdName(char *cmdName);
    static char *cmdName(void);
 
  private:
    char        *m_prompt;
    bool        m_continueStatus;
    //HostapdTask *m_hostapdTask;
};

#endif /*__READLINE_IF_H__*/
