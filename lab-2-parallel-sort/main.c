#include <mpi.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

enum { TabSize = 8 };
#define var __auto_type
#define loop for(;;)

enum { Start, End };

int Argc;
char **Argv;
char *Filename;
void handle_arguments(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: %s <file with numbers>\n", argv[0]);
    exit(1);
  }

  Argc = argc;
  Argv = argv;
  Filename = argv[1];
}

int raw[TabSize];
void read_numbers(void) {
  var file = fopen(Filename, "r");
  for (int i = 0; i < TabSize; ++i) fscanf(file, "%d", &raw[i]);
  fclose(file);
}
void show_numbers(void) {
  for (int i = 0; i < TabSize; ++i) printf("%d\n", raw[i]);
}

int sorted[TabSize];
void read_sorted_numbers(void) {
  for (int i = 0; i < TabSize; ++i) sorted[i] = raw[i];
}
void show_sorted_numbers(void) {
  for (int i = 0; i < TabSize; ++i) printf("%d\n", sorted[i]);
}

enum {
    Main = 0,
    Leaf = 1,
};
int Size, Rank;
void initialize_mpi(void) {
  MPI_Init(&Argc, &Argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
  MPI_Comm_size(MPI_COMM_WORLD, &Size);
}
void finalize_mpi(void) {
  MPI_Finalize();
}
void present_rank(void) {
  printf("Hello from rank %d/%d\n", Rank + 1, Size);
}

void send_numbers_to_leaves(void) {
  for (int i = 1; i < TabSize; ++i) {
    printf("Sending %d to %d\n", raw[i], i);
    MPI_Send(&raw[i], 1, MPI_INT, i, Start, MPI_COMM_WORLD);
  }
}

void receive_numbers_from_leaves(void) {
  for (int i = 1; i < TabSize; ++i) {
    MPI_Recv(&sorted[i], 1, MPI_INT, i, Start, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Received %d from %d\n", sorted[i], i);
  }
}

void main_logic(void) {
  read_numbers();
  show_numbers();

  send_numbers_to_leaves();
  receive_numbers_from_leaves();

  show_sorted_numbers();
}

int Min = -1;
int K;
void handle_number(void) {
  if (Min == -1) Min = K;
  else {
    if (K < Min) Min = K;

    var next = (Rank + 1) % Rank;
    if (next == 0) ++next;
    MPI_Send(&Min, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
  }
}

#define loop for(;;)
void read_numbers_from_main(void) {
  loop {
    MPI_Recv(&K, 1, MPI_INT, MPI_ANY_SOURCE, Start, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("I '%d' Received %d\n", Rank, K);

    handle_number();
    break;
  }

  MPI_Send(&Min, 1, MPI_INT, Main, End, MPI_COMM_WORLD);
}

void leaf_logic(void) {
  read_numbers_from_main();
}

void perform_logic(void) {
  switch (Rank) {
    case Main:
      return main_logic();
    default:
      return leaf_logic();
  }
}

int main(int argc, char **argv) {
  handle_arguments(argc, argv);

  initialize_mpi();

  present_rank();
  perform_logic();

  finalize_mpi();

  exit(0);
}
