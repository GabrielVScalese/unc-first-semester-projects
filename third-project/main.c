#include <stdio.h>

#define RGB_LENGTH  3 // Numero de posicoes num RGB
#define COLORS_TYPE_LENGTH  5 // Numero de cores a serem analisadas
#define MAX_INPUT_LENGTH    1000 // Maximo de caracteres a serem lidos numa entrada

// Insere valor zero em todas as posicoes da matriz
void resetMatrix (int line, int col, int matrix[line][col])
{
    for (int i = 0; i < line; i++)
        for (int j = 0; j < col; j++)
            matrix[i][j] = 0;
}

// Obtem indice de cada cor
int getColorIndex (int rgbValues[])
{
    // 0 0 0
    if (rgbValues[0] == 0 && rgbValues[1] == 0 && rgbValues[2] == 0)
        return 0;

    // 255 0 0 
    if (rgbValues[0] == 255 && rgbValues[1] == 0 && rgbValues[2] == 0)
        return 1;
    
    // 0 255 0
    if (rgbValues[0] == 0 && rgbValues[1] == 255 && rgbValues[2] == 0)
        return 2;
    
    // 0 0 255
    if (rgbValues[0] == 0 && rgbValues[1] == 0 && rgbValues[2] == 255)
        return 3;
    
    // 255 255 0
    if (rgbValues[0] == 255 && rgbValues[1] == 255 && rgbValues[2] == 0)
        return 4;
}

// Obtem frequencia de uma cor na matriz
int getColorFrequencyInMatrix (int colorIndex, int line, int col, int matrix[line][col])
{  
    int colorFrequency = 0;
    for (int i = 0; i < line; i++)
        if (matrix[i][colorIndex] != 0)
            colorFrequency += matrix[i][colorIndex];
    
    return colorFrequency;
}

// Obtem o indice da cor com maior frequencia na matriz
int getMaxColorFrequencyIndexInMatrix (int line, int col, int matrix[line][col])
{
    int maxValue = -1;
    int colorsFrenquencies[] = {getColorFrequencyInMatrix(0, line, col, matrix), getColorFrequencyInMatrix(1, line, col, matrix), 
                                getColorFrequencyInMatrix(2, line, col, matrix), getColorFrequencyInMatrix(3, line, col, matrix), 
                                getColorFrequencyInMatrix(4, line, col, matrix)};

    int maxColorFrequencyIndex;
    for (int i = 0; i < COLORS_TYPE_LENGTH; i++)
        if (colorsFrenquencies[i] > maxValue)
        {
            maxValue = colorsFrenquencies[i];
            maxColorFrequencyIndex = i;
        }
            
    return maxColorFrequencyIndex;
}

void setPixelColor (int lineCont, int colCont, int value, int matrixLine, int matrixCol, int matrix[matrixLine][matrixCol])
{
    matrix[lineCont][colCont] = value;
    matrix[lineCont][colCont + 1] = value;
    matrix[lineCont][colCont + 2] = value;
}

void printMatrix (int line, int col, int matrix[line][col])
{
    for (int i = 0; i < line; i++)
    {
        for (int j = 0; j < col; j++)
            printf("%d ", matrix[i][j]);
        
        printf("\n");
    }
}

