# ReadWriteConsoleApp
My Own System Memory ReadWriteLock
Includes app to test the ReadWriteLock

ReadWriteLock functions as follows.

Reads are allowed to be concurrent as long as not writing.

All locks are handled as First-In, First-Served

Currently set up to only perform locking within a process context, i.e., a process and its threads.

