1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

_We can use waitpid() to make sure that we wait on all child processes based on their process ids. We also close all our pipes to prevent leaks. If we don't call waitpid, then it might cause the program to run without termination if our child processes do not complete execution._

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

_If we don't close unused pipes, then that will lead to desciptor leak and we could eventually run out of available file descriptors._

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

_The command 'cd' involves changing directories. This can lead to issues regarding the location at which the fork() and exec() processes were initiated and ran. If we use execvp() on 'cd' and create child processes, we might move to a different directory than out initial location._

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

_One simple way of fixing this issue would be to prompt the user the number of commands or pipes that they intend to use. Once that value is inputted, we can allocate the appropriate amount of memory. An obvious downside to this would be that it would break the flow of a typical command prompt shell and require users to know beforehand the number of commands they need to run._
