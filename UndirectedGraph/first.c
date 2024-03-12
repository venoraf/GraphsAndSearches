#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
  char* name;
  int edgecount;
  struct edge {
    char namepath[15];
    int distance;
  }* edges; 
};

struct node* graph;
int nodeCt, totalNodes;

void printgraph() {
  for (int i=0 ; i < totalNodes ; i++) {
    printf("[[%s, %d][",(graph + i)->name, (graph + i)->edgecount);
    for (int j=0; j < (graph + i)->edgecount; j++) {
      struct edge *ptr = (graph + i)-> edges + j;
      printf("[%s,%d],",ptr->namepath, ptr->distance);
    }
    printf("]]\n");
  }
}

struct node* getNode(char* name){
  struct node *ptr = NULL; 
  for (int i = 0; i < totalNodes; i++) {
    if (strcmp((graph + i)->name, name) == 0)
      ptr = (graph + i); 
  }
  return ptr;
}

struct result {char val[100];
  struct result *next;
};
struct result *head = NULL;

void addResult(char* alpha){
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
    if (strcmp(ptr -> name, t2) != 0){
      for(int j=0; j<ptr->edgecount; j++){
	if( strcmp((ptr -> edges + j)->namepath, t2) == 0){
	  addResult(ptr-> name);
	}
      }
    } else {
      for(int j=0; j < ptr->edgecount; j++){
	addResult((ptr->edges + j)->namepath);
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
      for(int j=0; j < ptr->edgecount ;j++){
	if( strcmp((ptr -> edges + j)->namepath, t2) == 0){
	  deg++;
	}
      }
    } else {
      for(int j=0; j < ptr->edgecount; j++){
	deg++;
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
  FILE* pathFILE;
  char str[1000];
  dataFILE = fopen(argv[1], "r");

  if (NULL == dataFILE) {
    printf("file can't be opened\n");
    return -3;
  }

  fgets(str, 1000, dataFILE); 
  totalNodes = atoi(str);
  graph = malloc(sizeof(struct node) * totalNodes);
  
  for (int i = 0; i < totalNodes; i++) {
    fgets(str, 1000, dataFILE);
    str[strcspn(str, "\n")] = '\0';
    (graph + i) -> name = malloc(sizeof(char) * 15);
    (graph + i) -> edges = malloc(sizeof(struct edge) * totalNodes);
    strcpy((graph + i) -> name, str);
    (graph+i)->edgecount=0;
    for(int j=0; j<totalNodes; j++){
      ((graph+i)->edges+j)->distance=0;
    }
  }
  for (int i = 0; i < totalNodes; i++) {
    
  }

  while (fgets(str, 1000, dataFILE) != NULL) {
    str[strcspn(str, "\n")] = '\0';
    char* t1 = strtok(str, " ");
    char* t2 = strtok(NULL, " ");
    struct node* ptr = getNode(t1);
    struct edge* edgeptr = (ptr->edges) + (ptr->edgecount);
    strcpy(edgeptr->namepath, t2);
    ptr->edgecount++;
    }

  pathFILE = fopen(argv[2], "r");

  while(fgets(str, 1000, pathFILE) != NULL){
    str[strcspn(str, "\n")] = '\0';
    char* t1 = strtok(str," ");
    char* t2 = strtok(NULL," ");
    if( strcmp(t1,"a") ==0){
      adjacency(t1,t2);
    } else if (strcmp(t1,"d") == 0 ){
      degree(t1,t2);
    }
  }
  
  for (int i=0 ; i < totalNodes ; i++){
    free((graph + i)-> name);
    free((graph + i)-> edges);
  }
  
  free(graph);
  fclose(dataFILE);
  fclose(pathFILE);
  return 0;
}
