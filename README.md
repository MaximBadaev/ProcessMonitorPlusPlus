# ProcessMonitorPlusPlus
Windows process monitor with the ability of searching, sorting the processes, determining their filepaths, starting them from the snapshot after termination, recording the snapshot to a text file and reading it from the text file. 
Command list:
help - list all commands and their descriptions.

spl - show process list.
Syntax: spl [parameter]
List of parameters:
no parameter - show process list in the default order.
a-z - show process list in alphabet order. Capital letters first.
z-a - show process list in reverse alphabet order. Capital letters last.
spl [first letter] - show processes beginning with the symbol stated as first letter.

find - find the process by ID or .exe-file name.
Syntax: find [filename].exe, find [Process ID number], find [argument] [parameter].
List of parameters:
-f - find by amount of symbols in process filename

kill - terminate the process by ID or .exe-file name.
Syntax: kill [filename].exe, kill [Process ID number]

snap- takes a snapshot of the current process list
Syntax: snap(without any argument)- takes a snapshot of the current process list.
snap view- shows the process list in the snapshot
snap record [filepath]- records the process list in the snapshot into a file
snap read [filepath]- reads the process list in the specified file to the snapshot
start [[file].exe or [process ID]]- starts the exe file of the process saved in the snapshot
exit - exit program

cls - clear screen
