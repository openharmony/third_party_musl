#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <errno.h>
#include "functionalext.h"

/**
 * @tc.name      : getdelim_0100
 * @tc.desc      : Verify that the file content can be read with the specified delimiter (parameters are valid)
 * @tc.level     : Level 0
 */
void getdelim_0100()
{
    ssize_t result;
    char *wrstring = "helloworld";
    char *line = NULL;
    size_t len = 0;
    FILE *fp = fopen("getdelim.txt", "w+");
    fwrite(wrstring, sizeof(char), strlen(wrstring), fp);
    fseek(fp, 0, SEEK_SET);
    result = getdelim(&line, &len, 'l', fp);
    EXPECT_TRUE("getdelim_0100", result > 0);
    remove("getdelim.txt");
}

/**
 * @tc.name      : getdelim_0200
 * @tc.desc      : Verify that the delimiter cannot be specified to read the file content (the s parameter is invalid)
 * @tc.level     : Level 2
 */
void getdelim_0200()
{
    ssize_t result;
    char *line = NULL;
    char *wrstring = "helloworld";
    size_t a = 0;
    FILE *fp = fopen("getdelim.txt", "w+");
    result = getdelim(&line, NULL, 'l', fp);
    EXPECT_EQ("getdelim_0200", result, -1);
    remove("getdelim.txt");
}

/**
 * @tc.name      : getdelim_0300
 * @tc.desc      : Verify that the delimiter cannot be specified to read the file content (the n parameter is invalid)
 * @tc.level     : Level 2
 */
void getdelim_0300()
{
    ssize_t result;
    char *wrstring = "helloworld";
    size_t a = 0;
    FILE *fp = fopen("getdelim.txt", "w+");
    result = getdelim(NULL, &a, 'l', fp);
    EXPECT_EQ("getdelim_0300", result, -1);
    remove("getdelim.txt");
}

int main()
{
    getdelim_0100();
    getdelim_0200();
    getdelim_0300();

    return t_status;
}