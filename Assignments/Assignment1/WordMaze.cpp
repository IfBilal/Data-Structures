#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <stdio.h>
using namespace std;
char **createMatrix(int size)
{
    char **matrix = new char *[size];
    for (int i = 0; i < size; i++)
    {
        *(matrix + i) = new char[size];
    }
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            *(*(matrix + i) + j) = '-';
        }
    }
    return matrix;
}
char **fillDictionary(int maxSize, int &wordCount)
{
    char **dictionary = nullptr;
    ifstream File("dictionary.txt");
    string word;
    if (!File)
    {
        cout << "Dictionary not available!\n";
        exit(0);
    }
    if (File.peek() == EOF)
    {
        cout << "Dictionary file empty\n";
        exit(0);
    }
    while (getline(File, word, ','))
    {
        if (!word.empty() && word.length() <= maxSize)
            wordCount++;
    }
    File.clear();
    File.seekg(0);
    dictionary = new char *[wordCount];
    for (int i = 0; i < wordCount;)
    {
        getline(File, word, ',');
        if (!word.empty() && word.length() <= maxSize)
        {
            *(dictionary + i) = new char[word.length() + 1];
            strcpy(*(dictionary + i), word.c_str());
            i++;
        }
    }
    return dictionary;
}
void deleteMatrix(char **matrix, int row)
{
    if (matrix)
    {
        for (int i = 0; i < row; i++)
        {
            delete[] *(matrix + i);
        }
        delete[] matrix;
    }
}
string *placeWordsInMatrix(char **matrix, char **dictionary, int matrixSize, int dictionaryWordCount, int &placedWordsCount)
{
    int dx[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    int dy[8] = {1, 1, 0, -1, -1, -1, 0, 1};
    string *placedWords = new string[dictionaryWordCount];
    for (int i = 0; i < dictionaryWordCount; i++)
    {
        placedWords[i] = "";
    }
    srand(time(0));
    for (int h = 0; h < dictionaryWordCount; h++)
    {
        int wordLength = strlen(dictionary[h]);
        bool found = false;
        for (int attempts = 0; attempts < 50; attempts++)
        {
            int x = rand() % matrixSize;
            int y = rand() % matrixSize;
            for (int dir = 0; dir < 8; dir++)
            {
                bool canPlace = true;
                for (int l = 0; l < wordLength; l++)
                {
                    int r = x + dx[dir] * l;
                    int c = y + dy[dir] * l;
                    if (r < 0 || r >= matrixSize || c < 0 || c >= matrixSize)
                    {
                        canPlace = false;
                        break;
                    }
                    if (matrix[r][c] != '-' && matrix[r][c] != dictionary[h][l])
                    {
                        canPlace = false;
                        break;
                    }
                }
                if (canPlace)
                {
                    found = true;
                    placedWords[placedWordsCount++] = dictionary[h];
                    for (int l = 0; l < wordLength; l++)
                    {
                        matrix[x + l * dx[dir]][y + l * dy[dir]] = dictionary[h][l];
                    }
                    break;
                }
            }
            if (found)
                break;
        }
    }
    for (int i = 0; i < matrixSize; i++)
    {
        for (int j = 0; j < matrixSize; j++)
        {
            if (matrix[i][j] == '-')
                matrix[i][j] = static_cast<char>(rand() % 26 + 97);
        }
    }
    return placedWords;
}

bool wordSearch(char **matrix, string word, int matrixSize, int row, int col)
{
    int dx[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    int dy[8] = {1, 1, 0, -1, -1, -1, 0, 1};
    for (int dir = 0; dir < 8; dir++)
    {
        bool canBeUpperCased = true;
        for (int k = 0; k < word.length(); k++)
        {
            int r = row + dx[dir] * k;
            int c = col + dy[dir] * k;
            if (r < 0 || r >= matrixSize || c < 0 || c >= matrixSize)
            {
                canBeUpperCased = false;
                break;
            }
            if ((matrix[r][c] >= 'a' && matrix[r][c] <= 'z') ? (matrix[r][c] != word[k]) : (matrix[r][c] != word[k] - 32))
            {
                canBeUpperCased = false;
                break;
            }
        }
        if (canBeUpperCased)
        {
            for (int k = 0; k < word.length(); k++)
            {
                if (matrix[row + dx[dir] * k][col + dy[dir] * k] >= 'a' && matrix[row + dx[dir] * k][col + dy[dir] * k] <= 'z')
                    matrix[row + dx[dir] * k][col + dy[dir] * k] -= 32;
            }
            return true;
        }
    }
    return false;
}
void tellHint(char **matrix, string word, int matrixSize)
{
    int dx[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    int dy[8] = {1, 1, 0, -1, -1, -1, 0, 1};
    for (int i = 0; i < matrixSize; i++)
    {
        for (int j = 0; j < matrixSize; j++)
        {
            for (int dir = 0; dir < 8; dir++)
            {
                bool canBeUpperCased = true;
                for (int k = 0; k < word.length(); k++)
                {
                    int r = i + dx[dir] * k;
                    int c = j + dy[dir] * k;
                    if (r < 0 || r >= matrixSize || c < 0 || c >= matrixSize)
                    {
                        canBeUpperCased = false;
                        break;
                    }
                    if (matrix[r][c] != word[k])
                    {
                        canBeUpperCased = false;
                        break;
                    }
                }
                if (canBeUpperCased)
                {
                    cout << "The hint letter is at index ( " << i << " , " << j << " )\n";
                    return;
                }
            }
        }
    }
}

int main()
{
    char **matrix = nullptr;
    char **dictionary = nullptr;
    bool hintUsed = false;
    int foundWords = 0;
    int mistakeCount = 0;
    int attempts = 0;
    int dictionaryWordCount = 0;
    int matrixSize = 7;
    int placedWordCount = 0;
    int choice;
    cout << "Select Board Size: \n1: Default\n2: Personalized \n";
    cin >> choice;
    while (choice != 1 && choice != 2)
    {
        cout << "Invalid\nEnter again: ";
        cin >> choice;
    }
    if (choice == 2)
    {
        cout << "Enter size of board: (Minimum size is 3x3): ";
        cin >> matrixSize;
        while (matrixSize < 3)
        {
            cout << "Invalid\nMinimum size of board is 3x3\nEnter again: ";
            cin >> matrixSize;
        }
    }
    matrix = createMatrix(matrixSize);
    dictionary = fillDictionary(matrixSize, dictionaryWordCount);
    string *placedWords = placeWordsInMatrix(matrix, dictionary, matrixSize, dictionaryWordCount, placedWordCount);

    string userWord;
    time_t start, end;
    time(&start);
    while (true)
    {
        cout << "Mistake Count: " << mistakeCount << endl;
        cout << "Found Words: " << foundWords << endl;
        cout << "Remaining Words: " << placedWordCount << endl;
        if (placedWordCount == 0)
        {
            cout << "You won!\nAll words placed\n";
            cout << "Final Maze:\n";
            for (int i = 0; i < matrixSize; i++)
            {
                for (int j = 0; j < matrixSize; j++)
                {
                    cout << matrix[i][j] << " ";
                }
                cout << endl;
            }
            break;
        }
        int choice;
        cout << "1: Get Hint\n2: Play\n3: Maze\n4: Display Remaining words \n5: Exit\n";
        cin >> choice;
        while (choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5)
        {
            cout << "Invalid\nEnter again: ";
            cin >> choice;
        }
        if (choice == 1)
        {
            if (!hintUsed)
            {
                tellHint(matrix, placedWords[0], matrixSize);
                hintUsed = true;
            }
            else
            {
                cout << "Hint already used\n";
            }
        }
        else if (choice == 2)
        {
            while (true)
            {
                int row, col;
                cout << "Enter the word to search: ";
                cin >> userWord;
                cout << "Enter the row and column of the word: ";
                cin >> row >> col;
                if (row < 0 || row >= matrixSize || col < 0 || col >= matrixSize)
                {
                    cout << "Invalid position\n";
                    continue;
                }
                for (int i = 0; i < userWord.length(); i++)
                {
                    if (userWord[i] >= 'A' && userWord[i] <= 'Z')
                    {
                        userWord[i] += 32;
                    }
                }
                bool found = false;
                for (int i = 0; i < placedWordCount; i++)
                {
                    if (placedWords[i] == userWord)
                    {
                        found = true;
                    }
                }
                attempts++;
                if (found)
                {
                    bool capitalized = wordSearch(matrix, userWord, matrixSize, row, col);
                    if (capitalized)
                    {
                        cout << "Word found\n";
                        foundWords++;
                        for (int i = 0; i < placedWordCount; i++)
                        {
                            if (placedWords[i] == userWord)
                            {
                                for (int j = i; j < placedWordCount - 1; j++)
                                {
                                    placedWords[j] = placedWords[j + 1];
                                }
                                placedWordCount--;
                                break;
                            }
                        }
                    }
                    else
                    {
                        cout << "Word not found\n";
                        mistakeCount++;
                    }
                    break;
                }
                else
                {
                    cout << "Word not found\n";
                    mistakeCount++;
                    break;
                }
            }
        }
        else if (choice == 3)
        {
            for (int i = 0; i < matrixSize; i++)
            {
                for (int j = 0; j < matrixSize; j++)
                {
                    cout << matrix[i][j] << " ";
                }
                cout << endl;
            }
        }
        else if (choice == 4)
        {
            for (int i = 0; i < placedWordCount; i++)
            {
                cout << placedWords[i] << endl;
            }
        }
        else if (choice == 5)
        {
            break;
        }
    }
    time(&end);
    cout << "Time taken: " << difftime(end, start) << " seconds " << endl;
    cout << "Attempts taken : " << attempts << endl;
    deleteMatrix(dictionary, dictionaryWordCount);
    deleteMatrix(matrix, matrixSize);
    return 0;
}