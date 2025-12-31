/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <string.h>
#include "test.h"

/**
 * @tc.name      : index_0100
 * @tc.desc      : Test the index method to get the position of the first occurrence of the target character
 * @tc.level     : Level 0
 */
void index_0100(void)
{
    char *str = "hello world";
    char *result = index(str, 'e');
    if (strcmp(str + 1, result) != 0) {
        t_error("%s index get result is %s are not  want %s\n", __func__, result, str + 1);
    }
}

/**
 * @tc.name      : index_0200
 * @tc.desc      : The result returned by the index function when the target character appears multiple times in the
 * test string
 * @tc.level     : Level 1
 */
void index_0200(void)
{
    int add_size = 2;
    char *str = "hello world";
    char *result = index(str, 'l');
    if (strcmp(str + add_size, result) != 0) {
        t_error("%s index get result is %s are not  want %s\n", __func__, result, str + add_size);
    }
}

/**
 * @tc.name      : index_0300
 * @tc.desc      : The result returned by the index function when the target character does not appear in the test
 * string
 * @tc.level     : Level 1
 */
void index_0300(void)
{
    char *str = "hello world";
    char *result = index(str, 'a');
    if (result) {
        t_error("%s index get result is %s are not  want ''\n", __func__, result);
    }
}

/**
 * @tc.name      : index_0400
 * @tc.desc      : Verify index finds null terminator character
 * @tc.level     : Level 1
 */
void index_0400(void)
{
    char *str = "hello";
    char *result = index(str, '\0');
    if (result != str + 5) {
        t_error("%s index get result is %s are not want %s\n", __func__, result, str + 5);
    }
}

/**
 * @tc.name      : index_0500
 * @tc.desc      : Verify index works with empty string
 * @tc.level     : Level 2
 */
void index_0500(void)
{
    char *str = "";
    char *result = index(str, 'h');
    if (result != NULL) {
        t_error("%s index get result is %s are not want NULL\n", __func__, result);
    }
}

/**
 * @tc.name      : index_0600
 * @tc.desc      : Verify index finds first character in string
 * @tc.level     : Level 1
 */
void index_0600(void)
{
    char *str = "hello world";
    char *result = index(str, 'h');
    if (result != str) {
        t_error("%s index get result is %s are not want %s\n", __func__, result, str);
    }
}

/**
 * @tc.name      : index_0700
 * @tc.desc      : Verify index finds last character in string
 * @tc.level     : Level 1
 */
void index_0700(void)
{
    char *str = "hello world";
    char *result = index(str, 'd');
    if (result != str + 10) {
        t_error("%s index get result is %s are not want %s\n", __func__, result, str + 10);
    }
}

/**
 * @tc.name      : index_0800
 * @tc.desc      : Verify index works with binary data containing null bytes
 * @tc.level     : Level 2
 */
void index_0800(void)
{
    char data[] = {'a', 'b', '\0', 'c', 'd'};
    char *result = index(data, 'c');
    if (result != NULL) { // Expected to return NULL
        t_error("%s index should stop at '\\0', but got %p\n", __func__, result);
    }
}

/**
 * @tc.name      : index_0900
 * @tc.desc      : Test finding a space character.
 * @tc.level     : Level 1
 */
void index_0900(void)
{
    char *str = "hello world";
    char *result = index(str, ' ');
    if (result != str + 5) {
        t_error("%s: index(\"%s\", ' ') returned %s, expected %s\n", __func__, str, result, str + 5);
    }
}

/**
 * @tc.name      : index_1000
 * @tc.desc      : Test finding a tab character.
 * @tc.level     : Level 1
 */
void index_1000(void)
{
    char *str = "hello\tworld";
    char *result = index(str, '\t');
    if (result != str + 5) {
        t_error("%s: index(\"%s\", '\\\\t') returned %s, expected %s\n", __func__, str, result, str + 5);
    }
}

/**
 * @tc.name      : index_1100
 * @tc.desc      : Test finding a newline character.
 * @tc.level     : Level 1
 */
void index_1100(void)
{
    char *str = "hello\nworld";
    char *result = index(str, '\n');
    if (result != str + 5) {
        t_error("%s: index(\"%s\", '\\\\n') returned %s, expected %s\n", __func__, str, result, str + 5);
    }
}

