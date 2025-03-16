
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <fcntl.h>

#include "dshlib.h"
#include "rshlib.h"




/*
 * exec_remote_cmd_loop(server_ip, port)
 *      server_ip:  a string in ip address format, indicating the servers IP
 *                  address.  Note 127.0.0.1 is the default meaning the server
 *                  is running on the same machine as the client
 *              
 *      port:   The port the server will use.  Note the constant 
 *              RDSH_DEF_PORT which is 1234 in rshlib.h.  If you are using
 *              tux you may need to change this to your own default, or even
 *              better use the command line override -c implemented in dsh_cli.c
 *              For example ./dsh -c 10.50.241.18:5678 where 5678 is the new port
 *              number and the server address is 10.50.241.18    
 * 
 *      This function basically implements the network version of 
 *      exec_local_cmd_loop() from the last assignemnt.  It will:
 *  
 *          1. Allocate buffers for sending and receiving data over the 
 *             network
 *          2. Create a network connection to the server, getting an active
 *             socket by calling the start_client(server_ip, port) function.
 *          2. Go into an infinite while(1) loop prompting the user for
 *             input commands. 
 * 
 *             a. Accept a command from the user via fgets()
 *             b. Send that command to the server using send() - it should
 *                be a null terminated string
 *             c. Go into a loop and receive client requests.  Note each
 *                receive might not be a C string so you need to print it
 *                out using:
 *                     printf("%.*s", (int)bytes_received, rsp_buff);
 *                this version of printf() uses the "%.*s" flag that indicates
 *                that the rsp_buff might be a null terminated string, or
 *                it might not be, if its not, print exactly bytes_received
 *                bytes. 
 *             d. In the recv() loop described above. Each time you receive
 *                data from the server, see if the last byte received is the
 *                EOF character. This indicates the server is done and you can
 *                send another command by going to the top of the loop.  The
 *                best way to do this is as follows assuming you are receiving
 *                data into a buffer called recv_buff, and you received
 *                recv_bytes in the call to recv:
 * 
 *                  recv_bytes = recv(sock, recv_buff, recv_buff_sz, 0)
 *                  
 *                if recv_bytes:
 *                  <negative_number>: communication error
 *                    0:    Didn't receive anything, likely server down
 *                  > 0:    Got some data. Check if the last byte is EOF
 *                          is_eof = (recv_buff[recv_bytes-1] == RDSH_EOF_CHAR) ? 1 : 0;
 *                    if is_eof is true, this is the last part of the transmission
 *                    from the server and you can break out of the recv() loop. 
 * 
 *   returns:
 *          OK:      The client executed all of its commands and is exiting
 *                   either by the `exit` command that terminates the client
 *                   or the `stop-server` command that terminates both the
 *                   client and the server. 
 *          ERR_MEMORY:             If this function cannot allocate memory via
 *                                  malloc for the send and receive buffers
 *          ERR_RDSH_CLIENT:        If the client cannot connect to the server. 
 *                                  AKA the call to start_client() fails.
 *          ERR_RDSH_COMMUNICATION: If there is a communication error, AKA
 *                                  any failures from send() or recv().
 * 
 *   NOTE:  Since there are several exit points and each exit point must
 *          call free() on the buffers allocated, close the socket, and
 *          return an appropriate error code.  Its suggested you use the
 *          helper function client_cleanup() for these purposes.  For example:
 * 
 *   return client_cleanup(cli_socket, request_buff, resp_buff, ERR_RDSH_COMMUNICATION);
 *   return client_cleanup(cli_socket, request_buff, resp_buff, OK);
 *
 *   The above will return ERR_RDSH_COMMUNICATION and OK respectively to the main()
 *   function after cleaning things up.  See the documentation for client_cleanup()
 *      
 */
int exec_remote_cmd_loop(char *address, int port)
{
    //char *cmd_buff[SH_CMD_MAX * EXE_MAX + ARG_MAX];
    //char *rsp_buff[SH_CMD_MAX * EXE_MAX + ARG_MAX];
    int cli_socket;
    ssize_t io_size;
    int is_eof;
    //printf("hello1");
    // TODO set up cmd and response buffs
    char *cmd_buff = (char*)malloc((RDSH_COMM_BUFF_SZ)* sizeof(char));
    char *rsp_buff = (char*)malloc((RDSH_COMM_BUFF_SZ)* sizeof(char));
    //printf("hello2");
    cli_socket = start_client(address,port);
    if (cli_socket < 0){
        perror("start client");
        return client_cleanup(cli_socket, cmd_buff, rsp_buff, ERR_RDSH_CLIENT);
    }
    //printf("hello3");
    while (1) 
    {
        //printf("hello4");
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, RDSH_COMM_BUFF_SZ, stdin) == NULL)
        {
            break;
        }
        //printf("Testing: %s, %d", cmd_buff, strlen(cmd_buff));
        //if(strcmp(cmd_buff, EXIT_CMD) == 0) {
        ///    exit(0);
        //}
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';
        char* copy = (char*)malloc(strlen(cmd_buff)* sizeof(char));
        strcpy(copy, cmd_buff);
        copy[strcspn(cmd_buff, "\n")] = '\0';
        if(strcmp(EXIT_CMD, copy) == 0) {
            break;
        } 
        //printf("hello4534");
        int ret;
        //printf("hello9999");
        //char* buff = (char*)malloc((RDSH_COMM_BUFF_SZ)* sizeof(char));
        //printf("hello2");//malloc(RDSH_COMM_BUFF_SZ);
        int recv_size;
        //printf("hello6");
        
         
        ret = send(cli_socket, copy, (strlen(copy) + 1), 0);
        //printf("hello7");
        //int ctest = send(cli_socket, &RDSH_EOF_CHAR, 1, 0);
        int s = send(cli_socket, &RDSH_EOF_CHAR, 1, 0);
