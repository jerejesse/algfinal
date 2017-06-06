#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#define BLACK	1
#define RED     0

typedef struct{
	int xl,xu,yl,yu;
	int layer;
} Shape;


typedef struct{
	int xl,xu,yl,yu;
	int layer;
} Obstacle;

typedef struct{
	int x;
	int y;
	int layer;
	int radius;
} Vias;

typedef struct {
	int xl,xu,yl,yu;
	int layer;	
}line;

typedef struct RBTreeNode
{
	unsigned char color;
	int key;
	struct RBTreeNode *left;
	struct RBTreeNode *right;
	struct RBTreeNode *parent;
}Node, *RBTree;

typedef struct rb_root
{
	Node *node
};
//typedef union{
//	 Shape s;
//	 Obstacle o;
//} Square;

	
int ViaCost=0, Spacing=0, MetalLayers=0, RoutedShapes=0, RoutedVias=0, Obstacles=0;
int Boundary[4]={-1,-1,-1,-1};
//Square* s_head v_head;
Obstacle* o_head;
Vias* v_head;
Shape* s_head;
Obstacle** ol_head;
Shape** sl_head;

void read_data(char* s);
void operation(int l);
RBTreeNode* build_tree(int l);

int main(int argc, char** argv)
{
	if(argc!=2)
		perror("input error");
	read_data(argv[1]);

	for (int i = 0; i < MetalLayers; ++i)
	{
		operation(i);
	}
}

void operation(int l)
{
	RBTreeNode *root;
	root = build_tree(l);
}

