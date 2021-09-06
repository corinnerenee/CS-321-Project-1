#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>

#define LOGOUTCODE 0x0500 // hex code for logout
#define N 10000 //used for infiinite loop breakout


char command_history [100][100] ; // holds up to 100 commands with size of 100
int command_index=0;

int build_command() //Supposed to fill a map full commands???
{
    //search through the table of commands and return a code 
    // can use a switch
    // the code gets entered into execute command
    return 0;
}
int user_login() // function for authenticating input against a credentials.txt file
{
    char username[20];
    char password[20];
    char userString[40];
    printf("Username: ");
    fgets(username, 20,stdin);
    printf("Password: ");
    fgets(password, 20,stdin);

    username[strcspn(username, "\n")] = 0;   // without this, `strcmp` would return false
    password[strcspn(password, "\n")] = 0;   // without this, `strcmp` would return false

    strcat(userString ,username);
    strcat(userString ,password); // userstring is username+password

    //encrpyt userString 
    //crypt(userString); // (desired string , hash value for encryption)

    //userString[strcspn(userString, "\n")] = 0;   // without this, `strcmp` would return false

    size_t len = 100;
    char buffer [len];

    FILE *fp = fopen("credentials.txt", "r"); 
    if (fp != NULL){
        while ( fgets(buffer, len, fp)  != NULL) {
            buffer[strcspn(buffer, "\n")] = 0;   // without this, `strcmp` would return false
            if (strcmp(userString,buffer)==0){
                    return 1;
            }
        }
    }
    fclose(fp);
    return 0;
}

int type_prompt() // simply prints out the shell 
{
    printf("MSH> ");
    return 0;
}

int pwd(char * password){ // change the logged in suers password

}
int copy(char * fileName1, char * fileName2){ // copy a file from one directory to another.

}
int ps(){ // 

}
int df(){ //

}
int search(){

}
int history(){ // return the history of the commands entered -> probably need to stored this information in a file structure?
    for (size_t i = 0; i < 100; i++)
    {
        if (!(strcmp(command_history[i], ""))){ // only print the string if its not empty 
            printf("%s\n",command_history[i]);
        }
    }
    
}
void logout( int * status){ // terminate the script?
}

int read_command(char *  command, char parameters[10][10]) // reads in inputted line and parse the command from the arguments
{
    char  buffer[100];
    fgets(buffer,100,stdin);

    buffer[strcspn(buffer, "\n")] = 0;   
    // add this input to the command_history array to be read back with history()
    strcpy(command_history[command_index] ,buffer);
    command_index = command_index + 1;


    strcpy(command,strtok(buffer," "));

    int index = 0;
    char * parameter;
    while(parameter = strtok(NULL," ")){
        //parameters[index]= parameter;
        strcpy(parameters[index], parameter);
        index = index+1;
    }
    // need command checking
    return 1;
}

int exec_command(char *command, char parameters[10][10]) // pass in the command and up to 10 arguments
{
    for (size_t i = 0; i < 10; i++)
    {
        if(strcmp(parameters[i], "")){
            printf("%s\n", parameters[i]);
        }
    }
    
    return 0;
}

int main()
{
    build_command(); /* read in the commands into a table or hash table */
    if (user_login()){                /* Authenticate the user */
        int i=0;
        while (i < N)
        {  // repeat maximum N times, used to avoid the infinite loop 
            type_prompt(); // display prompt 
            char  command[20]; // commands can be only 20 characters long
            char parameters[10][10];// up to 10 parameters 
            memset(parameters, 0, sizeof(parameters[0][0]) * 10 * 10);
            int n = read_command(command, parameters); // input from terminal 
            int status;
            if (n > 0)
            { 
                if (wait(&status)>=0)
                { 
                    if (status == LOGOUTCODE)
                        exit(0); // status must be a hex number 
                }
                else
                { 
                    exec_command(command, parameters); // execute command 
                } 
            }
            else { 
                printf( "Invalid command, try again"); 
            }
            i++;
        }
    }else {
        printf("Wrong Username or Password!\n");
        main();
    }
    return (1);
}