/**
 * @tc.name      : index_1200
 * @tc.desc      : Test finding a carriage return character.
 * @tc.level     : Level 1
 */
void index_1200(void)
{
    char *str = "hello\rworld";
    char *result = index(str, '\r');
    if (result != str + 5) {
        t_error("%s: index(\"%s\", '\\\\r') returned %s, expected %s\n", __func__, str, result, str + 5);
    }
}

/**
 * @tc.name      : index_1300
 * @tc.name      : index_1300
 * @tc.desc      : Test finding a backslash character.
 * @tc.level     : Level 1
 */
void index_1300(void)
{
    char *str = "hello\\world";
    char *result = index(str, '\\');
    if (result != str + 5) {
        t_error("%s: index(\"%s\", '\\\\\\\\') returned %s, expected %s\n", __func__, str, result, str + 5);
    }
}

/**
 * @tc.name      : index_1400
 * @tc.desc      : Test finding a double quote character.
 * @tc.level     : Level 1
 */
void index_1400(void)
{
    char *str = "hello\"world";
    char *result = index(str, '\"');
    if (result != str + 5) {
        t_error("%s: index(\"%s\", '\\\\\"') returned %s, expected %s\n", __func__, str, result, str + 5);
    }
}

/**
 * @tc.name      : index_1500
 * @tc.desc      : Test finding a single quote character.
 * @tc.level     : Level 1
 */
void index_1500(void)
{
    char *str = "hello'world";
    char *result = index(str, '\'');
    if (result != str + 5) {
        t_error("%s: index(\"%s\", '\\\\'') returned %s, expected %s\n", __func__, str, result, str + 5);
    }
}

/**
 * @tc.name      : index_1600
 * @tc.desc      : Test finding a parenthesis character.
 * @tc.level     : Level 1
 */
void index_1600(void)
{
    char *str = "hello(world)";
    char *result = index(str, '(');
    if (result != str + 5) {
        t_error("%s: index(\"%s\", '(') returned %s, expected %s\n", __func__, str, result, str + 5);
    }
}

/**
 * @tc.name      : index_1700
 * @tc.desc      : Test finding a bracket character.
 * @tc.level     : Level 1
 */
void index_1700(void)
{
    char *str = "hello[world]";
    char *result = index(str, '[');
    if (result != str + 5) {
        t_error("%s: index(\"%s\", '[') returned %s, expected %s\n", __func__, str, result, str + 5);
    }
}

/**
 * @tc.name      : index_1800
 * @tc.desc      : Test finding a brace character.
 * @tc.level     : Level 1
 */
void index_1800(void)
{
    char *str = "hello{world}";
    char *result = index(str, '{');
    if (result != str + 5) {
        t_error("%s: index(\"%s\", '{') returned %s, expected %s\n", __func__, str, result, str + 5);
    }
}

/**
 * @tc.name      : index_1900
 * @tc.desc      : Test finding an angle bracket character.
 * @tc.level     : Level 1
 */
void index_1900(void)
{
    char *str = "hello<world>";
    char *result = index(str, '<');
    if (result != str + 5) {
        t_error("%s: index(\"%s\", '<') returned %s, expected %s\n", __func__, str, result, str + 5);
    }
}

/**
 * @tc.name      : index_2000
 * @tc.desc      : Test finding an underscore character.
 * @tc.level     : Level 1
 */
void index_2000(void)
{
    char *str = "hello_world";
    char *result = index(str, '_');
    if (result != str + 5) {
        t_error("%s: index(\"%s\", '_') returned %s, expected %s\n", __func__, str, result, str + 5);
    }
}

/**
 * @tc.name      : index_2100
 * @tc.desc      : Test finding a hyphen character.
 * @tc.level     : Level 1
 */
void index_2100(void)
{
    char *str = "hello-world";
    char *result = index(str, '-');
    if (result != str + 5) {
        t_error("%s: index(\"%s\", '-') returned %s, expected %s\n", __func__, str, result, str + 5);
    }
}

/**
 * @tc.name      : index_2200
 * @tc.desc      : Test finding a plus sign character.
 * @tc.level     : Level 1
 */
