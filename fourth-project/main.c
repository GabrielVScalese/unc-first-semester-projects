#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <ctype.h>

// Constantes
#define MIN_VALUE -1
#define MAX_VALUE 1000
#define TEXT_INPUT_LENGTH 10000
#define WORDS_NUMBER 1000
#define ONE_QUESTION_INPUT_LENGTH 4
#define POLARITY_TEXT_LENGTH 9
#define POSITIVE_POLARITY_TEXT "positiva"
#define NEGATIVE_POLARITY_TEXT "negativa"
#define NEUTRAL_POLARITY_TEXT "neutra"

// Estruturas
typedef struct TextAnalysis {
    int positiveWordCount;
    int negativeWordCount;
} TextAnalysis;

typedef struct Sentence  {
        int initialLineIndex;
        int finalLineIndex;
        int initialColIndex;
        int finalColIndex;
        int lettersNumber;
        int wordsNumber;
        char content[TEXT_INPUT_LENGTH];
} Sentence;

typedef struct TextPolarity {
    int positiveWordCount;
    int negativeWordCount;
    char polarity[POLARITY_TEXT_LENGTH];
} TextPolarity;

// Variaveis globais
int paragraphQnt;
char positiveWordList[WORDS_NUMBER][TEXT_INPUT_LENGTH];
char negativeWordList[WORDS_NUMBER][TEXT_INPUT_LENGTH];

int convertCharToInt (char charNumber)
{
    return charNumber - '0';
}

// Limpa lixo de string
void resetString (char oneString[])
{
    for (int i = 0; oneString[i] != '\0'; i++)
        oneString[i] = '\0';
}

// Preencher lista de palavras
void setWordList (char wordList[WORDS_NUMBER][TEXT_INPUT_LENGTH], char textInput[])
{
    char partialWord[TEXT_INPUT_LENGTH];
    resetString(partialWord); // Para uso do strncat

    int wordlistCount = 0;
    for (int i = 0; i < strlen(textInput); i++)
        if (textInput[i] == ' ' || textInput[i] == '\n')
        {
            strcpy(wordList[wordlistCount++], partialWord);
            resetString(partialWord);
        }
        else
            strncat(partialWord, &textInput[i], 1);
}

// Verifica se algum tipo de lista contem tal palavra
int listContainsWord(char list[WORDS_NUMBER][TEXT_INPUT_LENGTH], char word[])
{
    for (int i = 0; list[i][0] != '\0'; i++)
        if (strcasecmp(list[i], word) == 0)
            return 1;
    
    return 0;
}

Sentence getSentenceFromParagraph (char paragraph[], int initialLineIndex, int initialColIndex)
{
    Sentence oneSentence;
    oneSentence.initialLineIndex = initialLineIndex;
    oneSentence.initialColIndex = initialColIndex;
    oneSentence.lettersNumber = 0;
    oneSentence.wordsNumber = 0;
    resetString(oneSentence.content);

    for (int j = initialColIndex; j < strlen(paragraph); j++)
    {
        if (isalpha(paragraph[j]))
            oneSentence.lettersNumber++;

        if (paragraph[j] == ' ' || paragraph[j] == '.')
            oneSentence.wordsNumber++;
        
        strncat(oneSentence.content, &paragraph[j], 1);
        if (paragraph[j] == '.')
        {
            oneSentence.finalColIndex = j;
            oneSentence.lettersNumber = oneSentence.lettersNumber;
           
            return oneSentence;
        }
    }

    return oneSentence;
}

