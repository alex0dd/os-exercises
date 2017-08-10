# Exercise 2
Implement the process R that spawns a child whose job is to signal the process P(implemented in exercise 1).
The child process should instantly terminate(to avoid getting SIGKILL'd by the process P).