void index_2200(void)
{
    char *str = "hello+world";
    char *result = index(str, '+');
    if (result != str + 5) {
        t_error("%s: index(\"%s\", '+') returned %s, expected %s\n", __func__, str, result, str + 5);
    }
}

/**
 * @tc.name      : index_2300
 * @tc.desc      : Test finding an equals sign character.
 * @tc.level     : Level 1
 */
void index_2300(void)
{
    char *str = "hello=world";
    char *result = index(str, '=');
    if (result != str + 5) {
        t_error("%s: index(\"%s\", '=') returned %s, expected %s\n", __func__, str, result, str + 5);
    }
}

/**
 * @tc.name      : index_2400
 * @tc.desc      : Test finding an asterisk character.
 * @tc.level     : Level 1
 */
void index_2400(void)
{
    char *str = "hello*world";
    char *result = index(str, '*');
    if (result != str + 5) {
        t_error("%s: index(\"%s\", '*') returned %s, expected %s\n", __func__, str, result, str + 5);
    }
}

/**
 * @tc.name      : index_2500
 * @tc.desc      : Test finding a slash character.
 * @tc.level     : Level 1
 */
void index_2500(void)
{
    char *str = "hello/world";
    char *result = index(str, '/');
    if (result != str + 5) {
        t_error("%s: index(\"%s\", '/') returned %s, expected %s\n", __func__, str, result, str + 5);
    }
}

/**
 * @tc.name      : index_2600
 * @tc.desc      : Test finding a pipe character.
 * @tc.level     : Level 1
 */
void index_2600(void)
{
    char *str = "hello|world";
    char *result = index(str, '|');
    if (result != str + 5) {
        t_error("%s: index(\"%s\", '|') returned %s, expected %s\n", __func__, str, result, str + 5);
    }
}

/**
 * @tc.name      : index_2700
 * @tc.desc      : Test finding a caret character.
 * @tc.level     : Level 1
 */
void index_2700(void)
{
    char *str = "hello^world";
    char *result = index(str, '^');
    if (result != str + 5) {
        t_error("%s: index(\"%s\", '^') returned %s, expected %s\n", __func__, str, result, str + 5);
    }
}

/**
 * @tc.name      : index_2800
 * @tc.desc      : Test finding a tilde character.
 * @tc.level     : Level 1
 */
void index_2800(void)
{
    char *str = "hello~world";
    char *result = index(str, '~');
    if (result != str + 5) {
        t_error("%s: index(\"%s\", '~') returned %s, expected %s\n", __func__, str, result, str + 5);
    }
}

/**
 * @tc.name      : index_2900
 * @tc.desc      : Test finding a colon character.
 * @tc.level     : Level 1
 */
void index_2900(void)
{
    char *str = "hello:world";
    char *result = index(str, ':');
    if (result != str + 5) {
        t_error("%s: index(\"%s\", ':') returned %s, expected %s\n", __func__, str, result, str + 5);
    }
}

/**
 * @tc.name      : index_3000
 * @tc.desc      : Test finding a semicolon character.
 * @tc.level     : Level 1
 */
void index_3000(void)
{
    char *str = "hello;world";
    char *result = index(str, ';');
    if (result != str + 5) {
        t_error("%s: index(\"%s\", ';') returned %s, expected %s\n", __func__, str, result, str + 5);
    }
}

/**
 * @tc.name      : index_3100
 * @tc.desc      : Test finding a comma character.
 * @tc.level     : Level 1
 */
void index_3100(void)
{
    char *str = "hello,world";
    char *result = index(str, ',');
    if (result != str + 5) {
        t_error("%s: index(\"%s\", ',') returned %s, expected %s\n", __func__, str, result, str + 5);
    }
}

/**
 * @tc.name      : index_3200
 * @tc.desc      : Test finding a period character.
 * @tc.level     : Level 1
 */
void index_3200(void)
{
    char *str = "hello.world";
    char *result = index(str, '.');
    if (result != str + 5) {
        t_error("%s: index(\"%s\", '.') returned %s, expected %s\n", __func__, str, result, str + 5);
    }
}

/**
 * @tc.name      : index_3300
 * @tc.desc      : Test finding an exclamation mark character.
 * @tc.level     : Level 1
 */
