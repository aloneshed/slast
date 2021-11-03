#include <stdio.h>
#include <fcntl.h>
#include <utmp.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define VALID_ARGC 2

int main(int argc, char *argv[])
{
    if (argc != VALID_ARGC)
    {
        perror("invalid argument");
        exit(1);
    }

    struct utmp current_record;
    int utmpfd;
    int reclen = sizeof(struct utmp);
    int counter = 0;
    int size = atoi(argv[1]);
    if ((utmpfd = open(WTMP_FILE, O_RDONLY)) == -1)
    {
        perror(WTMP_FILE);
        exit(1);
    }

    while (read(utmpfd, &current_record, reclen) == reclen)
    {
        if (current_record.ut_type != DEAD_PROCESS && current_record.ut_type != RUN_LVL)
        {
            counter++;
        }
    }
    close(utmpfd);

    struct utmp data[counter];
    int new_counter = counter - 1;

    if ((utmpfd = open(WTMP_FILE, O_RDONLY)) == -1)
    {
        perror(WTMP_FILE);
        exit(1);
    }

    while (read(utmpfd, &current_record, reclen) == reclen)
    {
        if (current_record.ut_type != DEAD_PROCESS && current_record.ut_type != RUN_LVL)
        {
            data[new_counter--] = current_record;
        }
    }

    for (size_t i = 0; (i < size && i < counter); i++)
    {

        long my_time;
        if ((data[i].ut_type) == BOOT_TIME)
        {
            printf("%s\t ", data[i].ut_user);
            printf("system boot \t ");
            printf("%s\t", data[i].ut_host);
            my_time = data[i].ut_tv.tv_sec;
            printf("%.20s\t", ctime(&my_time)); // time

            printf("\n");
        }
        else
        {
            printf("%s\t ", data[i].ut_user);
            printf("%s\t\t", data[i].ut_line);
            printf(" %s\t\t\t", data[i].ut_host);

            my_time = data[i].ut_tv.tv_sec;
            printf("%.20s\t", ctime(&my_time));

            printf("\n");
        }
    }
    close(utmpfd);
    return 0;
}