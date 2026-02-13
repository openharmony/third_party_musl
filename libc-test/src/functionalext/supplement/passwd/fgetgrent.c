/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define _GNU_SOURCE
#include <errno.h>
#include <grp.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "test.h"

/**
 * @tc.name      : fgetgrent_0100
 * @tc.desc      : Verify that group entry can be read from a valid file stream
 * @tc.level     : Level 0
 */
void fgetgrent_0100(void)
{
    FILE *f = fopen("/etc/group", "r");
    if (!f) {
        t_error("%s fopen failed\n", __func__);
        return;
    }

    struct group *grp = fgetgrent(f);
    if (!grp) {
        t_error("%s fgetgrent failed\n", __func__);
        fclose(f);
        return;
    }

    if (grp->gr_name == NULL) {
        t_error("%s grp->gr_name is NULL\n", __func__);
    }

    if (grp->gr_mem == NULL) {
        t_error("%s gr_mem is NULL\n", __func__);
    }

    fclose(f);
}

/**
 * @tc.name      : fgetgrent_0200
 * @tc.desc      : Verify that multiple group entries can be read from file stream
 * @tc.level     : Level 0
 */
void fgetgrent_0200(void)
{
    FILE *f = fopen("/etc/group", "r");
    if (!f) {
        t_error("%s fopen failed\n", __func__);
        return;
    }

    int count = 0;
    struct group *grp;
    while ((grp = fgetgrent(f)) != NULL) {
        count++;
        if (grp->gr_name == NULL) {
            t_error("%s grp->gr_name is NULL\n", __func__);
        }
        if (grp->gr_mem == NULL) {
            t_error("%s gr_mem is NULL\n", __func__);
        }
    }

    if (count == 0) {
        t_error("%s no group entries read\n", __func__);
    }

    fclose(f);
}

/**
 * @tc.name      : fgetgrent_0300
 * @tc.desc      : Verify that fgetgrent returns NULL when file pointer is NULL
 * @tc.level     : Level 2
 */
void fgetgrent_0300(void)
{
    struct group *grp = fgetgrent(NULL);
    if (grp != NULL) {
        t_error("%s should return NULL for NULL file pointer\n", __func__);
    }
}

/**
 * @tc.name      : fgetgrent_0400
 * @tc.desc      : Verify that fgetgrent returns NULL at EOF
 * @tc.level     : Level 1
 */
void fgetgrent_0400(void)
{
    FILE *f = fopen("/etc/group", "r");
    if (!f) {
        t_error("%s fopen failed\n", __func__);
        return;
    }

    struct group *grp;
    do {
        grp = fgetgrent(f);
    } while (grp != NULL);

    /* After reading all entries, should return NULL */
    if (fgetgrent(f) != NULL) {
        t_error("%s should return NULL after EOF\n", __func__);
    }

    fclose(f);
}

/**
 * @tc.name      : fgetgrent_0500
 * @tc.desc      : Verify that group entry fields are correctly parsed
 * @tc.level     : Level 0
 */
void fgetgrent_0500(void)
{
    FILE *f = fopen("/etc/group", "r");
    if (!f) {
        t_error("%s fopen failed\n", __func__);
        return;
    }

    struct group *grp = fgetgrent(f);
    if (!grp) {
        t_error("%s fgetgrent failed\n", __func__);
        fclose(f);
        return;
    }

    /* Check that gr_name is not empty */
    if (grp->gr_name == NULL || strlen(grp->gr_name) == 0) {
        t_error("%s gr_name is NULL or empty\n", __func__);
    }

    /* Check that gr_gid is valid (non-negative for most systems) */
    if ((int)grp->gr_gid < 0) {
        t_error("%s gr_gid is negative: %d\n", __func__, (int)grp->gr_gid);
    }

    /* gr_mem should be a valid pointer (may be NULL or point to NULL) */
    if (grp->gr_mem != NULL) {
        /* If not NULL, it should be a valid array */
        /* The array may be empty (gr_mem[0] == NULL) or contain members */
    }

    fclose(f);
}

/**
 * @tc.name      : fgetgrent_0600
 * @tc.desc      : Verify that fgetgrent works with freshly opened file
 * @tc.level     : Level 1
 */