void index_3300(void)
{
    char *str = "hello!world";
    char *result = index(str, '!');
    if (result != str + 5) {
        t_error("%s: index(\"%s\", '!') returned %s, expected %s\n", __func__, str, result, str + 5);
    }
}

/**
 * @tc.name      : index_3400
 * @tc.desc      : Test finding a question mark character.
 * @tc.level     : Level 1
 */
void index_3400(void)
{
    char *str = "hello?world";
    char *result = index(str, '?');
    if (result != str + 5) {
        t_error("%s: index(\"%s\", '?') returned %s, expected %s\n", __func__, str, result, str + 5);
    }
}

/**
 * @tc.name      : index_3500
 * @tc.desc      : Test finding an at symbol character.
 * @tc.level     : Level 1
 */
void index_3500(void)
{
    char *str = "hello@world";
    char *result = index(str, '@');
    if (result != str + 5) {
        t_error("%s: index(\"%s\", '@') returned %s, expected %s\n", __func__, str, result, str + 5);
    }
}

/**
 * @tc.name      : index_3600
 * @tc.desc      : Test finding a hash symbol character.
 * @tc.level     : Level 1
 */
void index_3600(void)
{
    char *str = "hello#world";
    char *result = index(str, '#');
    if (result != str + 5) {
        t_error("%s: index(\"%s\", '#') returned %s, expected %s\n", __func__, str, result, str + 5);
    }
}

/**
 * @tc.name      : index_3700
 * @tc.desc      : Test finding a dollar sign character.
 * @tc.level     : Level 1
 */
void index_3700(void)
{
    char *str = "hello$world";
    char *result = index(str, '$');
    if (result != str + 5) {
        t_error("%s: index(\"%s\", '$') returned %s, expected %s\n", __func__, str, result, str + 5);
    }
}

/**
 * @tc.name      : index_3800
 * @tc.desc      : Test finding a percent sign character.
 * @tc.level     : Level 1
 */
void index_3800(void)
{
    char *str = "hello%world";
    char *result = index(str, '%');
    if (result != str + 5) {
        t_error("%s: index(\"%s\", '%%') returned %s, expected %s\n", __func__, str, result, str + 5);
    }
}

/**
 * @tc.name      : index_3900
 * @tc.desc      : Test finding an ampersand character.
 * @tc.level     : Level 1
 */
void index_3900(void)
{
    char *str = "hello&world";
    char *result = index(str, '&');
    if (result != str + 5) {
        t_error("%s: index(\"%s\", '&') returned %s, expected %s\n", __func__, str, result, str + 5);
    }
}

/**
 * @tc.name      : index_4000
 * @tc.desc      : Test with a string containing only the target character.
 * @tc.level     : Level 1
 */
void index_4000(void)
{
    char *str = "aaaaa";
    char *result = index(str, 'a');
    if (result != str) {
        t_error("%s: index(\"%s\", 'a') returned %s, expected %s\n", __func__, str, result, str);
    }
}

/**
 * @tc.name      : index_4100
 * @tc.desc      : Test with a string where the target character is the first character.
 * @tc.level     : Level 1
 */
void index_4100(void)
{
    char *str = "ahello";
    char *result = index(str, 'a');
    if (result != str) {
        t_error("%s: index(\"%s\", 'a') returned %s, expected %s\n", __func__, str, result, str);
    }
}

/**
 * @tc.name      : index_4200
 * @tc.desc      : Test with a string where the target character is the last character.
 * @tc.level     : Level 1
 */
void index_4200(void)
{
    char *str = "helloa";
    char *result = index(str, 'a');
    if (result != str + 5) {
        t_error("%s: index(\"%s\", 'a') returned %s, expected %s\n", __func__, str, result, str + 5);
    }
}

/**
 * @tc.name      : index_4300
 * @tc.desc      : Test with a string where the target character appears multiple times, but only the first is found.
 * @tc.level     : Level 1
 */
void index_4300(void)
{
    char *str = "ababab";
    char *result = index(str, 'b');
    if (result != str + 1) {
        t_error("%s: index(\"%s\", 'b') returned %s, expected %s\n", __func__, str, result, str + 1);
    }
}

