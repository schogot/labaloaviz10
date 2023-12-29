#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <queue>
#include <clocale>
#include <limits.h>

#define MAX_NODES 400

void generateRandomGraph(int graph[MAX_NODES][MAX_NODES], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            if (i == j) {
                graph[i][j] = 0; // Нет петель
            }
            else {
                int random_value = rand() % 2; // Генерируем 0 или 1
                graph[i][j] = random_value;
                graph[j][i] = random_value; // Граф неориентированный
            }
        }
    }
}

int minDistance(int dist[], int sptSet[], int V) {
    int min = INT_MAX, min_index;

    for (int v = 0; v < V; v++) {
        if (sptSet[v] == 0 && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }

    return min_index;
}

void dijkstra(int graph[MAX_NODES][MAX_NODES], int src, int V, int* dist) {
    int* sptSet = (int*)malloc(V * sizeof(int));

    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        sptSet[i] = 0;
    }

    dist[src] = 0;

    for (int count = 0; count < V - 1; count++) {
        int u = minDistance(dist, sptSet, V);
        sptSet[u] = 1;

        for (int v = 0; v < V; v++) {
            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    free(sptSet);
}

void bfs(int graph[MAX_NODES][MAX_NODES], int n, int startNode) {
    bool visited[MAX_NODES] = { false };
    int distances[MAX_NODES] = { 0 };
    std::queue<int> q;

    q.push(startNode);
    visited[startNode] = true;

    while (!q.empty()) {
        int currentNode = q.front();
        q.pop();

        for (int i = 0; i < n; i++) {
            if (graph[currentNode][i] && !visited[i]) {
                q.push(i);
                visited[i] = true;
                distances[i] = distances[currentNode] + 1;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        printf("Расстояние от узла %d до узла %d: %d\n", startNode, i, distances[i]);
    }
}

int findEccentricity(int graph[MAX_NODES][MAX_NODES], int n, int vertex) {
    bool visited[MAX_NODES] = { false };
    int distances[MAX_NODES] = { 0 };
    std::queue<int> q;

    q.push(vertex);
    visited[vertex] = true;

    while (!q.empty()) {
        int currentNode = q.front();
        q.pop();

        for (int i = 0; i < n; i++) {
            if (graph[currentNode][i] && !visited[i]) {
                q.push(i);
                visited[i] = true;
                distances[i] = distances[currentNode] + 1;
            }
        }
    }

    int eccentricity = 0;
    for (int i = 0; i < n; i++) {
        if (distances[i] > eccentricity) {
            eccentricity = distances[i];
        }
    }

    return eccentricity;
}

int findRadius(int graph[MAX_NODES][MAX_NODES], int n) {
    int minEccentricity = INT_MAX;

    for (int i = 0; i < n; i++) {
        int eccentricity = findEccentricity(graph, n, i);

        if (eccentricity < minEccentricity) {
            minEccentricity = eccentricity;
        }
    }

    return minEccentricity;
}

int findDiameter(int graph[MAX_NODES][MAX_NODES], int n) {
    int maxEccentricity = 0;

    for (int i = 0; i < n; i++) {
        int eccentricity = findEccentricity(graph, n, i);

        if (eccentricity > maxEccentricity) {
            maxEccentricity = eccentricity;
        }
    }

    return maxEccentricity;
}

void findCentralVertices(int graph[MAX_NODES][MAX_NODES], int n, int radius) {
    printf("Центральные вершины:\n");
    for (int i = 0; i < n; i++) {
        int eccentricity = findEccentricity(graph, n, i);

        if (eccentricity == radius) {
            printf("%d\n", i);
        }
    }
}

void findPeripheralVertices(int graph[MAX_NODES][MAX_NODES], int n, int diameter) {
    printf("Периферийные вершины:\n");
    for (int i = 0; i < n; i++) {
        int eccentricity = findEccentricity(graph, n, i);

        if (eccentricity == diameter) {
            printf("%d\n", i);
        }
    }
}

bool isIsolatedGraph(int graph[MAX_NODES][MAX_NODES], int n) {
    for (int i = 0; i < n; i++) {
        bool hasConnection = false;
        for (int j = 0; j < n; j++) {
            if (graph[i][j] == 1) {
                hasConnection = true;
                break;
            }
        }
        if (!hasConnection) {
            return true; // Найдена изолированная вершина
        }
    }
    return false; // Граф не изолированный
}

int main() {
    setlocale(LC_ALL, "Russian");
    srand(time(NULL)); // Инициализируем генератор случайных чисел
    int startNode = 0;
    int radius;
    int diameter;
    int n; // Количество узлов в графе
    printf("Введите количество ребер в графе: ");
    scanf_s("%d", &n);

    int graph[MAX_NODES][MAX_NODES] = { 0 };

    generateRandomGraph(graph, n);

    if (!isIsolatedGraph(graph, n)) {
        printf("Матрица смежности графа:\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                printf("%d ", graph[i][j]);
            }
            printf("\n");
        }

        printf("Введите начальную вершину для поиска расстояний: ");
        scanf_s("%d", &startNode);

        bfs(graph, n, startNode);
        radius = findRadius(graph, n);
        diameter = findDiameter(graph, n);

        printf("Радиус графа: %d\n", radius);
        printf("Диаметр графа: %d\n", diameter);

        findCentralVertices(graph, n, radius);
        findPeripheralVertices(graph, n, diameter);
    }
    else {
        printf("Граф изолированный. Информация недоступна.\n");
    }

    return 0;
}
