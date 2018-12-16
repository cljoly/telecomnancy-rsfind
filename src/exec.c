#include "exec.h"

void free_args (char* commands[max_pipe_number][max_arg_size], int arg_nb[max_pipe_number], int pipe_nb){
    int i=0; int j=0;
    while (i<pipe_nb){
        while(j<arg_nb[i]){
            free(commands[i][j]);
        j++;
        }
    i++;
    }
}

void exec(char* path, char* command) {
    char* commands[max_pipe_number][max_arg_size];
    int pipe_nb = 0;
    int arg_nb[max_pipe_number];
    for (int i = 0; i<max_pipe_number; i++) { arg_nb[i]=0;}

    //////////// Parsage de la commande/////////////
    char c; int c_pos = 0;
    char buffer[max_arg_size];
    int buffer_pos = 0;
    int buffer_empty = 1;
    int path_check = 0;

    while((c=command[c_pos]) != '\0'){
        switch (c) {
            case ' ':
                buffer[buffer_pos] = '\0';
                //printf ("printing buffer: '%s' with arg_nb: '%d' and pipe_nb: '%d'\n", &buffer, arg_nb[pipe_nb], pipe_nb);
                if (!buffer_empty){
                    commands[pipe_nb][arg_nb[pipe_nb]] = strdup(buffer);
                    arg_nb[pipe_nb]++;
                }
                buffer_pos = 0;
                buffer_empty = 1;
                break;
            case '{':
                if (buffer_empty)
                    path_check = 1;
                break;
            case '}':
                if (path_check) {
                    commands[pipe_nb][arg_nb[pipe_nb]] = strdup(path);
                    arg_nb[pipe_nb]++;
                    buffer_pos = 0;
                    buffer_empty = 1;
                    path_check = 0;
                }
                break;
            case '|':
                if (!buffer_empty){
                    buffer[buffer_pos] = '\0';
                    commands[pipe_nb][arg_nb[pipe_nb]] = strdup(buffer);
                    arg_nb[pipe_nb]++;
                }
                commands[pipe_nb][arg_nb[pipe_nb]] = NULL;
                pipe_nb++;
                buffer_pos = 0;
                buffer_empty = 1;
                break;
            default:
                buffer[buffer_pos] = c;
                buffer_empty = 0;
                buffer_pos++;
        }
        c_pos++;
    }
    //Inserting last arg if it exists
    if (!buffer_empty){
        //printf ("printing buffer: '%s' with arg_nb: '%d' and pipe_nb: '%d'\n", &buffer, arg_nb[pipe_nb], pipe_nb);
        buffer[buffer_pos] = '\0';
        commands[pipe_nb][arg_nb[pipe_nb]] = strdup(buffer);
        arg_nb[pipe_nb]++;
    }
    commands[pipe_nb][arg_nb[pipe_nb]] = NULL;
    arg_nb[pipe_nb]++;

    ////////////////// Execution de la commande //////////////


    if (pipe_nb == 0) { // Pas de pipe à gérer
        if (fork() > 0) { // père
            wait(NULL);
        }
        else { //fils
            execvp(commands[0][0], commands[0]);
        }
    }

    else
    {
    int fd[max_pipe_number][2];
    int dup_result = 0;

    for (int i=0; i<max_pipe_number; i++){
        if(pipe(fd[i]) == -1) {
            perror("Pipe failed");
            exit(1);
        }
    }

    //for (int i=0; i<max_pipe_number; i++){

    if(fork() == 0) {           //first fork
        close(STDOUT_FILENO);  //closing stdout
        dup_result = dup(fd[0][1]);         //replacing stdout with pipe write
        close(fd[0][0]);       //closing pipe read
        close(fd[0][1]);

        execvp(commands[0][0], commands[0]);
        perror("execvp 1 failed");
        exit(1);
    }

    if(fork() == 0) {         //creating 2nd child
        close(STDIN_FILENO);   //closing stdin
        dup_result = dup(fd[0][0]);         //replacing stdin with pipe read
        close(fd[0][1]);       //closing pipe write
        close(fd[0][0]);

        execvp(commands[1][0], commands[1]);
        perror("execvp 2 failed");
        exit(1);
    }

    if (dup_result < 0) {puts("erreur dup");}

    //}

    for (int i=0; i<pipe_nb; i++){
        close(fd[i][0]);
        close(fd[i][1]);
    }
    for (int i=0; i<pipe_nb; i++){
        wait(0);
    }

    }

//    pipe (fd[0]);
//    char* readbuffer[255];
//
//    for (int i=0; i<1; i++){
//        if (fork() > 0) { // père
//            dup2(1, fd[0][0]);
//            wait(NULL);
//            close(fd[0][0]);
//            close(fd[0][1]);
//            //execvp(commands[i+1][0], commands[i+1]);
//            //puts(readbuffer);
//        }
//        else { //fils
//            dup2(fd[0][1], 1);
//            execvp(commands[i][0], commands[i]);
//            close(fd[0][0]);
//            close(fd[0][1]);
//        }
//    }
    //execvp(commands[pipe_nb][0], commands[pipe_nb]);
    free_args(commands, arg_nb, pipe_nb);
}


void exec_printer (context *ctxt, char file[], char *cmd) {
	char path[255];
	complete_path(ctxt, file, path);
	
	exec(path, cmd);
}




















































