#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

void merge(int *, int *, int, int, int);
void mergeSort(int *, int *, int, int);

int main(int argc, char** argv) {



	/*Se crea y se rellena el array */
	/*reempplazar el argv por el valor 99 999*/

	int n = atoi(argv[1]);
	int *original_array = malloc(n * sizeof(int));

	int c;

	printf("El array : ");
	for(c = 0; c < n; c++) {

		original_array[c] = rand() % n;
		printf("%d ", original_array[c]);

		}

	printf("\n");

	/*Inicializar el MPI */

	int world_rank;
	int world_size;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);


	/* Se divide el array el tamaños iguales */
	int size = n/world_size;

	/* Se envia cada sub array a cada proceso */
	int *sub_array = malloc(size * sizeof(int));
	MPI_Scatter(original_array, size, MPI_INT, sub_array, size, MPI_INT, 0, MPI_COMM_WORLD);

	/*Se realiza el merge sort a cada proceso */
	int *tmp_array = malloc(size * sizeof(int));
	mergeSort(sub_array, tmp_array, 0, (size - 1));

	/*Se juntan todos los arrays */
	int *sorted = NULL;
	if(world_rank == 0) {
		sorted = malloc(n * sizeof(int));
		}
	MPI_Gather(sub_array, size, MPI_INT, sorted, size, MPI_INT, 0, MPI_COMM_WORLD);

	/*Se hace el ultimo merge */
	if(world_rank == 0) {

		int *other_array = malloc(n * sizeof(int));

		mergeSort(sorted, other_array, 0, (n - 1));

		/* Se muestra el array ordenado */

		printf("El array ordenado: ");
		for(c = 0; c < n; c++) {

			printf("%d ", sorted[c]);

			}

		printf("\n");
		printf("\n");

		/*Se limpia la raiz */

		free(sorted);
		free(other_array);


	/*Se realiza la limpieza */

	free(original_array);
	free(sub_array);
	free(tmp_array);

	/*Se termina el MPI */

	MPI_Barrier(MPI_COMM_WORLD);

	MPI_Finalize();

	}

/*funcion merge */
void merge(int *a, int *b, int l, int m, int r) {

	int h, i, j, k;
	h = l;
	i = l;
	j = m + 1;

	while((h <= m) && (j <= r)) {
		if(a[h] <= a[j]) {
			b[i] = a[h];
			h++;
			}
		else {
			b[i] = a[j];
			j++;
			}
		i++;
		}
	if(m < h) {
		for(k = j; k <= r; k++) {
			b[i] = a[k];
			i++;
			}
		}
	else {
		for(k = h; k <= m; k++) {
			b[i] = a[k];
			i++;
			}
		}
	for(k = l; k <= r; k++) {
		a[k] = b[k];
		}
	}

/*funcion recursiva del merge */

void mergeSort(int *a, int *b, int l, int r) {
	int m;
	if(l < r) {
		m = (l + r)/2;
		mergeSort(a, b, l, m);
		mergeSort(a, b, (m + 1), r);
		merge(a, b, l, m, r);

		}

	}
