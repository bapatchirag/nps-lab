#include <stdio.h>
#define MAXSIZE 100

typedef struct node {
    int weights[MAXSIZE];
    int source[MAXSIZE];
} Router;

void printDistances(Router[], int);

int main() {
    // Initialize matrices and router nodes

    int costMatrix[MAXSIZE][MAXSIZE];
    Router routers[MAXSIZE];

    // Get router count and cost matrix

    int routerCount;
    printf("Enter number of nodes: ");
    scanf("%d", &routerCount);

    printf("Enter the cost matrix: \n");
    for(int i = 0; i < routerCount; i++) {
        for(int j = 0; j < routerCount; j++) {
            scanf("%d", &costMatrix[i][j]);
            costMatrix[i][i] = 0;
            routers[i].weights[j] = costMatrix[i][j];
            routers[i].source[j] = j;
        }
    }

    // Bellman Ford starts here

    int traversedCount = -1;
    while(traversedCount != 0) {
        traversedCount = 0;

        //We choose arbitary vertex k and we calculate the direct distance from the node i to k using the cost matrix nd add the distance from k to node j

        for(int i = 0; i < routerCount; i++) {
            for(int j = 0; j < routerCount; j++) {
                for(int k = 0; k < routerCount; k++) {
                    if(routers[i].weights[j] > costMatrix[i][k] + routers[k].weights[j]) {
                        routers[i].weights[j] = routers[i].weights[k] + routers[k].weights[j];
                        routers[i].source[j] = k;
                        traversedCount++;
                    }
                }
            }
        }
    }

    printf("\n");
    printf("Number of routers: %d\n", routerCount);
    printf("Shortest paths: \n");
    printDistances(routers, routerCount);

    return 0;
}

/**
 * Prints formatted distances
 * @param routerList Array of router nodes
 * @param count Number of routers in list
 * @return Printed router distances and previous node
 */
void printDistances(Router routerList[], int count) {
    for(int i = 0; i < count; i++) {
        printf("For router %d: \n", (i+1));
        for(int j = 0; j < count; j++) {
            printf("Router %d via router %d: %d\n", (j+1), (routerList[i].source[j]+1), routerList[i].weights[j]);
        }
        printf("\n");
    }
}