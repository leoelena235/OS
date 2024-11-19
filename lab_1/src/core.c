#include "../inc/parent.h"

void get_program_path(char *progpath, size_t size) {
    ssize_t len = readlink("/proc/self/exe", progpath, size - 1);
    if (len == -1) {
        const char msg[] = "error: failed to read program path\n";
        write(STDERR_FILENO, msg, sizeof(msg) - 1);
        exit(EXIT_FAILURE);
    }
    progpath[len] = '\0';

    char *last_slash = strrchr(progpath, '/');
    if (last_slash != NULL) {
        *last_slash = '\0';
    }
}

void create_pipe(int channel[2]) {
    if (pipe(channel) == -1) {
        const char msg[] = "error: failed to create pipe\n";
        write(STDERR_FILENO, msg, sizeof(msg) - 1);
        exit(EXIT_FAILURE);
    }
}

void handle_child_process(int channel[2], char *progpath, char *filename) {
    pid_t pid = getpid();

    int file = open(filename, O_RDONLY);
    if (file == -1) {
        const char msg[] = "error: failed to open requested file\n";
        write(STDERR_FILENO, msg, sizeof(msg) - 1);
        exit(EXIT_FAILURE);
    }

    if (dup2(channel[0], STDIN_FILENO) == -1) {
        const char msg[] = "error: failed to duplicate channel to stdin\n";
        write(STDERR_FILENO, msg, sizeof(msg) - 1);
        close(file);
        exit(EXIT_FAILURE);
    }

    close(channel[0]);
    close(channel[1]);

    {
        char msg[64];
        const int32_t length = snprintf(msg, sizeof(msg),
            "%d: I'm a child\n", pid);
        write(STDOUT_FILENO, msg, length);
    }

    char path[1024];
    snprintf(path, sizeof(path), "%s/%s", progpath, CLIENT_PROGRAM_NAME);

    char *const args[] = {CLIENT_PROGRAM_NAME, filename, NULL};
    if (execv(path, args) == -1) {
        const char msg[] = "error: failed to exec into new executable image\n";
        write(STDERR_FILENO, msg, sizeof(msg) - 1);
        close(file);
        exit(EXIT_FAILURE);
    }
}

void handle_parent_process(int channel_1[2], int channel_2[2], pid_t child_1, pid_t child_2) {
    pid_t pid = getpid();
    {
        char msg[64];
        const int32_t length = snprintf(msg, sizeof(msg), "%d: I'm a parent, my children have PID: %d and %d\n", pid, child_1, child_2);
        write(STDOUT_FILENO, msg, length);
    }

    close(channel_1[0]);
    close(channel_2[0]);

    char buf[4096];
    ssize_t bytes_read;
    int line_number = 1;

    while ((bytes_read = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
    char *ptr = buf;
    while (ptr < buf + bytes_read) {
        char *endptr = strchr(ptr, '\n');
        if (endptr == NULL) {
            endptr = buf + bytes_read;
        } else {
            *endptr = '\0';
            endptr++;
        }

        if (strlen(ptr) == 0) { // Проверка на пустой ввод
            const char msg[] = "Empty input detected, exiting parent process...\n";
            write(STDOUT_FILENO, msg, sizeof(msg) - 1);
            return;
        }

        int target_channel = (line_number % 2 == 1) ? channel_1[1] : channel_2[1];
        if (write(target_channel, ptr, endptr - ptr) == -1) {
            const char msg[] = "error: failed to write to pipe\n";
            write(STDERR_FILENO, msg, sizeof(msg) - 1);
        }

        ptr = endptr;
        line_number++;
    }
}


    close(channel_1[1]);
    close(channel_2[1]);

    waitpid(child_1, NULL, 0);
    waitpid(child_2, NULL, 0);
}
