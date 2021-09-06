#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

static inline char *allocate_random_str(int len)
{
    char *str = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-";
    char *res = (char *)calloc(1, len + 1);
    int str_len = strlen(str);

    FILE *fp = fopen("/dev/urandom", "r");
    unsigned char ch;
    if(fp)
    {
        for(int n = 0; n < len; n++)
        {
            fread(&ch, 1, 1, fp);
            res[n] = str[ch % (str_len - 1)];
        }
        fclose(fp);
    }
    else
    {
        printf("ERROR\n");
        exit(0);
    }
    return res;
}

int read_until_alnum(char *buf, int len, char delim)
{
    char ch;
    int idx = 0;
    if(len < 0)
        return 0;
    while(idx < len)
    {
        int res = read(0, &ch, 1);
        if(res != 1 || ch == delim)
            break;
        else if(!isalnum(ch) && ch != '@' && ch != '.')
            continue;
        buf[idx] = ch;
        idx += res;
    }
    if(idx == len)
        buf[idx - 1] = 0;
    else
        buf[idx] = 0;
    return idx;
}

int read_until(char *buf, int len, char delim)
{
    char ch;
    int idx = 0;
    if(len < 0)
        return 0;
    while(idx < len)
    {
        int res = read(0, &ch, 1);
        if(res != 1 || ch == delim)
            break;
        buf[idx] = ch;
        idx += res;
    }
    if(idx == len)
        buf[idx - 1] = 0;
    else
        buf[idx] = 0;
    return idx;
}

void make_laby()
{
    char buf[0x100];
    char *ptr;
    int w, h;
    char *fname = allocate_random_str(64);
    struct stat buffer;

    while(stat(fname, &buffer) == 0)
    {
        free(fname);
        fname = allocate_random_str(64);
    }

    FILE *fp = fopen(fname, "w");

    printf("NAME> ");
    memset(buf, 0, 0x100);
    read_until_alnum(buf, 0xff, '\n');
    fprintf(fp, "%s ", buf);

    printf("EMAIL> ");
    memset(buf, 0, 0x100);
    read_until_alnum(buf, 0xff, '\n');
    fprintf(fp, "%s ", buf);

    printf("WIDTH> ");
    memset(buf, 0, 0x100);
    read_until(buf, 0xff, '\n');
    w = atoi(buf);
    fprintf(fp, "%d ", w);

    printf("HEIGHT> ");
    memset(buf, 0, 0x100);
    read_until(buf, 0xff, '\n');
    h = atoi(buf);
    fprintf(fp, "%d ", h);

    printf("INFO> ");
    memset(buf, 0, 0x100);
    read_until(buf, 0xff, '\n');
    fprintf(fp, "%s\n", buf);

    char ch;
    for(int i = 0; i < h; i ++)
    {
        for(int j = 0; j < w; j++)
        {
            if(read(0, &ch, 1) != 1)
            {
                puts("read error!");
                exit(0);
            }
            fwrite(&ch, 1, 1, fp);
        }
        fwrite("\n", 1, 1, fp);
    }
    fclose(fp);

    printf("SAVE_KEY is %s\n", fname);
    free(fname);
}

void show_info()
{
    char buf[0x100];
    printf("KEY>");
    memset(buf, 0, 0x100);
    read_until(buf, 0xff, '\n');

    FILE *fp = fopen(buf, "r");
    if(fp)
    {
        while(!feof(fp))
        {
            char ch;
            fread(&ch, 1, 1, fp);
            printf("%c", ch);
        }
        fclose(fp);
    }
    else
    {
        puts("Nope:)");
    }

}

int load_laby(char ***d_arr_ptr, char *fname, int *w, int *h, int *w_off, int *h_off)
{
    FILE *fp = fopen(fname, "r");
    if(fp)
    {
        char name[0x100], email[0x100], info[0x100];
        fscanf(fp, "%s %s %d %d %s\n", name, email, w, h, info);
        printf("You will play %s's laby!\n", name);

        *d_arr_ptr = calloc(sizeof(char *), *h);
        char **d_arr = *d_arr_ptr;
        for(int i = 0; i < *h; i++)
        {
            d_arr[i] = calloc(1, (*w) + 1);
            char c = 0;
            int idx = 0;
            while (!feof(fp))
            {
                fread(&c, 1, 1, fp);
                if(c == '\n')
                    break;
                else if(c == 'S')
                {
                    *w_off = idx;
                    *h_off = i;
                }
                d_arr[i][idx] = c;
                idx += 1;
            }
        }
        fclose(fp);
        return 1;
    }
    return 0;

}

