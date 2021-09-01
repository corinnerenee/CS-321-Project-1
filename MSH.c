#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

#define LOGOUTCODE 0x0500 // hex code for logout
#define N 10000 //used for infiinite loop breakout

int build_command()
{
    return 0;
}
int user_login()
{
    char username[20];
    char password[20];
    printf("Username: ");
    fgets(username, 20,stdin);
    printf("Password: ");
    fgets(password, 20,stdin);

    username[strcspn(username, "\n")] = 0;   // without this, `strcmp` would return false
    password[strcspn(password, "\n")] = 0;   // without this, `strcmp` would return false


    size_t len = 100;
    char buffer [len];
    char * file_username;
    char * file_password;

    FILE *fp = fopen("credentials.txt", "r");
    if (fp != NULL){
        while ( fgets(buffer, len, fp)  != NULL) {
            file_username = strtok(buffer,",");
            if (strcmp(username,file_username)==0){
                file_password = strtok(NULL,",");
                file_password[strcspn(file_password, "\n")] = 0;   // without this, `strcmp` would return false
                if(strcmp(password,file_password)==0){
                    return 1;
                }
            }
        }
    }
    fclose(fp);
    return 0;
}

int type_prompt()
{
    printf("\nMSH> ");
    return 0;
}
int read_command(char *  command, char parameters[10][10])
{
    char  buffer[50];
    fgets(buffer,50,stdin);
    buffer[strcspn(buffer, "\n")] = 0;   
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

int exec_command(char *command, char parameters[10][10])
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
                    exit(0);
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
