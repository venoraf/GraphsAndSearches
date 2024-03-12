#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

struct node {
  char* name;
  int edgecount;
  struct edge {
    char namepath[4];
    int distance; } *edges;
};

struct node *graph;
int nodeCt, totalNodes;

struct node* getNode(char* name){
  struct node *ptr = NULL;
  for (int i=0 ; i < totalNodes ; i++){
    //printf("%s, %s\n", (graph + i)-> name, name);
    if (strcmp((graph + i)-> name, name) == 0)
      ptr = (graph + i);
  }
  return ptr;
}

int getNodeInx(char* name) {
  int inx = -1;
  for (int i = 0; i < totalNodes; i++) {
    //printf("%s, %s\n", (graph + i)-> name, name);
    if (strcmp((graph + i)->name, name) == 0)
      inx = i;
  }
  return inx;
}

void addNode(char * x){
  int loc = 0;
  (graph+nodeCt)->name = malloc(sizeof(char) * 4);
  (graph+nodeCt)->name[0] ='\0';
  while (loc < nodeCt && strcmp((graph+loc)->name,x) < 0)
    loc++;
  //move everything over
  if (nodeCt > 0){
    for (int i = nodeCt; i >=loc ; i--){
      struct node * to = (graph+i);
      struct node * from = (graph+i-1);
      strncpy(to->name,from->name,4);
      to->edgecount = from->edgecount;
      for (int j = 0; j < from->edgecount; j++){
	strncpy((to->edges + j)->namepath,(from->edges + j)->namepath,4);
	(to->edges + j)->distance = (to->edges + j)->distance;
      }
      from->edgecount = 0;
    }
  }
  strncpy((graph+loc)->name,x,4);
  (graph+loc)->edgecount = 0;

  nodeCt++;
}

void addEdge(char * x, char* y, int dist){
  struct node * n = getNode(x);
  struct edge * e = n->edges;
  //Find location to insert
  int loc = 0;
  while (loc < n->edgecount && strcmp((e+loc)->namepath,y) < 0)
    loc++;
  //printf("loc: %d, ec: %d, x: %s y: %s\n", loc, n->edgecount, x,y);
  //if(strcmp(x,"A") == 0) printgraph();
  //move everything over
  //printf("%s, %s, %d\n", x, y, loc);
  if (n->edgecount > 0) {
    for (int i = n->edgecount - 1; i >= loc ; i--){
      //printf("moving i%d\n",i);
      strcpy((e+i+1)->namepath,(e+i)->namepath);
      (e+i+1)->distance = (e+i)->distance;
    }
  }
  //memset ((e+loc)->namepath, '\0', 10);
  // printf("after zero\n");
  strcpy((e+loc)->namepath,y);
  (e+loc)->distance = dist;
  n->edgecount++;
}

void printgraph() {
  for (int i=0 ; i < totalNodes ; i++){
    printf("[[%s, %d][",(graph + i)->name, (graph + i)->edgecount);
    for (int j=0; j < (graph + i)->edgecount; j++){
      struct edge *ptr = (graph + i)-> edges + j;
      printf("[%s,%d],",ptr->namepath, ptr->distance);
    }
    printf("]]\n");
  }
}

struct result {char val[100];
  struct result *next;
};
struct result *head = NULL;

void addResult(char* alpha){
  //printf("%s\n",alpha);
  if (head == NULL){
    struct result *r = malloc(sizeof(struct result));
    strcpy(r->val,alpha);
    r->next = NULL;
    head = r;
  } else {
    struct result *tmp = head;
    if (strcmp(tmp->val, alpha) > 0){
      if (!strcmp(tmp->val,alpha)==0){
	struct result *r = malloc(sizeof(struct result));
	strcpy(r->val,alpha);
	r->next = head;
	head = r;
      }
    } else {
      while (tmp->next != NULL && strcmp(alpha,tmp->next->val) > 0)
	tmp = tmp -> next;
      if (((tmp->next == NULL) && !strcmp(tmp->val,alpha)==0) || ((tmp->next != NULL) && !strcmp(tmp->next->val,alpha)==0)){
	struct result *r = malloc(sizeof(struct result));
	strcpy(r->val,alpha);
	r->next = tmp->next;
	tmp->next = r;
      }
    }
  }
}

