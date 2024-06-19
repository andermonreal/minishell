# minishell

---

### Execution

With the Makefile you can easily compile and run the programas, use:

`make all`

The C program simulates a minishell, providing a command-line interface where users can execute commands and interact with the operating system. It leverages subprocesses and pipes to facilitate communication between processes and manage command execution effectively.

Key Features:

User Interface: The program displays a command prompt ($ ) where users input commands such as ls, cd, echo, and others supported by the operating system.

Command Parsing: Input from the user is read using C input functions like fgets and parsed to separate commands and arguments using strtok.

Process Creation: To execute commands, the program forks a new child process using fork(). This child process handles the execution of the desired command.

Redirection and Pipes: The minishell supports input/output redirection (<, >), as well as pipes (|) for inter-process communication. It utilizes dup2() to redirect file descriptors and pipe() to establish communication channels between processes.

Command Execution: Within the child process, commands are executed using execvp(), which replaces the child process image with the executable of the requested command. Error handling is implemented to notify users if a command fails to execute.

Process Synchronization: The parent process waits for the child process to complete using wait() or waitpid(), ensuring proper synchronization and handling of completed processes.

Command Loop: After executing each command, the program returns to the command prompt, allowing users to enter subsequent commands. This iterative process continues until the user decides to exit the minishell.

Benefits and Use Cases:

This implementation not only provides a user-friendly command-line interface akin to a standard shell but also demonstrates advanced operating system concepts and C programming techniques. It enables users to interact with the system dynamically, execute commands efficiently, and manage processes effectively using subprocesses and inter-process communication mechanisms like pipes.

Overall, the minishell program showcases the power and flexibility of C programming in system-level operations, making it suitable for educational purposes, learning system calls, and practical application development where customized command-line interfaces are beneficial.
