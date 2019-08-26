#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <gnumake.h>

#define YES 1
#define NO 0

#define ERROR_PREFIX "*** ARITHMETIC ERROR: "

int plugin_is_GPL_compatible;

void throw_error(const char *str, int *error)
{
    fprintf(stderr, ERROR_PREFIX "String '%s' is not representable as a long!\n", str);
    *error = YES;
}

long strtol_or_die(const char *str, int *error)
{
    char *end;
    long ret;
    errno = 0;
    
    ret = strtol(str, &end, 0);

    if (*end != '\0' ||
        errno == EINVAL ||
        errno == ERANGE) {

        throw_error(str, error);
        return 0;
    }

    *error = NO;
    return ret;
}

char *add(const char *nm, unsigned argc, char **argv)
{
    long i, temp, sum = 0;
    int err = 0;
    for (i = 0; i < argc; ++i) {
        temp = strtol_or_die(argv[i], &err);
        if (err == YES)
            return NULL;
        sum += temp;
    }

    int size = snprintf(NULL, 0, "%ld", sum) + 1;
    char *buf = gmk_alloc(size + 1); // + 1 for terminating null
    snprintf(buf, size, "%ld", sum);
    return buf;
}

char *sub(const char *nm, unsigned argc, char **argv)
{
    long i, temp, difference;
    int err = 0;
    
    difference = strtol_or_die(argv[0], &err);
    if (err == YES)
        return NULL;

    for (i = 1; i < argc; ++i) {
        temp = strtol_or_die(argv[i], &err);
        if (err == YES)
            return NULL;
        difference -= temp;
    }

    int size = snprintf(NULL, 0, "%ld", difference) + 1;
    char *buf = gmk_alloc(size + 1); // + 1 for terminating null
    snprintf(buf, size, "%ld", difference);
    return buf;
}

char *mul(const char *nm, unsigned argc, char **argv)
{
    long i, temp, product = 1;
    int err = 0;
    for (i = 0; i < argc; ++i) {
        temp = strtol_or_die(argv[i], &err);
        if (err == YES)
            return NULL;
        product *= temp;
    }

    int size = snprintf(NULL, 0, "%ld", product) + 1;
    char *buf = gmk_alloc(size + 1); // + 1 for terminating null
    snprintf(buf, size, "%ld", product);
    return buf;
}

char *divide(const char *nm, unsigned argc, char **argv)
{
    long i, temp, quotient;
    int err = 0;
    
    quotient = strtol_or_die(argv[0], &err);
    if (err == YES)
        return NULL;

    for (i = 1; i < argc; ++i) {
        temp = strtol_or_die(argv[i], &err);
        if (err == YES)
            return NULL;
        if (temp == 0) {
            fprintf(stderr, ERROR_PREFIX "Cannot divide by zero!\n");
            return NULL;
        }
        quotient /= temp;
    }

    int size = snprintf(NULL, 0, "%ld", quotient) + 1;
    char *buf = gmk_alloc(size + 1); // + 1 for terminating null
    snprintf(buf, size, "%ld", quotient);
    return buf;
}

int math_gmk_setup()
{
    // 0 means no limit
    // name, func_ptr, min args, max args, expand or no?
    gmk_add_function ("add", add, 2, 0, GMK_FUNC_DEFAULT);
    gmk_add_function ("sub", sub, 2, 0, GMK_FUNC_DEFAULT);
    gmk_add_function ("mul", mul, 2, 0, GMK_FUNC_DEFAULT);
    gmk_add_function ("div", divide, 2, 0, GMK_FUNC_DEFAULT);
    return 1;
}
