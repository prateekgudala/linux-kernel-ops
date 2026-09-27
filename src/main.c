#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/history.h>
#include <readline/readline.h>
#include "token.h"
#include "lexer.h"
#include "parser.h"
#include "expand.h"
#include "builtin.h"
#include "executor.h"

int main(void) {
    printf("=====================================\n");
    printf("Shellforge \n");
    printf(" A Unix Style Shell written in C\n");
    printf("=====================================\n");

    char *line;
    token_list_t tokens;
    pipeline_t pipeline;

    while (1) {
        line = readline("shellforge$ ");
        if (line == NULL) break;
        
        if (strlen(line) == 0) {
            free(line);
            continue;
        }

        add_history(line);

        if (lexer(line, &tokens) == 0) {
            // Optional: comment out token_print(&tokens) here if you want a cleaner terminal
            token_print(&tokens); 
            
            if (parse(&tokens, &pipeline)) {
                expand_variables(&pipeline);
                // Optional: comment out pipeline_print(&pipeline) here if you want a cleaner terminal
                pipeline_print(&pipeline);

                execute_pipeline(&pipeline);
            }
        }
        free(line);
    }
    return 0;
}
