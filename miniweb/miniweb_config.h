#ifndef MINIWEB_CONFIG_H
#define MINIWEB_CONFIG_H

#define VERSION "1.1"
#define YEAR "2014"

#define COPYRIGHT_HOLDER "Nguyen Truong Minh"
#define AUTHORS "Nguyen Truong Minh (nguyentrminh@gmail.com)"
#define DESCRIPTION "A http server for receving, parsing and storing log data from clients."
#define PROGRAM_NAME "miniweb"
#define LICENSE " This is free software: you are free to change and redistribute it.\n" \
                " There is NO WARRANTY, to the extent permitted by law.\n"
#define DISPLAY_DESCRIPTION printf("%s %s\n %s\nCopyright (C) %s %s\nLicense %s\nWritten by %s.\n" \
                            ,PROGRAM_NAME, VERSION, DESCRIPTION, COPYRIGHT_HOLDER, YEAR, LICENSE, AUTHORS);

/* Error codes */
#define MNW_OK                0
#define MNW_ERR               -1

#define MNW_IOBUF_LEN         (1024*4)

/* Threads serving clients ordered by the accepting thread */
#define MNW_NUM_WORKER_THREADS    16

/* Asynchronous I/O Options */
#define AE_MAX_CLIENT_PER_WORKER 16000 /* Number of client pending at acceptor */
#define AE_MAX_EPOLL_EVENTS 1024
#define AE_FD_SET_SIZE (MNW_NUM_WORKER_THREADS*AE_MAX_CLIENT_PER_WORKER)    /* Max number of fd supported */
#define AE_MAX_CLIENT_IDLE_TIME 10 /* seconds */
#define MAX_REQUEST_PER_LOOP (AE_MAX_CLIENT_PER_WORKER*sizeof(void*))


#endif // MINIWEB_CONFIG_H
