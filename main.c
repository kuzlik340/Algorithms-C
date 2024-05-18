
#include <stdio.h>
#include <stdlib.h>

int isFirst = 0;
struct ListNode {
    int dest;
    int weight;
    struct ListNode* next;
}typedef ListNode;

struct Head {
    struct ListNode *head;
}typedef Head;

struct Graph {
    int size;
    struct Head* array;
}typedef Graph;

typedef struct {
    int distance;
    int parent;
    int index;
} HeapNode;

typedef struct {
    HeapNode *elements;
    int capacity;
    int size;
} MinHeap;

HeapNode newHeapNode(int vertex, int distance) {
    HeapNode node;
    node.parent = vertex;
    node.distance = distance;
    node.index = -1;
    return node;
}

MinHeap* createMinHeap(int capacity) {
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    minHeap->elements = (HeapNode*)malloc(capacity * sizeof(HeapNode));
    minHeap->capacity = capacity;
    minHeap->size = 0;
    return minHeap;
}

void swapHeapNode(HeapNode *a, HeapNode *b) {
    // After swapping, the indices of the nodes need to be updated
    int tempParent = a->parent;
    a->parent = b->parent;
    b->parent = tempParent;

    int tempDistance = a->distance;
    a->distance = b->distance;
    b->distance = tempDistance;
}

void minHeapify(MinHeap *minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->elements[left].distance <= minHeap->elements[smallest].distance) {
        smallest = left;
    }
    if (right < minHeap->size && minHeap->elements[right].distance <= minHeap->elements[smallest].distance) {
        smallest = right;
    }
    if (smallest != idx) {
        minHeap->elements[minHeap->elements[smallest].parent].index = idx;
        minHeap->elements[minHeap->elements[idx].parent].index = smallest;
        swapHeapNode(&minHeap->elements[idx], &minHeap->elements[smallest]);
        minHeapify(minHeap, smallest);
    }
}


HeapNode extractMin(MinHeap *minHeap) {
    if (minHeap->size <= 0) {
        return newHeapNode(-1, 0);
    }
    HeapNode root = minHeap->elements[0];

    if (minHeap->size == 1){
        minHeap->size--;
        return root;
    }
    minHeap->elements[minHeap->elements[minHeap->size - 1].parent].index = 0;
    minHeap->elements[minHeap->elements[0].parent].index = minHeap->size - 1;
    swapHeapNode(&minHeap->elements[0], &minHeap->elements[minHeap->size - 1]);
    minHeap->size--;
    minHeapify(minHeap, 0);
    return root;
}


void decreaseKey(MinHeap *minHeap, int vertex, int dist) {
    int i = minHeap->elements[vertex].index;
    minHeap->elements[i].distance = dist;

    while (i != 0 && minHeap->elements[(i - 1) / 2].distance >= minHeap->elements[i].distance) {
        minHeap->elements[minHeap->elements[i].parent].index = (i - 1) / 2;
        minHeap->elements[minHeap->elements[(i - 1) / 2].parent].index = i;
        swapHeapNode(&minHeap->elements[i], &minHeap->elements[(i - 1) / 2]);

        i = (i - 1) / 2;
    }
}



void insertHeap(MinHeap *minHeap, int vertex, int dist) {
    if (minHeap->size == minHeap->capacity) {
        return;
    }

    int i = minHeap->size;
    minHeap->elements[i].distance = dist;
    minHeap->elements[i].parent = vertex;
    minHeap->elements[i].index = vertex;
    minHeap->size++;

}

int isEmpty(MinHeap *minHeap) {
    return minHeap->size == 0;
}

