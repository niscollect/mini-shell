#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <assert.h>
#include <string.h>
// #include <common.h>

int main()
{
    char input[1024];  // can hold up to 1023 characters + '\0'
    // * we will later on try to make this dynamic, instead of specifying it beforehand



    while(1)
    {
        printf("$ ");  // It just prints the shell promot.

        if (fgets(input, sizeof(input), stdin) == NULL) {
            // Ctrl+D pressedprintf("\n");
            break;
        }

        // * but since  fgets() captures the newline(\n) character too, we need to remove it

        input[strcspn(input, "\n")] = '\0';
        // This line removes the newline character \n from the end of the string you got from fgets().

        if (strcmp(input, "exit") == 0) {
            break;
        }

        char *argv[100];

        int i = 0;
        argv[i] = strtok(input, " ");
        while (argv[i] != NULL && i < 99) {
            i++;
            argv[i] = strtok(NULL, " ");
        }
        argv[i] = NULL; // Very important for exec


        //* we can start running comands

        pid_t pid = fork();

        if(pid < 0)
        {
            perror("Unfortunate");
            exit(1);
        }
        else if(pid == 0)   //child
        {
            execvp(argv[0], argv);
            perror("execvp didn't work");
            exit(1);
        }
        else    // parent
        {
            int status;
            wait(&status);
        }


        printf("You typed: %s\n", input);
    }
    return 0;
}
