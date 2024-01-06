// gcc -Werror -pedantic -std=c99 -Wall -Wextra -fsanitize=address,undefined -g3 -o problem7 problem7.c
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

struct dirent
{
    int idx;
    char name[1024];
};

struct dir
{
    int size;
    bool scanned;
    int num_children;
    struct dirent children[1024];
};

static int find(char *name, struct dir *dir)
{
    for (int i = 0; dir->num_children; i++)
    {
        if (strcmp(name, dir->children[i].name) == 0)
        {
            return dir->children[i].idx;
        }
    }
    return -1;
}

struct dir fs[1024];
int sum = 0;

static int summarize(struct dir *dir)
{
    int size = dir->size;
    
    for (int i = 1; i < dir->num_children; i++)
    {
        size += summarize(fs + dir->children[i].idx);
    }
    if (size <= 100000)
    {
        sum += size;
    }
    dir->size = size;
    return size;
}

static void init_dir(struct dir *dir, int idx)
{
    dir->num_children = 1;
    strcpy(dir->children[0].name, "..");
    dir->children[0].idx = idx;
}

int main(void)
{
    char line[256];
    char name[1024];
    int size;
    int cur = 0;
    int alloc = 1;
    bool do_scan = false;
    int min = INT_MAX;
    int need;

    init_dir(fs, 0);
    while (fgets(line, sizeof line, stdin))
    {
        if (sscanf(line, "$ cd %s\n", name) == 1)
        {
            if (do_scan)
            {
                fs[cur].scanned = true;                
            }
            if (strcmp(name, "/") == 0)
            {
                cur = 0;
            }
            else
            {
                cur = find(name, fs + cur);
                if (cur == -1)
                {
                    return EXIT_FAILURE;
                }
            }
        }
        else if (sscanf(line, "%d %s\n", &size, name) == 2)
        {
            if (do_scan)
            {
                fs[cur].size += size;
            }
        }
        else if (sscanf(line, "dir %s\n", name) == 1)
        {
            if (do_scan)
            {
                init_dir(fs + alloc, cur);
                fs[cur].children[fs[cur].num_children].idx = alloc++;
                strcpy(fs[cur].children[fs[cur].num_children].name, name);
                fs[cur].num_children++;
            }
        }
        else if (strcmp("$ ls\n", line) == 0)
        {
            do_scan = !fs[cur].scanned;
        }
    }
    summarize(fs);
    need = 30000000 - (70000000 - fs[0].size);
    for (int i = 0; i < alloc; i++)
    {
        if (fs[i].size >= need)
        {
            min = MIN(min, fs[i].size);
        }
    }
    printf("%d\n", min);
    return EXIT_SUCCESS;
}
