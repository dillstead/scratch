// gcc -Werror -std=c11 -Wall -Wextra -Wno-error=unused-parameter -Wno-error=unused-function -Wno-error=unused-variable -Wconversion -Wno-error=sign-conversion -fsanitize=address,undefined -I ../../types -I../../arena -g3 -o io_uring_cp ../../arena/arena.c io_uring_cp.c
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <liburing.h>
#include "types.h"
#include "arena.h"

struct io_data
{
    off_t base_off;
    off_t off;
    size len;
    bool is_read;
    struct iovec iov;
    struct io_data *next;
    byte block[];
};

struct queue
{
    struct io_uring ring;
    size ring_entries;
    size entries_added;
};

static struct io_data *head;
static struct arena arena;
static size max_ring_entries;
static size block_sz;
static int in_fd;
static int out_fd;

static struct io_data *get_data(void)
{
    struct io_data *data = head;

    head = head->next;
    return data;
}

static void put_data(struct io_data *data)
{
    data->next = head;
    head = data;
}

static void init_data(void)    
{
    // TODO fix sizeof
    arena = new_arena((max_ring_entries << 2) * (sizeof(struct io_data) + block_sz));
    for (size i = 0; i < max_ring_entries; i++)
    {
        
        put_data(alloc(&arena, sizeof(struct io_data) + block_sz, alignof(struct io_data), 1));
    }
}

static int get_file_size(int fd, size *sz)
{
    struct stat st;

    if (fstat(fd, &st) != 0)
    {
        perror("fstat input file failed");
        return 1;
    }
    if (S_ISREG(st.st_mode))
    {
        *sz = st.st_size;
        return 0;
    }
    else if (S_ISBLK(st.st_mode))
    {
        u64 bytes;

        if (ioctl(fd, BLKGETSIZE64, &bytes) != 0)
        {
            perror("getting blocksize failed");
            return 1;
        }
        *sz =  (size) bytes;
        return 0;
    }
    return 1;
}

static int queue_read(struct queue *queue, size offset, size len)
{
    struct io_uring_sqe *sqe;
    struct io_data *data;
    
    sqe = io_uring_get_sqe(&queue->ring);
    if (!sqe)
    {
        fprintf(stderr, "queueing read failed\n");
        return 1; 
    }
    data = get_data();
    data->base_off = offset;
    data->off = offset;
    data->is_read = true;
    data->len = len;
    data->iov.iov_base = data->block;
    data->iov.iov_len = (size_t) data->len;
    io_uring_prep_readv(sqe, in_fd, &data->iov, 1, (unsigned) data->off);
    io_uring_sqe_set_data(sqe, data);
    queue->ring_entries++;
    queue->entries_added++;
    return 0;
}

static int queue_write(struct queue *queue, struct io_data *data)
{
    struct io_uring_sqe *sqe;
    
    sqe = io_uring_get_sqe(&queue->ring);
    if (!sqe)
    {
        fprintf(stderr, "queueing write failed\n");
        return 1; 
    }
    
    data->off = data->base_off;
    data->is_read = false;
    data->iov.iov_base = data->block;
    data->iov.iov_len = (size_t) data->len;
    io_uring_prep_writev(sqe, out_fd, &data->iov, 1, (unsigned) data->off);
    io_uring_sqe_set_data(sqe, data);
    queue->entries_added++;
    return 0;
}

static int requeue(struct queue *queue, struct io_data *data)
{
    struct io_uring_sqe *sqe;

    sqe = io_uring_get_sqe(&queue->ring);
    if (!sqe)
    {
        fprintf(stderr, "requeing failed\n");
        return 1; 
    }
    
    if (data->is_read)
    {
        printf("requeue read %ld\n", queue->ring_entries);
        io_uring_prep_readv(sqe, in_fd, &data->iov, 1, (unsigned) data->off);
    }
    else
    {
        printf("requeue_write %ld\n", queue->ring_entries);
        io_uring_prep_writev(sqe, out_fd, &data->iov, 1, (unsigned) data->off);
    }
    io_uring_sqe_set_data(sqe, data);
    queue->entries_added++;
    return 0;
}

