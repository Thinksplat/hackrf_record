#include <stdio.h>

int main(int argc, char *argv[])
{
    // Read 4k from stdin
    char buf[4096];
    while (true)
    {
        int n = fread(buf, 1, 4096, stdin);
        if (n <= 0)
        {
            break;
        }
        // convert each byte to unsigned
        for (int i = 0; i < n; i++)
        {
            buf[i] ^= 0x80;
        }
        // Write to stdout
        n = fwrite(buf, 1, n, stdout);
        if (n <= 0)
        {
            break;
        }
    }
    return 0;
}
