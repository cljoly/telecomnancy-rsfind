#include "exec.h"

void exec(char* commandd){

    char command[255];
    strcpy(command, commandd);
    //puts (command);
    const int max_arg_sz = 64;

    //////////////////Partie tri de la commande//////////////
    char cmd_name[max_arg_sz];
    char cmd_args[16][max_arg_sz];
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
            //piped_cmd_args[arg_pos+1] = NULL;
        }
        else {
            strcpy(cmd_args[arg_pos], buffer);
        }
    }

    //getting cmd name for first and snd cmd
    strcpy(cmd_name, cmd_args[0]);
    if (piped) strcpy(piped_cmd_name, piped_cmd_args[0]);
    //puts("fin tri cmd");


    //////////REDO/////////////////
    /*
    static char *cmd_name;
    static char *cmd_args[8];
    int piped=0;
    static char *piped_cmd_name;
    static char *piped_cmd_args[8];

    char c;
    int i=0; int arg_pos=0;
    char buffer[64]; int i_buf=0;
    int met_piped_cmd=0; int buf_empty;
    while((c=command[i]) != '\0'){
        //printf("%c\n",c);
        switch (c){
            case ' ' :
                buffer[i_buf] = '\0';
                if (!piped || (piped && met_piped_cmd))
                    printf ("printing buffer: '%s' with arg_pos: '%d' and piped: '%d'\n", buffer, arg_pos, piped);
                if (piped && !met_piped_cmd) {
                }
                if (piped && met_piped_cmd) {
                    piped_cmd_args[arg_pos] = buffer;
                    //puts(piped_cmd_args[arg_pos]);
                    arg_pos++;
                }
                if (!piped) {
                    cmd_args[arg_pos] = buffer;
                    puts("added buffer to cmd args");
                    arg_pos++;
                }
                i_buf = 0;
                puts (cmd_args[0]);
                break;
            case '|' :
                buffer[i_buf] = '\0';
                buf_empty = 1;
                //printf ("printing buffer: '%s'\n", &buffer);
                for (int i=0; i<strlen(buffer); i++){
                    if (buffer[i] != ' ') buf_empty = 0;
                }
                if (!buf_empty) {
                    //puts("added buffer to first cmd");
                    cmd_args[arg_pos] = buffer;
                    arg_pos++;
                }
                cmd_args[arg_pos] = NULL;

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
        printf("cmd arg0 = '%s'\n", cmd_args[0]);
    }
    //placing last arg if it exists
    buffer[i_buf] = '\0';
    buf_empty = 1;
    printf ("printing buffer: '%s' with arg_pos: '%d' and piped: '%d'\n", buffer, arg_pos, piped);
    for (int i=0; i<strlen(buffer); i++){
        if (buffer[i] != ' ') buf_empty = 0;
    }
    if (!buf_empty) {
    puts("added buffer to cmd args");
        if (piped) {
            piped_cmd_args[arg_pos] = buffer;
            piped_cmd_args[arg_pos+1] = NULL;
        }
        else {
            cmd_args[arg_pos] = buffer;
            cmd_args[arg_pos+1] = NULL;
        }
    }
    //getting cmd name for first and snd cmd
    cmd_name = cmd_args[0]; puts(cmd_name);
    if (piped) piped_cmd_name = piped_cmd_args[0];
    //puts("fin tri cmd");


    */
    ///////////Partie execution de la commande///////////
    if (fork() > 0) { // père
        wait(NULL);
    }
    else { //fils
        execlp("sh","sh","-c",command,NULL);
    }
    /*int fd[2];
    pipe(fd);
    if (fork() > 0) { // père
        close(fd[1]);
        return(fd[0]);
    }
    else {// fils
        close(fd[0]);
        dup2(fd[1],1);
        close(fd[1]);
        execvp(cmd_name, cmd_args);
    }*/
}


