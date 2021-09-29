#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>

#define LOGOUTCODE 0x0500 // hex code for logout
#define N 10000           //used for infiinite loop breakout

char command_history[100][100] = {0}; // holds up to 100 commands with size of 100
int command_index = 0;

typedef struct
{
    char *key;
    int value;
} hash_item;

typedef struct
{
    hash_item **items;
    int size;
    int count;
} hashtable;

hashtable *create_table(int size)
{
    hashtable *table = (hashtable *)malloc(sizeof(hashtable));
    table->size = size;
    table->count = 0;
    table->items = (hash_item **)calloc(table->size, sizeof(hash_item *));
    for (int i = 0; i < table->size; i++)
        table->items[i] = NULL;

    return table;
}

hash_item *create_item(char *key, int value)
{
    // Creates a pointer to a new hash table item
    hash_item *item = (hash_item *)malloc(sizeof(hash_item));
    item->key = (char *)malloc(strlen(key) + 1);
    strcpy(item->key, key);
    item->value = value;
    return item;
}

void table_insert(hashtable *table, char *key, int value)
{
    hash_item *item = create_item(key, value);

    hash_item *current_item = table->items[table->count];

    if (current_item == NULL)
    {
        // Key does not exist.
        if (table->count == table->size)
        {
            // Hash Table Full
            printf("Insert Error: Hash Table is full\n");
            return;
        }

        // Insert directly
        table->items[table->count] = item;
        table->count++;
    }

    else
    {
        if (strcmp(current_item->key, key) == 0)
        {
            table->items[table->count]->value = value;
            return;
        }

        else
        {
            return;
        }
    }
}

int hash_search(hashtable *table, char *key)
{
    // Searches the key in the hashtable
    // and returns NULL if it doesn't exist
    for (size_t index = 0; index < table->count; index++)
    {
        hash_item *item = table->items[index];

        // Ensure that we move to a non NULL item
        if (item != NULL)
        {
            if (strcmp(item->key, key) == 0)
                return item->value;
        }
    }
    return 0;
}

