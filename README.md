# UNIX command interpreter (Simple Shell) <img src="https://cdn-icons-png.flaticon.com/512/4792/4792314.png" width=45 align=center>

This is a simple shell that emulates the basic functionality of sh and supports built-in commands and some features
It is simliar to `sh` or `bash` shells

## Man-page <img src="https://cdn-icons-png.flaticon.com/128/781/781103.png" width=40 align=center>

the main page is a file contain details how this shell works
to see the full the manual-page

```bash
$ man ./man_1_simple_shell
```

## How To Use

```bash
git clone https://github.com/mahmoud-malek/simple_shell.git
or
getw https://github.com/mahmoud-malek/simple_shell/archive/refs/heads/main.zip

and then, cd simple_shell
```

### Compilation

```bash
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh

```

### Run

```bash
./hsh

Eaxmples:
:) /bin/ls
barbie_j       env-main.c  exec.c  fork.c  pid.c  ppid.c    prompt   prompt.c  shell_0.3  stat    test_scripting.sh  wait.c
env-environ.c  exec    fork    mypid   ppid   printenv  promptc  shell     shell.c    stat.c  wait
:) ls
barbie_j       env-main.c  exec.c  fork.c  pid.c  ppid.c    prompt   prompt.c  shell_0.3  stat    test_scripting.sh  wait.c
env-environ.c  exec    fork    mypid   ppid   printenv  promptc  shell     shell.c    stat.c  wait
:) ls -l /tmp
total 20
-rw------- 1 julien julien    0 Dec  5 12:09 config-err-aAMZrR
drwx------ 3 root   root   4096 Dec  5 12:09 systemd-private-062a0eca7f2a44349733e78cb4abdff4-colord.service-V7DUzr
drwx------ 3 root   root   4096 Dec  5 12:09 systemd-private-062a0eca7f2a44349733e78cb4abdff4-rtkit-daemon.service-ANGvoV
drwx------ 3 root   root   4096 Dec  5 12:07 systemd-private-062a0eca7f2a44349733e78cb4abdff4-systemd-timesyncd.service-CdXUtH
-rw-rw-r-- 1 julien julien    0 Dec  5 12:09 unity_support_test.0
```

## List of supported built-in commands

| built-in | Description                     |
| -------- | ------------------------------- |
| cd       | changes the current directory   |
| alias    | set or see aliases              |
| exit     | terminates the shell            |
| setenv   | sets an environment variable    |
| unsetenv | unset an environment variable   |
| env      | print all environment variables |
| history  | print, delete Histroy           |

## List of features

- handle #comments
- handle variable replacement
- handle $? and $$
- handle File as input
- handle (&&), (||), ( ; )
- Handle command lines with arguments
- Handle interruption signal (SIGINT, ctrl + c)
- handle pipes
- handle basic redirections
- handle combination of pipes and redirections (not work in all cases nor edge cases)

> Note: in pipes and redirctions there are no error handling, not support edge cases.
> It may have couple of issues, So, be careful when using redirections and pipes,

## Flowchart <img src="https://cdn-icons-png.flaticon.com/128/3051/3051633.png" width=40 align=center>

<img src="https://i.ibb.co/42YWY5r/Simple-Shell-1.png" alt="Simple-Shell-flowchart" border="0">

## 🔗 Useful Links

[Differenece between sh and bash](https://stackoverflow.com/questions/5725296/difference-between-sh-and-bash)

[Shell Command Language](https://pubs.opengroup.org/onlinepubs/009695399/utilities/xcu_chap02.html)

## Authors

- Mahmoud Malek <a href="https://github.com/mahmoud-malek/" rel="nofollow"><img align="center" alt="github" src="https://www.vectorlogo.zone/logos/github/github-tile.svg" height="24" /></a>
  <br>

> This was a project for ALX_SWE programme
