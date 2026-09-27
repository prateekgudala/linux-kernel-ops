#include <stdio.h>
#include <string.h>
#include "parser.h"

void command_init(command_t *cmd) {
    cmd->argc = 0;
    cmd->input[0] = '\0';
    cmd->output[0] = '\0';
    cmd->append = 0;
    cmd->background = 0;
    for (int i = 0; i < MAX_ARGS; i++) {
        cmd->argv[i] = NULL;
    }
}

int parse(const token_list_t *tokens, pipeline_t *pipeline) {
    pipeline->command_count = 1;
    int current = 0;
    command_init(&pipeline->commands[0]);

    for (int i = 0; i < tokens->count; i++) {
        const token_t *t = &tokens->tokens[i];

        if (t->type == TOKEN_WORD) {
            if (pipeline->commands[current].argc < MAX_ARGS - 1) {
                pipeline->commands[current].argv[pipeline->commands[current].argc++] = (char *)t->text;
            }
        } else if (t->type == TOKEN_INPUT) {
            if (i + 1 < tokens->count && tokens->tokens[i + 1].type == TOKEN_WORD) {
                strncpy(pipeline->commands[current].input, tokens->tokens[i + 1].text, MAX_TOKEN_LEN);
                i++;
            } else {
                printf("Error: filename expected after <\n");
                return 0;
            }
        } else if (t->type == TOKEN_OUTPUT) {
            if (i + 1 < tokens->count && tokens->tokens[i + 1].type == TOKEN_WORD) {
                strncpy(pipeline->commands[current].output, tokens->tokens[i + 1].text, MAX_TOKEN_LEN);
                pipeline->commands[current].append = 0;
                i++;
            } else {
                printf("Error: filename expected after >\n");
                return 0;
            }
        } else if (t->type == TOKEN_APPEND) {
            if (i + 1 < tokens->count && tokens->tokens[i + 1].type == TOKEN_WORD) {
                strncpy(pipeline->commands[current].output, tokens->tokens[i + 1].text, MAX_TOKEN_LEN);
                pipeline->commands[current].append = 1;
                i++;
            } else {
                printf("Error: filename expected after >>\n");
                return 0;
            }
        } else if (t->type == TOKEN_BACKGROUND) {
            pipeline->commands[current].background = 1;
        } else if (t->type == TOKEN_PIPE) {
            pipeline->commands[current].argv[pipeline->commands[current].argc] = NULL;
            current++;
            if (current >= MAX_COMMANDS) {
                printf("Error: Too many commands in pipeline.\n");
                return 0;
            }
            command_init(&pipeline->commands[current]);
            pipeline->command_count++;
        } else if (t->type == TOKEN_END) {
            break;
        }
    }
    pipeline->commands[current].argv[pipeline->commands[current].argc] = NULL;
    return 1;
}

void pipeline_print(const pipeline_t *pipeline) {
    printf("========== PIPELINE ==========\n");
    for (int i = 0; i < pipeline->command_count; i++) {
        const command_t *cmd = &pipeline->commands[i];
        printf("\nCommand %d\n", i + 1);
        printf("------------------------------\n");
        printf("Arguments\n");
        for (int j = 0; j < cmd->argc; j++) {
            printf("argv[%d] = %s\n", j, cmd->argv[j]);
        }
        printf("Input      : %s\n", cmd->input[0] != '\0' ? cmd->input : "None");
        printf("Output     : %s\n", cmd->output[0] != '\0' ? cmd->output : "None");
        printf("Append     : %s\n", cmd->append ? "Yes" : "No");
        printf("Background : %s\n", cmd->background ? "Yes" : "No");
        printf("==============================\n");
    }
}
