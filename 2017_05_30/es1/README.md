# Exercise 1
Implement a process P that will SIGKILL(signal 9) any process Q that attempts to signal it.
For example:
1)Process P started and waiting for signals.
2)Process Q signals the process P(any signal, except for SIGKILL).
3)Process P sends a SIGKILL to the process Q(making it terminate).