#include "project.h"
//open vec file
//assign the values to the inputs
//traverse the list and calculate logic.
//when an output is reached write to output file



void readgraph(NODE *Graph,PATTERN *vector,FILE *fres,int Max,int Tot,FAULT *fault_file, int Tfs)
{
	int i = 0;
	int k = 0, j = 0 , l = 0; //set iterators
	int correct[Max + 1],fault[Max + 1];
	while(j < Max + 1)
	{
		correct[j] = fault [j] = 0;
		j = j + 1;
	}
	j = 0;
	while(k < Tot)
	{
		fprintf(fres,"\ninput%d: %s \t",k+1,vector[k].piv);
		fprintf(fres,"\t fault free output%d: ",k+1);
		while(j < Max+1) //iterate through nodes
		{
			i = readnode(Graph,vector[k].piv[i],fres,j, 0, 0 , 0,correct) + i;//iterate through inputs
			j = j + 1;//iterator for graph
		}
		fprintf(fres,"\n");
		j = i = l =  0;
		while(l < Tfs)
		{
			fprintf(fres,"fault %d/%d: ",fault_file[l].nod, fault_file[l].sval);
			fprintf(fres,"fault injected output%d: ",k+1);
			while(j < Max+1) //iterate through nodes
			{
				if(j == fault_file[l].nod)
					{
						i = readnode(Graph,vector[k].piv[i],fres,j,1,fault_file,l,fault) + i;					
					}//********************************
				else if(j != fault_file[l].nod)	 
					{
						i = readnode(Graph,vector[k].piv[i],fres,j,0,fault_file,l,fault) + i;
						
					}//iterate through inputs
				j = j + 1;//iterator for graph
			}
			j = 0;
			while(j < Max + 1)
			{
				if(correct[j] != fault[j])
				{
					fprintf(fres,"\t Fault detected");
					break;
				}
				j = j + 1;

			}
			if(j == Max + 1)
				fprintf(fres, "\t Fault not detected");
			fprintf(fres,"\n");
			j = i = 0;
			l = l + 1;
		}
		l = 0;
		k = k + 1;
	}
	return;
}

int readnode(NODE *graph, char input,FILE *fres,int id,int faulty, FAULT* fault_file, int l,int *output)
{
	if(faulty == 0)//********************************
	{
		if(graph[id].typ == 1)		//input
		{
			if(input != 'x')//if the input node's input isnt x give it the input vector value
			{			
				if(input == '0')
				{
					graph[id].val = 0;
				}
				else if(input == '1')
				{
					graph[id].val = 1;
				}
			}
			else if(input == 'x')
			{
				graph[id].val = 2;//if it is x give it a 2
			}
			return 1;	//if 1 is returned iterate the input vector
		}
		else if(graph[id].typ == 10)		//from
		{
		
			graph[id].val = graph[graph[id].fin->id].val; //give it the value of the fanin
			return 0; //if this is returned dont iterate input vector
		}
		else if((graph[id].typ == 2)||(graph[id].typ == 3)||(graph[id].typ == 4)||(graph[id].typ == 5)||(graph[id].typ == 6)||(graph[id].typ == 7)||(graph[id].typ == 8)||(graph[id].typ == 9))
		{
			processnode(graph,id,graph[id].nfi);	//if this is returned dont iterate input vector
			if(graph[id].po == 1)	//print the output in the file
			{
				if(graph[id].val != 2)
				{
					
					output[id] = graph[id].val;
					fprintf(fres,"%d",graph[id].val);
				}
				else
				{
					output[id] = graph[id].val;
					fprintf(fres,"x");
				}
			}
			return 0;//if this is returned dont iterate input vector
		}
		else
			return;
	}
	else if(faulty == 1)
	{
		graph[id].val = fault_file[l].sval;//********************************
		if(graph[id].typ == 1)
			return 1;
		else
		{
			if(graph[id].po == 1)	//print the output in the file
			{
				if(graph[id].val != 2)
				{
					output[id] = graph[id].val;
					fprintf(fres,"%d",graph[id].val);
				}
				else
				{
					output[id] = graph[id].val;
					fprintf(fres,"x");
				}
			}
			return 0;
		}
	}
 
}