void printDestroy(){
  struct result *tmp = head;
  while (tmp != NULL){
    if (tmp->next != NULL)
      printf("%s ",tmp->val);
    else
      printf("%s",tmp->val);
    tmp = tmp->next;
    free(head);
    head = tmp;
  }
  printf("\n");
  head = NULL;
};

void adjacency(char* t1, char* t2){
  for (int i = 0; i < totalNodes; i++){
    struct node *ptr = (graph + i);
    if (strcmp(ptr -> name, t2) == 0){
      for(int j=0; j<ptr->edgecount ;j++){
	addResult((ptr -> edges + j)->namepath);
      }
    }
  }
  printDestroy();
}

void degree(char* t1, char* t2){
  int deg = 0;
  for (int i = 0; i < totalNodes; i++){
    struct node *ptr = (graph + i);
    if (strcmp(ptr -> name, t2) != 0){
      for(int j=0; j<ptr->edgecount ;j++){
	if( strcmp((ptr -> edges + j)->namepath, t2) == 0){
	  deg++;
	}
      }
    } else {
      for(int j=0; j<ptr->edgecount ;j++){
	deg++;
      }
    }
  }
  printf("%d\n", deg);
}

void outDegree(char* t1, char* t2) {
  int deg = 0;
  for (int i = 0; i < totalNodes; i++) {
    struct node* ptr = (graph + i);
    if (strcmp(ptr->name, t2) == 0) {
      for (int j = 0; j < ptr->edgecount; j++) {
	deg++;
      }
    }
  }
  printf("%d\n", deg);
}

void inDegree(char* t1, char* t2) {
  int deg = 0;
  for (int i = 0; i < totalNodes; i++) {
    struct node* ptr = (graph + i);
    if (strcmp(ptr->name, t2) != 0) {
      for (int j = 0; j < ptr->edgecount; j++) {
	if (strcmp((ptr->edges + j)->namepath, t2) == 0) {
	  deg++;
	}
      }
    }
  }
  printf("%d\n", deg);
}

char* visited;
char* printed;
char* queue;
void breadthfirst(char* top) {
  strcat(visited, top);
  strcat(visited, " ");
  char q1[50];
  while (strlen(queue) > 0) {
    //printf("%s is %d\n",printed, strlen(queue));
    int next = strcspn(queue, " ");
    strncpy(q1, queue, next);
    q1[next] = '\0';
    queue = queue + next + 1;
    struct node* ptr = getNode(q1);
    if (strstr(printed, ptr->name) == NULL) {
      strcat(printed, ptr->name);
      strcat(printed, " ");
    }
    struct edge* e = ptr->edges;
    for (int i = 0; i < ptr->edgecount; i++) {
      if (strstr(visited, (e + i)->namepath) == NULL) {
	strcat(visited, (e + i)->namepath);
	strcat(visited, " ");
      }
    }
  }
}

void depthfirst(char* top) {
  //printf("in %s -> %s\n", top, printed);
  struct node* ptr = getNode(top);
  if (strstr(printed, ptr->name) == NULL) {
    strcat(printed, ptr->name);
    strcat(printed, " ");
  }
  for (int i = 0; i < ptr->edgecount; i++) {
    if (strstr(printed, (ptr->edges + i)->namepath) == NULL) {
      strcat(printed, (ptr->edges + i)->namepath);
      strcat(printed, " ");
      depthfirst((ptr->edges + i)->namepath);
    }
  }
}

int* arr;

void printsspArray() {
  for (int i = 0; i < nodeCt; i++) {
    if (arr[i] == INT_MAX)
      printf("%s INF\n", (graph + i)->name);
    else
      printf("%s %i\n", (graph+i)->name, arr[i]);
  }
}

