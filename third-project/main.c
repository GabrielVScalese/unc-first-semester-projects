#include <stdio.h>

void resetMatrix (int line, int col, int matrix[line][col])
{
    for (int i = 0; i < line; i++)
        for (int j = 0; j < col; j++)
            matrix[i][j] = 0;
}

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

int getColorFrequencyInMatrix (int colorIndex, int line, int col, int matrix[line][col])
{  
    int colorFrequency = 0;
    for (int i = 0; i < line; i++)
        if (matrix[i][colorIndex] != 0)
            colorFrequency += matrix[i][colorIndex];
    
    return colorFrequency;
}

int getMaxColorFrequencyIndexInMatrix (int line, int col, int matrix[line][col])
{
    int maxValue = -1;
    int colorsFrenquencies[] = {getColorFrequencyInMatrix(0, line, col, matrix), getColorFrequencyInMatrix(1, line, col, matrix), 
                                getColorFrequencyInMatrix(2, line, col, matrix), getColorFrequencyInMatrix(3, line, col, matrix), 
                                getColorFrequencyInMatrix(4, line, col, matrix)};

    int maxColorFrequencyIndex;
    for (int i = 0; i < 5; i++)
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

int main ()
{
    // Obtendo primeira linha
    char fileFormat[2];
    int width;
    int height;
    int intensity;
    scanf("%c%c %d %d %d\n", &fileFormat[0], &fileFormat[1], &width, &height, &intensity); // Testar depois com %s

    // Matriz que ira armazenar as cores RGB de cada pixel
    const int RGB_MATRIX_LINE = height;
    const int RGB_MATRIX_COL = width * 3;
    int rgbMatrix[height][width * 3];

    // Matriz comprimida a partir de matriz RGB
    const int COMPRESSED_MATRIX_LINE = height;
    const int COMPRESSED_MATRIX_COL = 5;
    int compressesdMatrix[COMPRESSED_MATRIX_LINE][COMPRESSED_MATRIX_COL];
    resetMatrix(height, 5, compressesdMatrix);

    for (int i = 0; i < height; i++)
    {   
        char rgbValuesInput[1000];
        fgets(rgbValuesInput, sizeof(rgbValuesInput), stdin);

        int rgbMatrixColCount = 0;

        int pixelCont = 0;
        int onePixelRgbCont = 0;
        int onePixelRgb[3];

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
                j += 3;
            }               

            if (onePixelRgbCont == 3)
            {
                onePixelRgbCont = 0;
                compressesdMatrix[i][getColorIndex(onePixelRgb)]++;
                pixelCont++;
            }  
        }
    }

    // Transformando matriz para dar constraste
    int maxColorFrequencyIndex = getMaxColorFrequencyIndexInMatrix(COMPRESSED_MATRIX_LINE, COMPRESSED_MATRIX_COL, compressesdMatrix);

    if (maxColorFrequencyIndex == 0)
        for (int i = 0; i < RGB_MATRIX_LINE; i++)
            for (int j = 0; j < RGB_MATRIX_COL; j += 3)
                if (rgbMatrix[i][j] != 0 && rgbMatrix[i + 1][j + 1] != 0 && rgbMatrix[i + 2][j + 2] != 0)
                    setPixelColor(i, j, 255, RGB_MATRIX_LINE, RGB_MATRIX_COL, rgbMatrix);
    
    if (maxColorFrequencyIndex == 1)
        for (int i = 0; i < RGB_MATRIX_LINE; i++)
            for (int j = 0; j < RGB_MATRIX_COL; j += 3)
            {
                if (rgbMatrix[i][j] == 255 && rgbMatrix[i][j + 1] == 0 && rgbMatrix[i][j + 2] == 0)
                  setPixelColor(i, j, 0, RGB_MATRIX_LINE, RGB_MATRIX_COL, rgbMatrix);
                else
                    setPixelColor(i, j, 255, RGB_MATRIX_LINE, RGB_MATRIX_COL, rgbMatrix);
            }
    
    if (maxColorFrequencyIndex == 2)
        for (int i = 0; i < RGB_MATRIX_LINE; i++)
            for (int j = 0; j < RGB_MATRIX_COL; j += 3)
            {
                if (rgbMatrix[i][j] == 0 && rgbMatrix[i][j + 1] == 255 && rgbMatrix[i][j + 2] == 0)
                    setPixelColor(i, j, 0, RGB_MATRIX_LINE, RGB_MATRIX_COL, rgbMatrix);
                else
                    setPixelColor(i, j, 255, RGB_MATRIX_LINE, RGB_MATRIX_COL, rgbMatrix);
            }

    if (maxColorFrequencyIndex == 3)
        for (int i = 0; i < RGB_MATRIX_LINE; i++)
            for (int j = 0; j < RGB_MATRIX_COL; j += 3)
            {
                if (rgbMatrix[i][j] == 0 && rgbMatrix[i][j + 1] == 0 && rgbMatrix[i][j + 2] == 255)
                    setPixelColor(i, j, 0, RGB_MATRIX_LINE, RGB_MATRIX_COL, rgbMatrix);
                else
                    setPixelColor(i, j, 255, RGB_MATRIX_LINE, RGB_MATRIX_COL, rgbMatrix);
            }
    
    if (maxColorFrequencyIndex == 4)
        for (int i = 0; i < RGB_MATRIX_LINE; i++)
            for (int j = 0; j < RGB_MATRIX_COL; j += 3)
            {
                if (rgbMatrix[i][j] == 255 && rgbMatrix[i][j + 1] == 255 && rgbMatrix[i][j + 2] == 0)
                    setPixelColor(i, j, 0, RGB_MATRIX_LINE, RGB_MATRIX_COL, rgbMatrix);
                else
                    setPixelColor(i, j, 255, RGB_MATRIX_LINE, RGB_MATRIX_COL, rgbMatrix);
            }

    return 0;
}