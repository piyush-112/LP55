#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <omp.h>

#define MAX_NODES 100

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct Graph {
    int numNodes;
    Node* adjacencyList[MAX_NODES];
} Graph;

// Function to create a new graph node
Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// Function to add an edge to the graph
void addEdge(Graph* graph, int src, int dest) {
    Node* newNode = createNode(dest);
    newNode->next = graph->adjacencyList[src];
    graph->adjacencyList[src] = newNode;
}

// Function for Breadth-First Search
void BFS(Graph* graph, int startNode) {
    bool visited[MAX_NODES] = { false };

    visited[startNode] = true;
    printf("BFS traversal: %d ", startNode);

    #pragma omp parallel
    {
        #pragma omp single nowait
        {
            Node* queue = createNode(startNode);
            
            while (queue != NULL) {
                #pragma omp task
                {
                    int currentNode = queue->data;
                    Node* neighbor = graph->adjacencyList[currentNode];
                    
                    while (neighbor != NULL) {
                        int nextNode = neighbor->data;
                        if (!visited[nextNode]) {
                            #pragma omp critical
                            {
                                visited[nextNode] = true;
                                printf("%d ", nextNode);
                            }
                            #pragma omp task
                            {
                                Node* newNode = createNode(nextNode);
                                newNode->next = queue->next;
                                queue->next = newNode;
                            }
                        }
                        neighbor = neighbor->next;
                    }
                }
                Node* temp = queue;
                queue = queue->next;
                free(temp);
            }
        }
    }
}

// Function for Depth-First Search
void DFSUtil(Graph* graph, int currentNode, bool visited[]) {
    visited[currentNode] = true;
    printf("%d ", currentNode);

    #pragma omp parallel
    {
        #pragma omp single nowait
        {
            Node* neighbor = graph->adjacencyList[currentNode];
            
            while (neighbor != NULL) {
                int nextNode = neighbor->data;
                if (!visited[nextNode]) {
                    #pragma omp task
                    DFSUtil(graph, nextNode, visited);
                }
                neighbor = neighbor->next;
            }
        }
    }
}

void DFS(Graph* graph, int startNode) {
    bool visited[MAX_NODES] = { false };
    DFSUtil(graph, startNode, visited);
}

int main() {
    Graph graph;
    graph.numNodes = 7;
    
    for (int i = 0; i < MAX_NODES; i++) {
        graph.adjacencyList[i] = NULL;
    }
    
    addEdge(&graph, 0, 1);
    addEdge(&graph, 0, 2);
    addEdge(&graph, 1, 3);
    addEdge(&graph, 1, 4);
    addEdge(&graph, 2, 5);
    addEdge(&graph, 2, 6);

    printf("Parallel BFS: \n");
    BFS(&graph, 0);
    printf("\n\nParallel DFS: \n");
    DFS(&graph, 0);

    return 0;
}