#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include<string.h>
#include<ctype.h>
#define MAX_NODES 10
#define MAX_DEVICES 10
#define MAX_STATIONS 10
typedef struct Node
{
    char* name;
    char* type;
    int distance;
    int capacity;
    int num_devices;
    int visited;
    float speed;
    struct Edge* edges[MAX_NODES];
    struct Node* devices[MAX_DEVICES];
}Node;

typedef struct Edge
{
    int weight;
    struct Node* to;
}Edge;

typedef struct Graph
{
    int num_nodes;
    Node* nodes[MAX_NODES];
}Graph;

Node* maximum(Graph* graph);

Node* create_node(char* name,char* type)
{
    Node* node;
    node = (Node*)malloc(sizeof(Node));
    node->name= strdup(name);
    node->type=strdup(type);
    node->distance = INT_MAX;
    node->visited = 0;
    node->num_devices=0;
    if(strcmp(node->type,"BS")==0)
    {
        node->speed=rand()%100;
        //printf("Enter Capacity of Base station\n");
        node->capacity=2;//rand()%5;
       // scanf("%d",&node->capacity);
    }
    else
    {
        node->speed=0;
        node->capacity=0;
    }
    for(int i = 0; i < MAX_NODES; i++)
    {
        node->edges[i] = NULL;
        node->devices[i] = NULL;
    }
    return node;
}

Edge* create_edge(int weight, Node* to)
{
    Edge* edge = (Edge*)malloc(sizeof(Edge));
    edge->weight = weight;
    edge->to = to;
    return edge;
}

void add_node(Graph* graph,char* name,char* type)
{
    Node* node = create_node(name,type);
    graph->nodes[graph->num_nodes++] = node;
}

void add_edge(Node* node, int weight, Node* to)
{
    Edge* edge = create_edge(weight, to);
    for (int i = 0; i < MAX_NODES; i++)
    {
        if(node->edges[i]==NULL)
        {
            node->edges[i]=edge;
            break;
        }
    }
}

Graph* init_graph()
{
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->num_nodes = 0;
    for (int i = 0; i < MAX_NODES; i++)
        graph->nodes[i] = NULL;

    return graph;
}

Node* min_distance_node(Graph* graph)
{
    Node* min_node = NULL;
    int min_distance = INT_MAX;
    for (int i = 0; i < graph->num_nodes; i++)
    {
        Node* node = graph->nodes[i];
        if (!node->visited && node->distance < min_distance)
        {
            min_node = node;
            min_distance = node->distance;
        }
    }
    return min_node;
}