//send the command including the null byte
        memset(rsp_buff, 0, (RDSH_COMM_BUFF_SZ));
        while ((recv_size= recv(cli_socket, rsp_buff, sizeof(rsp_buff), 0)) > 0){
           // char* copy1 = (char*)malloc(strlen(rsp_buff)* sizeof(char));
            //strcpy(copy1, rsp_buff);
            //copy1[strcspn(rsp_buff, "\n")] = '\0';
           // if(strcmp(EXIT_CMD, copy1) == 0) {
           //     break;
           // }  
            //printf("\n%s\n", copy1);

            //we got recv_size bytes
            if (recv_size < 0){
                return ERR_RDSH_COMMUNICATION;
                break;
                //we got an error, handle it and break out of loop or return
                //from function
            }
            if (recv_size == 0){
                return ERR_RDSH_COMMUNICATION;
                break;
            }

            //At this point we have some data, lets see if this is the last chunk
            int is_last_chunk = ((char)rsp_buff[recv_size-1] == RDSH_EOF_CHAR) ? 1 : 0;

            if (is_last_chunk){
                rsp_buff[recv_size-1] = '\0'; //remove the marker and replace with a null
                                        //this makes string processing easier
            }
            //printf("\n%s\n", rsp_buff);
            //Now the data in buff is guaranteed to be null-terminated.  Handle in,
            //in our shell client we will just be printing it out. Note that we are
            //using a special printf marker "%.*s" that will print out the characters
            //until it encounters a null byte or prints out a max of recv_size
            //characters, whatever happens first. 
            //printf("%.*s", (int)recv_size, rsp_buff);

            //If we are not at the last chunk, loop back and receive some more, if it
            //is the last chunk break out of the loop
            if (is_last_chunk)
                break;
        }
        //ret = send(cli_socket, cmd_buff, (strlen(cmd_buff) + 1), 0);
        //int s = send(cli_socket, &RDSH_EOF_CHAR, 1, 0);
        //printf("RECV FROM SERVER -> %s\n",rsp_buff);
        //close(cli_socket);
        // TODO print prompt - done

        // TODO fgets input - done

        // TODO send() over cli_socket

        // TODO recv all the results

        // TODO break on exit command - done
    }

    return client_cleanup(cli_socket, cmd_buff, rsp_buff, OK);
}

/*
 * start_client(server_ip, port)
 *      server_ip:  a string in ip address format, indicating the servers IP
 *                  address.  Note 127.0.0.1 is the default meaning the server
 *                  is running on the same machine as the client
 *              
 *      port:   The port the server will use.  Note the constant 
 *              RDSH_DEF_PORT which is 1234 in rshlib.h.  If you are using
 *              tux you may need to change this to your own default, or even
 *              better use the command line override -c implemented in dsh_cli.c
 *              For example ./dsh -c 10.50.241.18:5678 where 5678 is the new port
 *              number and the server address is 10.50.241.18    
 * 
 *      This function basically runs the client by: 
 *          1. Creating the client socket via socket()
 *          2. Calling connect()
 *          3. Returning the client socket after connecting to the server
 * 
 *   returns:
 *          client_socket:      The file descriptor fd of the client socket
 *          ERR_RDSH_CLIENT:    If socket() or connect() fail
 * 
 */
int start_client(char *server_ip, int port){
    struct sockaddr_in addr;
    int cli_socket;
    int ret;
    //printf("hello9");
    // TODO set up cli_socket
    cli_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(cli_socket == -1) {
        perror("socket");
        exit(ERR_EXEC_CMD);
    }

    memset(&addr, 0, sizeof(struct sockaddr_in));
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(server_ip);
    addr.sin_port  = htons(port);

    ret = connect(cli_socket, (const struct sockaddr *) &addr, sizeof(struct sockaddr_in));
    //printf("hello8");
    if (ret == -1) {
        fprintf(stderr, "The server is down.\n");
        exit(ERR_RDSH_COMMUNICATION);
    }

    return cli_socket;
}

/*
 * client_cleanup(int cli_socket, char *cmd_buff, char *rsp_buff, int rc)
 *      cli_socket:   The client socket
 *      cmd_buff:     The buffer that will hold commands to send to server
 *      rsp_buff:     The buffer that will hld server responses
 * 
 *   This function does the following: 
 *      1. If cli_socket > 0 it calls close(cli_socket) to close the socket
 *      2. It calls free() on cmd_buff and rsp_buff
 *      3. It returns the value passed as rc
 *  
 *   Note this function is intended to be helper to manage exit conditions
 *   from the exec_remote_cmd_loop() function given there are several
 *   cleanup steps.  We provide it to you fully implemented as a helper.
 *   You do not have to use it if you want to develop an alternative
 *   strategy for cleaning things up in your exec_remote_cmd_loop()
 *   implementation. 
 * 
 *   returns:
 *          rc:   This function just returns the value passed as the 
 *                rc parameter back to the caller.  This way the caller
 *                can just write return client_cleanup(...)
 *      
 */
int client_cleanup(int cli_socket, char *cmd_buff, char *rsp_buff, int rc){
    //If a valid socket number close it.
    if(cli_socket > 0){
        close(cli_socket);
    }

    //Free up the buffers 
    free(cmd_buff);
    free(rsp_buff);

    //Echo the return value that was passed as a parameter
    return rc;
}