void read_data(char* s)
{
	int i;
	int tmp[4]={-1,-1,-1,-1};
	char buff[100];
	FILE *fp = NULL;
	fp = fopen(s,"r");
	fgets(buff,999,fp);
	for(i = 0; buff[i]!='\0'; i++){
		while(buff[i]<='9'&&buff[i]>='0'){
			ViaCost = ViaCost * 10 + buff[i]-'0';
			i++;
		}
		if(ViaCost)
			break;
	}
	fgets(buff,999,fp);
	for(i = 0; buff[i]!='\0'; i++){
		while(buff[i]<='9'&&buff[i]>='0'){
			Spacing = Spacing * 10 + buff[i]-'0';
			i++;
		}
		if(Spacing)
			break;
	}
	fgets(buff,999,fp);
	int j=0;
	for(i = 0; buff[i]!=0; i++){
		while(buff[i]>='0'&&buff[i]<='9'){
			if(Boundary[j]==-1)
				Boundary[j]=buff[i]-'0';
			else
				Boundary[j]=Boundary[j]*10+buff[i]-'0'; 
			i++;
		}
		j++;
	}
	fgets(buff,999,fp);
	for(i = 0; buff[i]!='\0'; i++){
		while(buff[i]<='9'&&buff[i]>='0'){
			MetalLayers = MetalLayers * 10 + buff[i]-'0';
			i++;
		}
		if(MetalLayers)
			break;
	}
	fgets(buff,999,fp);
	for(i = 0; buff[i]!='\0'; i++){
		while(buff[i]<='9'&&buff[i]>='0'){
			RoutedShapes = RoutedShapes * 10 + buff[i]-'0';
			i++;
		}
		if(RoutedShapes)
			break;
	}
	fgets(buff,999,fp);
	for(i = 0; buff[i]!='\0'; i++){
		while(buff[i]<='9'&&buff[i]>='0'){
			RoutedVias = RoutedVias * 10 + buff[i]-'0';
			i++;
		}
		if(RoutedVias)
			break;
	}
	fgets(buff,999,fp);
	for(i = 0; buff[i]!='\0'; i++){
		while(buff[i]<='9'&&buff[i]>='0'){
			Obstacles = Obstacles * 10 + buff[i]-'0';
			i++;
		}
		if(Obstacles)
			break;
	}
	//printf("%d,%d,%d,%d\n",RoutedShapes,RoutedVias,Obstacles,MetalLayers);
	s_head = (Shape*)malloc(RoutedShapes*sizeof(Shape));
	v_head = (Vias*)malloc(RoutedVias*sizeof(Vias));
	o_head = (Obstacle*)malloc(Obstacles*sizeof(Obstacle));
	int k;
	for(k=0;k<RoutedShapes;k++){
		int class = -1;
		j=0;
		fgets(buff,999,fp);
		for(i = 0; buff[i]!=0; i++){
			if(class==-1){
				while(buff[i]>='0'&&buff[i]<='9'){
					if(class==-1)
						class=buff[i]-'0';
					else
						class=class*10+buff[i]-'0'; 
					i++;
				}
			}
			while(buff[i]>='0'&&buff[i]<='9'){
				if(tmp[j]==-1)
					tmp[j]=buff[i]-'0';
				else
					tmp[j]=tmp[j]*10+buff[i]-'0'; 
				i++;
			}
			if(tmp[j]!=-1)
				j++;
			if(j>=4)
				break;
		}
		s_head[k].layer=class;
		class=-1;
		s_head[k].xl=tmp[0];
		s_head[k].yl=tmp[1];
		s_head[k].xu=tmp[2];
		s_head[k].yu=tmp[3];
		for(j=0;j<4;j++)
			tmp[j]=-1;
		
	}
	for(k=0;k<RoutedVias;k++){
		int class = -1;
		j=0;
		fgets(buff,999,fp);
		for(i = 0; buff[i]!=0; i++){
			if(class==-1){
				while(buff[i]>='0'&&buff[i]<='9'){
					if(class==-1)
						class=buff[i]-'0';
					else
						class=class*10+buff[i]-'0'; 
					i++;
				}
			}
			while(buff[i]>='0'&&buff[i]<='9'){
				if(tmp[j]==-1)
					tmp[j]=buff[i]-'0';
				else
					tmp[j]=tmp[j]*10+buff[i]-'0'; 
				i++;
			}
			if(tmp[j]!=-1)
				j++;
			if(j>=2)
				break;
		}
		v_head[k].layer=class;
		class=-1;
		v_head[k].x=tmp[0];
		v_head[k].y=tmp[1];
		for(j=0;j<2;j++){
			tmp[j]=-1;
		}
	}
	for(k=0;k<Obstacles;k++){
		int class = -1;
		j=0;
		fgets(buff,999,fp);
		for(i = 0; buff[i]!=0; i++){
			if(class==-1){
				while(buff[i]>='0'&&buff[i]<='9'){
					if(class==-1)
						class=buff[i]-'0';
					else
						class=class*10+buff[i]-'0'; 
					i++;
				}
			}
			while(buff[i]>='0'&&buff[i]<='9'){
				if(tmp[j]==-1)
					tmp[j]=buff[i]-'0';
				else
					tmp[j]=tmp[j]*10+buff[i]-'0'; 
				i++;
			}
			if(tmp[j]!=-1)
				j++;
			if(j>=4)
				break;
		}
		o_head[k].layer=class;
		class=-1;
		o_head[k].xl=tmp[0];
		o_head[k].yl=tmp[1];
		o_head[k].xu=tmp[2];
		o_head[k].yu=tmp[3];
		for(j=0;j<4;j++)
			tmp[j]=-1;
	}
	fclose(fp);
	sl_head = (Shape**)malloc((MetalLayers+1)*sizeof(Shape*));
	ol_head = (Obstacle**)malloc((MetalLayers+1)*sizeof(Obstacle*));
	sl_head[0] = s_head;
	j=0;
	for (i = 1; i < RoutedShapes; ++i)
	{
		if(s_head[i].layer>s_head[i-1].layer)
			sl_head[s_head[i-1].layer]=&(s_head[i]);
	}
	sl_head[MetalLayers]=&(s_head[RoutedShapes]);
	for (i = 1; i < Obstacles; ++i)
	{
		if(o_head[i].layer>o_head[i-1].layer)
			ol_head[o_head[i-1].layer]=&(o_head[i]);
	}
	ol_head[MetalLayers]=&(o_head[RoutedShapes]);
	//i layer is from sl_head[i-1] to sl_head[i]-1
	for (i = 0; i < RoutedShapes; ++i)
		s_head[i].radius = (xu+yu-xl-yl)/2;
	

}