int getDagSsp(char *name) {
  struct node *n = getNode(name);
  struct edge *e = n->edges;
  int thisinx = getNodeInx(n->name);
  arr[thisinx] = 0;
  visited[0] = '\0';
  int valchanged = 0;

  while (thisinx < 9999999) {
    strcat(visited, n->name);
    strcat(visited, " ");
    e = n->edges;
    for (int i = 0; i < n->edgecount; i++) {
      int inx = getNodeInx((e + i)->namepath);
      if (arr[inx] > arr[thisinx] + (e + i)->distance) {
	arr[inx] = arr[thisinx] + (e + i)->distance;
	valchanged = 1;
      }
    }

    thisinx = 9999999;
    int dist = 99999999;
    for (int i = 0; i < nodeCt; i++) {
      if (strstr(visited, (graph + i)->name) == NULL && (thisinx = 9999999 || arr[i] < dist)) {
	dist = arr[i];
	thisinx = i;
	n = (graph + i);
      }
    }
  }
  return valchanged;
}

void getDkstraSsp(char* name) {
  struct node* n = getNode(name);
  int inx = getNodeInx(n->name);
  arr[inx] = 0;
  //printf("running for %s\n", name);
  //Bellman ford for n-1 times
  for (int i = 0; i < totalNodes - 1; i++) {
    // loop all nodes
    for (int a = 0; a < totalNodes; a++) {
      struct node *ptr = (graph + a);
      struct edge* e = ptr->edges;
      int finx = getNodeInx(ptr->name);
      for (int b = 0; b < ptr->edgecount; b++) {
	int tinx = getNodeInx((e + b)->namepath);
	if (arr[finx] != INT_MAX && arr[tinx] > arr[finx] + (e + b)->distance) {
	  arr[tinx] = arr[finx] + (e + b)->distance;
	}
      }
      //for (int j = 0; j < nodeCt; j++)
      //  if (arr[j] == INT_MAX)
      //    printf("INF ");
      //  else
      //    printf("%d ", arr[j]);
      //printf("\n");
    }
  }
}

int main(int argc, char* argv[]) {
  if (argc == 1) {
      printf("\nNo arg recieved");
      return -1;
    }
  if (argc > 3) {
      printf("\nToo many arguments: %d where only 1 expected", argc);
      return -2;
    }

  FILE* dataFile;
  char str[1000];

  // open file
  dataFile = fopen(argv[1], "r");

  if (NULL == dataFile) {
    printf("file can't be opened \n");
    return -3;
  }

  fgets(str, 1000, dataFile);
  totalNodes = atoi(str);
  graph = (struct node *)malloc( sizeof(struct node) * totalNodes);
  for (int i=0 ; i < totalNodes ; i++){
    (graph + i)-> edges = (struct edge *)malloc( sizeof(struct edge) * totalNodes);
  }
  nodeCt = 0;
  //create graph
  for (int i=0 ; i < totalNodes ; i++){
    //read the nodes
    fgets(str, 1000, dataFile);
    str[strcspn(str, "\n")] = '\0';
    addNode(str);
  }

  while(fgets(str, 50, dataFile) != NULL){
    str[strcspn(str, "\n")] = '\0';
    char* t1 = strtok(str," ");
    char* t2 = strtok(NULL," ");
    int dist = atoi(strtok(NULL," "));
    //printf("%s, %s \n",t1,t2);
    addEdge(t1,t2,dist);
  }

  fclose(dataFile);
  // open file
  dataFile = fopen(argv[2], "r");
  //printgraph();
  arr = (int*)malloc(nodeCt * sizeof(int*));

  while (fgets(str, 1000, dataFile) != NULL) {
    str[strcspn(str, "\n")] = '\0';
    for (int i = 0; i < nodeCt; i++)
      arr[i] = INT_MAX;
    getDkstraSsp(str);
    printsspArray(arr);
    printf("\n");
  }
  free(arr);
  for (int i=0 ; i < totalNodes ; i++){
    free((graph + i)-> name);
    free((graph + i)-> edges);
  }
  free(graph);
  fclose(dataFile);
  return 0;
}
