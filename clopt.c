// immediate mode option parser in C99
// <https://codeberg.org/NRK/slashtmp/src/branch/master/parsers/clopt.c>
//
// This is free and unencumbered software released into the public domain.
// For more information, please refer to <https://unlicense.org/>

#include <stddef.h>
#include <string.h>

enum { ClErrUnknown = 1, ClErrTooMany, ClErrNotEnough };
static const char *const CL_ERR[] = {
    [ClErrUnknown]   = "unknown flag",
    [ClErrTooMany]   = "doesn't accept any argument",
    [ClErrNotEnough] = "requires an argument",
};
typedef struct {
    char **argv, *flag, *arg;
    int err, len;
} CLOpt;

static CLOpt
clinit(char **argv)
{
    CLOpt o = {0};
    o.argv = argv + (argv[0] != NULL); // argv[0] can be NULL, don't blindly skip it.
    return o;
}

static int
clnext(CLOpt *o)
{
    if (o->err) return 0;
    if (o->len > 1 && o->flag[o->len] == '=' &&
        o->arg != o->flag + o->len + 1) return !(o->err = ClErrTooMany);
    if (!(o->len == 1 && o->flag[1] != '\0' && o->arg != o->flag+1)) {
        char *s = o->flag = *o->argv++;
        if (!s || s[0] != '-' || s[1] == '\0') return (--o->argv, 0); // non-flag, unconsume & stop
        if (s[1] == '-' && s[2] == '\0') return 0; // "--", stop
        o->len = (s[1] == '-') ? strcspn(s+1, "=") : 1;
    }
    ++o->flag; o->err = ClErrUnknown;
    return 1;
}

static int
cloptarg(CLOpt *o)
{
    return o->flag[o->len] ? !!(o->arg = o->flag + o->len + (o->len > 1)) : 0;
}

static int
clarg(CLOpt *o)
{
    if (cloptarg(o)) return 1;
    else if (*o->argv == NULL) return !(o->err = ClErrNotEnough);
    else return !!(o->arg = *o->argv++);
}

static int
clopt(CLOpt *o, int sopt, const char *lopt)
{
    int match = (sopt && o->len == 1 && *o->flag == sopt);
    if (!match && lopt) {
        match = strlen(lopt) == ((size_t)o->len - 1) &&
            memcmp(o->flag+1, lopt, o->len - 1) == 0;
    }
    if (match && o->err == ClErrUnknown) o->err = 0;
    return match;
}


//////////////////////

#ifdef DEMO

#include <stdio.h>

extern int
main(int argc, char *argv[])
{
    (void) argc;
    CLOpt o[1] = { clinit(argv) };
    while (clnext(o)) {
        /****/ if (clopt(o, 'y', NULL)) {
            printf("-y: enabled!\n");
        } else if (clopt(o, 0, "long")) {
            printf("--long: enabled\n");
        } else if (clopt(o, 'n', "name") && clarg(o)) {
            printf("name: %s\n", o->arg);
        } else if (clopt(o, 0, "optional")) {
            if (cloptarg(o)) {
                printf("optional: %s\n", o->arg);
            } else {
                printf("optional: <default>\n");
            }
        }
    }
    if (o->err) {
        fprintf(stderr, "ERROR: -%.*s: %s\n", o->len, o->flag, CL_ERR[o->err]);
        return 1;
    }

    printf("Remaining args: { ");
    for (; o->argv[0]; ++o->argv)
        printf("%s%s", o->argv[0], o->argv[1] ? ", " : " ");
    printf("}\n");
}

#endif // DEMO


//////////////////////

#ifdef TEST

#define ASSERT(X) ((X) ? (void)0 : __builtin_trap())

static int
str_eq(const char *a, const char *b)
{
    if (a == b) return 1;
    if (a == NULL || b == NULL) return 0;
    return strcmp(a, b) == 0;
}

extern int
main(int argc, char *argv[])
{
    static char darg_default[] = "";
    struct test {
        char *input[8];
        int aflag;
        int bflag;
        char *carg;
        char *darg;
        int err;
        char *arguments[8];
    };
    struct test testv[] = {
        {
            { 0 },
            0, 0, NULL, NULL, 0, { 0 },
        },
        {
            { "" },
            0, 0, NULL, NULL, 0, { 0 },
        },
        {
            { "", "--" },
            0, 0, NULL, NULL, 0, { 0 },
        },
        {
            { "", "--", "--aflag", "" },
            0, 0, NULL, NULL, 0, { "--aflag", "" },
        },
        {
            { "", "-" },
            0, 0, NULL, NULL, 0, { "-" },
        },
        {
            { "", "ok", "hello" },
            0, 0, NULL, NULL, 0, { "ok", "hello" },
        },
        {
            { "", "-a", "ok" },
            1, 0, NULL, NULL, 0, { "ok" },
        },
        {
            { "", "-bab", "ok", "" },
            1, 2, NULL, NULL, 0, { "ok", "" },
        },
        {
            { "", "-bac", "ok", "" },
            1, 1, "ok", NULL, 0, { "" },
        },
        {
            { "", "-bacOK", "ok" },
            1, 1, "OK", NULL, 0, { "ok" },
        },
        {
            { "", "-a", "-b", "-c", "OK", "-d", "ok" },
            1, 1, "OK", darg_default, 0, { "ok" },
        },
        {
            { "", "--dflag", "ok" },
            0, 0, NULL, darg_default, 0, { "ok" },
        },
        {
            { "", "--dflag=ok", "hi" },
            0, 0, NULL, "ok", 0, { "hi" },
        },
        {
            { "", "--dflag=ok", "-dOK" },
            0, 0, NULL, "OK", 0, { 0 },
        },
        {
            { "", "--dflag=ok", "-dOK", "-d", "hi" },
            0, 0, NULL, darg_default, 0, { "hi" },
        },
        {
            { "", "-ab", "--unknown=ok", "-dOK" },
            1, 1, NULL, NULL, ClErrUnknown, {0},
        },
        {
            { "", "--aflag=ok", "-dOK" },
            1, 0, NULL, NULL, ClErrTooMany, {0},
        },
        {
            { "", "--cflag" },
            0, 0, NULL, NULL, ClErrNotEnough, {0},
        },
    };
    struct test *testend = testv + sizeof testv / sizeof *testv;

    for (struct test *t = testv; t < testend; ++t) {
        CLOpt o[1] = { clinit(t->input) };
        int aflag = 0, bflag = 0;
        char *carg = NULL, *darg = NULL;
        while (clnext(o)) {
            /****/ if (clopt(o, 'a', "aflag")) {
                ++aflag;
            } else if (clopt(o, 'b', NULL)) {
                ++bflag;
            } else if (clopt(o, 'c', "cflag") && clarg(o)) {
                carg = o->arg;
            } else if (clopt(o, 'd', "dflag")) {
                if (cloptarg(o)) {
                    darg = o->arg;
                } else {
                    darg = darg_default;
                }
            }
        }

        ASSERT(aflag == t->aflag);
        ASSERT(bflag == t->bflag);
        ASSERT(str_eq(carg, t->carg));
        ASSERT(str_eq(darg, t->darg));
        ASSERT(o->err == t->err);
        for (int i = 0; !o->err && 1; ++i) {
            if (t->arguments[i] == NULL) {
                ASSERT(o->argv[i] == NULL);
                break;
            } else {
                ASSERT(strcmp(t->arguments[i], o->argv[i]) == 0);
            }
        }
    }
}

#endif // TEST
