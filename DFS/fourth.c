#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
  char* name;
  int edgecount;
  struct edge {
    char namepath[4];
    int distance;
  }* edges; 
};

struct node* graph;
int nodeCt, totalNodes;

struct node* getNode(char* name){
  struct node *ptr = NULL;
  for (int i=0 ; i < totalNodes ; i++){
    if (strcmp((graph + i)-> name, name) == 0)
      ptr = (graph + i);
  }
  return ptr;
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

void addNode(char * x){
  int loc = 0;
  (graph+nodeCt)->name = malloc(sizeof(char) * 10);
  (graph+nodeCt)->name[0] ='\0';
  while (loc < nodeCt && strcmp((graph+loc)->name,x) < 0)
    loc++;
  //move everything over
  if (nodeCt > 0){
    for (int i = nodeCt; i >=loc ; i--){
      struct node * to = (graph+i);
      struct node * from = (graph+i-1);
      strncpy(to->name,from->name,10);
      to->edgecount = from->edgecount;
      for (int j = 0; j < from->edgecount; j++){
	strncpy((to->edges + j)->namepath,(from->edges + j)->namepath,10);
	(to->edges + j)->distance = (to->edges + j)->distance;
      }
      from->edgecount = 0;
    }
  }
  strncpy((graph+loc)->name,x,10);
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

struct result {char val[100];
  struct result *next;
};
struct result *head = NULL;

char* visited;
char* printed;
char* queue;

void breadthfirst(char* top) {
  strcat(visited, top);
  strcat(visited, " ");
  //printf("%s\n", visited);
  char q1 [50];
  while (strlen(queue) > 0) {
    //printf("%s\n", queue);
    int next = strcspn(queue, " ");
    strncpy(q1, queue, next);
    q1[next] = '\0';
    queue = queue + next + 1;
    //printf("%s\n", q1); 
    struct node * ptr = getNode(q1);
    if (strstr(printed, ptr->name) == NULL) {
      strcat (printed, ptr->name);
      strcat (printed, " ");
    }
    struct edge *e = ptr->edges;
    for (int i = 0; i <ptr->edgecount; i++) {
      if (strstr(visited, (e + i)->namepath) == NULL) {
	strcat(visited, (e+i)->namepath);
	strcat(visited, " ");
      }
    }
  }
}

void depthfirst(char * top) {
  struct node *ptr = getNode(top);
  if (strstr (printed, ptr->name) == NULL) {
    strcat(printed, ptr->name);
    strcat(printed, " ");
  }
  
  for (int i = 0; i < ptr->edgecount; i++) {
    if (strstr(printed, (ptr->edges + i)-> namepath) == NULL) {
      strcat (printed, (ptr->edges + i)->namepath);
      strcat (printed, " ");
      depthfirst((ptr->edges + i)->namepath);
    }
  }
}

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
  for (int i = 0; i < totalNodes; i++) {
    struct node *ptr = (graph + i);
    if (strcmp(ptr -> name, t2) != 0){
      for(int j=0; j<ptr->edgecount ;j++){
	if( strcmp((ptr -> edges + j)->namepath, t2) == 0){
	  deg++;
	}
      }
    } else {
      for(int j=0; j < ptr->edgecount ;j++){
	deg++;
      }
    }
  }
  printf("%d\n",deg);
}

void outDegree(char* t1, char* t2){
  int deg = 0;
  for (int i = 0; i < totalNodes; i++){
    struct node *ptr = (graph + i);
    if (strcmp(ptr -> name, t2) == 0){
      for(int j=0; j<ptr->edgecount ;j++){
	deg++;
      }
    }
  }
  printf("%d\n",deg);
}

void inDegree(char* t1, char* t2){
  int deg = 0;
  for (int i = 0; i < totalNodes; i++){
    struct node *ptr = (graph + i);
    if (strcmp(ptr -> name, t2) != 0){
      for(int j=0; j<ptr->edgecount ;j++){
	if( strcmp((ptr -> edges + j)->namepath, t2) == 0){
	  deg++;
	}
      }
    }
  }
  printf("%d\n",deg);
}

int main (int argc, char* argv[]) {
  if (argc == 1) {
    printf("\nNo file recieved");
    return -1;
  }

  if (argc > 3) {
    printf("\nToo many arguments %d recieved", argc);
    return -2;
  }

  FILE* dataFILE;
  char str[1000];
  dataFILE = fopen(argv[1], "r");

  if (NULL == dataFILE) {
    printf("file can't be opened\n");
    return -3;
  }

  fgets(str, 1000, dataFILE); 
  totalNodes = atoi(str);
  // printf("%d read\n", totalNodes);
  graph = (struct node *)malloc(sizeof(struct node) * (totalNodes));
  //printf("read\n");

  for (int i = 0; i < totalNodes; i++) {
    (graph + i)->edges = (struct edge *)malloc(sizeof(struct edge) * (totalNodes));
  }

  nodeCt = 0;
  
  for (int i = 0; i < totalNodes; i++) {
    //printf("in loop : %d\n", i);
    fgets(str, 1000, dataFILE);
    str[strcspn(str, "\n")] = '\0';
    addNode(str);
  }

  while (fgets(str, 1000, dataFILE) != NULL) {
    str[strcspn(str, "\n")] = '\0';
    char* t1 = strtok(str, " ");
    char* t2 = strtok(NULL, " ");
    // printf("adding %s, %s\n",t1,t2);
    addEdge(t1, t2, 0);
    }
  //printgraph();
  // printf("%s\n", argv[2]);
  visited = malloc((sizeof(char) * 30)* (totalNodes * 5));
  printed = malloc((sizeof(char) * 30)* (totalNodes * 5));
  visited[0]='\0';
  printed[0]='\0';
  depthfirst(graph->name);

  for (int i = 0; i < totalNodes; i++) {
    if (strstr(printed, (graph + i)->name) == NULL) {
      //printf("%s\n", printed);
      depthfirst((graph + i)->name);
    }
  }

  printf("%s\n", printed);

  free(visited);
  free(printed);
  for (int i=0 ; i < totalNodes ; i++){
    free((graph + i)-> name);
    free((graph + i)-> edges);
  }
  
  free(graph);
  fclose(dataFILE);
  return 0;
}
