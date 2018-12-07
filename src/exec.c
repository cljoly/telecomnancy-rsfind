#include "exec.h"

void exec(char* commandd){

    char command[255];
    strcpy(command, commandd);
    //puts (command);
    const int max_arg_sz = 64;

    //////////////////Partie tri de la commande//////////////

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
                //if (!piped || (piped && met_piped_cmd))
                //    printf ("printing buffer: '%s' with arg_pos: '%d' and piped: '%d'\n", buffer, arg_pos, piped);
                if (piped && !met_piped_cmd) {
                }
                if (piped && met_piped_cmd) {
                    char temp[64];
                    strcpy(temp, buffer);
                    piped_cmd_args[arg_pos] = temp;
                    //puts(piped_cmd_args[arg_pos]);
                    arg_pos++;
                }
                if (!piped) {
                    char temp[64];
                    strcpy(temp, buffer);
                    cmd_args[arg_pos] = temp;
                    //puts("added buffer to cmd args");
                    arg_pos++;
                }
                i_buf = 0;
                //puts (cmd_args[0]);
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
                    char temp[64];
                    strcpy(temp, buffer);
                    cmd_args[arg_pos] = temp;
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
        //printf("cmd arg0 = '%s'\n", cmd_args[0]);
    }
    //placing last arg if it exists
    buffer[i_buf] = '\0';
    buf_empty = 1;
    //printf ("printing buffer: '%s' with arg_pos: '%d' and piped: '%d'\n", buffer, arg_pos, piped);
    for (int i=0; i<strlen(buffer); i++){
        if (buffer[i] != ' ') buf_empty = 0;
    }
    if (!buf_empty) {
    //puts("added buffer to cmd args");
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
    cmd_name = cmd_args[0]; //puts(cmd_name);
    if (piped) piped_cmd_name = piped_cmd_args[0];
    //puts("fin tri cmd");

    ///////////Partie execution de la commande///////////
    if (fork() > 0) { // père
        wait(NULL);
    }
    else {
        execvp(cmd_name, cmd_args);
    }
}
