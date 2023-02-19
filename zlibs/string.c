#include <syscall.h>
#include <i_iolib.h>
#include <i_string.h>
#include <type.h>
#include <stdlib.h>

void init_func();

int main() {
    init_func();
    return 0;
}

void init_func() {
    c_kprint("Init of the string lib !\n");
}

char *basename(const char *path) {
    register const char *s;
    register const char *p;

    p = s = path;

    while (*s) {
        if (*s++ == '/') {
            p = s;
        }
    }

    return (char *) p;
}

void bcopy(const void *s2, void *s1, size_t n) {
    register char *s;
    register const char *p;

    s = s1;
    p = s2;
    if (p >= s) {
        while (n) {
            *s++ = *p++;
            --n;
        }
    } else {
        while (n) {
            --n;
            s[n] = p[n];
        }
    }
}

void bzero(void *s, size_t n) {
    register unsigned char *p = s;

    while (n) {
        *p++ = 0;
        --n;
    }
}


char *dirname(char *path)
{
    static const char null_or_empty_or_noslash[] = ".";
    register char *s;
    register char *last;
    char *first;

    last = s = path;

    if (s != NULL) {

    LOOP:
        while (*s && (*s != '/')) ++s;
        first = s;
        while (*s == '/') ++s;
        if (*s) {
            last = first;
            goto LOOP;
        }

        if (last == path) {
            if (*last != '/') {
                goto DOT;
            }
            if ((*++last == '/') && (last[1] == 0)) {
                ++last;
            }
        }
        *last = 0;
        return path;
    }
 DOT:
    return (char *) null_or_empty_or_noslash;
}

int ffs(int i) {
    fsprint("ffs not implemented yet, WHY DO YOU USE IT ?\n");
    return 0;
}

int ffsll(long long int i) {
    fsprint("ffsll not implemented yet, WHY DO YOU USE IT ?\n");
    return 0;
}

void *memccpy(void * __restrict s1, const void * __restrict s2, int c, size_t n) {
    register char *r1 = s1;
    register const char *r2 = s2;

    while (n-- && (((unsigned char)(*r1++ = *r2++)) != ((unsigned char) c)));

    return (n == (size_t) -1) ? NULL : r1;
}

Wvoid *memchr(const Wvoid *s, Wint c, size_t n) {
    fsprint("Wmemchr not implemented yet, WHY DO YOU USE IT ?\n");
    return 0;
}

int memcmp(const Wvoid *s1, const Wvoid *s2, size_t n) {
    register const Wuchar *r1 = (const Wuchar *) s1;
    register const Wuchar *r2 = (const Wuchar *) s2;

    int r = 0;

    while (n-- && ((r = ((int)(*r1++)) - *r2++) == 0));
    return r;
}

void *memcpy(void *source, void *dest, size_t nbytes) {
    char *src = (char *) source;
    char *dst = (char *) dest;
    for (unsigned int i = 0; i < nbytes; i++) {
        dst[i] = src[i];
    }
    return dest;
}

void *memmem(const void *haystack, size_t haystacklen,
             const void *needle, size_t needlelen) {
    register const char *ph;
    register const char *pn;
    const char *plast;
    size_t n;

    if (needlelen == 0) {
        return (void *) haystack;
    }

    if (haystacklen >= needlelen) {
        ph = (const char *) haystack;
        pn = (const char *) needle;
        plast = ph + (haystacklen - needlelen);

        do {
            n = 0;
            while (ph[n] == pn[n]) {
                if (++n == needlelen) {
                    return (void *) ph;
                }
            }
        } while (++ph <= plast);
    }

    return NULL;
}

Wvoid *memmove(Wvoid *s1, const Wvoid *s2, size_t n) {
    register Wchar *s = (Wchar *) s1;
    register const Wchar *p = (const Wchar *) s2;

    if (p >= s) {
        while (n) {
            *s++ = *p++;
            --n;
        }
    } else {
        while (n) {
            --n;
            s[n] = p[n];
        }
    }

    return s1;
}

Wvoid *mempcpy(Wvoid * __restrict s1, const Wvoid * __restrict s2, size_t n) {
   register Wchar *r1 = s1;
   register const Wchar *r2 = s2;

   while (n) {
      *r1++ = *r2++;
      --n;
   }

   return r1;
}

