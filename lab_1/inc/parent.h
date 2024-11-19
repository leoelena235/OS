#ifndef __PARENT_H__
#define __PARENT_H__

#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#define CLIENT_PROGRAM_NAME "child"

void get_program_path(char *progpath, size_t size);
void create_pipe(int channel[2]);
void handle_child_process(int channel[2], char *progpath, char *filename);
void handle_parent_process(int channel_1[2], int channel_2[2], pid_t child_1, pid_t child_2);

#endif