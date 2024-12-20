/*
** EPITECH PROJECT, 2024
** ftp
** File description:
** sc
*/

#include "server.h"

void my_user(user_node_t *socket, char *command)
{
    char *new_command = my_command(command);
    char response[256];

    if (strlen(command) == 7) {
        strcpy(response, "501 incorrect Command USER.\r\n");
        write(socket->socket, response, strlen(response));
        return;
    }
    if (socket->is_connected) {
        strcpy(response, "230 Already logged in.\r\n");
        write(socket->socket, response, strlen(response));
        return;
    }
    free(socket->user);
    if (strcmp(new_command, "Anonymous") == 0)
        socket->user = strdup("Anonymous");
    else
        socket->user = strdup(new_command);
    strcpy(response, "331 User name okay, need password.\r\n");
    write(socket->socket, response, strlen(response));
}

char *get_dir(char *path)
{
    char *last_slash = strrchr(path, '/');
    size_t path_length = last_slash - path;
    char *new_path = malloc(path_length + 1);

    if (!last_slash) {
        return strdup(path);
    }
    if (last_slash == path) {
        return strdup("/");
    }
    if (!new_path) {
        perror("Failed to allocate memory for new path");
        exit(EXIT_FAILURE);
    }
    strncpy(new_path, path, path_length);
    new_path[path_length] = '\0';
    return new_path;
}

char *my_command(const char *command)
{
    const char *space = strchr(command, ' ');
    char *new;
    int length;

    if (!space) {
        return NULL;
    }
    length = strcspn(space + 1, "\r\n");
    new = malloc(length + 1);
    if (!new) {
        perror("Failed to allocate memory for new command");
        exit(EXIT_FAILURE);
    }
    strncpy(new, space + 1, length);
    new[length] = '\0';
    return new;
}

char *get_command(user_node_t **socket)
{
    char buffer[1024];
    ssize_t len = read((*socket)->socket, buffer, sizeof(buffer) - 1);

    if (len == -1) {
        perror("Error receiving the client command");
        exit(EXIT_FAILURE);
    }
    buffer[len] = '\0';
    return strdup(buffer);
}

void list_command(void)
{
    printf("USER : Specify user for authentication\n");
    printf("PASS : Specify password for authentication\n");
    printf("CWD : Change working directory\n");
    printf("CDUP : Change working directory to parent directory\n");
    printf("QUIT : Disconnection\n");
    printf("PWD : Print working directory\n");
    printf("NOOP : Do nothing\n");
}
