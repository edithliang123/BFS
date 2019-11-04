#include<stdio.h>
#include<stdlib.h>
#include"Graph.h"
#include"List.h"

int main(int argc, char* argv[]) {
	FILE* in;
	FILE* out;
	List path;
	Graph A;
	int count = 1;
	int n, x, y;

	// check command line for correct number of arguments 
	if (argc != 3) {
		printf("Usage: %s input-file output-file\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// open input file for reading 
	if ((in = fopen(argv[1], "r")) == NULL) {
		printf("Unable to read from file %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	// open output file for writing 
	if ((out = fopen(argv[2], "w")) == NULL) {
		printf("Unable to write to file %s\n", argv[2]);
		exit(EXIT_FAILURE);
	}

	while (!feof(in)) {
		fscanf(in, "%d", &n);
		A = newGraph(n);
		for (int i = 0; i < n; i++) {
			fscanf(in, "%d %d", &x, &y);
			addEdge(A, x, y);
		}
		fscanf(in, "%d %d", &x, &y);
		if (x == 0 && y == 0) {
			printGraph(out, A);
		}
		fscanf(in, "%d %d", &x, &y);
		while (x != 0 && y != 0) {
			BFS(A, x);
			if (getDis(A, y) != INF) {
				fprintf(out, "\nThe distance from %d to %d is %d\n", x, y, getDist(A, y));
				getPath(path, A, y);
				fprintf(out, "A shortest %d-%d path is: ", x, y);
				printList(out, path);
			}
			else {
				fprintf(out, "\nThe distance from %d to %d is infinity\n", x, y);
				fprintf(out, "No %d-%d path exists", x, y);
			}
		}
		break;
	}

	freeGraph(&A);
	freeList(&path);

	fclose(in);
	fclose(out);

	return 0;
}