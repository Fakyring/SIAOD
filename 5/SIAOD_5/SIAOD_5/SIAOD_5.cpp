#include <iostream>
#include <vector>
using namespace std;
#define INF 99999
bool mode = true;

// List to matrix convertation
vector<vector<int>> convert(vector<pair<int, int>> adjList[], int vectorSize)
{
	vector<vector<int>> matrix(vectorSize, vector<int>(vectorSize, INF));
	for (int i = 0; i < vectorSize; i++) {
		for (auto j : adjList[i])
			matrix[i][j.first] = j.second;
	}
	for (int i = 0; i < vectorSize; i++)
	{
		matrix[i][i] = 0;
	}
	return matrix;
}

// To add an edge
vector<pair<int, int>>* addEdge(vector<pair<int, int>> adj[], int u, int v, int wt, int& vectorSize)
{

	if (u >= vectorSize || v >= vectorSize)
	{
		vector<pair<int, int>>* adjList = new vector<pair<int, int>>[max(u, v) + 1];
		for (int i = 0; i < vectorSize; i++)
		{
			adjList[i] = adj[i];
		}
		adj = adjList;
		vectorSize = max(u, v) + 1;
	}
	adj[u].push_back(make_pair(v, wt));
	if (mode)
	{
		adj[v].push_back(make_pair(u, wt));
	}
	return adj;
}

// Floyd method
vector<vector<int>> floydWarshall(vector<vector<int>> adjMatrix, int vectorSize)
{
	int i, j, k;
	vector<vector<int>> dist(vectorSize, vector<int>(vectorSize, -1));
	for (i = 0; i < vectorSize; i++)
		for (j = 0; j < vectorSize; j++)
			dist[i][j] = adjMatrix[i][j];
	for (i = 0; i < vectorSize; i++) {
		for (j = 0; j < vectorSize; j++) {
			for (k = 0; k < vectorSize; k++) {
				if (dist[j][k] > (dist[j][i] + dist[i][k]) && (dist[i][k] != INF && dist[j][i] != INF))
					dist[j][k] = dist[j][i] + dist[i][k];
			}
		}
	}
	return dist;
}

// Print adjacency list/matrix representation of graph
void printList(vector <pair<int, int>> adjList[], int vectorSize)
{
	for (int i = 0; i < vectorSize; i++) {
		cout << i;
		for (auto j : adjList[i])
			cout << " --> " << j.first << "(" << j.second << ")";
		cout << endl;
	}
	cout << endl;
}
void printMatrix(vector<vector<int> > adj, int vectorSize)
{
	cout << "   ";
	for (int i = 0; i < vectorSize; i++)
	{
		cout << i << "\t";
	}
	cout << endl;
	for (int i = 0; i < vectorSize; i++) {
		cout << i << "| ";
		for (int j = 0; j < vectorSize; j++) {
			if (i != j && adj[i][j] == INF)
			{
				cout << "#\t";
			}
			else
				cout << adj[i][j] << "\t";
		}
		cout << endl;
	}
	cout << endl;
}

// Generate new list
void generateList(vector<pair<int, int>> adjList[], int vectorSize) {
	srand(time(NULL));
	bool exists = false;
	int  from, to, count;
	for (int i = 0; i < vectorSize; i++)
	{
		count = mode ? 1 : (rand() % 10 < 2 ? 2 : 1);
		from = i;
		for (int j = 0; j < count; j++)
		{
			do
			{
				exists = false;
				to = rand() % vectorSize;

				for (int k = 0; k < vectorSize && !exists; k++)
				{
					for (auto node : adjList[k])
					{
						if ((k == from && node.first == to) || (k == to && node.first == from))
						{
							exists = true;
							break;
						}
					}
				}
			} while (from == to || exists);
			adjList = addEdge(adjList, from, to, rand() % 9 + 1, vectorSize);
		}
	}
}

int main()
{
	setlocale(0, "");
	int vectorSize = 0, a, from, to, weight;
	vector<pair<int, int>>* adjList = new vector<pair<int, int>>[vectorSize];
	vector<vector<int>> adjMatrix;
	vector<vector<int>> minDist;
	bool exists = false;

	while (true) {
		cout << "0-Сменить тип графа\n";
		cout << "1-Создать новый список\n";
		cout << "2-Добавить новый граф/связь\n";
		cout << "3-Алгоритм Флойда\n";
		cout << "4-Найти минимальный путь между двумя узлами\n";
		cout << "Выбор действия: ";
		cin >> a;
		switch (a)
		{
		case 0: {
			mode = !mode;
			cout << "Новый тип графа: " << (mode ? "неориентированный" : "ориентированный") << endl;
			break;
		}
		case 1: {
			cout << "Кол-во узлов: ";
			cin >> vectorSize;
			adjList = new vector<pair<int, int>>[vectorSize];
			generateList(adjList, vectorSize);
			printList(adjList, vectorSize);
			break;
		}
		case 2: {
			cout << "Из какого узла: ";
			cin >> from;
			if (from < 0)
			{
				cout << "Значение неверно" << endl;
				break;
			}
			cout << "В какой узел: ";
			cin >> to;
			if (to < 0)
			{
				cout << "Значение неверно" << endl;
				break;
			}
			cout << "Вес грани: ";
			cin >> weight;
			if (weight <= 0 || weight >= 20)
			{
				cout << "Значение неверно" << endl;
				break;
			}
			for (int j = 0; j < vectorSize && !exists; j++)
			{
				for (auto node : adjList[j])
				{
					if ((j == from && node.first == to) || (j == to && node.first == from))
					{
						exists = true;
						break;
					}
				}
			}
			if (exists)
			{
				cout << "Такая связь уже есть" << endl;
				break;
			}
			adjList = addEdge(adjList, from, to, weight, vectorSize);
			printList(adjList, vectorSize);
			break;
		}
		case 3: {
			adjMatrix = convert(adjList, vectorSize);
			printMatrix(adjMatrix, vectorSize);
			minDist = floydWarshall(adjMatrix, vectorSize);
			printMatrix(minDist, vectorSize);
			break;
		}
		case 4: {
			if (minDist.size() == 0)
			{
				cout << "Необходимо использовать метод Флойда\n";
				break;
			}
			cout << "Отправной узел: ";
			cin >> from;
			if (from >= vectorSize || from < 0)
			{
				cout << "Значение неверно\n";
				break;
			}
			cout << "Итоговый узел: ";
			cin >> to;
			if (to >= vectorSize || to < 0)
			{
				cout << "Значение неверно\n";
				break;
			}
			cout << "Минимальный путь: " << minDist[from][to] << endl;
			break;
		}
		default:
			break;
		}
	}
	return 0;
}