static int process_cqe(struct queue *queue, struct io_uring_cqe *cqe, size *write_sz)
{
    struct io_data *data = io_uring_cqe_get_data(cqe);
    int rc = 1;

    *write_sz = 0;
    if (cqe->res < 0)
    {
        if (cqe->res == -EAGAIN)
        {
            if (requeue(queue, data) != 0)
            {
                goto cleanup;
            }
        }
        else
        {
            fprintf(stderr, "%s operation failed: %s\n", data->is_read ? "read" : "write",
                    strerror(abs(cqe->res)));
            goto cleanup;
        }
    }
    else if ((size_t) cqe->res < data->iov.iov_len)
    {
        // Short read or write
        *write_sz = data->is_read ? 0 : cqe->res;
        data->off += cqe->res;
        data->iov.iov_base += cqe->res;
        data->iov.iov_len = (size_t) (data->len - data->off);
        if (requeue(queue, data) != 0)
        {
            goto cleanup;
        }
    }
    else
    {
        // Successful read or write
        if (data->is_read)
        {
            queue_write(queue, data);
        }
        else
        {
            put_data(data);
            *write_sz = cqe->res;
            queue->ring_entries--;
        }
    }
    rc = 0;

cleanup:
    io_uring_cqe_seen(&queue->ring, cqe);
    return rc;
}

static int copy_file(struct queue *queue, size file_sz)
{
    size bytes_to_read = file_sz;
    size bytes_to_write = file_sz;
    size offset = 0;
    size read_sz;
    size write_sz;
    struct io_uring_cqe *cqe;
    int rc;

    while (bytes_to_write)
    {
        // Fill up the ring with as many read entries as possible.
        queue->entries_added = 0;
        while (bytes_to_read && queue->ring_entries < max_ring_entries)
        {
            read_sz = bytes_to_read > block_sz ? block_sz : bytes_to_read;
            if (queue_read(queue, offset, read_sz) != 0)
            {
                return 1;
            }
            bytes_to_read -= read_sz;
            offset += read_sz;
        }
        // If any new reads have been added, submit them.
        if (queue->entries_added)
        {
            rc = io_uring_submit(&queue->ring);
            if (rc < 0)
            {
                fprintf(stderr, "submitting read entry failed: %s\n", strerror(abs(rc)));
                return 1;
            }
        }
        // Wait for at least one entry to finish.
        queue->entries_added = 0;
        rc = io_uring_wait_cqe(&queue->ring, &cqe);
        if (rc < 0)
        {
            fprintf(stderr, "waiting for entry failed: %s\n", strerror(abs(rc)));
            return 1;
        }
        rc = process_cqe(queue, cqe, &write_sz);
        if (rc < 0)
        {
            return 1;
        }
        bytes_to_write -= write_sz;
        // If there are more entries that are finished, process them too.
        while ((rc = io_uring_peek_cqe(&queue->ring, &cqe)) == 0)
        {
            rc = process_cqe(queue, cqe, &write_sz);
            if (rc < 0)
            {
                return 1;
            }
            bytes_to_write -= write_sz;
        }
        if (rc != -EAGAIN)
        {
            fprintf(stderr, "peeking entry failed: %s\n", strerror(abs(rc)));
            return 1;
        }
        // If any new entries have been added, submit them.
        if (queue->entries_added)
        {
            rc = io_uring_submit(&queue->ring);
            if (rc < 0)
            {
                fprintf(stderr, "submitting read entry failed: %s\n", strerror(abs(rc)));
                return 1;
            }
        }
    }
    return 0;
}
    
int main(int argc, char **argv)
{
    struct queue queue;
    size file_sz;
    int rc;
    
    if (argc < 5)
    {
        fprintf(stderr, "Usage: %s <infile> <outfile> <ring entries> <block size>\n",
                argv[0]);
        return EXIT_FAILURE;
    }

    max_ring_entries = atoi(argv[3]);
    block_sz = atoi(argv[4]);
    init_data();
    
    in_fd = open(argv[1], O_RDONLY);
    if (in_fd < 0)
    {
        perror("open input file");
        return EXIT_FAILURE;
    }

    out_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out_fd < 0)
    {
        perror("open output file");
        return EXIT_FAILURE;
    }

    if (get_file_size(in_fd, &file_sz) != 0)
    {
        return EXIT_FAILURE;
    }

    memset(&queue, 0, sizeof queue);
    rc = io_uring_queue_init((unsigned) max_ring_entries, &queue.ring, 0);
    if (rc != 0)
    {
        fprintf(stderr, "init uring queue failed: %s\n", strerror(abs(rc)));
        return EXIT_FAILURE;
    }

    rc = copy_file(&queue, file_sz);
    if (rc != 0)
    {
        return EXIT_FAILURE;
    }

    close(in_fd);
    close(out_fd);
    io_uring_queue_exit(&queue.ring);
    return EXIT_SUCCESS;
}
