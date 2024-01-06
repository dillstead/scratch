// gcc -Werror -pedantic -std=c99 -Wall -Wextra -fsanitize=address,undefined -o problem15b problem15b.c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define ROW_END 4000000

struct range
{
    int x;
    int y;
    int len;
};

static void sort(struct range *ranges, int num_ranges)
{
    struct range r;
    int j;
    
    for (int i = 1; i < num_ranges; i++)
    {
        r = ranges[i];
        for (j = i; j > 0 && ranges[j - 1].x > r.x; j--)
        {
            ranges[j] = ranges[j - 1];
        }
        ranges[j] = r;
    }
}

static bool get_unavail_range(struct range *sensor, int row, struct range *range)
{
    int32_t td = abs(sensor->y - row);

    if (td > sensor->len)
    {
        return false;
    }
    range->x = sensor->x - sensor->len + td;
    range->y = row;
    range->len = (sensor->len * 2 + 1) - (td * 2);
    return true;
}

static bool check_coverage(struct range *ranges, int num_ranges, int row, struct range *gap)
{
    gap->x = 0;
    gap->y = row;
    gap->len = 1;
    ranges[num_ranges].x = ROW_END;
    ranges[num_ranges].y = row;
    ranges[num_ranges++].len = 1;

    if (num_ranges == 1)
    {
        return false;
    }
    for (int i = 0; i < num_ranges - 1; i++)
    {
        if (ranges[i + 1].x > ranges[i].x + ranges[i].len)
        {
            gap->x = ranges[i].x + ranges[i].len;
            gap->len = ranges[i + 1].x - gap->x;
            return false;
        }
        ranges[i + 1].len = MAX(ranges[i + 1].len, ranges[i].len - (ranges[i + 1].x - ranges[i].x));
    }
    return true;
}

int main(void)
{
    int32_t sx, sy;
    int32_t bx, by;
    struct range sensors[25];
    struct range ranges[25];
    struct range gap;
    int num_sensors = 0;
    int num_ranges;

    while (fscanf(stdin, "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d\n",
                  &sx, &sy, &bx, &by) == 4)
    {
        sensors[num_sensors].x = sx;
        sensors[num_sensors].y = sy;
        sensors[num_sensors++].len = abs(bx - sx) + abs(by - sy);
    }
    for (int i = 0; i <= ROW_END; i++)
    {
        num_ranges = 0;
        for (int j = 0; j < num_sensors; j++)
        {
            if (get_unavail_range(sensors + j, i, ranges + num_ranges)
                && ranges[num_ranges].x + ranges[num_ranges].len > 0
                && ranges[num_ranges].x < ROW_END)
            {
                num_ranges++;
            }
        }
        sort(ranges, num_ranges);
        if (!check_coverage(ranges, num_ranges, i, &gap))
        {
            printf("%lu\n", (unsigned long) gap.x * ROW_END + gap.y);
            break;
        }
    }
    return EXIT_SUCCESS;
}