Sentence getLongestSentence (char allParagraphs[paragraphQnt][TEXT_INPUT_LENGTH])
{
    Sentence longestSentence;
    longestSentence.lettersNumber = MIN_VALUE;
    for (int i = 0; i < paragraphQnt; i++)
        for (int j = 0; allParagraphs[i][j] != '\n';)
        {
            Sentence oneSentence = getSentenceFromParagraph(allParagraphs[i], i, j);

            if (oneSentence.lettersNumber > longestSentence.lettersNumber)
            {
                longestSentence.initialLineIndex = i;
                longestSentence.finalLineIndex = i;
                longestSentence.initialColIndex = oneSentence.initialColIndex;
                longestSentence.finalColIndex = oneSentence.finalColIndex;
                longestSentence.lettersNumber = oneSentence.lettersNumber;
                longestSentence.wordsNumber = oneSentence.wordsNumber; 
                strcpy(longestSentence.content, oneSentence.content);
            }

            j = oneSentence.finalColIndex + 1;
        }

    return longestSentence;
}

Sentence getSmallestSentence (char allParagraphs[paragraphQnt][TEXT_INPUT_LENGTH])
{
    Sentence smallestSentence;
    smallestSentence.wordsNumber = MAX_VALUE;
    for (int i = 0; i < paragraphQnt; i++)
        for (int j = 0; allParagraphs[i][j] != '\n';)
        {
            Sentence oneSentence = getSentenceFromParagraph(allParagraphs[i], i, j);

            if (oneSentence.wordsNumber < smallestSentence.wordsNumber)
            {
                smallestSentence.initialLineIndex = i;
                smallestSentence.finalLineIndex = i;
                smallestSentence.initialColIndex = oneSentence.initialColIndex;
                smallestSentence.finalColIndex = oneSentence.finalColIndex;
                smallestSentence.wordsNumber = oneSentence.wordsNumber;
                strcpy(smallestSentence.content, oneSentence.content);
            }

            j = oneSentence.finalColIndex + 1;
        }

    return smallestSentence;
}

// Obtem polaridade (positiva ou negativa) de um conteudo a partir da contagem de palavras positivas e negativas
TextPolarity getPolarityFromAnalysis (TextAnalysis textAnalysis)
{
    TextPolarity textPolarity;
    textPolarity.positiveWordCount = textAnalysis.positiveWordCount;
    textPolarity.negativeWordCount = textAnalysis.negativeWordCount;
    
    if ((textPolarity.positiveWordCount - textPolarity.negativeWordCount) > 3)
        strcpy(textPolarity.polarity, POSITIVE_POLARITY_TEXT);
    
    if (textPolarity.positiveWordCount > 0 && textPolarity.negativeWordCount == 0)
         strcpy(textPolarity.polarity, POSITIVE_POLARITY_TEXT);
    
    if ((textPolarity.negativeWordCount - textPolarity.positiveWordCount) > 3)
        strcpy(textPolarity.polarity, NEGATIVE_POLARITY_TEXT);
    
    if (textPolarity.positiveWordCount == 0 && textPolarity.negativeWordCount > 0)
        strcpy(textPolarity.polarity, NEGATIVE_POLARITY_TEXT);
    
    if (textPolarity.positiveWordCount > 0 && textPolarity.negativeWordCount > 0 && abs(textPolarity.positiveWordCount - textPolarity.negativeWordCount) <= 3)
        strcpy(textPolarity.polarity, NEUTRAL_POLARITY_TEXT);
    
    if (textPolarity.positiveWordCount  == 0 && textPolarity.negativeWordCount == 0)
        strcpy(textPolarity.polarity, NEUTRAL_POLARITY_TEXT);

    return textPolarity;
}

// Obtem contagem de palavras positivas e negativas de um trecho
TextAnalysis getAnalysisFromText (char text[])
{
    TextAnalysis textAnalysis;
    textAnalysis.positiveWordCount = 0;
    textAnalysis.negativeWordCount = 0;
    char partialWord[TEXT_INPUT_LENGTH];
    resetString(partialWord);
    for (int i = 0; i < strlen(text); i++)
    {
        if (text[i] == ' ' || text[i] == '.' || text[i] == ',' || text[i] == ';' || text[i] == '?' || text[i] == '!') // Se isso ocorre, termina uma palavra 
        {
            if (listContainsWord(positiveWordList, partialWord))
                textAnalysis.positiveWordCount++;
        
            if (listContainsWord(negativeWordList, partialWord))
                textAnalysis.negativeWordCount++;
        
            resetString(partialWord);
        }
        else
            strncat(partialWord, &text[i], 1);
    }

    return textAnalysis;
}

