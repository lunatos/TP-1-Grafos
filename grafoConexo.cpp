#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <conio.h>
#include <stack>

using namespace std;

string selectFile()
{
    string graphFiles[] = {"graph-test-20.txt", "graph-test-100000.txt"};
    int numGraphFiles = sizeof(graphFiles) / sizeof(graphFiles[0]);
    int currentChoice = 0;

    while (true)
    {
        system("CLS");
        cout << "Selecione o arquivo:" << endl;
        for (int i = 0; i < numGraphFiles; i++)
        {
            if (i == currentChoice)
            {
                cout << "> " << graphFiles[i] << endl;
            }
            else
            {
                cout << "  " << graphFiles[i] << endl;
            }
        }

        int c = getch(); // o que foi digitado no console
        switch (c)
        {
        case 0x48: // seta para cima
            currentChoice = (currentChoice - 1 + numGraphFiles) % numGraphFiles;
            break;
        case 0x50: // seta para baixo
            currentChoice = (currentChoice + 1) % numGraphFiles;
            break;
        case 0x0D: // Enter
            return graphFiles[currentChoice];
        }
    }
}

class Graph
{
private:
    vector<vector<int>> adjList;
    int numVertices;
    int numEdges;

public:
    Graph(const string &filename)
    {
        ifstream file(filename);
        if (!file)
        {
            cerr << "Arquivo: " << filename << " não existe" << endl;
            return;
        }

        file >> numVertices >> numEdges;

        adjList.resize(numVertices + 1); // +1 para considerar o vértice 0

        int counter = 0;
        for (int i = 0; i < numEdges; i++)
        {
            int v1, v2;
            file >> v1 >> v2;
            adjList[v1].push_back(v2);
            adjList[v2].push_back(v1);

            counter++;
            if (counter % (numEdges / 10) == 0)
            {
                system("CLS");
                cout << "Arestas lidas: " << (counter * 100) / numEdges << "%" << endl;
            }
        }

        file.close();

        cout << "Leitura das arestas completa." << endl;
    }

    bool isConnected()
    {
        vector<bool> visited(numVertices + 1, false);
        stack<int> s;

        s.push(1);
        visited[1] = true;

        while (!s.empty())
        {
            int vertex = s.top();
            s.pop();

            for (int neighbor : adjList[vertex])
            {
                if (!visited[neighbor])
                {
                    s.push(neighbor);
                    visited[neighbor] = true;
                }
            }
        }


        for (int i = 1; i <= numVertices; i++)
        {
            if (!visited[i])
            {
                return false; // algum vertice não foi visitado entao o grafo não é conexo
            }
        }
        return true; // todos os vértices foram visitados, então o grafo é conexo
    }
};

int main()
{
    string filename = selectFile();
    Graph test = Graph(filename);

    if (test.isConnected())
    {
        cout << "eh conexo" << endl;
    }
    else
    {
        cout << "nao eh conexo" << endl;
    }

    return 0;
}