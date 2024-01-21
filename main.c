#include <stdio.h>
#include <stdlib.h>

void getData(int *pDimension, float ***pA, float **pX, float **pB);

float **allocateMatrix(int dimension);

float *allocateVector(int dimension);

void logError(const char *string);

void displayResult(int dimension, float **matrixA, float *vectorX, float *vectorB);

void showVector(float *vector, int dimension);

int main() {
    printf("Trouver x par la methode de Cholesky\n");
    // Donnees
    int dimension;
    float **A = NULL,       /// matrice carree du systeme
        *b = NULL,          /// vecteur second membre
        *x = NULL;          /// vecteur solution

    getData(&dimension, &A, &x, &b);

    // Traitement

    // Result
    displayResult(dimension, A, x, b);

    return 0;
}

void displayResult(int dimension, float **matrixA, float *vectorX, float *vectorB) {
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

void showVector(float *vector, int dimension) {
    for (int i = 0; i < dimension; ++i) {
        printf("%g\n", vector[i]);
    }
}

void getData(int *pDimension, float ***pA, float **pX, float **pB) {
    /// open file if exists
    char filePath[] = "../data.txt";                                // must begin with '../'
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

        float **matrixA = allocateMatrix(dimension);                // load matrix A from file
        for (int i = 0; i < dimension; ++i) {
            for (int j = 0; j < dimension; ++j)
                fscanf(file, "%f", &matrixA[i][j]);
        }

        float *vectorB = allocateVector(dimension),                // allocate vector b -- second member
        *vectorX = allocateVector(dimension);                  // allocate vector x -- solution
        for (int i = 0; i < dimension; ++i) {
            fscanf(file, "%f", &vectorB[i]);
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

float **allocateMatrix(int dimension) {
    float **A = NULL;
    A = malloc(dimension * sizeof(float*));
    for (int i = 0; i < dimension; ++i) {
        A[i] = allocateVector(dimension);
    }
    if (A == NULL) logError("Cannot allocate matrix!");
    return A;
}

float *allocateVector(int dimension) {
    float *vector = NULL;
    vector = malloc(dimension * sizeof (float));
    if (vector == NULL) logError("Cannot allocate vector!");
    return vector;
}

void logError(const char *string) {
    printf("%s\n", string);
    exit(2);
}