hashtable *build_command()
{
    size_t len = 100;
    char buffer[len];
    int i = 0;
    hashtable *commands = create_table(100);

    FILE *fp = fopen("shellcommands.txt", "r");

    if (fp != NULL)
    {
        while (fgets(buffer, len, fp) != NULL)
        {
            char *command = strtok(buffer, ":");
            int cid = atoi(strtok(NULL, ":"));
            table_insert(commands, command, cid);
        }
    }
    return commands;
}
int user_login() // function for authenticating input against a credentials.txt file
{
    char username[20];
    char password[20];
    char userString[40];
    memset(userString, 0, 40);
    printf("Username: ");
    fgets(username, 20, stdin);
    printf("Password: ");
    fgets(password, 20, stdin);

    username[strcspn(username, "\n")] = 0; // without this, `strcmp` would return false
    password[strcspn(password, "\n")] = 0; // without this, `strcmp` would return false

    strcat(userString, username);
    strcat(userString, ";");
    strcat(userString, password); // userstring is username+password

    //encrpyt userString
    strcpy(userString, crypt(userString, "AA")); // (desired string , hash value for encryption)

    userString[strcspn(userString, "\n")] = 0; // without this, `strcmp` would return false

    size_t len = 100;
    char buffer[len];
    return 1;
    FILE *fp = fopen("credentials.txt", "r");
    if (fp != NULL)
    {
        while (fgets(buffer, len, fp) != NULL)
        {
            printf("%s\n", userString);
            buffer[strcspn(buffer, "\n")] = 0; // without this, `strcmp` would return false
            if (strcmp(userString, buffer) == 0)
            {
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

int pwd()
{ // change the logged in users password
    char username[20];
    char password[20];
    char userString[40];
    memset(userString, 0, 40);

    printf("Username: ");
    fgets(username, 20, stdin);
    printf("Enter new password: ");
    fgets(password, 20, stdin);

    username[strcspn(username, "\n")] = 0; // without this, `strcmp` would return false
    password[strcspn(password, "\n")] = 0; // without this, `strcmp` would return false

    strcat(userString, username);
    strcat(userString, ";");
    strcat(userString, password); // userstring is username+password

    //encrpyt userString
    strcpy(userString, crypt(userString, "AA")); // (desired string , hash value for encryption)

    userString[strcspn(userString, "\n")] = 0; // without this, `strcmp` would return false

    size_t len = 100;
    char buffer[len];

    FILE *fp = fopen("credentials.txt", "r");
    if (fp != NULL)
    {
        while (fgets(buffer, len, fp) != NULL)
        {
            buffer[strcspn(buffer, "\n")] = 0; // without this, `strcmp` would return false
            if (strcmp(userString, buffer) == 0)
            {
                return 1;
            }
        }
    }
    fclose(fp);
    return 1;
}

int history()
{ // return the history of the commands entered -> probably need to stored this information in a file structure?
    for (size_t i = 0; i < 100; i++)
    {
        if ((strcmp(command_history[i], "") != 0))
        { // only print the string if its not empty
            printf("%s\n", command_history[i]);
        }
    }
}
void logout(int *status)
{ // terminate the script?
}

int read_command(char *command, hashtable *commands, char parameters[10][10]) // reads in inputted line and parse the command from the arguments
{
    char buffer[100];
    fgets(buffer, 100, stdin);
    if (buffer[0] != '\0')
    {
        buffer[strcspn(buffer, "\n")] = 0;

        strcpy(command, strtok(buffer, " "));
        strcpy(command_history[command_index], command);
        // add this input to the command_history array to be read back with history()
        command_index = command_index + 1;

        int index = 0;
        char *parameter;
        while (parameter = strtok(NULL, " "))
        {
            //parameters[index]= parameter;
            strcpy(parameters[index], parameter);
            index = index + 1;
        }
        return hash_search(commands, command);
    }
    return 1;
}

int exec_command(int cid, char parameters[10][10]) // pass in the command and up to 10 arguments
{
    if (cid == 1)
    { //change password
        pwd();
    }
    else if (cid == 2)
    { // copy
        if (parameters[0][0] != '\0' && parameters[1][0] != '\0')
        { // make sure params are valid
            char command_string[100] = "cp ";
            strcat(command_string, parameters[0]);
            strcat(command_string, " ");
            strcat(command_string, parameters[1]);
            system(command_string);
        }
        else
        {
            printf("Missing Parameters!\n");
        }
    }
    else if (cid == 3)
    { // ps
        char command_string[100] = "ps -ef";
        if (parameters[0][0] != '\0')
        {
            strcat(command_string, "| grep ");
            strcat(command_string, parameters[0]);
        }
        system(command_string);
    }
    else if (cid == 4)
    { // df
        char command_string[100] = "df -k";
        if (parameters[0][0] != '\0')
        {
            strcat(command_string, "| grep ");
            strcat(command_string, parameters[0]);
        }
        system(command_string);
    }
    else if (cid == 5)
    { //search
        if (parameters[0][0] != '\0' && parameters[1][0] != '\0')
        { // make sure params are valid
            char command_string[100] = "grep ";
            strcat(command_string, parameters[0]);
            strcat(command_string, " ");
            strcat(command_string, parameters[1]);
            system(command_string);
        }
        else
        {
            printf("Missing Parameters!\n");
        }
    }
    else if (cid == 6)
    { //history
        history();
    }
    else if (cid == 7)
    { //logout
        exit(0);
    }

    return 0;
}

int main()
{
    hashtable *commands = build_command();
    hash_item *item = commands->items[0];
    if (user_login())
    { /* Authenticate the user */
        int i = 0;
        while (i < N)
        {                            // repeat maximum N times, used to avoid the infinite loop
            type_prompt();           // display prompt
            char command[20];        // commands can be only 20 characters long
            char parameters[10][10]; // up to 10 parameters
            memset(parameters, 0, sizeof(parameters[0][0]) * 10 * 10);
            int cid = read_command(command, commands, parameters); // input from terminal
            int status;
            if (cid > 0)
            {
                if (wait(&status) >= 0)
                {
                    if (status == LOGOUTCODE)
                        exit(0); // status must be a hex number
                }
                else
                {
                    exec_command(cid, parameters); // execute command
                }
            }
            else
            {
                printf("Invalid command, try again\n");
            }
            i++;
        }
    }
    else
    {
        printf("Wrong Username or Password!\n");
        main();
    }
    return (1);
}