void *memrchr(const void *s, int c, size_t n) {
    register const unsigned char *r;

    r = ((unsigned char *)s) + ((size_t) n);

    while (n) {
        if (*--r == ((unsigned char)c)) {
            return (void *) r;    /* silence the warning */
        }
        --n;
    }

    return NULL;
}

Wvoid *memset(Wvoid *s, Wint c, size_t n) {
    register Wuchar *p = (Wuchar *) s;

    while (n) {
        *p++ = (Wuchar) c;
        --n;
    }

    return s;
}

void psignal(int signum, register const char *message) {
    fsprint("psignal not implemented yet, WHY DO YOU USE IT ?\n");
}

void *rawmemchr(const void *s, int c) {
    register const unsigned char *r = s;

    while (*r != ((unsigned char)c)) ++r;

    return (void *) r;    /* silence the warning */
}

Wchar *stpcpy(register Wchar * __restrict s1, const Wchar * __restrict s2) {
    while ( (*s1++ = *s2++) != 0 );

    return s1 - 1;
}

Wchar *stpncpy(register Wchar * __restrict s1,
                register const Wchar * __restrict s2,
                size_t n) {
    Wchar *s = s1;
    const Wchar *p = s2;

    while (n) {
        if ((*s = *s2) != 0) s2++; /* Need to fill tail with 0s. */
        ++s;
        --n;
    }
    return s1 + (s2 - p);
}

int strcasecmp(register const Wchar *s1, register const Wchar *s2) {
    fsprint("strcasecmp not implemented yet, WHY DO YOU USE IT ?\n");
    return 0;
}

int strcasecmp_l(register const Wchar *s1, register const Wchar *s2, locale_t loc) {
    fsprint("strcasecmp_l not implemented yet, WHY DO YOU USE IT ?\n");
    return 0;
}

char *strcasestr(const char *s1, const char *s2) {
    fsprint("strcasestr not implemented yet, WHY DO YOU USE IT ?\n");
    return 0;
}

Wchar *strcat(Wchar * __restrict s1, register const Wchar * __restrict s2) {
    size_t i,j;
    for (i = 0; s1[i] != '\0'; i++)
        ;
    for (j = 0; s2[j] != '\0'; j++)
        s1[i+j] = s2[j];
    s1[i+j] = '\0';
    return s1;
}

Wchar *strchr(const char *p, int ch) {
    char c;
	c = ch;
	for (;; ++p) {
		if (*p == c)
			return ((char *)p);
		if (*p == '\0')
			return (NULL);
	}
}

Wchar *strchrnul(register const Wchar *s, Wint c) {
    fsprint("Wstrchrnul not implemented yet, WHY DO YOU USE IT ?\n");
    return NULL;
}
size_t strlen(const Wchar *s);
int strcmp(register const Wchar *s1, register const Wchar *s2) {
    if (strlen(s1) != strlen(s2)) {
        return -1;
    }
    int i;
    for (i = 0; s1[i] == s2[i]; i++) {
        if (s1[i] == '\0') return 0;
    }
    return s1[i] - s2[i];
}

Wchar *strcpy(Wchar * __restrict s1, const Wchar * __restrict s2) {
    int i;
    for (i = 0; s2[i] != '\0'; ++i) {
        s1[i] = s2[i];
    }
    s1[i] = '\0';
    return s1;
}

size_t strcspn(const Wchar *s1, const Wchar *s2) {
    fsprint("strcspn not implemented yet, WHY DO YOU USE IT ?\n");
    return 0;
}

Wchar *strdup(register const Wchar *s1) {
    fsprint("strdup not implemented yet, WHY DO YOU USE IT ?\n");
    return NULL;
}

char *strerror(int errnum) {
    fsprint("strerror not implemented yet, WHY DO YOU USE IT ?\n");
    return NULL;
}

size_t strlcat(register char *__restrict dst,
               register const char *__restrict src,
               size_t n) {
    size_t len;
    char dummy[1];

    len = 0;

    while (1) {
        if (len >= n) {
            dst = dummy;
            break;
        }
        if (!*dst) {
            break;
        }
        ++dst;
        ++len;
    }

    while ((*dst = *src) != 0) {
        if (++len < n) {
            ++dst;
        }
        ++src;
    }

    return len;
}

