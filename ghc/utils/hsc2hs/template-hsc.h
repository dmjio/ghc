#include <HsFFI.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#ifndef offsetof
#define offsetof(t, f) ((size_t) &((t *)0)->f)
#endif

static int hsc_options_started = 0;

static void hsc_option (const char *s)
{
    if (!hsc_options_started)
    {
        printf ("{-# OPTIONS");
        hsc_options_started = 1;
    }
    printf (" %s", s);
}

static void hsc_end_options (void)
{
    if (hsc_options_started) printf (" #-}\n");
}

#define hsc_const(x)                        \
    if ((x) < 0)                            \
        printf ("%ld", (long)(x));          \
    else                                    \
        printf ("%lu", (unsigned long)(x));

#define hsc_const_str(x)                                          \
    {                                                             \
        const char *s = (x);                                      \
        printf ("\"");                                            \
        while (*s != '\0')                                        \
        {                                                         \
            if (*s == '"' || *s == '\\')                          \
                printf ("\\%c", *s);                              \
            else if (*s >= 0x20 && *s <= 0x7E)                    \
                printf ("%c", *s);                                \
            else                                                  \
                printf ("\\%d%s",                                 \
                        (unsigned char) *s,                       \
                        s[1] >= '0' && s[1] <= '9' ? "\\&" : ""); \
            s++;                                                  \
        }                                                         \
        printf ("\"");                                            \
    }

#define hsc_type(t)                                         \
    if ((t)(int)(t)1.4 == (t)1.4)                           \
        printf ("%s%d",                                     \
                (t)(-1) < (t)0 ? "Int" : "Word",            \
                sizeof (t) * 8);                            \
    else                                                    \
        printf ("%s",                                       \
                sizeof (t) >  sizeof (double) ? "LDouble" : \
                sizeof (t) == sizeof (double) ? "Double"  : \
                "Float");

#define hsc_peek(t, f) \
    printf ("(\\hsc_ptr -> peekByteOff hsc_ptr %ld)", (long) offsetof (t, f));

#define hsc_poke(t, f) \
    printf ("(\\hsc_ptr -> pokeByteOff hsc_ptr %ld)", (long) offsetof (t, f));

#define hsc_ptr(t, f) \
    printf ("(\\hsc_ptr -> hsc_ptr `plusPtr` %ld)", (long) offsetof (t, f));

