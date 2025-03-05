1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  `gets()` is a good choice in this case because we can store the input directly into a variable. We can also keep running a loop to get user input until the user decides to terminate with a NULL value input. 

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  If we don't use malloc to allocate some memory in `cmd_buff`, then we won't have a dynamically allocated memory. We would need to increase or decrease our memory base as needed eventually.


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  If we don't trim leading and trailing spaces from each command/paramater, then when we try to run these processes, it might not recognise which command/parameter to run. 

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  Output redirection can be used to save the outputs of an command into a file, such as all the files present in a directory. Input redirection could be used to read through the contents of a file and output it onto the console. Error redirection could be used to save the errors details/logs on a file for further inspection.

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  Redirection is usually used to direct a output to a file stream, while piping can be used to redirect one process to another which helps in combining multiple processes together.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  It helps to seperate STDERR and STDOUT because in certain cases, we might want to redirect our STDOUT to a different file, while printing out our STDERR or vice versa. 

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  Because it is necessary to know if a command has produced an error or not, we could handle such issues in our custom shell such that if we expect STDOUT from a command, we could redirect it to a file, but if it sends a STDERR, it could still get printed on the console.