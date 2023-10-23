# C-SHELL

## List of User Commands
- `activities` : Displays processes spawned by the shell which are either `running` or `stopped`.

```bash
USAGE: activities
```

- `bg` : Continues the `stopped` process in the `background`.

```bash
USAGE: bg <pid>
```

- `fg` : Continues the `stopped` process in the `foreground`.

```bash
USAGE: fg <pid>
```

- `iMan` : Displays the `man` page of the command

```bash
USAGE: iMan <command_name>
```

- `neonate` : Prints the `PID` of the recently created system process every `n` seconds and stops printing when the user presses the `x` key.

```bash
USAGE: neonate -n <time_in_seconds>
```

- `pastevents` : Prints the last 15 `unique` commands executed in the shell. The `execute` flag along with the index `i` executes the command at `i`. The `purge` flag clears the pastevents.

```bash
USAGE: 1. pastevents
       2. pastevents execute <index>
       3. pastevents purge
```

- `peek` : Prints all the files and directories in the specified directories in lexicographic order. The `-l` flag lists the files with all the details. The `-a` flag lists all the files in the directory. Both the flags can be used simultaneously. Flags like `- , ~ , .. , .` are also supported.

```bash
USAGE: peek <flags> <path/name>
```

- `ping` : It is used to send signals to processes.

```bash
USAGE: ping <PID> <signal_number>
```

- `proclore` : It is used to obtain information regarding a process. If no `PID` is given, then it prints the information of the shell.

```bash
USAGE: proclore <PID>
```

- `seek` : It looks for a file/directory in the specified target directory (or current if no directory is specified). It returns a list of relative paths (from target directory) of all matching files/directories. The `-f` flag returns only `files`. The `-d` returns only `directories`. The `-e` flag prints the content if target is a file and goes to the target directory if target is a directory. Both `-f` and `-d` cannot be used together.

```bash
USAGE: seek <flags> <search> <target_directory>
```

- `warp` : It changes the directory that the shell is currently in. It works with the usual flags such as `- , ~ , .. , .`. If no argument is given then it goes to the `homeDirectory` of the shell.

```bash
USAGE: warp <path>
```

- `I/O redirection` : I/O redirection can also be done using `> , < , >>` and `|`.

## Assumptions
1. The user is expected to run the shell in `root` mode (sudo ./a.out) to give access to `proclore`.
2. The user is expected to not delete the `.prev*dir*` , `history.txt` and `proc` files while and after running the shell.
3. The user is expected to give at max `30 commands` to the shell at the same time and command length should not exceed `4096 bytes`.
4. `CTRL+C` also kills the background process.
5. `CTRL+C` will print newline if no process is running.
6. `neonate` will terminate only after `x` is pressed and there might be a delay of `1 second`.
7. The user is expected to not give more than `100` background processes at the same time.
8. `vim` might cause issues with the shell. So testing is preferred on `gedit` and `emacs`.
9. The shell does not support text enclosed within double quotes(`"`). That means, a command like `echo "Lorem Ipsum"` will be printed as `"Lorem Ipsum"` and not as `Lorem Ipsum`. Hence, commands like `sed` will not work.
10. User should run the shell on Ubuntu/Kali/any other Linux Distro.
11. Redirection using user defined functions might not work/might print invalid characters.
12. User is advised to give the path to the target directory while using `warp`.

**On a side-note, `C-Shell` does not sell `seashells` on the seashore :(.**