size_t strlcpy(register Wchar *__restrict dst,
                  register const Wchar *__restrict src,
                  size_t n) {
    const Wchar *src0 = src;
    Wchar dummy[1];

    if (!n) {
        dst = dummy;
    } else {
        --n;
    }

    while ((*dst = *src) != 0) {
        if (n) {
            --n;
            ++dst;
        }
        ++src;
    }

    return src - src0;
}

size_t strlen(const Wchar *s) {
    register const Wchar *p;

    for (p=s ; *p ; p++);

    return p - s;
}

int strncasecmp(register const Wchar *s1, register const Wchar *s2, size_t n) {
    fsprint("strncasecmp not implemented yet, WHY DO YOU USE IT ?\n");
    return 0;
}


int strncasecmp_l(register const Wchar *s1, register const Wchar *s2, size_t n, locale_t loc) {
    fsprint("strncasecmp_l not implemented yet, WHY DO YOU USE IT ?\n");
    return 0;
}

Wchar *strncat(Wchar * __restrict s1, register const Wchar * __restrict s2,
                size_t n) {
    register Wchar *s = s1;

    while (*s++);
    --s;
    while (n && ((*s = *s2++) != 0)) {
        --n;
        ++s;
    }
    *s = 0;

    return s1;
}

int strncmp(register const Wchar *s1, register const Wchar *s2, size_t n) {
    fsprint("strncmp not implemented yet, WHY DO YOU USE IT ?\n");
    return 0;
}


Wchar *strncpy(Wchar * __restrict s1, register const Wchar * __restrict s2,
               size_t n) {
    register Wchar *s = s1;

    while (n) {
        if ((*s = *s2) != 0) s2++; /* Need to fill tail with 0s. */
        ++s;
        --n;
    }

    return s1;
}

size_t strnlen(const Wchar *s, size_t max);
char *strndup(register const char *s1, size_t n) {
    fsprint("strndup not implemented yet, WHY DO YOU USE IT ?\n");
    return NULL;
}

size_t strnlen(const Wchar *s, size_t max) {
    register const Wchar *p = s;

    while (max && *p) {
        ++p;
        --max;
    }

    return p - s;
}

Wchar *strpbrk(const Wchar *s1, const Wchar *s2)
{
    register const Wchar *s;
    register const Wchar *p;

    for ( s=s1 ; *s ; s++ ) {
        for ( p=s2 ; *p ; p++ ) {
            if (*p == *s) return (Wchar *) s; /* silence the warning */
        }
    }
    return NULL;
}

Wchar *strrchr(register const  Wchar *s, Wint c)
{
    register const Wchar *p;

    p = NULL;
    do {
        if (*s == (Wchar) c) {
            p = s;
        }
    } while (*s++);

    return (Wchar *) p;            /* silence the warning */
}

char *strsep(char ** __restrict s1, const char * __restrict s2) {
    fsprint("strsep not implemented yet, WHY DO YOU USE IT ?\n");
    return NULL;
}

char *strsignal(int signum) {
    fsprint("strsignal not implemented yet, WHY DO YOU USE IT ?\n");
    return NULL;
}

size_t strspn(const Wchar *s1, const Wchar *s2) {
    fsprint("Wstrspn not implemented yet, WHY DO YOU USE IT ?\n");
    return 0;
}

Wchar *strstr(const Wchar *s1, const Wchar *s2) {
    fsprint("Wstrstr not implemented yet, WHY DO YOU USE IT ?\n");
    return NULL;
}

Wchar *strtok(Wchar * __restrict s1, const Wchar * __restrict s2) {
    fsprint("Wstrtok not implemented yet, WHY DO YOU USE IT ?\n");
    return NULL;
}

Wchar *strtok_r(Wchar * __restrict s1, const Wchar * __restrict s2,
                 Wchar ** __restrict next_start) {
    fsprint("Wstrtok_r not implemented yet, WHY DO YOU USE IT ?\n");
    return NULL;
}

int strverscmp(const char *s1, const char *s2) {
    fsprint("strverscmp not implemented yet, WHY DO YOU USE IT ?\n");
    return 0;
}
