#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int int_arg(char *arg, const char *name) {
    int result;
    int r1 = sscanf(arg, "%d", &result);
    if (r1 != 1) {
        printf("Error parsing %s '%s'.\n", name, arg);
        exit(1);
    } else {
        //fprintf(stderr, "parsed %s = %d (from %s)\n", name, result, arg);
    }
    //printf("%s = %d\n", name, result);
    return result;
}


unsigned long rand48() {
  return ((unsigned long) random() << (48-31)) ^ random();
}

unsigned long convert_scale(unsigned long n, unsigned long maxn, unsigned long maxy) {
    // Compute basic result
    unsigned long prod = maxy * n;
    unsigned long result = prod / maxn;

    // Add remainder for rounding accuracy
    if (prod % maxn > (maxn >> 1)) {
        result++;
    }

    return result;
}


int main(int argc, char *argv[])
{
    char *dev_name;
    int blocksize, blockcount, ops;

    void *block;
    FILE *dev;
    struct timeval tv;
    struct timeval start_t;
    struct timeval end_t;
    int i;
    double elapsed_s;

    // Read args
    if (argc < 5) {
        printf("Usage: %s <device> <block-size-bytes> <block-count> <number-of-ops>\n", argv[0]);
        return 3;
    }
    dev_name = argv[1];
    blocksize = int_arg(argv[2], "block size");
    blockcount = int_arg(argv[3], "block count");
    ops = int_arg(argv[4], "ops");

    // Init
    block = malloc(blocksize);
    if (block == NULL) {
        printf("malloc failed\n");
        return 1;
    }
    dev = fopen(dev_name, "r");
    if (dev == NULL) {
        printf("fopen failed for %s\n", dev_name);
        return 1;
    }

    // Ramdon seed
    gettimeofday(&tv,(void*)0);
    srandom(tv.tv_sec ^ tv.tv_usec);

    // Start time
    gettimeofday(&start_t,(void*)0);

    // Do ops
    i = 0;
    while (i++ < ops) {
        long blk_n = convert_scale(rand48(), 1L<<47, blockcount);
        fprintf(stderr, "%ld\n", blk_n);
        if (fseek(dev, blk_n * blocksize, SEEK_SET) != 0) {
            printf("fseek failed.\n");
            exit(1);
        }
        if (fread(block, blocksize, 1, dev) != 1) {
            printf("fread failed.\n");
            exit(1);
        }
    }

    // End time
    gettimeofday(&end_t,(void*)0);

    elapsed_s = (end_t.tv_sec-start_t.tv_sec) + ((double)end_t.tv_usec - start_t.tv_usec)/1000000.0;
    printf("%.2f\n", ops / elapsed_s);

    // Cleanup
    fclose(dev);
    free(block);
    return 0;
}

