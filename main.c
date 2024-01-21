#include <stdio.h>
#include <math.h>
#include <stdlib.h>

void getData(int *pDimension, double ***pA, double **pX, double **pB);

double **allocateMatrix(int dimension);

double *allocateVector(int dimension);

void logError(const char *string);

void displayResult(int dimension, double **matrixA, double *vectorX, double *vectorB);

void showVector(double *vector, int dimension);

void cholesky(int dimension, double **matrixA, double *vectorX, double *vectorB);

void factorisation(int dimension, double **matrixA);

void solveTriangleInf(int dimension, double **matrixA, double *vectorB);

void solveTriangleSup(int dimension, double **matrixA, double *vectorX, const double *vectorB);

int main() {
    printf("Trouver x par la methode de Cholesky\n");
    // Data
    int dimension;
    double **A = NULL,       /// square matrix of the system
        *b = NULL,          /// vector second member
        *x = NULL;          /// vector solution

    getData(&dimension, &A, &x, &b);
    displayResult(dimension, A, x, b);
    printf("---\n");

    // Process
    cholesky(dimension, A, x, b);

    // Result
    displayResult(dimension, A, x, b);

    return 0;
}

void cholesky(int dimension, double **matrixA, double *vectorX, double *vectorB) {
    factorisation(dimension, matrixA);
    solveTriangleInf(dimension, matrixA, vectorB);
    solveTriangleSup(dimension, matrixA, vectorX, vectorB);
}

void solveTriangleSup(int dimension, double **matrixA, double *vectorX, const double *vectorB) {
    int i, j;
    for (i = dimension - 1; i >= 0; --i) {
        double sum = 0;
        for (j = i + 1; j < dimension; ++j) {
            sum += matrixA[j][i] * vectorX[j];
        }
        vectorX[i] = ( vectorB[i] - sum ) / matrixA[i][i];
    }
}

void solveTriangleInf(int dimension, double **matrixA, double *vectorB) {
    double sum;
    for (int i = 0; i < dimension; ++i) {
        sum = 0;
        for (int j = 0; j < i; ++j) {
            sum += matrixA[i][j] * vectorB[j];
        }
        vectorB[i] = ( vectorB[i] - sum ) / matrixA[i][i];
    }
}

void factorisation(int dimension, double **matrixA) {
    double temp1, temp2;
    int i, j, k;

    for (i = 0; i < dimension; ++i) {
        temp1 = 0;

        for (j = 0; j < i; ++j) {
            temp2 = 0;

            for (k = 0; k <= j; ++k) {
                temp1 += matrixA[i][k] * matrixA[j][k];
                temp2 += matrixA[i][k] * matrixA[i][k];
            }
            matrixA[i][j] = ( matrixA[i][j] - temp1 ) / matrixA[j][j];
        }
        matrixA[i][i] = sqrt( matrixA[i][i] - temp2 );
    }
}

void displayResult(int dimension, double **matrixA, double *vectorX, double *vectorB) {
    printf("The matrix \n");
    for (int i = 0; i < dimension; ++i) {
        for (int j = 0; j < dimension; ++j)
            printf("%g\t", matrixA[i][j]);
        printf("\n");
    }

    printf("The second member \n");
    showVector(vectorB, dimension);

    printf("The solution \n");
    showVector(vectorX, dimension);
}

void showVector(double *vector, int dimension) {
    for (int i = 0; i < dimension; ++i) {
        printf("%g\n", vector[i]);
    }
}

void getData(int *pDimension, double ***pA, double **pX, double **pB) {
    /// open file if exists
    char filePath[] = "../data2.txt";                                // must begin with '../'
    FILE *file = NULL;
    file = fopen(filePath, "r");

    if (file == NULL) {
        printf("File %s not found!\n", filePath);
        exit(2);
    }
    else {
        printf("File %s found!\n", filePath);

        /// load data from the file
        int dimension;                                              // load dimension from file
        fscanf(file, "%d", &dimension);

        double **matrixA = allocateMatrix(dimension);                // load matrix A from file
        for (int i = 0; i < dimension; ++i) {
            for (int j = 0; j < dimension; ++j)
                fscanf(file, "%lf", &matrixA[i][j]);
        }

        double *vectorB = allocateVector(dimension),                // allocate vector b -- second member
        *vectorX = allocateVector(dimension);                  // allocate vector x -- solution
        for (int i = 0; i < dimension; ++i) {
            fscanf(file, "%lf", &vectorB[i]);
            vectorX[i] = 0;
        }

        /// close file
        fclose(file);

        /// send data from the file to main()
        *pDimension = dimension;
        *pA = matrixA;
        *pX = vectorX;
        *pB = vectorB;
    }
}

double **allocateMatrix(int dimension) {
    double **A = NULL;
    A = malloc(dimension * sizeof(double*));
    for (int i = 0; i < dimension; ++i) {
        A[i] = allocateVector(dimension);
    }
    if (A == NULL) logError("Cannot allocate matrix!");
    return A;
}

double *allocateVector(int dimension) {
    double *vector = NULL;
    vector = malloc(dimension * sizeof (double));
    if (vector == NULL) logError("Cannot allocate vector!");
    return vector;
}

void logError(const char *string) {
    printf("%s\n", string);
    exit(2);
}
