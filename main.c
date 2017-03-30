#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

int dataI[8][5];
int dataT[64][115];

// 读取文件 & 解析数据
void readFileAndParse(char fileFlag) {
    // 读取文件
    char fileName[20] = "I-sets.txt";
    if (fileFlag == 'T') {
        fileName[0] = 'T';
    }
    FILE * fp;
    char tempStr[100000], tempChar;
    int strSize = 0;
    if((fp=fopen(fileName,"r"))==NULL) {
        printf("file cannot be opened\n");
        exit(1);
    }
    while(!feof(fp)) {
        tempChar = fgetc(fp);
        if (tempChar == ' ') {
            tempStr[strSize] = '0';
        } else {
            tempStr[strSize] = tempChar;
        }
        strSize++;
    }
    fclose(fp);
    // 解析数据
    int i = 0, j = 0, x, y;
    int a, b, c;
    for (x = 0; x < strSize; x++) {
        if (tempStr[x] == '{'){
            for (y = x; tempStr[y] != '}'; y = y+4, j++) {
                a = tempStr[y+1] - '0';
                b = tempStr[y+2] - '0';
                c = tempStr[y+3] - '0';
                if (fileFlag == 'I') {
                    dataI[i][j] = 100 * a + 10 * b + c;
                } else {
                    dataT[i][j] = 100 * a + 10 * b + c;
                }
            }
            i++;
            j = 0;
            x = y;
        }
    }
}

void solve(int indexI) {
    int count = 0;
    int SupSet[64];
    for (int T = 0; T < 64; T++) {
        int flag[5] = {0};
        for (int i = 0; i < 5; i++) {
            for (int t = 0; t < 115; t++) {
                if (dataI[indexI][i] == dataT[T][t]) {
                    flag[i] = 1;
                }
            }
        }
        int sum = 0;
        for (int i = 0; i < 5; i++) {
            sum = sum + flag[i];
        }
        if (sum == 5) {
            SupSet[count] = T;
            count++;
        }
    }
    printf("SupSet(%d)={", indexI);
    for (int i = 0; i < count-1; ++i) {
        printf("%d,", SupSet[i]);
    }
    if (count!=0) {
        printf("%d", SupSet[count-1]);
    }
    printf("};count=%d\n", count);
}

int main (int argc, char **argv) {
    // 读取文件 & 解析数据
    readFileAndParse('I');
    readFileAndParse('T');
    // MPI
    int rank, size;
    MPI_Init (&argc, &argv);  /* starts MPI */
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);    /* get current process id */
    MPI_Comm_size (MPI_COMM_WORLD, &size);    /* get number of processes */
    // printf( "Hello world from process %d of %d\n", rank, size );
    clock_t timing = clock();
    if (size == 1) {
        if (rank == 0) {
            solve(0);
            solve(1);
            solve(2);
            solve(3);
            solve(4);
            solve(5);
            solve(6);
            solve(7);
        }
    } else if (size == 2) {
        if (rank == 0) {
            solve(0);
            solve(2);
            solve(4);
            solve(6);
        } else if (rank == 1) {
            solve(1);
            solve(3);
            solve(5);
            solve(7);
        }
    } else if (size == 4) {
        if (rank == 0) {
            solve(0);
            solve(4);
        } else if (rank == 1) {
            solve(1);
            solve(5);
        } else if (rank == 2) {
            solve(2);
            solve(6);
        } else if (rank == 3) {
            solve(3);
            solve(7);
        }
    } else if (size == 8) {
        if (rank == 0) {
            solve(0);
        } else if (rank == 1) {
            solve(1);
        } else if (rank == 2) {
            solve(2);
        } else if (rank == 3) {
            solve(3);
        } else if (rank == 4) {
            solve(4);
        } else if (rank == 5) {
            solve(5);
        } else if (rank == 6) {
            solve(6);
        } else if (rank == 7) {
            solve(7);
        }
    }
    double time = (double)(clock() - timing) / CLOCKS_PER_SEC;
    printf("Time cost: %lf, when using %d nodes\n", time, size);
    MPI_Finalize();
    return 0;
}
