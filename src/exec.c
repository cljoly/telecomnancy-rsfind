#include "exec.h"

static int const max_arg_sz = 64;

void exec(char* pathh, char* commandd){
	
		char path[255];
		strcpy(path, pathh);
    char command[255];
    strcpy(command, commandd);
    //puts (command);
    //const int max_arg_sz = 64;
	

    //////////////////Partie tri de la commande//////////////
    char cmd_name[max_arg_sz];
    char cmd_args[16][max_arg_sz];
    int no_arg_cmd = 0;
    int piped=0;
    char piped_cmd_name[max_arg_sz];
    char piped_cmd_args[16][max_arg_sz];

    char c;
    int i=0; int arg_pos=0;
    char buffer[max_arg_sz]; int i_buf=0;
    int met_piped_cmd=0; int buf_empty;
    int buflen;
    while((c=command[i]) != '\0'){
        //printf("%c\n",c);
        switch (c){
            case ' ' :
                buffer[i_buf] = '\0';
                //if (!piped || (piped && met_piped_cmd))
                //    printf ("printing buffer: '%s' with arg_pos: '%d' and piped: '%d'\n", &buffer, arg_pos, piped);
                if (piped && !met_piped_cmd) {
                }
                if (piped && met_piped_cmd) {
                    strcpy(piped_cmd_args[arg_pos], buffer);
                    arg_pos++;
                }
                if (!piped) {
                    strcpy(cmd_args[arg_pos], buffer);
                    arg_pos++;
                }
                i_buf = 0;
                break;
            case '|' :
                buffer[i_buf] = '\0';
                buf_empty = 1;
                //printf ("printing buffer: '%s'\n", &buffer);
                buflen = strlen(buffer);
                for (int i=0; i<buflen; i++){
                    if (buffer[i] != ' ') buf_empty = 0;
                }
                if (!buf_empty) {
                    //puts("added buffer to first cmd");
                    strcpy(cmd_args[arg_pos], buffer);
                    arg_pos++;
                }
                no_arg_cmd = arg_pos;
                arg_pos = 0;
                i_buf = 0;
                piped = 1;
                break;
            default :
                if (piped) met_piped_cmd=1;
                buffer[i_buf] = c;
                i_buf++;
        }
        i++;
    }
    //placing last arg if it exists
    buffer[i_buf] = '\0';
    buf_empty = 1;
    //printf ("printing buffer: '%s'\n", &buffer);
    buflen = strlen(buffer);
    for (int i=0; i<buflen; i++){
        if (buffer[i] != ' ') buf_empty = 0;
    }
    if (!buf_empty) {
    //puts("added buffer to first cmd");
        if (piped) {
            strcpy(piped_cmd_args[arg_pos], buffer);
            arg_pos++;
            //piped_cmd_args[arg_pos+1][0] = NULL;
        }
        else {
            strcpy(cmd_args[arg_pos], buffer);
            no_arg_cmd = arg_pos+1;
            //cmd_args[arg_pos+1][0] = NULL;
        }
    }

    //getting cmd name for first and snd cmd
    strcpy(cmd_name, cmd_args[0]);
    if (piped) strcpy(piped_cmd_name, piped_cmd_args[0]);
    //puts("fin tri cmd");

    ///////////Partie execution de la commande///////////
    char * cmd_args_corrected[16];
    //char * piped_cmd_args_corrected[8];
    for (i=0; i<no_arg_cmd; i++){
        cmd_args_corrected[i] = cmd_args[i];
    } 
    cmd_args_corrected[no_arg_cmd] = path;
    cmd_args_corrected[no_arg_cmd+1] = NULL;
    //for (i=0; i<arg_pos; i++){
    //    piped_cmd_args_corrected[i] = cmd_args[i];
    //} piped_cmd_args_corrected[arg_pos] = NULL;

    int fd[2];
    pipe (fd);


    if (piped)
    {
    if (fork() > 0) { // père
        wait(NULL);
    }
    else { //fils
        execlp("sh","sh","-c",command,NULL);
    }
    /*
    if (fork() > 0) { // père
        puts("0");
        wait(NULL);
    }
    else {// fils
        if (fork() > 0) { //sous pere
            dup2(0, fd[1]);
            wait(NULL);
            puts("2");puts(piped_cmd_name);
            execvp(piped_cmd_name, piped_cmd_args_corrected);
            close(fd[0]); close(fd[1]);
        }
        else {//sous fils
            puts("1");puts(cmd_name);
            dup2(fd[1], 1);
            execvp(cmd_name, cmd_args_corrected);
            close(fd[0]); close(fd[1]);
        }
    */
    }
    else {
        if (fork() > 0) { // père
            //puts("0");
            wait(NULL);
        }
        else {
            execvp(cmd_name, cmd_args_corrected);
        }
    }
}

void exec_printer (context *ctxt, char file[], char *cmd) {
	char path[255];
	complete_path(ctxt, file, path);
	
	exec(path, cmd);
}




















































