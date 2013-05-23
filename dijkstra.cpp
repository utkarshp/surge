#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<boost/heap/fibonacci_heap.hpp>
#define INFINITY 100000

using namespace boost::heap;

int *dist;

struct node
{
	int id;
	int weight;
	struct node* next;
	
	node()
	{
		id=0;
		weight=0;
		next=NULL;
	}
	node(const int& x)
	{
		id=x;
		weight=0;
		next=NULL;
	}
};


struct node_less 
{
	bool operator () (const node & x, const node & y) const
	{
		return dist[x.id] > dist[y.id];			//for making it a min-heap. For max-heap use "<" operator.
	}

};

typedef fibonacci_heap <node, compare<node_less> > FibonacciHeap;
typedef fibonacci_heap <node, compare<node_less> > :: handle_type HeapHandle;
void print_graph(struct node *list,const int & n)
{
	int i;
	struct node *temp;
	for(i=0;i<n;i++)
	{
		temp=&list[i];
		while(temp!=NULL)
		{
			printf("%d----%d----%d\n",i,temp->weight,temp->id);
			temp=temp->next;
		}
	}
}

void create_edge(struct node *list,const int& node1,const int & node2,const int & weight)
{
	struct node *current= &list[node1];
	struct node *temp=current;

	while(temp->next!=NULL)
		if(temp->next->weight <= weight)
			temp=temp->next;
		else break;
	current=(struct node *)malloc(sizeof(struct node));
	current->next=temp->next;
	temp->next=current;

	current->id = node2;
	current->weight=weight;

}




void dijkstra(struct node *list,int n, int source, int target, int *prev, int *dist)
{
	int i,new_dist;
	struct node current,*neighbour,temp;
	HeapHandle *handle;

	handle = (HeapHandle *)malloc(sizeof(HeapHandle)*n);

	for (i=0;i<n;i++)
		dist[i]=INFINITY;	
	
	dist[source]=0;

	FibonacciHeap Q;
	for (i=0;i<n;i++)
		handle[i]=Q.push(list[i]);

	while (!Q.empty())
	{
		current=Q.top();
		Q.pop();
		if( dist[current.id] == INFINITY || current.id == target) break;
		neighbour = current.next ;
		while (neighbour!=NULL)
		{
			new_dist = dist[current.id] + neighbour->weight;
			if (new_dist < dist[neighbour->id])
			{
				dist[neighbour->id] = new_dist;
				prev[neighbour->id] = current.id;
				Q.update(handle[neighbour->id]);
			}
				neighbour = neighbour->next;
		}
	}

}

int main()
{
	char c;
	printf("Input the number of nodes in the graph ");
	int n,node1,node2,weight,i;
	scanf("%d",&n);

	struct node *list;
	
	list = (struct node *)malloc(sizeof(struct node)*n);

	for (i=0;i<n;i++)
		 list[i] =  node (i);


	printf("Nodes are numbered from 0 to %d\n",n-1);
	printf("Enter the edges in form <node 1> <node 2> <weight>\nEnd the input with an N\n");

	scanf(" %c",&c);
	while(c!='N')
	{
		ungetc((int) c,stdin);
		scanf("%d",&node1);
		scanf("%d",&node2);
		scanf("%d",&weight);
		if (weight >= INFINITY)
		{
			printf("Edge weights too large\n");
			return 0;
		}
		create_edge(list,node1,node2,weight);	
		create_edge(list,node2,node1,weight);
		scanf(" %c",&c);
	}
	print_graph(list,n);

	printf("Please enter node id to start Dijkstra from ");
	scanf("%d",&node1);
	
	printf("Now enter the target node ");
	scanf("%d",&node2);

	int *prev;
	dist = (int *)malloc(sizeof(int)*n);
	prev = (int *)malloc(sizeof(int)*n);	

	dijkstra(list,n,node1,node2,prev,dist);

	for (i=0;i<n;i++)
		printf("%d\t%d\n",list[i].id,dist[list[i].id]);

	return 0;
}