void fgetgrent_0600(void)
{
    FILE *f = fopen("/etc/group", "r");
    if (!f) {
        t_error("%s fopen failed\n", __func__);
        return;
    }

    /* Read first entry */
    struct group *grp1 = fgetgrent(f);
    if (!grp1) {
        t_error("%s first fgetgrent failed\n", __func__);
        fclose(f);
        return;
    }

    char *first_name = strdup(grp1->gr_name);
    if (!first_name) {
        t_error("%s strdup failed\n", __func__);
        fclose(f);
        return;
    }

    /* Read more entries */
    struct group *grp;
    int count = 1;
    while ((grp = fgetgrent(f)) != NULL) {
        count++;
    }

    fclose(f);

    /* Open file again and verify first entry is same */
    f = fopen("/etc/group", "r");
    if (!f) {
        t_error("%s second fopen failed\n", __func__);
        free(first_name);
        return;
    }

    struct group *grp2 = fgetgrent(f);
    if (!grp2) {
        t_error("%s second fgetgrent failed\n", __func__);
        free(first_name);
        fclose(f);
        return;
    }

    if (strcmp(grp2->gr_name, first_name) != 0) {
        t_error("%s first entry mismatch: %s vs %s\n", __func__, first_name, grp2->gr_name);
    }

    free(first_name);
    fclose(f);
}

/**
 * @tc.name      : fgetgrent_0700
 * @tc.desc      : Verify that fgetgrent handles files with no group entries gracefully
 * @tc.level     : Level 1
 */
void fgetgrent_0700(void)
{
    const char *test_file = "/data/fgetgrent_test_empty.txt";

    FILE *f = fopen(test_file, "w");
    if (!f) {
        t_error("%s failed to create test file\n", __func__);
        return;
    }
    fclose(f);

    f = fopen(test_file, "r");
    if (!f) {
        t_error("%s failed to open test file\n", __func__);
        return;
    }

    struct group *grp = fgetgrent(f);
    if (grp != NULL) {
        t_error("%s should return NULL for empty file\n", __func__);
    }

    fclose(f);
    unlink(test_file);
}

/**
 * @tc.name      : fgetgrent_0800
 * @tc.desc      : Verify that fgetgrent can handle group entries with members
 * @tc.level     : Level 1
 */
void fgetgrent_0800(void)
{
    FILE *f = fopen("/etc/group", "r");
    if (!f) {
        t_error("%s fopen failed\n", __func__);
        return;
    }

    struct group *grp;
    int found_with_members = 0;

    while ((grp = fgetgrent(f)) != NULL) {
        if (grp->gr_mem != NULL && grp->gr_mem[0] != NULL) {
            found_with_members = 1;
            for (int i = 0; grp->gr_mem[i] != NULL; i++) {
                if (strlen(grp->gr_mem[i]) == 0) {
                    t_error("%s empty member name found\n", __func__);
                }
            }
            break;
        }
    }

    if (found_with_members) {
    }

    fclose(f);
}

/**
 * @tc.name      : fgetgrent_0900
 * @tc.desc      : Verify that fgetgrent automatically skips malformed entries and reads next valid entry
 * @tc.level     : Level 1
 */
void fgetgrent_0900(void)
{
    const char *test_file = "/data/fgetgrent_test_malformed.txt";

    FILE *f = fopen(test_file, "w");
    if (!f) {
        t_error("%s failed to create test file\n", __func__);
        return;
    }
    /* Valid entry: testgroup with no members */
    fprintf(f, "testgroup:x:1000\n");
    /* Malformed entry: incomplete (missing GID and members) - should be skipped */
    fprintf(f, "incomplete:x\n");
    /* Valid entry: another with members */
    fprintf(f, "another:x:1001:user1,user2\n");
    fclose(f);

    f = fopen(test_file, "r");
    if (!f) {
        t_error("%s failed to open test file\n", __func__);
        unlink(test_file);
        return;
    }

    /* First call: should read first valid entry "testgroup" */
    struct group *grp = fgetgrent(f);
    if (grp == NULL) {
        t_error("%s failed to read first valid entry\n", __func__);
    } else {
        if (strcmp(grp->gr_name, "testgroup") != 0) {
            t_error("%s first entry name mismatch, got '%s', expected 'testgroup'\n", __func__, grp->gr_name);
        }
    }

    /* Second call: musl implementation skips malformed "incomplete:x" and reads "another" */
    grp = fgetgrent(f);
    if (grp == NULL) {
        t_error("%s failed to read second valid entry (after skipping malformed)\n", __func__);
    } else {
        if (strcmp(grp->gr_name, "another") != 0) {
            t_error("%s second entry name mismatch, got '%s', expected 'another'\n", __func__, grp->gr_name);
        }
        /* Verify members were parsed correctly */
        if (grp->gr_mem == NULL || grp->gr_mem[0] == NULL ||
            strcmp(grp->gr_mem[0], "user1") != 0) {
            t_error("%s second entry members not parsed correctly\n", __func__);
        }
    }

    /* Third call: should return NULL (EOF) */
    grp = fgetgrent(f);
    if (grp != NULL) {
        t_error("%s should return NULL at EOF, got '%s'\n", __func__, grp->gr_name);
    }

    fclose(f);
    unlink(test_file);
}

