/*
** EPITECH PROJECT, 2024
** ftp
** File description:
** sc
*/

#include "server.h"

void handle_error(char *command, user_node_t **socket)
{
    char response[256];

    if (strcmp(command, "USER")) {
        strcpy(response, "501 incorrect command USER.\r\n");
        write((*socket)->socket, response, strlen(response));
        return;
    }
    if (strcmp(command, "PASS")) {
        strcpy(response, "501 incorrect command PASS.\r\n");
        write((*socket)->socket, response, strlen(response));
        return;
    }
    strcpy(response, "500 Unknown command.\r\n");
    write((*socket)->socket, response, strlen(response));
    return;
}

void setup_fd_set(int sockfd, user_node_t *user_list, fd_set *readfds)
{
    user_node_t *current = user_list;

    FD_ZERO(readfds);
    FD_SET(sockfd, readfds);
    while (current != NULL) {
        FD_SET(current->socket, readfds);
        current = current->next;
    }
}

void handle_commands(user_node_t **user_list, fd_set *readfds, path_t **path)
{
    user_node_t *current = *user_list;
    user_node_t **ptr_to_current = user_list;
    user_node_t *temp_next;
    bool node_deleted;

    while (current != NULL) {
        node_deleted = false;
        if (FD_ISSET(current->socket, readfds)) {
            temp_next = current->next;
            start_command(&current, path);
            node_deleted = (current == NULL);
            *ptr_to_current = node_deleted ? temp_next : *ptr_to_current;
            ptr_to_current = node_deleted ? ptr_to_current : &current->next;
            current = node_deleted ? temp_next : current->next;
        } else {
            ptr_to_current = &current->next;
            current = current->next;
        }
    }
}

user_node_t *new_connection(int sockfd, user_node_t **user_list,
    struct sockaddr_in *address)
{
    char response[256];
    int addrlen = sizeof(struct sockaddr_in);
    int new_socket = accept(sockfd, (struct sockaddr *)address,
    (socklen_t *) &addrlen);
    user_node_t *actual_socket;

    if (new_socket < 0) {
        perror("Error accept");
        return NULL;
    }
    actual_socket = get_user(new_socket, user_list);
    if (actual_socket) {
        strcpy(response, "220 Service ready for new user.\r\n");
        write(actual_socket->socket, response, strlen(response));
        actual_socket->socket = new_socket;
        actual_socket->user = NULL;
        actual_socket->password = NULL;
        actual_socket->is_connected = 0;
    }
    return actual_socket;
}

void run(int sockfd, struct sockaddr_in *address, char *path)
{
    user_node_t *user_list = NULL;
    path_t *Path = malloc(sizeof(path_t));
    fd_set readfds;

    if (Path) {
        Path->path = strdup(path);
    }
    while (1) {
        setup_fd_set(sockfd, user_list, &readfds);
        if (select(FD_SETSIZE, &readfds, NULL, NULL, NULL) < 0) {
            perror("Error  select");
            exit(EXIT_FAILURE);
        }
        if (FD_ISSET(sockfd, &readfds)) {
            new_connection(sockfd, &user_list, address);
        }
        handle_commands(&user_list, &readfds, &Path);
    }
    close(sockfd);
}
