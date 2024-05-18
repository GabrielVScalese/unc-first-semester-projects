#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <ctype.h>

#define TEXT_INPUT_LENGTH 1000
#define WORDS_NUMBER 1000

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
    int polarity;
} TextPolarity;

int paragraphQnt;
char positiveWordList[WORDS_NUMBER][TEXT_INPUT_LENGTH];
char negativeWordList[WORDS_NUMBER][TEXT_INPUT_LENGTH];

void resetString (char oneString[])
{
    for (int i = 0; oneString[i] != '\0'; i++)
        oneString[i] = '\0';
}

void setWordList (char wordList[WORDS_NUMBER][TEXT_INPUT_LENGTH], char textInput[])
{
    char partialWord[TEXT_INPUT_LENGTH];

    int wordlistCount = 0;
    for (int i = 0; ; i++)
    {
        if (textInput[i] == ' ' || textInput[i] == '\n')
        {
            strcpy(wordList[wordlistCount++], partialWord);
            resetString(partialWord);

            if (textInput[i] == '\n')
                break;
        }
        else
            strncat(partialWord, &textInput[i], 1);
    }
}

int listContainsWord(char list[WORDS_NUMBER][TEXT_INPUT_LENGTH], char word[])
{
    for (int i = 0; list[i][0] != '\0'; i++)
        if (strcasecmp(list[i], word) == 0)
            return 1;
    
    return 0;
}

Sentence getSentenceFromParagraph (char text[], int initialLineIndex, int initialColIndex)
{
    Sentence oneSentence;
    oneSentence.initialLineIndex = initialLineIndex;
    oneSentence.initialColIndex = initialColIndex;
    oneSentence.lettersNumber = 0;
    oneSentence.wordsNumber = 0;
    resetString(oneSentence.content);

    for (int j = initialColIndex; text[j] != '\n';j++)
    {
        if (isalpha(text[j]))
            oneSentence.lettersNumber++;

        if (text[j] == ' ')
            oneSentence.wordsNumber++;
        
        strncat(oneSentence.content, &text[j], 1);
        if (text[j] == '.')
        {
            oneSentence.finalColIndex = j;
            oneSentence.lettersNumber = oneSentence.lettersNumber;
           
            return oneSentence;
        }
    }
}

Sentence getLongestSentence (char text[paragraphQnt][TEXT_INPUT_LENGTH])
{
    Sentence oneSentence;
    Sentence longestSentence;

    longestSentence.lettersNumber = 0;
    for (int i = 0; i < paragraphQnt; i++)
    {
        for (int j = 0; text[i][j] != '\n';)
        {
            Sentence oneSentence = getSentenceFromParagraph(text[i], i, j);

            if (oneSentence.lettersNumber > longestSentence.lettersNumber)
            {
                longestSentence.initialLineIndex = i;
                longestSentence.finalLineIndex = i;
                longestSentence.initialColIndex = oneSentence.initialColIndex;
                longestSentence.finalColIndex = oneSentence.finalColIndex;
                longestSentence.lettersNumber = oneSentence.lettersNumber;
            }

            j = oneSentence.finalColIndex + 1;
        }
    }

    return longestSentence;
}