int dijkstra(Graph* graph, Node* start, Node* end)
{
    /*Node* max=NULL;
    printf("\n%d %d",end->num_devices,end->capacity);
    if(end->capacity>end->num_devices)
    {end->devices[end->num_devices++]=start;}
    else{
    end->visited=1;
    max=maximum(graph);printf("\n2nd maximum : %0.2f %d %d\n",max->speed,max->num_devices,max->capacity);
    max->devices[max->num_devices++]=start;printf("\n%d ",max->num_devices);}*/
    int distance;
    start->distance = 0;
    Node* neighbor;
    for (int i = 0; i < graph->num_nodes; i++)
    {
        Node* current = min_distance_node(graph);
        current->visited = 1;

        for (int j = 0; j < MAX_NODES; j++)
        {
            Edge* edge = current->edges[j];
            if (edge == NULL)
                break;
            neighbor = edge->to;
            distance = current->distance + edge->weight;
            if (!neighbor->visited && distance < neighbor->distance)
            {
                neighbor->distance = distance;
            }
            if(neighbor==end)
            {
                return neighbor->distance;
            }

        }
    }
}
void display(Graph* graph,FILE *fp3)
{
    for (int i = 0; i < graph->num_nodes; i++)
    {
        Node* node = graph->nodes[i];
        fprintf(fp3,"Name: %s: Network Speed: %0.2f\n", node->name,node->speed);
        for (int j = 0; j < MAX_NODES; j++)
        {
            if (node->edges[j] != NULL)
            {
                fprintf(fp3,"  %s (%d)\n", node->edges[j]->to->name, node->edges[j]->weight);
            }
        }
    }
}
Node* maximum(Graph* graph)
{
    int i;
    Node* max_node=NULL;
    float max=0;
    for(i=0;i<graph->num_nodes;i++)
    {
        Node* temp=graph->nodes[i];
        if(temp->speed>max&&!temp->visited)
        {
            max=temp->speed;
            max_node = temp;
        }
    }
    return max_node;
}
/*void add_device_to_station(Device* device, Station* station)
{
    if(station->num_devices < station->capacity)
        station->devices[station->num_devices++] = device;
    else
       printf("Error: Station %s is at full capacity.\n", station->name);
}*/
void add_users_to_station(Graph* graph,FILE* fp3)
{
    printf("\n");
    int i,j=0,d,k;
    Node* temp;
    Node* TEMP;
    Node* TEMP1;
    for(i=0;i<graph->num_nodes;i++)
    {
        temp=graph->nodes[i];
        if(strcmp(temp->type,"USER")==0)

        {
            //User_nodes[j]=temp;
            TEMP=maximum(graph);
            if(TEMP->capacity>TEMP->num_devices)
            {
                TEMP->devices[TEMP->num_devices++]=temp;
                fprintf(fp3,"\nBS WITH MAX SPEED :%s (%0.2f)\n",TEMP->name,TEMP->speed);
                d=dijkstra(graph,temp,TEMP);
                fprintf(fp3,"distance:%d con_device_name: %s \n",d,TEMP->devices[j]->name);
                j++;
            }
            else
            {
                for(k=0;i<graph->num_nodes;k++)
                {
                    if(graph->nodes[k]->speed==TEMP->speed)
                    {
                        graph->nodes[k]->visited=1;
                        fprintf(fp3,"\n%s\n",graph->nodes[k]->name);
                        break;
                    }
                }
                k=0;
                TEMP1=maximum(graph);
                TEMP1->devices[TEMP1->num_devices++]=temp;
                fprintf(fp3,"\nBS WITH MAX SPEED : %s (%0.2f)\n",TEMP1->name,TEMP1->speed);
                d=dijkstra(graph,temp,TEMP1);
                fprintf(fp3,"distance:%d con_device_name: %s \n",d,TEMP1->devices[k]->name);
                k++;
            }
        }
    }
}

/*BS1 BS 5
BS2 BS 5
BS3 BS 5
USER1 USER
USER2 USER*/

int main()
{
    char name[10];
    char type[10];
    int i,j;
    FILE *fp1;
    fp1=fopen("5g.txt","r");
    Graph* graph = init_graph();
    for(i=0;i<4;i++)
    {
        fscanf(fp1,"%s %s",name,type);
        add_node(graph,name,type);
        for(j=0;j<graph->num_nodes;j++)
        {
            if(i==j)
            {
                continue;//add_edge(graph->nodes[i],0,graph->nodes[j]);
            }
            else
            {
            add_edge(graph->nodes[i],rand()%10,graph->nodes[j]);
            add_edge(graph->nodes[j],rand()%10,graph->nodes[i]);
            }
        }
    }
    fclose(fp1);
    //add_node(graph,"USER 1","USER");
    //add_node(graph,"USER 2","USER");
    /*add_node(graph,"NETWORK PROVIDER 3","BS");
    add_node(graph,"NETWORK PROVIDER 4","BS");
    add_node(graph,"USER1","USER");*/
    FILE *fp2;
    fp2=fopen("5g2.txt","r");
    for(i=0;i<2;i++)
    {
        fscanf(fp2,"%s %s",name,type);
        add_node(graph,name,type);
        add_edge(graph->nodes[4+i],rand()%10,graph->nodes[0]);
    }
    fclose(fp2);
    FILE *fp3;

    fp3=fopen("5output.txt","w");
    display(graph,fp3);
    //fclose(fp3);
    printf("\n");
    add_users_to_station(graph,fp3);
    fclose(fp3);
    /*Node* temp=maximum(graph);
    fprintf(fp3,"\nMAXIMUM NETWORK SPEED: %0.2f\n",temp->speed);


    int d=dijkstra(graph,graph->nodes[3],temp);
    fprintf(fp3,"\nSTATION NAME: %s \tdevice_name:%s\ttype:%s no of devices %d\n",temp->name,temp->devices[0]->name,temp->devices[0]->type,temp->num_devices);
    fprintf(fp3,"\nDistance : %d\n",d);
    */
    return 0;
}
