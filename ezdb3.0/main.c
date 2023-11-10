#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "declaration.c"
#include "function.c"

int main() {
    FILE *file = NULL;
    char filename2[256],command[256],command_2[256],key[256];
    float value;

    declaration();
    printf("Welcome to ezDB v3.0");

    while (1) {
        printf("\n> ");
        fgets(command,sizeof(command),stdin);
        command[strcspn(command, "\n")] = '\0';
        strcpy(command_2,command);

        char *token = strtok(command, " ");
        if (token != NULL) {
            // OPTION 1: OPEN
            if (strcmp(token,"OPEN") == 0) {
                char *filename = strtok(NULL, " ");
                strcpy(filename2,filename);

                if (file != NULL) {
                    fclose(file);
                    file = NULL;
                    printf("Closing existing file... Run command again.");
                } else {
                    // <unistd.h> has access
                    file = fopen(filename, "r");
                    if (file != NULL) {
                        file = fopen(filename, "ab+");
                        printf("Working on database file: %s",filename);
                        openRecords(file);
                    } else {
                        printf("File don't exist");
                    }
                }

            // OPTION 2: SHOW ALL
            } else if (strcmp(token, "SHOW") == 0) {
                token = strtok(NULL, " ");
                if (token != NULL && strcmp(token, "ALL") == 0) {
                    if (file == NULL) {
                        printf("Select a file first.");
                    } else {
                        viewRecords(file);
                    }
                } else {
                    printf("Unknown command, do you mean SHOW ALL?");
                }

            // OPTION 3: INSERT
            } else if (strcmp(token, "INSERT") == 0) {
                if (file == NULL) {
                    printf("Select a file first.");
                }
                else {
                    if (sscanf(command_2, "INSERT %199[^0-9]%f", key, &value) == 2) {
                        removeSpace(key);
                        insertRecord(key, value);
                    }
                    else {
                        printf("Invalid INSERT input format.");
                    }
                }

            // OPTION 4: QUERY
            } else if (strcmp(token, "QUERY") == 0) {
                token = strtok(NULL, " ");
                if (file == NULL) {
                    printf("Select a file first.");
                }
                else {
                    // excecute query mode
                    if (sscanf(command_2, "QUERY %199[^0-9]", key) == 1) {
                        queryRecord(key);
                    } else {
                        printf("Invalid QUERY input format.");
                    }
                }
            
            // OPTION 5: UPDATE
            } else if (strcmp(token, "UPDATE") == 0) {
                if (file == NULL) {
                    printf("Select a file first.");
                } 
                else {
                    if (sscanf(command_2, "UPDATE %199[^0-9]%f",key, &value) == 2) {
                        if (key != NULL || value != 0.0) {
                            removeSpace(key);
                            updateRecord(key, value);
                        } else {
                            printf("Invalid salary format.\n");
                        }
                    } else {
                        printf("Invalid UPDATE format");
                    }
                }

            // OPTION 6: DELETE
            } else if (strcmp(token, "DELETE") == 0) {
                if (file == NULL) {
                    printf("Select a file first.");
                } else if (token == NULL) {
                    printf("Invalid DELETE input format");
                } else {
                    char *key = strtok(NULL, "");
                    deleteRecord(key);
                }

            // OPTION 7: SAVE
            } else if (strcmp(token, "SAVE") == 0) { 
                if (file == NULL) {
                    printf("Select a file first.");
                } else {
                    char *filename = strtok(NULL, " ");
                    if (strcmp(filename2,filename) == 0) {   
                        saveRecords(filename2);
                    } else {
                        printf("Error. Must be the same file you opened.");
                    }
                }

            } else {
                printf("Invalid command");
            }
        } else {
            printf("No input found.");
        }
    }

    return 0;
}
