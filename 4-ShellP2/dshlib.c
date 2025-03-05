#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/prctl.h>
#include <unistd.h>
#include "dshlib.h"

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
int build_cmd_buff(char *cmd_line, cmd_buff_t* cmd)
{
    char* commands = (char*)malloc(strlen(cmd_line)* sizeof(char));
    commands = strtok(cmd_line, " ");
    char* args[ARG_MAX + 1];
    int count = 0;
    char* copy = (char*)malloc(strlen(commands)* sizeof(char));
    strcpy(copy, commands);
    copy[strcspn(commands, "\n")] = '\0';
    char* delimiter;
    if(strcmp(copy, "echo") == 0) {
        delimiter = "\"";
    }
    else {
        delimiter = " "; 
    }
    while((commands != NULL) && (commands[1] != '\n')) {
        copy = (char*)malloc(strlen(commands)* sizeof(char));
        strcpy(copy, commands);
        copy[strcspn(commands, "\n")] = '\0';
        args[count] = (char*)malloc(strlen(copy)* sizeof(char));
        strcpy(args[count], copy);
        count = count + 1;
        if(count > ARG_MAX) {
            printf(CMD_ARGV_MAX, ARG_MAX);
            return 0;
        }
        commands = strtok(NULL,delimiter);        
    }
    
    args[count] = 0;
    if (strcmp(args[0], "cd") == 0) {
        if(args[1] != NULL) {
            chdir(args[1]);
        }
    }
    else {
        int f_result, c_result;
        f_result = fork();
        if (f_result < 0)
        {
            perror("fork error");
            exit(1);
        }
        if (f_result == 0)
        {
            prctl(PR_SET_PDEATHSIG, SIGTERM);
            int rc = 0;
            rc = execvp(args[0], args);
            if (rc < 0)
            {
                perror("fork error");
                exit(42);
            }
        }
        else
        {
            wait(&c_result);
        }
    }
    return 0;
    exit(0);
}
int exec_local_cmd_loop()
{
    char *cmd_buff[SH_CMD_MAX * EXE_MAX + ARG_MAX];
    
    while (1)
    {
        printf("%s", SH_PROMPT);
        if (fgets(&cmd_buff, ARG_MAX, stdin) == NULL)
        {
            break;
        }
        if(strlen(cmd_buff) == 1) {
            printf(CMD_WARN_NO_CMD);    
        }
        else {
            char* copy = (char*)malloc(strlen(cmd_buff)* sizeof(char));
            strcpy(copy, cmd_buff);
            copy[strcspn(cmd_buff, "\n") - 1] = '\0';
            if(strcmp(EXIT_CMD, copy) == 0) {
                break;
            }      
            cmd_buff_t *cmd;
            build_cmd_buff(&cmd_buff, &cmd);
        }
    }
    return OK;
}
