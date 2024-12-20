/*
** EPITECH PROJECT, 2024
** ftp
** File description:
** sc
*/

#include "server.h"

void server(int port, char *path)
{
    int sock;
    struct sockaddr_in address;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(port);
    if (bind(sock, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind error");
        exit(EXIT_FAILURE);
    }
    if (listen(sock, 5) < 0) {
        perror("Listen error");
        exit(EXIT_FAILURE);
    }
    run(sock, &address, path);
}

void remove_user(user_node_t **socket_list, int socket)
{
    user_node_t *temp = *socket_list;
    user_node_t *prev = NULL;

    while (temp != NULL && temp->socket != socket) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL)
        return;
    if (prev == NULL) {
        *socket_list = temp->next;
    } else {
        prev->next = temp->next;
    }
    free(temp->user);
    free(temp->password);
    free(temp);
}

user_node_t *get_user(int socket, user_node_t **head)
{
    user_node_t *temp = *head;
    user_node_t *prev = NULL;
    user_node_t *new_node;

    while (temp != NULL) {
        if (temp->socket == socket) {
            return temp;
        }
        prev = temp;
        temp = temp->next;
    }
    new_node = (user_node_t *)malloc(sizeof(user_node_t));
    new_node->socket = socket;
    new_node->next = NULL;
    if (prev == NULL) {
        *head = new_node;
    } else {
        prev->next = new_node;
    }
    return new_node;
}

void start_command(user_node_t **socket, path_t **path)

{
    char *command = get_command(socket);

    if (strncmp(command, "USER ", 5) == 0) {
        my_user(*socket, command);
        return;
    }
    if (strncmp(command, "PASS ", 5) == 0) {
        password(*socket, command);
        return;
    }
    if (strncmp(command, "QUIT", 4) == 0) {
        close((*socket)->socket);
        remove_user(socket, (*socket)->socket);
        return;
    }
    if (strncmp(command, "NOOP", 4) == 0) {
        noop(socket);
        return;
    }
    start_command2(command, path, socket);
}

void start_command2(char *command, path_t **path, user_node_t **socket)
{
    if (strncmp(command, "CWD", 3) == 0) {
        cwd(socket, command, path);
        return;
    }
    if (strncmp(command, "PWD", 3) == 0) {
        pwd(socket, path);
        return;
    }
    if (strncmp(command, "CDUP", 4) == 0) {
        cd_up(socket, path);
        return;
    }
    if (strncmp(command, "HELP", 4) == 0) {
        list_command();
        return;
    }
    handle_error(command, socket);
}