// Obtem polaridade de paragrafo(s)
TextPolarity getPolarityFromParagraphs (char paragraphs[paragraphQnt][TEXT_INPUT_LENGTH], int initialParagraph, int finalParagraph)
{
    TextAnalysis allParagraphsAnalysis;
    allParagraphsAnalysis.positiveWordCount = 0;
    allParagraphsAnalysis.negativeWordCount = 0;
    for (int i = initialParagraph; i <= finalParagraph; i++)
    {
        TextAnalysis paragraphAnalysis = getAnalysisFromText(paragraphs[i]);
        allParagraphsAnalysis.positiveWordCount += paragraphAnalysis.positiveWordCount;
        allParagraphsAnalysis.negativeWordCount += paragraphAnalysis.negativeWordCount;
    }

    return getPolarityFromAnalysis(allParagraphsAnalysis);
}

// Obtem polaridade de uma sentenca
TextPolarity getPolarityFromSentence (Sentence oneSentence)
{
    TextAnalysis sentenceAnalysis = getAnalysisFromText(oneSentence.content);
    return getPolarityFromAnalysis(sentenceAnalysis);
}

int main ()
{
    scanf("%d\n", &paragraphQnt);
    char allParagraphs[paragraphQnt][TEXT_INPUT_LENGTH];

    // Leitura do texto
    for (int i = 0; i < paragraphQnt; i++)
    {
        char textInput[TEXT_INPUT_LENGTH];
        fgets(textInput, sizeof(textInput), stdin);
        strcpy(allParagraphs[i], textInput);
    }

    // Leitura das palavras positivas e negativas
    char textInput[TEXT_INPUT_LENGTH];
    fgets(textInput, sizeof(textInput), stdin);
    setWordList(positiveWordList, textInput);

    fgets(textInput, sizeof(textInput), stdin);
    setWordList(negativeWordList, textInput);

    // Leitura das questoes
    int questionQnt;
    scanf("%d\n", &questionQnt);
    char questions[questionQnt][ONE_QUESTION_INPUT_LENGTH];
    for (int i = 0; i < questionQnt;i++)
    {
        char textInput[TEXT_INPUT_LENGTH];
        fgets(textInput, sizeof(textInput), stdin);
        strcpy(questions[i], textInput);
    }
    
    // Tratamento para cada tipo de questao
    for (int i = 0; i < questionQnt; i++)
    {
        TextPolarity textPolarity;
        switch(questions[i][0])
        {
            // Obter polaridade de todos os paragrafos
            case '1':
                textPolarity = getPolarityFromParagraphs(allParagraphs, 0, paragraphQnt - 1);
            break;
            // Obter polaridade de um paragrafo i ou da sentenca mais longa
            case '2': 
            {
                if (strlen(questions[i]) > 2)
                    textPolarity = getPolarityFromParagraphs(allParagraphs, convertCharToInt(questions[i][2]), convertCharToInt(questions[i][2]));
                else
                {
                    Sentence longestSentence = getLongestSentence(allParagraphs);
                    textPolarity = getPolarityFromSentence(longestSentence);
                }
            } 
            break;
            // Obter polaridade da sentenca mais curta
            case '3':
            {
                Sentence smallestSentence = getSmallestSentence(allParagraphs);
                textPolarity = getPolarityFromSentence(smallestSentence);
            }
            break;
        }
        printf("%d Palavras positivas, %d Palavras negativas: Polaridade %s\n", textPolarity.positiveWordCount, textPolarity.negativeWordCount, textPolarity.polarity);
    }
    
    return 0;
}