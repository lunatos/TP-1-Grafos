#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <conio.h>
#include <stack>
#include <chrono>

using namespace std;

string selectFile()
{
    string graphFiles[] = {
        "graph-test-5x.txt",
        "graph-test-5.txt",
        "graph-test-100.txt",
        "graph-test-1000.txt",
        "graph-test-10000.txt",
        "graph-test-100000.txt",
    };
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
            cerr << "Arquivo: " << filename << " nao existe" << endl;
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
            if (counter % 1000 == 0)
            {
                system("CLS");
                cout << "Arestas lidas: " << (counter * 100) / numEdges << "%" << endl;
            }
        }

        file.close();

        cout << "Leitura das arestas completa." << endl;
    }

    bool find_articulations_points()
    {
        vector<bool> visited(numVertices + 1, false);
        vector<int> discvery(numVertices + 1, 0);
        vector<int> low(numVertices + 1, 0);
        vector<bool> articulation(numVertices + 1, false);
        int time = 0;

        auto start = chrono::high_resolution_clock::now();

        for (int i = 1; i <= numVertices; i++)
        {
            if (!visited[i])
            {
                dfs(i, -1, visited, discvery, low, articulation, time);
            }
        }
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
        auto seconds = duration.count() / 1000;
        auto minutes = seconds / 60;
        seconds %= 60;
        auto milliseconds = duration.count() % 1000;
        system ("CLS");
        cout << "Algoritmo executado em: " << minutes << " minutos, " << seconds << " segundos e " << milliseconds << " milissegundos" << endl;

        bool result = true;
        // procurando se foi encontrado pontos de articulação
        for (int i = 1; i <= numVertices; i++)
        {
            if (articulation[i])
            {
                result = false;
            }
        }
        return result;
    }

private:
    void dfs(int u, int parent, vector<bool> &visited, vector<int> &discvery, vector<int> &low, vector<bool> &articulation, int &time)
    {
        visited[u] = true;
        discvery[u] = low[u] = ++time;

        int children = 0;
        for (int v : adjList[u])
        {
            if (!visited[v])
            {
                children++;
                dfs(v, u, visited, discvery, low, articulation, time);
                low[u] = min(low[u], low[v]);

                if ((parent != -1 && low[v] >= discvery[u]) || (parent == -1 && children > 1))
                {
                    articulation[u] = true;
                }
            }
            else if (v != parent)
            {
                low[u] = min(low[u], discvery[v]);
            }
        }
    }
};

int main()
{
    string filename = selectFile();
    Graph test = Graph(filename);
    if (test.find_articulations_points())
    {
        cout << "O grafo eh biconexo." << endl;
    }
    else
    {
        cout << "O grafo nao eh biconexo." << endl;
    }

    return 0;
}