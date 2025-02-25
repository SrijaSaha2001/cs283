#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */

int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    char* hasArgs = (char*)malloc(strlen(cmd_line)* sizeof(char));
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
        char *copy_commands = (char*)malloc(strlen(commands)* sizeof(char));
        strncpy(copy_commands, (commands + i), strlen(commands));
        commands = strtok(NULL, "|");
        //strncpy(clist->commands[count].exe, copy_commands, strlen(copy_commands));
        int l = strcspn(copy_commands, "\n");
        clist->commands[count].exe[l] = '\0';
        //strncpy(clist->commands[count].exe, NULL, 0);

        
        clist->num = count;
        if(count >= 8) {
            printf(CMD_ERR_PIPE_LIMIT, 8);
            return OK;
        }
        int cmdFound = 0;
        int start = 0;
        int end = 0;
        int hasArgument = 0;
        while(end < strlen(copy_commands)) {
            if((copy_commands[end] == ' ') || (end == (strlen(copy_commands) - 1))) {
                if(cmdFound == 0) {
                    char* extract = (char*)malloc((end - start)* sizeof(char));
                    int idx = 0;
                    while(start < end) {
                        extract[idx] = copy_commands[start];
                        idx = idx + 1;
                        start = start + 1;
                    }
                    strncpy(clist->commands[count].exe, extract, strlen(extract));
                    int l = strcspn(extract, "\n");
                    clist->commands[count].exe[l] = '\0';
                    cmdFound = 1;
                    //free(extract);
                }
                else {
                    hasArgument = 1;
                    char* extract = (char*)malloc((strlen(copy_commands) - start)* sizeof(char));
                    int idx = 0;
                    while(start < (strlen(copy_commands) - 1)) {
                        extract[idx] = copy_commands[start];
                        idx = idx + 1;
                        start = start + 1;
                    }
                    strncpy(clist->commands[count].args, extract, strlen(extract));
                    int l = strcspn(extract, "\n");
                    
                    clist->commands[count].args[l] = '\0';    
                    cmdFound = 1;
                    //free(extract);
                    break;
                }
                start = end + 1;
            }
            end = end + 1;
            
        }///while loop ends
        if(hasArgument == 0) {
            hasArgs[count] = 'N';
        }
        else {
            hasArgs[count] = 'Y';
        }
        count = count + 1;
        //free(copy_commands);
  
    }
    
    clist->num = clist->num + 1;
    printf(CMD_OK_HEADER, clist->num);
    int l = 0;
    while (l < clist->num) {
        if(hasArgs[l] == 'N') {
            printf("<%d> %s\n", (l + 1), clist->commands[l].exe, strlen(clist->commands[l].exe));
        }
        else {
            printf("<%d> %s [%s]\n", (l + 1), clist->commands[l].exe, clist->commands[l].args);
        }
        
        l = l + 1;
    }
    //cmd1 a b | cmd2 g b | cmd3 | cmd4 kk l*/
    return OK;
}