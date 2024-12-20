/*
** EPITECH PROJECT, 2024
** my.h
** File description:
** sc
*/

#pragma once

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
    #include <sys/stat.h>
    #include <sys/select.h>
    #include <stdbool.h>

typedef struct user_node_t {
    int socket;
    char *user;
    char *password;
    int is_connected;
    struct user_node_t *next;
} user_node_t;

typedef struct path_t {
    char *path;
} path_t;

void handle_error(char *command, user_node_t **socket);
char *my_command(const char *command);
user_node_t *find_socket(int socket, user_node_t **head);
char *get_command(user_node_t **socket);
void remove_user(user_node_t **socket_list, int socket);
void server(int port, char *path);
void noop(user_node_t **socket);
void start_command(user_node_t **socket, path_t **path);
void start_command2(char *command, path_t **path, user_node_t **socket);
void my_user(user_node_t *socket, char *command);
void password(user_node_t *socket, char *command);
void cwd(user_node_t **socket, char *command, path_t **path);
void pwd(user_node_t **socket, path_t **path);
void cd_up(user_node_t **socket, path_t **path);
void customer(char *server_ip, int server_port);
void run(int sockfd, struct sockaddr_in *address, char *path);
int check_path(const char *path);
char *get_dir(char *path);
void make_no_op(user_node_t **actual_socket);
user_node_t *get_user(int socket, user_node_t **head);
void list_command(void);
