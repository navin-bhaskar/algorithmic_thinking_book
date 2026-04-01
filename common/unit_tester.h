#ifndef _UNIT_TESTER_H
#define _UNIT_TESTER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Minimal header-only C unit-test framework.
 *
 * Example usage:
 *
 *   #include "unit_tester.h"
 *
 *   TEST_CASE(test_add) {
 *       ASSERT_EQ(1 + 1, 2);
 *   }
 *
 *   int main(void) {
 *       REGISTER_TEST(test_add);
 *       return RUN_ALL_TESTS();
 *   }
 */

typedef void (*unit_test_fn)(void);

typedef struct unit_test_entry {
    const char *name;
    unit_test_fn fn;
    struct unit_test_entry *next;
} unit_test_entry;

static unit_test_entry *ut_head = NULL;
static unit_test_entry *ut_tail = NULL;
static int ut_total = 0;
static int ut_failed = 0;
static const char *ut_current_test = NULL;

static void ut_register(const char *name, unit_test_fn fn) {
    unit_test_entry *entry = (unit_test_entry *)malloc(sizeof(unit_test_entry));
    if (!entry) {
        fprintf(stderr, "unit_tester: out of memory registering test '%s'\n", name);
        exit(EXIT_FAILURE);
    }
    entry->name = name;
    entry->fn = fn;
    entry->next = NULL;

    if (!ut_head) {
        ut_head = ut_tail = entry;
    } else {
        ut_tail->next = entry;
        ut_tail = entry;
    }
}

#define TEST_CASE(fn_name) \
    static void fn_name(void)

#define REGISTER_TEST(fn_name) ut_register(#fn_name, fn_name)

#define ASSERT(cond) do { \
    if (!(cond)) { \
        fprintf(stderr, "\nFAILED: %s\n  test: %s\n  file: %s\n  line: %d\n  condition: %s\n", \
            __func__, ut_current_test, __FILE__, __LINE__, #cond); \
        ut_failed++; \
        return; \
    } \
} while (0)

#define ASSERT_EQ(actual, expected) do { \
    if ((actual) != (expected)) { \
        fprintf(stderr, "\nFAILED: %s\n  test: %s\n  file: %s\n  line: %d\n  actual: %lld\n  expected: %lld\n", \
            __func__, ut_current_test, __FILE__, __LINE__, (long long)(actual), (long long)(expected)); \
        ut_failed++; \
        return; \
    } \
} while (0)

#define ASSERT_STR_EQ(actual, expected) do { \
    const char *_a = (actual); const char *_e = (expected); \
    if ((_a == NULL && _e != NULL) || (_a != NULL && _e == NULL) || (_a && _e && strcmp(_a, _e) != 0)) { \
        fprintf(stderr, "\nFAILED: %s\n  test: %s\n  file: %s\n  line: %d\n  actual: '%s'\n  expected: '%s'\n", \
            __func__, ut_current_test, __FILE__, __LINE__, _a ? _a : "(null)", _e ? _e : "(null)"); \
        ut_failed++; \
        return; \
    } \
} while (0)

static int RUN_ALL_TESTS(void) {
    unit_test_entry *e;
    printf("Running unit tests...\n");

    for (e = ut_head; e; e = e->next) {
        int before = ut_failed;

        ut_total++;
        ut_current_test = e->name;
        printf("[ %-8s] %s\n", "RUNNING", e->name);

        e->fn();

        if (ut_failed == before) {
            printf("[ %-8s] %s\n", "OK", e->name);
        } else {
            printf("[ %-8s] %s\n", "FAILED", e->name);
        }
    }

    printf("\nTests run: %d, Failures: %d\n", ut_total, ut_failed);
    return ut_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

#endif /* _UNIT_TESTER_H */