/**
 * @tc.name      : index_4400
 * @tc.desc      : Test with a string containing digits.
 * @tc.level     : Level 1
 */
void index_4400(void)
{
    char *str = "a1b2c3d4";
    char *result = index(str, '3');
    if (result != str + 4) {
        t_error("%s: index(\"%s\", '3') returned %s, expected %s\n", __func__, str, result, str + 4);
    }
}

/**
 * @tc.name      : index_4500
 * @tc.desc      : Test with a string containing uppercase letters.
 * @tc.level     : Level 1
 */
void index_4500(void)
{
    char *str = "helloWORLD";
    char *result = index(str, 'W');
    if (result != str + 5) {
        t_error("%s: index(\"%s\", 'W') returned %s, expected %s\n", __func__, str, result, str + 5);
    }
}

/**
 * @tc.name      : index_4600
 * @tc.desc      : Test with a string containing lowercase letters.
 * @tc.level     : Level 1
 */
void index_4600(void)
{
    char *str = "HELLOworld";
    char *result = index(str, 'w');
    if (result != str + 5) {
        t_error("%s: index(\"%s\", 'w') returned %s, expected %s\n", __func__, str, result, str + 5);
    }
}

/**
 * @tc.name      : index_4700
 * @tc.desc      : Test with a string containing a mix of letters, digits, and symbols.
 * @tc.level     : Level 1
 */
void index_4700(void)
{
    char *str = "a1!b2@c3#";
    char *result = index(str, '@');
    if (result != str + 4) {
        t_error("%s: index(\"%s\", '@') returned %s, expected %s\n", __func__, str, result, str + 4);
    }
}

/**
 * @tc.name      : index_4800
 * @tc.desc      : Test with a very long string.
 * @tc.level     : Level 2
 */
void index_4800(void)
{
    char str[1024];
    memset(str, 'x', 1023);
    str[512] = 'y'; // Place the target in the middle
    str[1023] = '\0';
    char *result = index(str, 'y');
    if (result != str + 512) {
        t_error("%s: index(long_str, 'y') returned %s, expected %s\n", __func__, result, str + 512);
    }
}

/**
 * @tc.name      : index_4900
 * @tc.desc      : Test with a string that is a single character.
 * @tc.level     : Level 2
 */
void index_4900(void)
{
    char *str = "a";
    char *result = index(str, 'a');
    if (result != str) {
        t_error("%s: index(\"%s\", 'a') returned %s, expected %s\n", __func__, str, result, str);
    }
}

/**
 * @tc.name      : index_5000
 * @tc.desc      : Test with a string that is a single character, but searching for a different one.
 * @tc.level     : Level 2
 */
void index_5000(void)
{
    char *str = "a";
    char *result = index(str, 'b');
    if (result != NULL) {
        t_error("%s: index(\"%s\", 'b') returned %s, expected NULL\n", __func__, str, result);
    }
}

/**
 * @tc.name      : index_5100
 * @tc.desc      : Test with a string that is empty and searching for the null terminator.
 * @tc.level     : Level 2
 */
void index_5100(void)
{
    char *str = "";
    char *result = index(str, '\0');
    if (result != str) {
        t_error("%s: index(\"\", '\\\\0') returned %s, expected %s\n", __func__, result, str);
    }
}

/**
 * @tc.name      : index_5200
 * @tc.desc      : Test with a string that starts with the null terminator.
 * @tc.level     : Level 2
 */
void index_5200(void)
{
    char *str = "\0hello";
    char *result = index(str, 'h');
    if (result != NULL) {
        t_error("%s: index(\"\\\\0hello\", 'h') returned %s, expected NULL\n", __func__, result);
    }
}

/**
 * @tc.name      : index_5300
 * @tc.desc      : Test with a string that contains multiple null terminators (though only the first is effective).
 * @tc.level     : Level 2
 */
void index_5300(void)
{
    char *str = "hello\0world";
    char *result = index(str, 'w');
    if (result != NULL) {
        t_error("%s: index(\"hello\\\\0world\", 'w') returned %s, expected NULL\n", __func__, result);
    }
}

/**
 * @tc.name      : index_5400
 * @tc.desc      : Test with a string that contains only null terminators.
 * @tc.level     : Level 2
 */
