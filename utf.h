#include <stdio.h>

typedef int Rune;

enum {
UTFmax    = 6,       /* maximum bytes per rune */
Runeself  = 0x80,    /* rune and utf are equal (<) */
Runeerror = 0xFFFD,  /* decoding error in utf */
Runemax   = 0x10FFFF /* maximum rune value */
};

int runetochar(char *, const Rune *);
int chartorune(Rune *, const char *);
int charntorune(Rune *, const char *, size_t);
int runelen(Rune);
size_t runenlen(const Rune *, size_t);
int fullrune(const char *, size_t);
char *utfecpy(char *, char *, const char *);
size_t utflen(const char *);
size_t utfnlen(const char *, size_t);
char *utfrune(const char *, Rune);
char *utfrrune(const char *, Rune);
char *utfutf(const char *, const char *);

int isalnumrune(Rune);
int isalpharune(Rune);
int isblankrune(Rune);
int iscntrlrune(Rune);
int isdigitrune(Rune);
int isgraphrune(Rune);
int islowerrune(Rune);
int isprintrune(Rune);
int ispunctrune(Rune);
int isspacerune(Rune);
int istitlerune(Rune);
int isupperrune(Rune);
int isxdigitrune(Rune);

Rune tolowerrune(Rune);
Rune toupperrune(Rune);

int utftorunestr(const char*, Rune *);

int fgetrune(Rune *, FILE *);
int efgetrune(Rune *, FILE *, const char *);
int fputrune(const Rune *, FILE *);
int efputrune(const Rune *, FILE *, const char *);