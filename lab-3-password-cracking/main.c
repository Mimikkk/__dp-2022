#define __XOPEN_SOURCE
#define _XOPEN_SOURCE
#include <crypt.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <mpi.h>

/* 4 literowe
aa5UYq6trT5u.
bahAZ9Hk7SCf6
ddoo3WocSpthU
jkmD2RlhoMyuA
zzm4NUIIb7VIk
kkv864igyJC9o

5 literowe
aaSPfLTmjh3fU

6 literowe
aaLTdQr7DyHuU 
*/

#define SIZE 4
const char stro[] = "aa5UYq6trT5u.";

/* odkomentować dla wersji z mpi */

pthread_t thread_message_handler;
void *msg_handler(void *parameter) {
  printf("START!\n");
  char password[SIZE + 1] = {0};
  MPI_Status status;
  MPI_Recv(password, SIZE, MPI_BYTE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
  printf("Hasło to: %s\n", password);
  MPI_Abort(MPI_COMM_WORLD, 0);
  exit(0);
}

#define var __auto_type

int main(int argc, char **argv) {
  int size, rank;
  int provided;
  MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
  if (provided != MPI_THREAD_MULTIPLE) {
    perror("Brak wsparcia dla wątków");
    MPI_Finalize();
    exit(0);
  }

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (rank == 0) {
    pthread_create(&thread_message_handler, 0, msg_handler, 0);
  }


  char salt[3] = {stro[0], stro[1], '\0'};

  char *cmp = calloc(SIZE + 1, sizeof(char));
  cmp[0] = 'a' + rank;

  for (int j = 'a'; j <= 'z'; ++j) {
    for (int k = 'a'; k <= 'z'; ++k) {
      for (int n = 'a'; n <= 'z'; ++n) {
        cmp = (char[]) {cmp[0], j, k, n, '\0'};
        var decoded = crypt(cmp, salt);
        if ((strcmp(decoded, stro)) == 0) {
          MPI_Send(cmp, SIZE + 1, MPI_BYTE, 0, 0, MPI_COMM_WORLD);
          MPI_Finalize();
          exit(0);
        }
      }
    }
  }

  if (rank == 0) pthread_join(thread_message_handler, 0);
  MPI_Finalize();
}
