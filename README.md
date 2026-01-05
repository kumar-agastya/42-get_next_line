====================================================================
GET NEXT LINE – COMPLETE PROJECT EXPLANATION
Author: agastya-kumar
====================================================================

This document explains in detail how the Get Next Line project works.
It covers all three files used in the mandatory part of the project:

1. get_next_line.h
2. get_next_line_utils.c
3. get_next_line.c

Every function and every important line of code is explained in detail.
The goal of this document is to demonstrate full understanding of the
implementation and the algorithm used in the project.

====================================================================
1. get_next_line.h
====================================================================

This file is the header file of the project. Its purpose is to:
- declare the get_next_line function
- declare helper functions
- define BUFFER_SIZE if it is not already defined
- include required standard libraries

------------------------------------------------------------
Header guard
------------------------------------------------------------

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

The header guard prevents multiple inclusions of this file.
If the header is included more than once, the compiler will
ignore subsequent inclusions, avoiding redefinition errors.

------------------------------------------------------------
Standard library includes
------------------------------------------------------------

# include <unistd.h>

This header is required for the read() system call.
read() is used to read data from a file descriptor.

# include <stdlib.h>

This header is required for:
- malloc()
- free()

Dynamic memory allocation is essential for this project.

------------------------------------------------------------
BUFFER_SIZE definition
------------------------------------------------------------

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

BUFFER_SIZE controls how many bytes are read at a time from the file.
If BUFFER_SIZE is not provided at compilation time, it defaults to 42.

This allows the program to be tested with different buffer sizes.

------------------------------------------------------------
Function prototypes
------------------------------------------------------------

char *get_next_line(int fd);

This is the main function required by the subject.
It returns the next line read from the file descriptor fd.

size_t ft_strlen(const char *s);

Returns the length of a string.

char *ft_strchr(const char *s, int c);

Searches for a character in a string.

char *ft_strjoin(char *s1, char *s2);

Concatenates two strings and returns a newly allocated string.
The first string is freed inside this function.

char *ft_substr(char const *s, unsigned int start, size_t len);

Extracts a substring from a string.

------------------------------------------------------------
End of header guard
------------------------------------------------------------

#endif

====================================================================
2. get_next_line_utils.c
====================================================================

This file contains helper functions used by get_next_line.
These functions replace common libc or libft utilities, since
the project does not allow using external libraries.

------------------------------------------------------------
ft_strlen
------------------------------------------------------------

size_t ft_strlen(const char *s)

This function calculates the length of a string.

if (!s)
    return (0);

If the input string is NULL, the length is considered 0.
This prevents segmentation faults.

while (s[i])
    i++;

The loop increments i until the null terminator is reached.

return (i);

The total number of characters is returned.

------------------------------------------------------------
ft_strchr
------------------------------------------------------------

char *ft_strchr(const char *s, int c)

This function searches for the first occurrence of character c
in the string s.

if (!s)
    return (NULL);

If the string is NULL, there is nothing to search.

while (s[i])
{
    if (s[i] == (char)c)
        return ((char *)&s[i]);
}

The loop checks each character.
If the target character is found, a pointer to that position
is returned.

if ((char)c == '\0')
    return ((char *)&s[i]);

If the character searched is '\0', a pointer to the end of the
string is returned.

return (NULL);

If the character is not found, NULL is returned.

------------------------------------------------------------
ft_substr
------------------------------------------------------------

char *ft_substr(char const *s, unsigned int start, size_t len)

This function extracts a substring from string s, starting at
index start and copying at most len characters.

if (!s || start >= ft_strlen(s))
    return (NULL);

If the string is NULL or the start index is out of bounds,
there is nothing to extract.

str = malloc(len + 1);

Memory is allocated for the substring plus a null terminator.

while (i < len && s[start + i])
{
    str[i] = s[start + i];
    i++;
}

Characters are copied one by one from the original string.

str[i] = '\0';

The new string is null-terminated.

return (str);

The allocated substring is returned.

------------------------------------------------------------
ft_strjoin
------------------------------------------------------------

char *ft_strjoin(char *s1, char *s2)

This function concatenates s1 and s2 into a new string.

str = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);

Memory is allocated for both strings plus a null terminator.

while (s1 && s1[i])
{
    str[i] = s1[i];
    i++;
}

The first string is copied.

while (s2 && s2[j])
{
    str[i + j] = s2[j];
    j++;
}

The second string is appended.

str[i + j] = '\0';

The new string is null-terminated.

free(s1);

The first string is freed because it is no longer needed.
This avoids memory leaks.

return (str);

The concatenated string is returned.

====================================================================
3. get_next_line.c
====================================================================

This file contains the core logic of the project.
It manages reading from the file descriptor and returning
one line per function call.

------------------------------------------------------------
read_and_store
------------------------------------------------------------

static char *read_and_store(int fd, char *store)

This function reads from the file descriptor and appends
data to the static store until a newline is found or EOF is reached.

buf = malloc(BUFFER_SIZE + 1);

A temporary buffer is allocated for reading.

bytes = 1;

bytes is initialized to a positive value so the loop starts.

while (bytes > 0 && !ft_strchr(store, '\n'))

The loop continues while:
- the last read was successful
- no newline is found in the stored data

bytes = read(fd, buf, BUFFER_SIZE);

read() reads up to BUFFER_SIZE bytes from the file.

if (bytes < 0)

If read() fails, memory is freed and NULL is returned.

buf[bytes] = '\0';

The buffer is null-terminated so it can be treated as a string.

store = ft_strjoin(store, buf);

The new data is appended to the existing stored data.

free(buf);

The temporary buffer is freed.

return (store);

The updated stored data is returned.

------------------------------------------------------------
extract_line
------------------------------------------------------------

static char *extract_line(char *store)

This function extracts a single line from the stored data.

if (!store || !store[0])
    return (NULL);

If there is no stored data, there is no line to extract.

while (store[i] && store[i] != '\n')
    i++;

The loop finds the end of the line.

if (store[i] == '\n')
    i++;

If a newline exists, it is included in the line.

return (ft_substr(store, 0, i));

The line is extracted and returned.

------------------------------------------------------------
clean_store
------------------------------------------------------------

static char *clean_store(char *store)

This function removes the extracted line from the stored data.

while (store[i] && store[i] != '\n')
    i++;

The loop finds the end of the line.

if (!store[i])

If no newline exists, the entire store was used and can be freed.

new = ft_substr(store, i + 1, ft_strlen(store) - i - 1);

The remaining part of the string is saved for the next call.

free(store);

The old store is freed.

return (new);

The leftover data is returned.

------------------------------------------------------------
get_next_line
------------------------------------------------------------

char *get_next_line(int fd)

This is the main function called by the user.

static char *store;

The static variable preserves unread data between calls.

if (fd < 0 || BUFFER_SIZE <= 0)
    return (NULL);

Invalid input is rejected.

store = read_and_store(fd, store);

Data is read and appended to the store.

if (!store)
    return (NULL);

If reading fails or EOF is reached with no data, NULL is returned.

line = extract_line(store);

A line is extracted from the store.

store = clean_store(store);

The store is updated to keep only unread data.

return (line);

The extracted line is returned to the caller.

====================================================================
CONCLUSION
====================================================================

This implementation follows the project requirements closely.
It uses a static buffer to handle partial reads and ensures that
each call to get_next_line returns exactly one line.

The code prioritizes clarity, correctness, and adherence to the
42 subject over unnecessary complexity or over-defensive behavior.

====================================================================
END OF DOCUMENT
====================================================================