void processnode(NODE* graph,int id, int nfi)//fix by using nfi with a loop to check values for multiple input gates
{
	
	if(graph[id].typ == 2) //and
	{
		LIST *tmp = NULL;
		tmp = graph[id].fin;
		int i = 0;
		//printf("\n%d", nfi);
		if(graph[graph[id].fin->id].val == 2)
		{
			i = i + 1;
		}
		else if(graph[graph[id].fin->id].val == 0)
		{
			graph[id].val = 0;
			return;
		}
		while(tmp->nxt != NULL)//iterate through fan ins
		{
			if(graph[tmp->nxt->id].val == 2)
			{	
				i = i + 1;
			}
			else if(graph[tmp->nxt->id].val == 0)//if there is a 0 set it to 0
			{				
				graph[id].val = 0;
				return;
			}
			tmp = tmp->nxt;
		}
		if(i == 0)//if there are any 2's and no 0's then set it to 2
		{
			graph[id].val = 1;
			return;
		}
		else if(i > 0)
		{
			graph[id].val= 2;//if they are all not 0 or 2, making them 1's, then set it to 1
			return;
		}
			
	}
	else if(graph[id].typ == 3) //nand
	{
		LIST *tmp = NULL;
		tmp = graph[id].fin;
		int i = 0;
		//printf("\n%d", nfi);
		if(graph[graph[id].fin->id].val == 2)
		{
			i = i + 1;
		}
		else if(graph[graph[id].fin->id].val == 0)
		{
			graph[id].val = 1;
			return;
		}
		while(tmp->nxt != NULL)//iterate through fan ins
		{
			if(graph[tmp->nxt->id].val == 2)//count the 2's
			{	
				i = i + 1;
			}
			else if(graph[tmp->nxt->id].val == 0)//if there is a 0 set it to 1
			{				
				graph[id].val = 1;
				return;
			}
			tmp = tmp->nxt;
		}
		if(i == 0)
		{
			graph[id].val = 0;//if they are all 0
			return;
		}
		else if(i > 0)//if there are any 2's and no 0's then set it to 2
		{
			graph[id].val = 2;//if they are all not 0 or 2, making them 1's, then set it to 0
			return;
		}
			
	}
	else if(graph[id].typ == 4) //or
	{
		int i = 0;
		LIST *tmp = NULL;
		tmp = graph[id].fin;
		//printf("\n%d", nfi);
		if(graph[tmp->id].val == 1)
		{
			graph[id].val = 1;
			return;
		}
		else if(graph[tmp->id].val == 2)
			i = i + 1;
		
		while(tmp->nxt != NULL)//iterate through fan ins
		{
			if(graph[tmp->nxt->id].val == 1)//if there is a 1 set it to 1
			{	
				graph[id].val = 1;
				return;
			}
			else if(graph[tmp->nxt->id].val == 2)//check the number of 2's
			{				
				i = i + 1;
			}
			tmp = tmp->nxt;
		}
		if(i > 0)//if there is any 2's but no 1's set to 2
			{graph[id].val = 2;return;}
		else
			{graph[id].val = 0;//if there are no 2's or 1's then they are all 0 set it to 0
		return;}
		
	}
	else if(graph[id].typ == 5) //nor
	{
		int i = 0;
		LIST *tmp = NULL;
		tmp = graph[id].fin;
		//printf("\n%d", nfi);
		if(graph[tmp->id].val == 1)
		{
			graph[id].val = 0;
			return;
		}
		else if(graph[tmp->id].val == 2)
			i = i + 1;
		
		while(tmp->nxt != NULL)//iterate through fan ins
		{
			if(graph[tmp->nxt->id].val == 1)//if there is a 1 set it to 0
			{	
				graph[id].val = 0;
				return;
			}
			else if(graph[tmp->nxt->id].val == 2)//check the number of 2's
			{				
				i = i + 1;
			}
			tmp = tmp->nxt;
		}
		if(i > 0)//if there is any 2's but no 1's set to 2
			{graph[id].val = 2;return;}
		else
			{graph[id].val = 1;//if there are no 2's or 1's then they are all 0 set it to 1
		return;}
	}
	else if(graph[id].typ == 6) //xor
	{
		int i = 0, j = 0,k = 0;
		LIST *tmp = NULL;
		tmp = graph[id].fin;
		printf("\n%d", nfi);
		if(graph[tmp->id].val== 1)
		{
			i = i + 1;
		}
		else if(graph[tmp->id].val == 2)
		{
			j = j + 1;
		}
		else if(graph[tmp->id].val == 0)
			{k = k + 1;}
		while(tmp->nxt != NULL)//iterate through fan ins
		{
			if(graph[tmp->nxt->id].val == 1)//check the number of 1's
			{	
				i = i + 1;
			}
			else if(graph[tmp->nxt->id].val == 2)//check the number of 2's
			{				
				j = j + 1;
			}
			else if(graph[tmp->nxt->id].val == 0)//check the number of 0's
				k = k + 1;
			tmp = tmp->nxt;
		}
		if(i == nfi)//if they are all 1's set it to 0
			{graph[id].val = 0;}
		else if(k == graph[id].nfi)
			{graph[id].val = 0;}//if there are all 0's set to 0
		else if((i + j)==nfi && i > 0 && j > 0)
			{graph[id].val = 2;}//if they are all 1 and 2 or 0 and 2 set it to 2
		else if((j + k)==nfi && j > 0 && k > 0)
			{graph[id].val = 2;}
		else
			{graph[id].val = 1;}//if there are 1's and or 2's but not all 1's then set to 1
		return;

	}
	else if(graph[id].typ == 7) //xnor
	{
		int i = 0, j = 0,k =0;
		LIST *tmp = NULL;
		tmp = graph[id].fin;
		printf("\n%d", nfi);
		if(graph[tmp->id].val== 1)
		{
			i = i + 1;
		}
		else if(graph[tmp->id].val == 2)
		{
			j = j + 1;
		}
		else if(graph[tmp->id].val == 0)
			{k = k + 1;}
		while(tmp->nxt != NULL)//iterate through fan ins
		{
			if(graph[tmp->nxt->id].val == 1)//check the number of 1's
			{	
				i = i + 1;
			}
			else if(graph[tmp->nxt->id].val == 2)//check the number of 2's
			{				
				j = j + 1;
			}
			else if(graph[tmp->nxt->id].val == 0)//check the number of 0's
				k = k + 1;
			tmp = tmp->nxt;
		}
		if(i == nfi)//if they are all 1's set it to 1
			{graph[id].val = 1;}
		else if(k == nfi)
			{graph[id].val = 1;}//if there are all 0's set to 1
		else if((i + j) == nfi && i > 0 && j > 0)
			{graph[id].val = 2;}
		else if((j + k) == nfi && j > 0 && k > 0)
			{graph[id].val = 2;}
		else
			{graph[id].val = 0;}//if there are 1's and or 2's but not all 1's then set to 1
		return;
	}
	else if(graph[id].typ == 8) //buff
	{	
		graph[id].val = graph[graph[id].fin->id].val;//give it the value of the fanin
	}

	else if(graph[id].typ == 9) //not
	{
		if(graph[graph[id].fin->id].val == 2)//if it is a dont care give it 2
			{graph[id].val = 2;}
		else if(graph[graph[id].fin->id].val == 1)//give it the opposite value of the input
			{graph[id].val = 0;}
		else if(graph[graph[id].fin->id].val == 0)
			{graph[id].val = 1;}
	}
	return;

}

int Readfault(FILE *ffau,FAULT *struck)
{
	char line[Mlin];
	int i = 0,j = 0,k = 0;
	while(i < Mft)
	{
		struck[i].nod = struck[i].sval = 0;
		i = i + 1;
	}
	i = 0;
	fgets(line,Mlin,ffau);
	while(!feof(ffau))
	{
		sscanf(line, "%d/%d",&j,&k); 
		struck[i].nod = j;
		struck[i].sval = k;
		fgets(line,Mlin,ffau);
		i = i + 1;
	}
	return i;
}//end of Readfault
//new
