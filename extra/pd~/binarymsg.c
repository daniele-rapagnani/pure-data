#ifdef MAX
#define A_PDFLOAT 1
#define A_PDSYMBOL 2
#define A_PDSEMI 4
#else
#define A_PDFLOAT A_FLOAT
#define A_PDSYMBOL A_SYMBOL
#define A_PDSEMI A_SEMI
#endif

static void pd_tilde_putfloat(float f, FILE *fd)
{
    sys_fs_putc(A_PDFLOAT, fd);
    sys_fs_fwrite(&f, sizeof(f), 1, fd);
}

static void pd_tilde_putsymbol(t_symbol *s, FILE *fd)
{
    const char *sp = s->s_name;
    sys_fs_putc(A_PDSYMBOL, fd);
    do
        sys_fs_putc(*sp, fd);
    while (*sp++);
}

static void pd_tilde_putsemi(FILE *fd)
{
    sys_fs_putc(A_PDSEMI, fd);
}

static int pd_tilde_getatom(t_atom *ap, FILE *fd)
{
    char buf[MAXPDSTRING];
    while (1)
    {
        int type = sys_fs_getc(fd), fill;
        float f;
        switch (type)
        {
        case EOF:
            return (0);
        case A_PDSEMI:
            SETSEMI(ap);
            return (1);
        case A_PDFLOAT:
            if (sys_fs_fread(&f, sizeof(f), 1, fd) >= 1)
            {
                SETFLOAT(ap, f);
                return (1);
            }
            else return (0);
        case A_PDSYMBOL:
            for (fill = 0; fill < MAXPDSTRING; fill++)
            {
                int c = sys_fs_getc(fd);
                if (c == EOF)
                    return (0);
                else buf[fill] = c;
                if (!c)
                {
                    SETSYMBOL(ap, gensym(buf));
                    return (1);
                }
            }
            return (0);
        }
    }
}