void render_laby(char **d_arr, int w, int h, int w_off, int h_off)
{
    printf("\e[2J\e[H");
    for(int a = 0; a < w+2; a++)
        printf("##");

    printf("\n");
    for(int _h = 0; _h < h; _h++)
    {
        printf("##");
        for(int _w = 0 ; _w < w; _w++)
        {
            if(w_off == _w && h_off == _h)
            {
                printf("><");
            }
            else if(d_arr[_h][_w] == 'E')
            {
                printf("[]");
            }
            else if(d_arr[_h][_w] == 'W')
            {
                printf("##");
            }
            else
            {
                printf("  ");
            }
        }
        printf("##");
        printf("\n");
    }
    for(int a = 0; a < w+2; a++)
        printf("##");
    printf("\n");
}

void win()
{
    char buf[0x100];
    puts("Wow you're solver!!!");
    printf("Mame length>");
    memset(buf, 0, 0x100);
    read_until(buf, 0xff, '\n');
    int len = atoi(buf);

    printf("name>");
    char *winner = malloc(len+1);
    read_until(winner, len+1, '\n');

    printf("Comments Length>");
    memset(buf, 0, 0x100);
    read_until(buf, 0xff, '\n');
    len = atoi(buf);
    char *comment = malloc(len + 1);
    printf("comment> ");
    read_until(comment, len+1, '\n');

    puts("==== Solver ====");
    printf("1. %s %s", winner, comment);
    free(comment);
    free(winner);
}

void test_laby()
{
    char buf[0x100];
    char **d_arr;
    int w, h, w_off = 0, h_off = 0;
    char ch[4];

    printf("KEY>");
    memset(buf, 0, 0x100);
    read_until(buf, 0xff, '\n');
    if(load_laby(&d_arr, buf, &w, &h, &w_off, &h_off))
    {
        while(d_arr[h_off][w_off] != 'E')
        {
            render_laby(d_arr, w, h, w_off, h_off);
            if(read_until(ch, 2, '\n') > 0)
            {
                switch(ch[0])
                {
                    case 'W':
                        if (h_off-1 >= 0 && h_off-1 < h && d_arr[h_off-1][w_off] != 'W')
                        {
                            h_off -= 1;
                        }
                        break;
                    case 'A':
                        if (w_off-1 >= 0 && w_off-1 < w && d_arr[h_off][w_off-1] != 'W')
                        {
                            w_off -= 1;
                        }
                        break;
                    case 'S':
                        if (h_off+1 >= 0 && h_off+1 < h && d_arr[h_off+1][w_off] != 'W')
                        {
                            h_off += 1;
                        }
                        break;
                    case 'D':
                        if (w_off+1 >= 0 && w_off+1 < w && d_arr[h_off][w_off+1] != 'W')
                        {
                            w_off += 1;
                        }
                        break;
                    case 'E':
                        puts("Exit~~");
                        goto CLEANUP;
                        return;
                    default:
                        continue;
                }

            }
        }
        win();

CLEANUP:
        for(int i = 0; i < w; i++)
        {
            free(d_arr[i]);
        }
        free(d_arr);
        return;
    }
    puts("Nope!");
}

void init_prob(int time)
{
    setvbuf(stdout, 0, _IONBF, 0);
    alarm(time);
    if(chdir("labys") && (mkdir("labys", 0x1C0u) || chdir("labys")))
        exit(0);
}

int menu()
{
    puts("1. make_labyrinth");
    puts("2. do_labyrinth");
    puts("3. show_info");
    puts("4. exit");
    return 1;
}

int main(int argc, char **argv)
{
    init_prob(argc == 1 ? 40 : atoi(argv[1]));

    puts("Welcome to labyrinth management program");
    while(menu())
    {
        printf("> ");
        char buffer[0x100];
        memset(buffer, 0, 0x100);
        read_until(buffer, 0xff, '\n');
        switch(atoi(buffer))
        {
            case 1:
                make_laby();
                break;
            case 2:
                test_laby();
                break;
            case 3:
                show_info();
                break;
            case 4:
                puts("bye:P");
                exit(0);
            default:
                puts("UNKNOWN OPTION.");
        }
    }
}
