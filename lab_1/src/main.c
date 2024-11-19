#include "../inc/parent.h"

void get_input(char *input, size_t size) ;

int main() {

    char progpath[4096];
    get_program_path(progpath, sizeof(progpath));

    char file_1[4096], file_2[4096];

    {
        char msg[32];
        const int32_t length = snprintf(msg, sizeof(msg), "Print first file name:\n");
        write(STDOUT_FILENO, msg, length);    
    }
    get_input(file_1, sizeof(file_1));

    {
        char msg[32];
        const int32_t length = snprintf(msg, sizeof(msg), "Print second file name:\n");
        write(STDOUT_FILENO, msg, length);
    }
    get_input(file_2, sizeof(file_2));

    int channel_1[2], channel_2[2];
    create_pipe(channel_1);
    create_pipe(channel_2);

    const pid_t child_1 = fork();
    if (child_1 == 0)
        handle_child_process(channel_1, progpath, file_1);

    const pid_t child_2 = fork();
    if (child_2 == 0)
        handle_child_process(channel_2, progpath,file_2);

    if (child_1 == -1 || child_2 == -1) {
        const char msg[] = "error: failed to spawn new process\n";
        write(STDERR_FILENO, msg, sizeof(msg));
        exit(EXIT_FAILURE);
    }

    handle_parent_process(channel_1, channel_2, child_1, child_2);

    return 0;
}

void get_input(char *input, size_t size) 
{
    if (fgets(input, size, stdin) == NULL) {   
        char msg[32];
        const int32_t length = snprintf(msg, sizeof(msg), "error: failed to read input\n");
        write(STDERR_FILENO, msg, sizeof(msg));
        exit(EXIT_FAILURE);
    }

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }

    if (len == 1 && input[0] == '\0') { 
        // Пустой ввод, завершаем программу
        const char msg[] = "Empty input detected, exiting...\n";
        write(STDOUT_FILENO, msg, sizeof(msg) - 1);
        exit(EXIT_SUCCESS);
    }
}
