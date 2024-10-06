#include <iostream>
#include <vector>
#include <stack>
#include <fstream>
#include <conio.h>
#include <set>
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

vector<int> findCycle(int v, stack<int> st)
{
    vector<int> cycle;
    stack<int> stCopy = st;

    for (int i = 0; i < st.size(); i++)
    {
        int top = stCopy.top();
        stCopy.pop();
        cycle.push_back(top);
        if (top == v)
        {
            break;
        }
    }

    return cycle;
}

vector<int> unifyCycles(const vector<int> &cycleA, const vector<int> &cycleB)
{
    set<int> vertices;
    for (int v : cycleA)
    {
        vertices.insert(v);
    }
    for (int v : cycleB)
    {
        vertices.insert(v);
    }
    vector<int> result;
    result.reserve(vertices.size());
    for (int v : vertices)
    {
        result.push_back(v);
    }
    return result;
}

bool find(const vector<int> &list, int v)
{
    for (int i = 0; i < list.size(); i++)
    {
        if (list[i] == v)
        {
            return true;
        }
    }
    return false;
}

bool hasTwoCommonVertices(const vector<int> &listA, const vector<int> &listB)
{
    int commonVertices = 0;
    for (int v : listA)
    {
        if (find(listB, v))
        {
            commonVertices++;
            if (commonVertices >= 2)
            {
                return true;
            }
        }
    }
    return false;
}

class Graph
{
private:
    vector<vector<int>> adjList;
    int numVertices;
    set<vector<int>> cycles;

public:
    Graph(const string &filename)
    {
        ifstream file(filename);
        if (!file)
        {
            cerr << "Arquivo: " << filename << " nao existe" << endl;
            return;
        }

        file >> numVertices;

        adjList.resize(numVertices + 1); // +1 para considerar o vértice 0

        int numEdges;
        file >> numEdges;

        for (int i = 0; i < numEdges; i++)
        {
            int v1, v2;
            file >> v1 >> v2;
            adjList[v1].push_back(v2);
            adjList[v2].push_back(v1);
        }

        file.close();

        cout << "Leitura das arestas completa." << endl;
    }

    bool isBiconnected()
    {
        vector<bool> visited(numVertices + 1, false);
        stack<int> st;
        auto start = chrono::high_resolution_clock::now();

        for (int i = 1; i <= numVertices; i++)
        {

            if (!visited[i])
            {
                if (i != 1)
                {
                    return false; // o grafo não é conexo
                }
                dfs(i, visited, st);
            }
        }

        if (cycles.empty())
        {
            return false; // Grafo não tem ciclos
        }

        // Unir todos os ciclos
        vector<int> unifiedCycle = unionCycles();

        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
        auto seconds = duration.count() / 1000;
        auto minutes = seconds / 60;
        seconds %= 60;
        auto milliseconds = duration.count() % 1000;

        system ("CLS");
        cout << "Algoritmo executado em: " << minutes << " minutos, " << seconds << " segundos e " << milliseconds << " milissegundos" << endl;

        // Verificar se o número de vértices é igual ao do ciclo unificado
        // caso seja significa que o grafo é biconexo
        return unifiedCycle.size() == numVertices;
    }

private:
    vector<int> unionCycles()
    {
        int oldSize = 0;
        vector<int> unifiedCycle = *(cycles.begin());
        while (unifiedCycle.size() != oldSize)
        {
            oldSize = unifiedCycle.size();
            bool merged = false;
            for (auto it = cycles.begin(); it != cycles.end();)
            {
                if (hasTwoCommonVertices(unifiedCycle, *it))
                {
                    unifiedCycle = unifyCycles(unifiedCycle, *it);
                    it = cycles.erase(it);
                    merged = true;
                }
                else
                {
                    ++it;
                }
            }
            if (!merged)
            {
                break;
            }
        }

        return unifiedCycle;
    }

    void dfs(int u, vector<bool> &visited, stack<int> &st)
    {
        visited[u] = true;
        st.push(u);

        for (int v : adjList[u])
        {
            if (!visited[v])
            {
                dfs(v, visited, st);
            }
            else
            {
                // Encontrou um vértice já visitado, há um ciclo
                vector<int> cycle = findCycle(v, st);
                if (cycle.size() > 2)
                {
                    this->cycles.insert(cycle);
                }
            }
        }

        st.pop();
    }
};

int main()
{
    string filename = selectFile();
    Graph test = Graph(filename);
    if (test.isBiconnected())
    {
        cout << "O grafo eh biconexo." << endl;
    }
    else
    {
        cout << "O grafo nao eh biconexo." << endl;
    }

    return 0;
}