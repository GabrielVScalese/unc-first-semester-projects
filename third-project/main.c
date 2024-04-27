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

int main ()
{
    // Obtendo primeira linha
    char fileFormat[2];
    int width;
    int height;
    int intensity;
    scanf("%c%c %d %d %d\n", &fileFormat[0], &fileFormat[1], &width, &height, &intensity); // Testar depois com %s

    // Obtendo proximas linhas
    int compressesdMatrix[height][5];
    resetMatrix(height, 5, compressesdMatrix);
    for (int i = 0; i < height; i++)
    {
        int compressMatrixColCont = 0;
        char rgbValuesInput[1000];
        fgets(rgbValuesInput, sizeof(rgbValuesInput), stdin);

        int cont = 0;
        int onePixelRgbCont = 0;
        int onePixelRgb[3];
        for (int j = 0; cont != width;)
        {    
            if (rgbValuesInput[j] == '0' || rgbValuesInput[j] == ' ')
            {
                if (rgbValuesInput[j] == '0')
                    onePixelRgb[onePixelRgbCont++] =  0;

                j++;
            }
            else if (rgbValuesInput[j] == '2')
            {
                onePixelRgb[onePixelRgbCont++] = 255;
                j += 3;
            }               

            if (onePixelRgbCont == 3)
            {
                onePixelRgbCont = 0;
                compressesdMatrix[i][getColorIndex(onePixelRgb)]++;
                cont++;
            }  
        }
    }

    return 0;
}