int main ()
{
    char fileFormat[2]; // Formato do arquivo
    int width; // Comprimento da imagem em pixels
    int height; // Altura da imagem em pixels
    int intensity; // Intervalo de intensidade usado no RGB

    // Obtendo primeira linha
    scanf("%c%c %d %d %d\n", &fileFormat[0], &fileFormat[1], &width, &height, &intensity);

    // Matriz que ira armazenar as cores RGB de cada pixel
    const int RGB_MATRIX_LINE = height;
    const int RGB_MATRIX_COL = width * RGB_LENGTH;
    int rgbMatrix[height][width * RGB_LENGTH];

    // Matriz RGB comprimida
    const int COMPRESSED_MATRIX_LINE = height;
    const int COMPRESSED_MATRIX_COL = COLORS_TYPE_LENGTH;
    int compressedMatrix[COMPRESSED_MATRIX_LINE][COMPRESSED_MATRIX_COL];
    resetMatrix(COMPRESSED_MATRIX_LINE, COMPRESSED_MATRIX_COL, compressedMatrix); // Garantir que a matriz comece zerada

    for (int i = 0; i < RGB_MATRIX_LINE; i++)
    {   
        char rgbValuesInput[MAX_INPUT_LENGTH]; // Entrada para cada linha da matriz
        fgets(rgbValuesInput, sizeof(rgbValuesInput), stdin);

        int rgbMatrixColCount = 0; // Contador de coluna da matriz RGB

        int pixelCont = 0; // Conta quantos pixels foram lidos
        int onePixelRgb[RGB_LENGTH]; // Vetor que guarda os valores RGB de um pixel
        int onePixelRgbCont = 0; // Conta quantos elementos foram inseridos no vetor onePixelRgb
        
        for (int j = 0; pixelCont != width;)
        {    
            if (rgbValuesInput[j] == '0' || rgbValuesInput[j] == ' ')
            {
                if (rgbValuesInput[j] == '0')
                {
                    rgbMatrix[i][rgbMatrixColCount++] = 0;
                    onePixelRgb[onePixelRgbCont++] =  0;
                }
                    
                j++;
            }
            else if (rgbValuesInput[j] == '2')
            {
                rgbMatrix[i][rgbMatrixColCount++] = 255;
                onePixelRgb[onePixelRgbCont++] = 255;
                j += RGB_LENGTH;
            }               

            if (onePixelRgbCont == RGB_LENGTH) // Um pixel foi lido
            {
                onePixelRgbCont = 0;
                compressedMatrix[i][getColorIndex(onePixelRgb)]++;
                pixelCont++;
            }  
        }
    }

    int maxColorFrequencyIndex = getMaxColorFrequencyIndexInMatrix(COMPRESSED_MATRIX_LINE, COMPRESSED_MATRIX_COL, compressedMatrix);

    // Transformando matriz RGB para matriz RGB contrastada

    // 0 0 0 tem destaque 
    if (maxColorFrequencyIndex == 0)
        for (int i = 0; i < RGB_MATRIX_LINE; i++)
            for (int j = 0; j < RGB_MATRIX_COL; j += 3)
                if (rgbMatrix[i][j] != 0 && rgbMatrix[i + 1][j + 1] != 0 && rgbMatrix[i + 2][j + 2] != 0)
                    setPixelColor(i, j, 255, RGB_MATRIX_LINE, RGB_MATRIX_COL, rgbMatrix);
    
    // 255 0 0 tem destaque
    if (maxColorFrequencyIndex == 1)
        for (int i = 0; i < RGB_MATRIX_LINE; i++)
            for (int j = 0; j < RGB_MATRIX_COL; j += 3)
                if (rgbMatrix[i][j] == 255 && rgbMatrix[i][j + 1] == 0 && rgbMatrix[i][j + 2] == 0)
                  setPixelColor(i, j, 0, RGB_MATRIX_LINE, RGB_MATRIX_COL, rgbMatrix);
                else
                    setPixelColor(i, j, 255, RGB_MATRIX_LINE, RGB_MATRIX_COL, rgbMatrix);
    
    // 0 255 0 tem destaque
    if (maxColorFrequencyIndex == 2)
        for (int i = 0; i < RGB_MATRIX_LINE; i++)
            for (int j = 0; j < RGB_MATRIX_COL; j += 3)
                if (rgbMatrix[i][j] == 0 && rgbMatrix[i][j + 1] == 255 && rgbMatrix[i][j + 2] == 0)
                    setPixelColor(i, j, 0, RGB_MATRIX_LINE, RGB_MATRIX_COL, rgbMatrix);
                else
                    setPixelColor(i, j, 255, RGB_MATRIX_LINE, RGB_MATRIX_COL, rgbMatrix);

    // 0 0 255 tem destaque
    if (maxColorFrequencyIndex == 3)
        for (int i = 0; i < RGB_MATRIX_LINE; i++)
            for (int j = 0; j < RGB_MATRIX_COL; j += 3)
                if (rgbMatrix[i][j] == 0 && rgbMatrix[i][j + 1] == 0 && rgbMatrix[i][j + 2] == 255)
                    setPixelColor(i, j, 0, RGB_MATRIX_LINE, RGB_MATRIX_COL, rgbMatrix);
                else
                    setPixelColor(i, j, 255, RGB_MATRIX_LINE, RGB_MATRIX_COL, rgbMatrix);
    
    // 255 255 0 tem destaque
    if (maxColorFrequencyIndex == 4)
        for (int i = 0; i < RGB_MATRIX_LINE; i++)
            for (int j = 0; j < RGB_MATRIX_COL; j += 3)
                if (rgbMatrix[i][j] == 255 && rgbMatrix[i][j + 1] == 255 && rgbMatrix[i][j + 2] == 0)
                    setPixelColor(i, j, 0, RGB_MATRIX_LINE, RGB_MATRIX_COL, rgbMatrix);
                else
                    setPixelColor(i, j, 255, RGB_MATRIX_LINE, RGB_MATRIX_COL, rgbMatrix);

    // Printando matriz RGB comprimida
    printMatrix(COMPRESSED_MATRIX_LINE, COMPRESSED_MATRIX_COL, compressedMatrix);

    // Printando matriz RGB contrastada
    printMatrix(RGB_MATRIX_LINE, RGB_MATRIX_COL, rgbMatrix);

    return 0;
}