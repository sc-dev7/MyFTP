/*
** EPITECH PROJECT, 2024
** ftp
** File description:
** sc
*/

#include "server.h"

void help(void)
{
    printf("USAGE: ./myftp port path\n");
    printf("\tport is the port number on which the server socket listens\n");
    printf("\tpath is the path to the home directory ");
    printf("for the Anonymous user\n");
}

int check_path(const char *path)
{
    struct stat path_stat;

    if (stat(path, &path_stat) == 0) {
        return 0;
    } else {
        return -1;
    }
}

int main(int ac, char **av)
{
    int port;

    if (strcmp(av[1], "-help") == 0) {
        help();
        return 0;
    }
    if (ac == 3) {
        port = atoi(av[1]);
        if (check_path(av[2]) == -1)
            return 84;
        if (port <= 0) {
            return EXIT_FAILURE;
        }
        server(port, av[2]);
    }
    return 84;
}