ListNode* newListNode(int dest, int weight) {
    ListNode* newNode = (ListNode*) malloc(sizeof(ListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}


Graph* createGraph(int size) {
    Graph* graph = (Graph*) malloc(sizeof(Graph));
    graph->size = size;
    graph->array = (Head*) malloc(size * sizeof(Head));
    int i;
    for (i = 0; i < size; ++i)
        graph->array[i].head = NULL;

    return graph;
}


void addEdge(Graph* graph, int src, int dest, int weight) {
    ListNode* newNode = newListNode(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;

    newNode = newListNode(src, weight);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}

void insert(Graph* graph, int src, int dest, int weight) {
    ListNode* temp = graph->array[src].head;
    while(temp != NULL){
        if(temp->dest == dest || dest == src){
            if(!isFirst){
                printf("insert %d %d failed", src, dest);
                isFirst = 1;
            }
            else{
                printf("\ninsert %d %d failed", src, dest);
            }
            return;
        }
        temp = temp -> next;
    }
    addEdge(graph, src, dest, weight);
}

int delete(Graph* graph, int src, int dest){
    ListNode* temp1 = graph->array[src].head;
    ListNode* temp2 = graph->array[src].head;
    while(temp1 != NULL){
        if(temp1->dest == dest){
            if(temp1 == graph->array[src].head){
                graph->array[src].head = temp1->next;
                free(temp1);
                return 1;
            }
            temp2 -> next = temp1 -> next;
            free(temp1);
            return 1;
        }
        temp2 = temp1;
        temp1 = temp1 -> next;
    }
    if(!isFirst){
        printf("delete %d %d failed", src, dest);
        isFirst = 1;
    }
    else{
        printf("\ndelete %d %d failed", src, dest);
    }
    return 0;
}


int update(Graph* graph, int src, int dest, int weight_new){
    ListNode* temp = graph->array[src].head;
    while(temp != NULL){
        if(temp->dest == dest){
            int m = temp->weight + weight_new;
            if(m < 0){
                if(!isFirst){
                    printf("update %d %d failed", src, dest);
                    isFirst = 1;
                }
                else{
                    printf("\nupdate %d %d failed", src, dest);
                }
                return 0;
            }
            else{
                temp->weight = m;
                return 1;
            }
        }
        temp = temp -> next;
    }
    if(!isFirst){
        printf("update %d %d failed", src, dest);
        isFirst = 1;
    }
    else {
        printf("\nupdate %d %d failed", src, dest);
    }
    return 0;
}
void freeMinHeap(MinHeap* minHeap) {
    if (minHeap == NULL) return;
    free(minHeap->elements);  // Assuming you have an array of elements in the heap.
    free(minHeap);
}


void dijkstraSearch(Graph* graph, int src, int distance[], int predecessors[]) {
    MinHeap* minHeap = createMinHeap(graph->size);
    for (int v = 0; v < graph->size; ++v) {
        distance[v] = 999999;
        predecessors[v] = -1;
        insertHeap(minHeap, v, distance[v]);
    }

    distance[src] = 0;
    decreaseKey(minHeap, src, distance[src]);

    while (!isEmpty(minHeap)) {
        HeapNode minNode = extractMin(minHeap);
        int u = minNode.parent;

        ListNode* pCrawl = graph->array[u].head;
        while (pCrawl != NULL) {
            int v = pCrawl->dest;
            int newDist = distance[u] + pCrawl->weight;

            if (minHeap->elements[v].index < minHeap->size &&  newDist < distance[v]) {
                distance[v] = newDist;
                predecessors[v] = u;
                decreaseKey(minHeap, v, newDist);
            }
            pCrawl = pCrawl->next;
        }
    }
    freeMinHeap(minHeap);
}


void printPath(int predecessors[], int dest) {
    int path[100];
    int path_length = 0;

    for (int current = dest; current != -1; current = predecessors[current]) {
        path[path_length++] = current;
    }

    printf("[");
    for (int i = path_length - 1; i > 0; i--) {
        printf("%d, ", path[i]);
    }
    printf("%d]", path[0]);
}

void search(Graph* graph, int src, int dest) {
    int distance[graph->size];
    int predecessors[graph->size];
    dijkstraSearch(graph, src, distance, predecessors);
    if(isFirst){
        printf("\n");
    }
    if(distance[dest] == 999999){
        printf("search failed");
    }
    else {
        printf("%d: ", distance[dest]);
        printPath(predecessors, dest);
        isFirst =1;
    }
}

int main() {
    int N, M;
    int src, dst, weight, weight_new;
    char c;
    scanf("%d %d", &N, &M);
    Graph* graph = createGraph(N);
    for(int i = M; i > 0; i--){
        scanf(" (%d, %d, %d)", &src, &dst, &weight);
        insert(graph, src, dst, weight);
    }
    while(scanf(" %c ", &c) != EOF){
        if(c == 'i'){
            scanf("%d %d %d", &src, &dst, &weight);
            insert(graph, src, dst, weight);
        }
        else if(c == 'd'){
            scanf("%d %d", &src, &dst);
            int g = delete(graph, src, dst);
            if(g){
                delete(graph, dst, src);
            }
        }
        else if(c == 'u'){
            scanf("%d %d %d", &src, &dst, &weight_new);
            int g = update(graph, src, dst, weight_new);
            if(g){
                update(graph, dst, src, weight_new);
            }
        }
        else if(c == 's'){
            scanf("%d %d", &src, &dst);
            search(graph, src, dst);
        }
    }


    return 0;
}