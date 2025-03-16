#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"

/**** 
 **** FOR REMOTE SHELL USE YOUR SOLUTION FROM SHELL PART 3 HERE
 **** THE MAIN FUNCTION CALLS THIS ONE AS ITS ENTRY POINT TO
 **** EXECUTE THE SHELL LOCALLY
 ****
 */

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
int execute_pipeline(command_list_t *clist) {
    int num_commands = clist->num;
    int pipes[num_commands - 1][2];
    pid_t pids[num_commands]; 
    for (int i = 0; i < num_commands - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < num_commands; i++) {
        pids[i] = fork();
        if (pids[i] == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pids[i] == 0) 
        {
            if (i > 0) {
                dup2(pipes[i-1][0], STDIN_FILENO);
            }
            if (i < num_commands - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }
            //for searching comments
            for (int j = 0; j < num_commands - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            execvp(clist->commands[i].argv[0], clist->commands[i].argv);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < num_commands - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    for (int i = 0; i < num_commands; i++) {
        waitpid(pids[i], NULL, 0);
    }
}
int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    int count = 0;
    char* commands = (char*)malloc(strlen(cmd_line)* sizeof(char));
    commands = strtok(cmd_line, "|");
    while (commands != NULL) {
        int len = strlen(commands);
        int i = 0;
        while(i < len) {
            if(commands[i] != ' ') {
                break;
            }
            i = i + 1;
        }     
        clist->num = count;
        while((commands != NULL) && (commands[1] != '\n')) {
            int i = 0;
            int start = 0;           
            int count = 0;           
            for(i = 0; i < strlen(commands); i++) {
                if(commands[i] == '\"') {
                    for(int k = (i + 1); k < strlen(commands); k++) {
                        if((commands[k] == '\"')) {
                            int allQuotes = 0;
                            for(int j = (i + 1); j < k; j++) {
                                if(commands[j] == ' ') { //for searching comments
                                    allQuotes = allQuotes + 1;           
                                }
                            }                           
                            char* command = (char*)malloc((k - i)* sizeof(char));
                            strncpy(command, (commands + i + 1), (k - i - 1));
                            command[strcspn(command, "\n")] = '\0';
                            if(allQuotes == strlen(command)) {
                                break;
                            }
                            if((command != NULL) || (command[0] != ' ') || (command[0] != '\t')) {
                                i = k;
                                clist->commands[clist->num].argv[count] = malloc(strlen(command)*sizeof(char));
                                strcpy(clist->commands[clist->num].argv[count], command);
                                clist->commands[clist->num].argc = count + 1;
                                start = k + 1;
                                count = count + 1; //for searching comments
                            }
                        }
                        //ll k | cd plea jk | kk ll pp oo| ljks | echo "Hello Wold" | ll "ll kl" "k" | kkl "l" opopo | llll             pp        "lksdshd"           qqq
                        //ll k | cd plea jk | kk ll pp oo| ljks | echo "Hello Wold" | ll "ll kl" "k" | kkl "l" opopo | llll             pp        "lksdshd"           qqq|    ll "kl;k;l"              "pp"
                    }
                    continue;
                }
                if((commands[i] != ' ') || (i == (strlen(commands) - 1))) {
                    int k = 0;
                    for(k = (i+ 1); k < (strlen(commands) - 1); k++) {
                        if(commands[k + 1] == ' ') {
                            break;
                        }
                    }
                    //for searching comments
                    k = k + 1;
                    char* token = (char*)malloc((k - i + 1)* sizeof(char));
                    strncpy(token, (commands + i), (k - i));
                    //ll k | cd plea jk | kk ll pp oo| ljks | echo "Hello Wold" | ll "ll kl" "k" | kkl "l" opopo
                    token[strcspn(token, "\n")] = '\0';
                    while((token != NULL) && (strlen(token) != 0) && (token[0] != ' ')) {
                        
                        clist->commands[clist->num].argv[count] = malloc(strlen(token)*sizeof(char));
                        strcpy(clist->commands[clist->num].argv[count], token);
                        clist->commands[clist->num].argc = count + 1;
                        start = i + 1;
                        count = count + 1;
                        break;
                    }
                    i = k;
                }    
            } 
            clist->commands[clist->num].argv[count] = NULL;
           //clist->commands[clist->num].argc = clist->commands[clist->num].argc + 1;
            count = 0;
            clist->num = clist->num + 1;
            commands = strtok(NULL, "|");      
        }
    }
    return OK;
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
            copy[strcspn(cmd_buff, "\n")] = '\0';
            if(strcmp(EXIT_CMD, copy) == 0) {
                break;
            }      
            command_list_t clist; //for searching comments
            build_cmd_list(&cmd_buff, &clist);
            
            if(clist.num > 8) {
                printf(CMD_ERR_PIPE_LIMIT, 8);
            }
            else {
                int checkArgsLength = 0;
                for(int i = 0; i < clist.num; i++) {
                    if(clist.commands[i].argc > CMD_ARGV_MAX) {
                        checkArgsLength = 1;
                        break;
                    }
                }
                if(checkArgsLength == 0) {
                    execute_pipeline(&clist);
                }
            }          
        }
    }
    return OK;
}