Sentence getSmallestSentence (char text[paragraphQnt][TEXT_INPUT_LENGTH])
{
    Sentence oneSentence;
    Sentence smallestSentence;

    smallestSentence.wordsNumber = 1000;
    for (int i = 0; i < paragraphQnt; i++)
    {
        for (int j = 0; text[i][j] != '\n';)
        {
            Sentence oneSentence = getSentenceFromParagraph(text[i], i, j);

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
    }

    return smallestSentence;
}

TextPolarity getPolarityFromAnalysis (TextAnalysis textAnalysis)
{
    TextPolarity textPolarity;
    textPolarity.positiveWordCount = textAnalysis.positiveWordCount;
    textPolarity.negativeWordCount = textAnalysis.negativeWordCount;
    
    if ((textPolarity.positiveWordCount - textPolarity.negativeWordCount) > 3)
        textPolarity.polarity = 1;
    
    if (textPolarity.positiveWordCount > 0 && textPolarity.negativeWordCount == 0)
        textPolarity.polarity = 1;
    
    if ((textPolarity.negativeWordCount - textPolarity.positiveWordCount) > 3)
       textPolarity.polarity = -1;
    
    if (textPolarity.positiveWordCount == 0 && textPolarity.negativeWordCount > 0)
        textPolarity.polarity = -1;
    
    if (textPolarity.positiveWordCount > 0 && textPolarity.negativeWordCount > 0 && abs(textPolarity.positiveWordCount - textPolarity.negativeWordCount) <= 3)
        textPolarity.polarity = 0;

    return textPolarity;
}

TextAnalysis getAnalysisFromText (char text[])
{
    int positiveWordCount = 0;
    int negativeWordCount = 0;
    char partialWord[TEXT_INPUT_LENGTH];
    resetString(partialWord);

    for (int i = 0; i < strlen(text); i++)
    {
        if (text[i] == ' ' || text[i] == '.' || text[i] == ',')
        {
            if (listContainsWord(positiveWordList, partialWord))
                positiveWordCount++;
        
            if (listContainsWord(negativeWordList, partialWord))
                negativeWordCount++;
        
            resetString(partialWord);
        }
        else
            strncat(partialWord, &text[i], 1);
    }

    TextAnalysis textAnalysis;
    textAnalysis.positiveWordCount = positiveWordCount;
    textAnalysis.negativeWordCount = negativeWordCount;

    return textAnalysis;
}

TextPolarity getPolarityFromParagraphs (char paragraphs[paragraphQnt][TEXT_INPUT_LENGTH], int initialParagraph, int finalParagraph)
{
    char partialWord[TEXT_INPUT_LENGTH];
    int positiveWordCount = 0;
    int negativeWordCount = 0;

    for (int i = initialParagraph; i <= finalParagraph; i++)
    {
        TextAnalysis paragraphAnalysis = getAnalysisFromText(paragraphs[i]);
        positiveWordCount += paragraphAnalysis.positiveWordCount;
        negativeWordCount += paragraphAnalysis.negativeWordCount;
    }
    
    TextAnalysis allParagraphsAnalysis;
    allParagraphsAnalysis.positiveWordCount = positiveWordCount;
    allParagraphsAnalysis.negativeWordCount = negativeWordCount;

    return getPolarityFromAnalysis(allParagraphsAnalysis);
}

TextPolarity getPolarityFromSentence (Sentence oneSentence)
{
    TextAnalysis sentenceAnalysis = getAnalysisFromText(oneSentence.content);
    return getPolarityFromAnalysis(sentenceAnalysis);
}

// Falta observacao sobre sinal a mais sobre palavra do dicionario
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
    scanf("%d", &questionQnt);;

    TextPolarity textPolarity;
    for (int i = 0; i < questionQnt; i++)
    {
        int questionNumber, questionIndex = -1;
        scanf("%d", &questionNumber, &questionIndex);

        switch(questionNumber)
        {
            case 1:
            {
                textPolarity = getPolarityFromParagraphs(allParagraphs, 0, paragraphQnt - 1);
                printf("%d Palavras positivas, %d Palavras negativas: Polaridade %d", textPolarity.positiveWordCount, textPolarity.negativeWordCount, textPolarity.polarity);
            }
               
            break;
            case 2: 
            {
                Sentence smallestSentence = getSmallestSentence(allParagraphs);
                textPolarity = getPolarityFromSentence(smallestSentence);
                printf("%d Palavras positivas, %d Palavras negativas: Polaridade %d", textPolarity.positiveWordCount, textPolarity.negativeWordCount, textPolarity.polarity);
                /*if (questionIndex != -1)
                    //textPolarity = getPolarityFromText(text, 0, paragraphQnt - 1);
                else
                {
                    Sentence longestSentence = getLongestSentence(text);
                    // textPolarity = getTextPolarity(text, longestSentence.initialLineIndex, longestSentence.initialColIndex, longestSentence.finalLineIndex, longestSentence.finalColIndex);
                }*/
            } 
            case 3:
            {
                Sentence smallestSentence = getSmallestSentence(allParagraphs);
                textPolarity = getPolarityFromSentence(smallestSentence);
                printf("%d Palavras positivas, %d Palavras negativas: Polaridade %d", textPolarity.positiveWordCount, textPolarity.negativeWordCount, textPolarity.polarity);
            }
            break;
        }
    }
    
    return 0;
}