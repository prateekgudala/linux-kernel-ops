#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/history.h>
#include <readline/readline.h>
#include "token.h"
#include "lexer.h"

int main(void) {
    printf("=====================================\n");
    printf("Shellforge \n");
    printf(" A Unix Style Shell written in C\n");
    printf("=====================================\n");

    char *line;
    token_list_t list;

    while (1) {
        line = readline("shellforge$ ");
        if (line == NULL) {
            printf("\nExiting...\n");
            break;
        }
        if (strlen(line) == 0) {
            free(line);
            continue;
        }

        add_history(line);

        if (strcmp(line, "exit") == 0) {
            free(line);
            printf("Exiting...\n");
            break;
        } else if (strcmp(line, "history") == 0) {
            printf("------ Command History ------\n");
            HIST_ENTRY **the_list = history_list();
            if (the_list) {
                for (int i = 0; the_list[i]; i++) {
                    printf(" %2d  %s\n", i + 1, the_list[i]->line);
                }
            }
            printf("-----------------------------\n");
            free(line);
            continue;
        }

        if (lexer(line, &list) == 0) {
            token_print(&list);
        }

        free(line);
    }
    return 0;
}
