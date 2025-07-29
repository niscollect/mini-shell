At its core, a **shell** is just a **REPL** — Read-Eval-Print Loop — that runs commands for the user.

A shell is fundamentally just a loop that:
- Reads a line of input
- Parses it into a command and argument
- Creates a process ( fork() )
- Runs the command( execvp() )
- Waits for it finish( wait() )
- Repeats


Imagine:

shell
`$ echo hello 
`hello`
`$ ls -l`
`total 4`
`-rw-r--r-- 1 user user 0 Jul 17 file.txt`
`$ exit`

Each line:
- You typed something (input)
    
- Shell split it up (tokenizing)
    
- Shell created a child process
    
- Shell replaced that process image with `echo` or `ls`
    
- Waited for it to complete
    
- Printed output
    
- Repeated
    

That’s it.

> So your first goal: Build a loop that **takes a command**, **executes it**, **waits for it to finish**, and repeats.

-----------
Plan of Action:
1) Basic Input and exit
2) Parse input into arguments
3) fork() and exec()
4) Error Handling and edge cases
5) Path prompt + current directory
6) Built in commands

------------------
## How to take INPUT from the user?

 Use `fgets()`. Why? Why not `scanf()`. Coz it breaks on spaces, also it is vulnerable to overflows and other unwanted things, you wouldn't imagine using it for usage in a shell.


`fgets()` : 
	fgets(input, 100, stdin);  // reads at most 99 chars + \0  { fgets() adds a \0 at the end to terminate }


Also there's one more problem with fgets(). It capture the newline character too, when ENTER is pressed.
To get rid of it, we use `strcspn()`
`strcspn(s1, s2)` returns the index of the first character in `s1` that exists in `s2`
So we find it using this: strcspn(input, "\n");, and then replace it with '\0'

Now after we've taken an input, we'll need to take action based on it. 
For that, we'll first have to compare first.
`This is where one must walk consciously, or else would just trip`
We use `strcmp(s1, s2)` for it. It return a value: 0 - if s1 and s2 are same; >0 - if s1 > s2; <0 - if s2>s1 lexicographically. [It compares ASCII values, character-by-character]

-------
Now, the next step is tokenization of the command.
## Implementing Tokenization

We use strtok() to tokenize a string, into separate words.
##### `argv[i] = strtok(input, " ");`
The first call to `strtok()` splits `input` using `" "` (space) as a delimiter.  
It returns a pointer to the **first token**.

`char input[] = "ls -l /home";
`char *token = strtok(input, " ");
`while (token != NULL) {
`  printf("%s\n", token);
` token = strtok(NULL, " "); // this makes it to keep performing it on the same input
`}
`argv[i] = NULL; // Very important for exec`

Well the last statement as we can see, states that it is very important for `exec()`. Why?
Coz `exec()` functions expect the **argument array** ( typically named `argv`), to follow a very specific format:
> A **null-terminated array** of character pointers (`char *argv[]`), where:
> 
> - `argv[0]` is the name of the command,
> - `argv[1]` to `argv[n-1]` are the arguments,
> - and `argv[n]` is **`NULL`**, indicating the end of the argument list.`

So, the last line is **required** to tell `execvp()` where the argument list ends.