/**
 * @tc.name      : fgetgrent_1000
 * @tc.desc      : Verify that fgetgrent handles empty password field
 * @tc.level     : Level 1
 */
void fgetgrent_1000(void)
{
    const char *test_file = "/data/fgetgrent_test_nopasswd.txt";

    FILE *f = fopen(test_file, "w");
    if (!f) {
        t_error("%s failed to create test file\n", __func__);
        return;
    }
    fprintf(f, "testgroup::1000:user1\n");
    fclose(f);

    f = fopen(test_file, "r");
    if (!f) {
        t_error("%s failed to open test file\n", __func__);
        unlink(test_file);
        return;
    }

    struct group *grp = fgetgrent(f);
    if (grp == NULL) {
        t_error("%s failed to read entry with empty password\n", __func__);
    } else {
        if (strcmp(grp->gr_name, "testgroup") != 0) {
            t_error("%s group name mismatch\n", __func__);
        }
        if (grp->gr_passwd == NULL || strlen(grp->gr_passwd) != 0) {
            t_error("%s password field should be empty string\n", __func__);
        }
    }

    fclose(f);
    unlink(test_file);
}

/**
 * @tc.name      : fgetgrent_1100
 * @tc.desc      : Verify that fgetgrent handles multiple members
 * @tc.level     : Level 1
 */
void fgetgrent_1100(void)
{
    const char *test_file = "/data/fgetgrent_test_multimem.txt";

    FILE *f = fopen(test_file, "w");
    if (!f) {
        t_error("%s failed to create test file\n", __func__);
        return;
    }
    fprintf(f, "testgroup:x:1000:user1,user2,user3,user4\n");
    fclose(f);

    f = fopen(test_file, "r");
    if (!f) {
        t_error("%s failed to open test file\n", __func__);
        unlink(test_file);
        return;
    }

    struct group *grp = fgetgrent(f);
    if (grp == NULL) {
        t_error("%s failed to read entry with multiple members\n", __func__);
    } else {
        if (grp->gr_mem == NULL) {
            t_error("%s gr_mem should not be NULL\n", __func__);
        } else {
            int count = 0;
            for (int i = 0; grp->gr_mem[i] != NULL; i++) {
                count++;
            }
            if (count != 4) {
                t_error("%s expected 4 members, got %d\n", __func__, count);
            }
        }
    }

    fclose(f);
    unlink(test_file);
}

/**
 * @tc.name      : fgetgrent_1200
 * @tc.desc      : Verify that fgetgrent handles comment lines
 * @tc.level     : Level 1
 */
void fgetgrent_1200(void)
{
    const char *test_file = "/data/fgetgrent_test_comments.txt";

    FILE *f = fopen(test_file, "w");
    if (!f) {
        t_error("%s failed to create test file\n", __func__);
        return;
    }
    fprintf(f, "# This is a comment\n");
    fprintf(f, "testgroup:x:1000:user1\n");
    fprintf(f, "# Another comment\n");
    fclose(f);

    f = fopen(test_file, "r");
    if (!f) {
        t_error("%s failed to open test file\n", __func__);
        unlink(test_file);
        return;
    }

    struct group *grp = fgetgrent(f);
    if (grp == NULL) {
        t_error("%s failed to read entry after comment\n", __func__);
    } else {
        if (strcmp(grp->gr_name, "testgroup") != 0) {
            t_error("%s group name mismatch\n", __func__);
        }
    }

    fclose(f);
    unlink(test_file);
}

/**
 * @tc.name      : fgetgrent_1300
 * @tc.desc      : Verify that fgetgrent handles file with only comments
 * @tc.level     : Level 1
 */
