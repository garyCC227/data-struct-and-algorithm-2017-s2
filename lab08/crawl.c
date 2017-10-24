// crawl.c ... build a graph of part of the web
// Written by John Shepherd, September 2015
// Uses the cURL library and functions by Vincent Sanders <vince@kyllikki.org>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <curl/curl.h>
#include "stack.h"
#include "set.h"
#include "graph.h"
#include "queue.h"
#include "html.h"
#include "url_file.h"
#include <stdbool.h>

#define BUFSIZE 1024

void setFirstURL(char *, char *);
void normalise(char *, char *, char *, char *, int);
bool beenVisited(Link head, char *str);
bool inGraph(char *str, char **names);

int main(int argc, char **argv)
{
	URL_FILE *handle;
	char buffer[BUFSIZE];
	char baseURL[BUFSIZE];
	char firstURL[BUFSIZE];
	char next[BUFSIZE];
	int  maxURLs;

	if (argc > 2) {
		strcpy(baseURL,argv[1]);
		setFirstURL(baseURL,firstURL);
		maxURLs = atoi(argv[2]);
		//if (maxURLs < 40) maxURLs = 40;
	}
	else {
		fprintf(stderr, "Usage: %s BaseURL MaxURLs\n",argv[0]);
		exit(1);
	}
		
	// You need to modify the code below to implement:
	//
	// add firstURL to the ToDo list
	// initialise Graph to hold up to maxURLs
	// initialise set of Seen URLs
	// while (ToDo list not empty and Graph not filled) {
	//    grab Next URL from ToDo list
	//    if (not allowed) continue
	//    foreach line in the opened URL {
	//       foreach URL on that line {
	//          if (Graph not filled or both URLs in Graph)
	//             add an edge from Next to this URL
	//          if (this URL not Seen already) {
	//             add it to the Seen set
	//             add it to the ToDo list
	//          }
	//       }
    //    }
	//    close the opened URL
	//    sleep(1)
	// }
	Graph g = newGraph(maxURLs);
	Queue q = newQueue();
	enterQueue(q,firstURL);
	Set visited = newSet();
	int seqOfVertex = 0;
	
	while(emptyQueue(q) != 1){	
		strcpy(firstURL,leaveQueue(q));		
		// *to get all next URL 
		if (!(handle = url_fopen(firstURL, "r"))) {
			fprintf(stderr,"Couldn't open %s\n", next);
			exit(1);
		}
		// check URL has been visited?
		if(!beenVisited(visited->elems,firstURL)) continue;
		if(g->nV <= maxURLs){
			// store visited URL into set 
			insertInto(visited, firstURL);
			// store visited URL into graph by sequence
			addVertex(firstURL,g->vertex,seqOfVertex);
			g->nV++;
			seqOfVertex++;
		}else{
			break;
		}
		
		
		while(!url_feof(handle)) {
			url_fgets(buffer,sizeof(buffer),handle);
			//fputs(buffer,stdout);
			int pos = 0;
			char result[BUFSIZE];
			memset(result,0,BUFSIZE);
			while ((pos = GetNextURL(buffer, firstURL, result, pos)) > 0) {
				//printf("Found: '%s'\n",result);
				//Graph not filled or both URLs in Graph)
				if(g->nV < maxURLs && inGraph(result,g->vertex)){
					addEdge(g, firstURL, result);
				}
				//this URL not Seen already
				if(beenVisited(visited->elems, result)){
					enterQueue(q,result);
				}
				memset(result,0,BUFSIZE);
			}
		}
		url_fclose(handle);
		//sleep(1);
		// end *
	
	}	
	showGraph(g, 0);
	return 0;
}

//check if the vertex has been visited
//fix later!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!----------------
bool beenVisited(Link head, char *str)
{
	Link curr = head;
	while (curr != NULL) {
		if(strcmp(curr->val,str) == 0){
			return 0;
		}
		curr = curr->next;
	}
	// this str is not in the set
	return 1;
}

bool inGraph(char *str, char **names){
	int i = 0;
	while(names[i] != '\0'){
		if(strcmp(str,names[i]) == 0) return 1;
		i++;
	}
	// if not in graph, return 0
	return 0;
}
// setFirstURL(Base,First)
// - sets a "normalised" version of Base as First
// - modifies Base to a "normalised" version of itself
void setFirstURL(char *base, char *first)
{
	char *c;
	if ((c = strstr(base, "/index.html")) != NULL) {
		strcpy(first,base);
		*c = '\0';
	}
	else if (base[strlen(base)-1] == '/') {
		strcpy(first,base);
		strcat(first,"index.html");
		base[strlen(base)-1] = '\0';
	}
	else {
		strcpy(first,base);
		strcat(first,"/index.html");
	}
}
