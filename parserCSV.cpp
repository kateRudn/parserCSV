#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>

#define INT_MAX  2147483647
using namespace std;

const regex cellNumbers("(-)?[0-9]+");
const regex nameOfColumns("[a-zA-Z]+");
const regex firstStrInTable("(,[a-zA-Z]+)+");
const regex strInTable("[0-9]+(,((-)?[0-9]+)|(,=(((-)?[0-9]+)|([a-zA-Z]+[0-9]*))[-+/*](([0-9]+)|([a-zA-Z]+[0-9]*))))+");

int parseCell(string expressionString, vector<vector<string> > valuesTable)
{
    vector<char> ariphOperation{ '+', '-', '*', '/' };
    int posOperation = 0;
    for (int i = 0; i < ariphOperation.size(); i++)
    {
        posOperation = expressionString.find(ariphOperation[i]);
        if (posOperation>0)
        {
            break;
        }
    }
    string argOne = expressionString.substr(0, posOperation);
    string argTwo = expressionString.substr(posOperation+1, string::npos);
    
    // if is not letters then size = 0
    string oneLetters = regex_replace(argOne, cellNumbers, ""); //replace numbers
    string oneDigits = regex_replace(argOne, nameOfColumns, ""); //replace letters

    string twoLetters = regex_replace(argTwo, cellNumbers, ""); //replace numbers
    string twoDigits = regex_replace(argTwo, nameOfColumns, ""); //replace letters

    int i1 = 0, j1 = 0, i2 = 0, j2 = 0;
    int firstArg = 0, secondArg = 0;
    for (int j = 1; j < valuesTable[0].size(); j++)
    {
        if (oneLetters == valuesTable[0][j] && oneLetters.size()!=0)
        {
            j1 = j;
        }
        if (twoLetters == valuesTable[0][j] && twoLetters.size() != 0)
        {
            j2 = j;
        }
    }

    if (j1 == 0 && oneLetters.size() != 0)
    {
        cout << "Error: invalid name of column " << oneLetters << "!" << endl;
        return INT_MAX;
    }
    if (j2 == 0 && twoLetters.size() != 0)
    {
        cout << "Error: invalid name of column " << twoLetters << "!" << endl;
        return INT_MAX;
    }

    for (int i = 1; i < valuesTable.size(); i++)
    {
        if (oneDigits == valuesTable[i][0] && oneLetters.size() != 0)
        {
            i1 = i;
        }
        if (twoDigits == valuesTable[i][0] && twoLetters.size() != 0)
        {
            i2 = i;
        }
    }

    if (i1 == 0 && oneLetters.size() != 0)
    {
        cout << "Error: invalid number of row " << oneDigits << "!" << endl;
        return INT_MAX;
    }
    if (i2 == 0 && twoLetters.size() != 0)
    {
        cout << "Error: invalid number of row " << twoDigits << "!" << endl;
        return INT_MAX;
    }
    
    if (oneLetters.size() == 0)
    {
        firstArg = stoi(oneDigits);
    }
    else {
        if (valuesTable[i1][j1][0] == '=')
        {
            string tmpStr = valuesTable[i1][j1];
            firstArg = parseCell(tmpStr.erase(0, 1), valuesTable); //again
        }
        else {
            firstArg = stoi(valuesTable[i1][j1]);
        }
    }
    if (twoLetters.size() == 0)
    {
        secondArg = stoi(twoDigits);
    }
    else {
        if (valuesTable[i2][j2][0] == '=')
        {
            string tmpStr = valuesTable[i2][j2];
            secondArg = parseCell(tmpStr.erase(0, 1), valuesTable); //again
            
        }
        else {
            secondArg = stoi(valuesTable[i2][j2]);
        }
        
    }
    
    switch (expressionString[posOperation])
    {
        case ('+'):
            return firstArg + secondArg;
        case ('-'):
            return firstArg - secondArg;
        case ('*'):
            return firstArg * secondArg;
        case ('/'):
            if (secondArg == 0)
            { 
                cout << "Error: you can't divide by zero!" << endl;
                return INT_MAX;
            }
            return firstArg / secondArg;
        default:
            break;
    }
        
    return 0;
}

int calculationMatrix(vector<vector<string> > valuesTable, vector<vector<int> > &resultMatrix)
{
    resultMatrix.resize(resultMatrix.size() + valuesTable.size());

    for (int i = 1; i < valuesTable.size(); i++)
    {
        resultMatrix[i].resize(resultMatrix[i].size() + valuesTable[i].size());
        resultMatrix[i][0] = stoi(valuesTable[i][0]);
        for (int j = 1; j < valuesTable[i].size(); j++)
        {
            if (valuesTable[i][j][0] == '=')
            {
                string tmpStr = valuesTable[i][j];
                resultMatrix[i][j] = parseCell(tmpStr.erase(0, 1), valuesTable);
            }
            else
            {
                resultMatrix[i][j] = stoi(valuesTable[i][j]);
            }
        }
    }
    return 0;
}

int checkCorrectness(string str, const regex reg)
{
    if (regex_match(str, reg))
    {
        return 0;
    }
    return 1;
}

void writeTable(vector<string> tableColumns, vector<vector<int> > resultMatrix)
{
    for (int j = 0; j < tableColumns.size(); j++)
    {
        cout << tableColumns[j];
        if (j != tableColumns.size() - 1)
        {
            cout << ",";
        }
    }
    for (int i = 0; i < resultMatrix.size(); i++)
    {
        for (int j = 0; j < resultMatrix[i].size(); j++)
        {
            cout << resultMatrix[i][j];
            if (j != resultMatrix[i].size() - 1)
            {
                cout << ",";
            }
        }
        cout << endl;
    }
}

int readFile(char* filename, vector<vector<string> > &valuesTable)
{
    ifstream file;
    file.open(filename, ios::in);
    if (!file)
    {
        cout << "No such file or directory. Try again." << endl;
        return -1;
    }

    string tmpString;
    string valTable;
    string lineSeparator = ",";
    int i = 0;
    while (!file.eof())
    {
        valuesTable.resize(valuesTable.size()+1);
        file >> tmpString;
        if (checkCorrectness(tmpString, firstStrInTable) && i == 0)
        {
            cout << "Invalid format in the first row!" << endl;
            return 1;
        }
        else if (checkCorrectness(tmpString, strInTable) && i != 0)
        {
            cout << "Invalid format in the " << i << " row!" << endl;
            return 1;
        }
        size_t position = 0;
        while ((position = tmpString.find(lineSeparator)) != string::npos) {
            valTable = tmpString.substr(0, position);
            valuesTable[i].push_back(valTable);
            tmpString.erase(0, position + lineSeparator.length());
        }
        valuesTable[i].push_back(tmpString);
        if (valuesTable[i].size() != valuesTable[0].size())
        {
            cout << "Invalid format of the table in the " << i << " row!" << endl;
            return 1;
        }
        i++;
    }
    file.close();

    return 0;
}

int main(int argc, char* argv[])
{
    vector<vector<string> > valuesTable;
    vector<vector<int> > resultMatrix;

    if (argc < 2)
    {
        cout << "Input filename!" << endl;
        cout << "Example for usage: ./parser FILENAME.csv" << endl;
        return -1;
    }

    if (readFile(argv[1], valuesTable))
    {
        return -1;
    }

    calculationMatrix(valuesTable, resultMatrix);
    writeTable(valuesTable[0], resultMatrix);
  
    return 0;
}