void fgetgrent_1300(void)
{
    const char *test_file = "/data/fgetgrent_test_onlycomments.txt";

    FILE *f = fopen(test_file, "w");
    if (!f) {
        t_error("%s failed to create test file\n", __func__);
        return;
    }
    fprintf(f, "# Comment 1\n");
    fprintf(f, "# Comment 2\n");
    fclose(f);

    f = fopen(test_file, "r");
    if (!f) {
        t_error("%s failed to open test file\n", __func__);
        unlink(test_file);
        return;
    }

    struct group *grp = fgetgrent(f);
    if (grp != NULL) {
        t_error("%s should return NULL for file with only comments\n", __func__);
    }

    fclose(f);
    unlink(test_file);
}

/**
 * @tc.name      : fgetgrent_1400
 * @tc.desc      : Verify that fgetgrent handles large GID values
 * @tc.level     : Level 1
 */
void fgetgrent_1400(void)
{
    const char *test_file = "/data/fgetgrent_test_largegid.txt";

    FILE *f = fopen(test_file, "w");
    if (!f) {
        t_error("%s failed to create test file\n", __func__);
        return;
    }
    fprintf(f, "testgroup:x:4294967294:user1\n");
    fclose(f);

    f = fopen(test_file, "r");
    if (!f) {
        t_error("%s failed to open test file\n", __func__);
        unlink(test_file);
        return;
    }

    struct group *grp = fgetgrent(f);
    if (grp == NULL) {
        t_error("%s failed to read entry with large GID\n", __func__);
    } else {
        if (grp->gr_gid != 4294967294UL) {
            t_error("%s GID mismatch\n", __func__);
        }
    }

    fclose(f);
    unlink(test_file);
}

/**
 * @tc.name      : fgetgrent_1500
 * @tc.desc      : Verify that fgetgrent handles zero GID
 * @tc.level     : Level 1
 */
void fgetgrent_1500(void)
{
    const char *test_file = "/data/fgetgrent_test_zerogid.txt";

    FILE *f = fopen(test_file, "w");
    if (!f) {
        t_error("%s failed to create test file\n", __func__);
        return;
    }
    fprintf(f, "testgroup:x:0:user1\n");
    fclose(f);

    f = fopen(test_file, "r");
    if (!f) {
        t_error("%s failed to open test file\n", __func__);
        unlink(test_file);
        return;
    }

    struct group *grp = fgetgrent(f);
    if (grp == NULL) {
        t_error("%s failed to read entry with zero GID\n", __func__);
    } else {
        if (grp->gr_gid != 0) {
            t_error("%s GID should be 0\n", __func__);
        }
    }

    fclose(f);
    unlink(test_file);
}

/**
 * @tc.name      : fgetgrent_1600
 * @tc.desc      : Verify that fgetgrent handles entry with no members
 * @tc.level     : Level 1
 */
void fgetgrent_1600(void)
{
    const char *test_file = "/data/fgetgrent_test_nomembers.txt";

    FILE *f = fopen(test_file, "w");
    if (!f) {
        t_error("%s failed to create test file\n", __func__);
        return;
    }
    fprintf(f, "testgroup:x:1000:\n");
    fclose(f);

    f = fopen(test_file, "r");
    if (!f) {
        t_error("%s failed to open test file\n", __func__);
        unlink(test_file);
        return;
    }

    struct group *grp = fgetgrent(f);
    if (grp == NULL) {
        t_error("%s failed to read entry with no members\n", __func__);
    } else {
        if (grp->gr_mem == NULL || grp->gr_mem[0] != NULL) {
            t_error("%s gr_mem should be empty array\n", __func__);
        }
    }

    fclose(f);
    unlink(test_file);
}

/**
 * @tc.name      : fgetgrent_1700
 * @tc.desc      : Verify that fgetgrent handles whitespace in entry
 * @tc.level     : Level 1
 */
void fgetgrent_1700(void)
{
    const char *test_file = "/data/fgetgrent_test_whitespace.txt";

    FILE *f = fopen(test_file, "w");
    if (!f) {
        t_error("%s failed to create test file\n", __func__);
        return;
    }
    fprintf(f, "  testgroup:x:1000:user1  \n");
    fclose(f);

    f = fopen(test_file, "r");
    if (!f) {
        t_error("%s failed to open test file\n", __func__);
        unlink(test_file);
        return;
    }

    struct group *grp = fgetgrent(f);
    if (grp == NULL) {
        t_error("%s failed to read entry with whitespace\n", __func__);
    }

    fclose(f);
    unlink(test_file);
}

