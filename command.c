/*
** EPITECH PROJECT, 2024
** ftp
** File description:
** sc
*/

#include "server.h"

void cwd(user_node_t **socket, char *command, path_t **path)
{
    char *new_command = my_command(command);
    int check = check_path(new_command);
    char response[256];

    if ((*socket)->user == NULL ||
    (*socket)->password == NULL) {
        strcpy(response, "530 Please login with USER and PASS.\r\n");
    } else {
        if (check == -1) {
            strcpy(response, "550 Failed to change directory.\r\n");
        } else {
            free((*path)->path);
            (*path)->path = strdup(new_command);
            strcpy(response, "250 Directory successfully changed.\r\n");
        }
    }
    write((*socket)->socket, response, strlen(response));
}

void pwd(user_node_t **socket, path_t **path)
{
    const char *login_response =
    "530 Please login with USER and PASS.\r\n";
    size_t size_response =
    strlen("257 \"") + strlen((*path)->path) + strlen("\"\r\n") + 1;
    char *smart_response = malloc(size_response);
    const char *response;

    if ((*socket)->user == NULL ||
    (*socket)->password == NULL) {
        response = login_response;
    } else {
        snprintf(smart_response,
        size_response, "257 \"%s\"\r\n", (*path)->path);
        response = smart_response;
    }
    write((*socket)->socket, response, strlen(response));
    if (smart_response) {
        free(smart_response);
    }
}

void cd_up(user_node_t **socket, path_t **path)
{
    char *dir_up = get_dir((*path)->path);
    int check = check_path(dir_up);
    char response[256];

    if ((*socket)->user == NULL ||
    (*socket)->password == NULL) {
        strcpy(response, "530 Please login with USER and PASS.\r\n");
        write((*socket)->socket, response, strlen(response));
    } else {
        if (check == -1) {
            strcpy(response, "550 Failed to change directory.\r\n");
        } else {
            free((*path)->path);
            (*path)->path = strdup(dir_up);
            strcpy(response, "250 Directory successfully changed.\r\n");
        }
    }
    write((*socket)->socket, response, strlen(response));
    free(dir_up);
}

void noop(user_node_t **socket)
{
    char response[] = "200 NOOP okay.\r\n";
    const char *required =
    "530 Please login with USER and PASS.\r\n";

    if ((*socket)->user == NULL ||
    (*socket)->password == NULL) {
        write((*socket)->socket, required, strlen(required));
    } else {
        write((*socket)->socket, response, strlen(response));
    }
}

void password(user_node_t *socket, char *command)
{
    char *new_command = my_command(command);
    char response[256];

    if (socket->is_connected == 1) {
        strcpy(response, "230 Already logged in.\r\n");
        write(socket->socket, response, strlen(response));
        return;
    }
    free(socket->password);
    if (strcmp(socket->user, "Anonymous") == 0) {
        socket->is_connected = 1;
        socket->password = strdup(new_command);
        strcpy(response, "230 User logged in, proceed.\r\n");
    } else {
        strcpy(response, "530 Login incorrect.\r\n");
    }
    write(socket->socket, response, strlen(response));
    free(new_command);
}
