1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  fork() is used to create a new process that is simply a copy of original process. If we had used just execvp(), then it would have replaced the process with a new one.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  If fork() fails, then an error is printed that displays the issue and the process is exited with a return status of 1.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  When execvp() is used, the first argument is treated as a filename and is looked for among the system variables to execute it. In this case, the command that is passed through the argument is looked for among the system environment variables.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  wait() is used to stop execution of the current process until one of its children stops.If we don't use wait(), -1 is returned as the status code.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  WEXITSTATUS() gives us the exit status of the child when it is terminated. This exit status can be passed onto the parent process of the child.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  build_cmd_buff() uses double quotes as a delimiter to separate the command and the actual string that needs to be printed. By doing so, we can ensure that the string within the quotes stay intact, instead of getting divided as would have been the case, if the delimiter was set to whitespace.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  A major change I did in this assignment was change the delimeter from whitespace to double quotes, so that if there are arguments for commands like echo in double quotes, then they are properly tokenized. One difficulty I had with this assignment was to make sure to divide the commands properly and take care of whitespace/newline characters.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  A signal can be referred to as a way to interrupt processes, as well as something that allows processes to communicate acsynchronously.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  SIGINT is to inform a process that a termination has been requested. SIGTERM is used to terminate a program and can be chosen to be ignored by the process. On the other hand, SIGKILL causes immendiate termination and cannot be ignored or blocked.

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  Unlike the previous signals SIGSTOP suspends the process, i.e., it kills a process only to be started later. This signal cannot be caught or ignored. 

References:
1. https://linuxhandbook.com/sigterm-vs-sigkill/
2. https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8
3. https://superuser.com/questions/594508/whats-the-difference-between-sigkill-and-sigstop
4. https://www.man7.org/linux/man-pages/man3/exec.3.html
5. https://www.man7.org/linux/man-pages/man2/wait.2.html
6. https://tldp.org/HOWTO/Secure-Programs-HOWTO/signals.html
7. https://www.man7.org/linux/man-pages/man3/exec.3.html
