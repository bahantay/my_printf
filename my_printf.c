/* 
**  for testing used this command:
**  gcc my_printf.c -o my_printf -Wall -Wextra -Werror -g3 -fsanitize=address
**  ./my_printf
*/

#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h> 

void my_putchar(char c)
{
    write(1, &c, 1);
}

int my_putstr(char* s)
{
    if (!s)
    {
        return 0;
    }
    int i = 0;
    while (s[i])
    {
        my_putchar(s[i++]);
    }
    return i;
}

char* itoa_unsigned_base(unsigned int number, int base)
{
    unsigned int original_n = number;
    unsigned int len = 0;
    char* result = NULL;
    unsigned int temp_n;

    original_n = original_n > 0 ? original_n : -original_n;
    if (number != 0)
    {
        while (number)
        {
            number = number / base;
            len++;
        }
        result = (char*)malloc(sizeof(char) * len + 1);
    }
    else
    {
        len = 1;
        result = (char*)malloc(sizeof(char) * (len + 1)); //2
    }
    if (!(result))
    {
        return NULL;
    }
    *(result + len) = '\0';
    len--;
    while (original_n > 0)
    {
        temp_n = original_n % base;
        if (temp_n < 10)
            *(result + len) = temp_n + '0';
        else
            *(result + len) = ((temp_n)-10 + 97);
        original_n = original_n / base;
        len--;
    }
    if (len == 0 && result[1] == '\0')
    {
        *(result) = '0';
    }
    return result;
}

char* itoa_signed_base(int number, int base)
{
    char* result;
    int len;
    int temp_n;
    int original_n = number;

    len = original_n > 0 ? 0 : 1;
    original_n = original_n > 0 ? original_n : -original_n;
    while (number)
    {
        number = number / base;
        len++;
    }
    if (!(result = (char*)malloc(sizeof(char) * len + 1)))
    {
        return NULL;
    }
    *(result + len) = '\0';
    len--;
    while (original_n > 0)
    {
        temp_n = original_n % base;
        if (base == 16 && original_n < 0)
        {
            temp_n = 15 - temp_n + 1;
        }
        if (temp_n < 10)
            *(result + len) = temp_n + '0';
        else
            *(result + len) = ((temp_n)-10 + 97);
        original_n = original_n / base;
        len--;
    }
    if (len == 0 && result[1] == '\0')
    {
        *(result + len) = '0';
    }
    else if (len == 0 && result[1] != '\0')
    {
        *(result + len) = '-';
    }
    return result;
}

char* pointer_to_string(unsigned long int number, int base)
{
    unsigned long int original_n = number;
    unsigned int len = 0;
    char* result = NULL;
    unsigned long int temp_n;

    original_n = original_n > 0 ? original_n : -(original_n);
    while (number)
    {
        number = number / base;
        len++;
    }
    if (!(result = (char*)malloc(sizeof(char) * len + 1)))
    {
        return NULL;
    }
    *(result + len) = '\0';
    len--;
    while (original_n > 0)
    {
        temp_n = original_n % base;
        if (temp_n < 10)
            *(result + len) = temp_n + '0';
        else
            *(result + len) = ((temp_n)-10 + 97);
        original_n = original_n / base;
        len--;
    }
    return result;
}

int my_printf(char* restrict format, ...)
{
    va_list args_p;
    char* sval;
    char* p;
    char* res_convert = NULL;
    intptr_t value_p;
    int len;
    int value;
    int written_chars = 0;

    va_start(args_p, format);
    for (p = format; *p; p++)
    {
        if (*p != '%')
        {
            my_putchar(*p);
            written_chars++;
            continue;
        }
        switch (*++p)
        {
            case 'd':
            {
                value = va_arg(args_p, int);
                res_convert = itoa_signed_base(value, 10);
                len = my_putstr(res_convert);
                written_chars += len;
                break;
            }
            case 'o':
            {
                value = va_arg(args_p, int);
                res_convert = itoa_unsigned_base(value, 8);
                len = my_putstr(res_convert);
                written_chars += len;
                break;
            }
            case 'u':
            {
                value = va_arg(args_p, int);
                res_convert = itoa_unsigned_base(value, 10);
                len = my_putstr(res_convert);
                written_chars += len;
                break;
            }
            case 'x':
            {
                value = va_arg(args_p, int);
                res_convert = itoa_unsigned_base(value, 16);
                len = my_putstr(res_convert);
                written_chars += len;
                break;
            }
            case 'c':
            {
                value = va_arg(args_p, int);
                my_putchar(value);
                written_chars++;
                break;
            }
            case 's':
            {
                sval = va_arg(args_p, char*);
                len = my_putstr(sval);
                written_chars += len;
                break;
            }
            case 'p':
            {
                value_p = va_arg(args_p, intptr_t);
                res_convert = pointer_to_string(value_p, 16);
                write(1, "0x", 2);
                len = my_putstr(res_convert);
                written_chars += len + 2;
                break;
            }
        }
        if (res_convert != NULL)
        {
            free(res_convert);
            res_convert = NULL;
        }
    }
    va_end(args_p);
    return written_chars;
}

int main()
{
    my_printf("my test for d: %d, %d, %d\n", 2048, -1337, 0);
    printf("test for d: %d, %d, %d\n", 2048, -1337, 0);
    my_printf("\n");

    my_printf("my test for o: %o, %o, %o, %o\n", 1000, -8, 8, 0);
    printf("test for o: %o, %o, %o, %o\n", 1000, -8, 8, 0);
    my_printf("\n");

    my_printf("my test for u: %u, %u, %u\n", 1337, 256, 0);
    printf("test for u: %u, %u, %u\n", 1337, 256, 0);
    my_printf("\n");

    my_printf("my test for x: %x, %x, %x, %x, %x\n", 14, 16, 3405723, -3405723, 0);
    printf("test for x: %x, %x, %x, %x, %x\n", 14, 16, 3405723, -3405723, 0);
    my_printf("\n");

    char ch1 = 'C';
    char ch2 = '4';
    char ch3 = '~';
    my_printf("my test for c: %c, %c, %c\n", ch1, ch2, ch3);
    printf("test for c: %c, %c, %c\n", ch1, ch2, ch3);
    my_printf("\n");

    int a = 10;
    int *b = &a;
    my_printf("my test for p: %p\n", b);
    printf("test for p: %p\n", b);
    my_printf("\n");

    char* s1 = "Hello, ";
    char* s2 = "World!";
    my_printf("my test for s: %s %s\n", s1, s2);
    printf("test for s: %s %s\n", s1, s2);
	return 0;
}