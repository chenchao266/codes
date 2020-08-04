const char *str = "hello";

extern "C" {
    void *malloc(int);
    char *strcpy(char *, const char *);
    int printf(const char *, ...);
    int exit(int);
    int strlen(const char *);
}

int main()
{   /* C language program */
    char *s = (char *)malloc(strlen(str)+1);
    strcpy(s, str);
    printf("%s, world \n", s);
    return 0;
}