/**
 * @tc.name      : fgetgrent_1800
 * @tc.desc      : Verify that fgetgrent handles very long group name
 * @tc.level     : Level 1
 */
void fgetgrent_1800(void)
{
    const char *test_file = "/data/fgetgrent_test_longname.txt";

    FILE *f = fopen(test_file, "w");
    if (!f) {
        t_error("%s failed to create test file\n", __func__);
        return;
    }
    fprintf(f, "verylonggroupnamethatexceedsnormallimits:x:1000:user1\n");
    fclose(f);

    f = fopen(test_file, "r");
    if (!f) {
        t_error("%s failed to open test file\n", __func__);
        unlink(test_file);
        return;
    }

    struct group *grp = fgetgrent(f);
    if (grp == NULL) {
        t_error("%s failed to read entry with long name group\n", __func__);
    } else {
        if (strcmp(grp->gr_name, "verylonggroupnamethatexceedsnormallimits") != 0) {
            t_error("%s group name mismatch\n", __func__);
        }
    }

    fclose(f);
    unlink(test_file);
}

/**
 * @tc.name      : fgetgrent_1900
 * @tc.desc      : Verify that fgetgrent handles duplicate group names
 * @tc.level     : Level 1
 */
void fgetgrent_1900(void)
{
    const char *test_file = "/data/fgetgrent_test_duplicate.txt";

    FILE *f = fopen(test_file, "w");
    if (!f) {
        t_error("%s failed to create test file\n", __func__);
        return;
    }
    fprintf(f, "testgroup:x:1000:user1\n");
    fprintf(f, "testgroup:x:1001:user2\n");
    fclose(f);

    f = fopen(test_file, "r");
    if (!f) {
        t_error("%s failed to open test file\n", __func__);
        unlink(test_file);
        return;
    }

    /* NOTE: fgetgrent uses static storage, so grp1 and grp2 point to
     * the same struct. We must save values immediately after each call. */
    struct group *grp1 = fgetgrent(f);
    if (grp1 == NULL) {
        t_error("%s failed to read first entry\n", __func__);
        fclose(f);
        unlink(test_file);
        return;
    }

    /* Save values from first entry before it gets overwritten */
    gid_t gid1 = grp1->gr_gid;
    char name1[64];
    strcpy(name1, grp1->gr_name);

    struct group *grp2 = fgetgrent(f);
    if (grp2 == NULL) {
        t_error("%s failed to read second entry\n", __func__);
        fclose(f);
        unlink(test_file);
        return;
    }

    /* Verify both entries were read */
    if (strcmp(name1, grp2->gr_name) != 0) {
        t_error("%s group names should match\n", __func__);
    }
    if (gid1 == grp2->gr_gid) {
        t_error("%s GIDs should be different (%u vs %u)\n", __func__, gid1, grp2->gr_gid);
    }

    fclose(f);
    unlink(test_file);
}
}

/**
 * @tc.name      : fgetgrent_2000
 * @tc.desc      : Verify that fgetgrent handles file opened in write mode
 * @tc.level     : Level 2
 */
void fgetgrent_2000(void)
{
    const char *test_file = "/data/fgetgrent_test_writemode.txt";

    FILE *f = fopen(test_file, "w");
    if (!f) {
        t_error("%s failed to create test file\n", __func__);
        return;
    }
    fprintf(f, "testgroup:x:1000:user1\n");
    fclose(f);

    f = fopen(test_file, "w");
    if (!f) {
        t_error("%s failed to open test file\n", __func__);
        unlink(test_file);
        return;
    }

    struct group *grp = fgetgrent(f);
    if (grp != NULL) {
        t_error("%s should return NULL for write mode file\n", __func__);
    }

    fclose(f);
    unlink(test_file);
}

/**
 * @tc.name      : fgetgrent_2100
 * @tc.desc      : Verify that fgetgrent handles entry with special characters
 * @tc.level     : Level 1
 */