void index_5400(void)
{
    char *str = "\0\0\0";
    char *result = index(str, '\0');
    if (result != str) {
        t_error("%s: index(\"\\\\0\\\\0\\\\0\", '\\\\0') returned %s, expected %s\n", __func__, result, str);
    }
}

/**
 * @tc.name      : index_5500
 * @tc.desc      : Test with a string that contains only the target character repeated.
 * @tc.level     : Level 2
 */
void index_5500(void)
{
    char *str = "!!!!!!!!!!";
    char *result = index(str, '!');
    if (result != str) {
        t_error("%s: index(\"%s\", '!') returned %s, expected %s\n", __func__, str, result, str);
    }
}

/**
 * @tc.name      : index_5600
 * @tc.desc      : Test with a string that contains only characters other than the target.
 * @tc.level     : Level 2
 */
void index_5600(void)
{
    char *str = "aaaaaaaaaa";
    char *result = index(str, 'b');
    if (result != NULL) {
        t_error("%s: index(\"%s\", 'b') returned %s, expected NULL\n", __func__, str, result);
    }
}

/**
 * @tc.name      : index_5700
 * @tc.desc      : Test with a string that contains a mix of all printable ASCII characters.
 * @tc.level     : Level 2
 */
void index_5700(void)
{
    // String containing all printable ASCII characters from '!' to '~'
    char *str = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
    char *result = index(str, 'M');
    if (result != str + 41) { // 'M' is the 42nd character (0-indexed 41)
        t_error("%s: index(mixed_str, 'M') returned %s, expected %s\n", __func__, result, str + 41);
    }
}

/**
 * @tc.name      : index_5800
 * @tc.desc      : Test with a string that contains a mix of all printable ASCII characters, searching for a symbol.
 * @tc.level     : Level 2
 */
void index_5800(void)
{
    char *str = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
    char *result = index(str, '{');
    if (result != str + 91) { // '{' is the 92nd character (0-indexed 91)
        t_error("%s: index(mixed_str, '{') returned %s, expected %s\n", __func__, result, str + 91);
    }
}

/**
 * @tc.name      : index_5900
 * @tc.desc      : Test with a string that contains a mix of all printable ASCII characters, searching for a digit.
 * @tc.level     : Level 2
 */
void index_5900(void)
{
    char *str = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
    char *result = index(str, '5');
    if (result != str + 20) { // '5' is the 21st character (0-indexed 20)
        t_error("%s: index(mixed_str, '5') returned %s, expected %s\n", __func__, result, str + 20);
    }
}

/**
 * @tc.name      : index_6000
 * @tc.desc      : Test with a string that contains a mix of all printable ASCII characters, searching for a lowercase letter.
 * @tc.level     : Level 2
 */
void index_6000(void)
{
    char *str = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
    char *result = index(str, 'z');
    if (result != str + 89) { // 'z' is the 90th character (0-indexed 89)
        t_error("%s: index(mixed_str, 'z') returned %s, expected %s\n", __func__, result, str + 89);
    }
}

int main(int argc, char *argv[])
{
    index_0100();
    index_0200();
    index_0300();
    index_0400();
    index_0500();
    index_0600();
    index_0700();
    index_0800();
    index_0900();
    index_1000();
    index_1100();
    index_1200();
    index_1300();
    index_1400();
    index_1500();
    index_1600();
    index_1700();
    index_1800();
    index_1900();
    index_2000();
    index_2100();
    index_2200();
    index_2300();
    index_2400();
    index_2500();
    index_2600();
    index_2700();
    index_2800();
    index_2900();
    index_3000();
    index_3100();
    index_3200();
    index_3300();
    index_3400();
    index_3500();
    index_3600();
    index_3700();
    index_3800();
    index_3900();
    index_4000();
    index_4100();
    index_4200();
    index_4300();
    index_4400();
    index_4500();
    index_4600();
    index_4700();
    index_4800();
    index_4900();
    index_5000();
    index_5100();
    index_5200();
    index_5300();
    index_5400();
    index_5500();
    index_5600();
    index_5700();
    index_5800();
    index_5900();
    index_6000();

    return t_status;
}