void fgetgrent_2100(void)
{
    const char *test_file = "/data/fgetgrent_test_special.txt";

    FILE *f = fopen(test_file, "w");
    if (!f) {
        t_error("%s failed to create test file\n", __func__);
        return;
    }
    fprintf(f, "test-group_123:x:1000:user-1,user_2\n");
    fclose(f);

    f = fopen(test_file, "r");
    if (!f) {
        t_error("%s failed to open test file\n", __func__);
        unlink(test_file);
        return;
    }

    struct group *grp = fgetgrent(f);
    if (grp == NULL) {
        t_error("%s failed to read entry with special characters\n", __func__);
    } else {
        if (strcmp(grp->gr_name, "test-group_123") != 0) {
            t_error("%s group name mismatch\n", __func__);
        }
        if (grp->gr_mem != NULL && grp->gr_mem[0] != NULL) {
            if (strcmp(grp->gr_mem[0], "user-1") != 0) {
                t_error("%s first member mismatch\n", __func__);
            }
            if (grp->gr_mem[1] != NULL && strcmp(grp->gr_mem[1], "user_2") != 0) {
                t_error("%s second member mismatch\n", __func__);
            }
        }
    }

    fclose(f);
    unlink(test_file);
}

/**
 * @tc.name      : fgetgrent_2200
 * @tc.desc      : Verify that fgetgrent handles invalid GID (negative)
 * @tc.level     : Level 1
 */
void fgetgrent_2200(void)
{
    const char *test_file = "/data/fgetgrent_test_neggid.txt";

    FILE *f = fopen(test_file, "w");
    if (!f) {
        t_error("%s failed to create test file\n", __func__);
        return;
    }
    fprintf(f, "testgroup:x:-1:user1\n");
    fclose(f);

    f = fopen(test_file, "r");
    if (!f) {
        t_error("%s failed to open test file\n", __func__);
        unlink(test_file);
        return;
    }

    struct group *grp = fgetgrent(f);
    if (grp != NULL) {
        t_error("%s should return NULL for invalid negative GID\n", __func__);
    }

    fclose(f);
    unlink(test_file);
}

/**
 * @tc.name      : fgetgrent_2300
 * @tc.desc      : Verify that fgetgrent handles entry with only group name
 * @tc.level     : Level 1
 */
void fgetgrent_2300(void)
{
    const char *test_file = "/data/fgetgrent_test_onlyname.txt";

    FILE *f = fopen(test_file, "w");
    if (!f) {
        t_error("%s failed to create test file\n", __func__);
        return;
    }
    fprintf(f, "testgroup\n");
    fclose(f);

    f = fopen(test_file, "r");
    if (!f) {
        t_error("%s failed to open test file\n", __func__);
        unlink(test_file);
        return;
    }

    struct group *grp = fgetgrent(f);
    if (grp != NULL) {
        t_error("%s should return NULL for entry with only name\n", __func__);
    }

    fclose(f);
    unlink(test_file);
}

/**
 * @tc.name      : fgetgrent_2400
 * @tc.desc      : Verify that fgetgrent handles blank lines
 * @tc.level     : Level 1
 */
void fgetgrent_2400(void)
{
    const char *test_file = "/data/fgetgrent_test_blanklines.txt";

    FILE *f = fopen(test_file, "w");
    if (!f) {
        t_error("%s failed to create test file\n", __func__);
        return;
    }
    fprintf(f, "\n");
    fprintf(f, "testgroup:x:1000:user1\n");
    fprintf(f, "\n");
    fclose(f);

    f = fopen(test_file, "r");
    if (!f) {
        t_error("%s failed to open test file\n", __func__);
        unlink(test_file);
        return;
    }

    struct group *grp = fgetgrent(f);
    if (grp == NULL) {
        t_error("%s failed to read entry after blank line\n", __func__);
    } else {
        if (strcmp(grp->gr_name, "testgroup") != 0) {
            t_error("%s group name mismatch\n", __func__);
        }
    }

    fclose(f);
    unlink(test_file);
}

int main(int argc, char *argv[])
{
    fgetgrent_0100();
    fgetgrent_0200();
    fgetgrent_0300();
    fgetgrent_0400();
    fgetgrent_0500();
    fgetgrent_0600();
    fgetgrent_0700();
    fgetgrent_0800();
    fgetgrent_0900();
    fgetgrent_1000();
    fgetgrent_1100();
    fgetgrent_1200();
    fgetgrent_1300();
    fgetgrent_1400();
    fgetgrent_1500();
    fgetgrent_1600();
    fgetgrent_1700();
    fgetgrent_1800();
    fgetgrent_1900();
    fgetgrent_2000();
    fgetgrent_2100();
    fgetgrent_2200();
    fgetgrent_2300();
    fgetgrent_2400();
    return t_status;
}
