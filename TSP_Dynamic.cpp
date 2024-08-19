#include <iostream>
#include <time.h>
using namespace std;

int iterations;
int vertexNo;
int* edgeLen;
int* bestPath;
int bestPathLen;

int sizeOfSet(long long int set){
	//funkcja zwraca ile wierzcho�k�w znajduje si� w zestawie zakodowanym jako "set"
	int result = 0;
	while(set>0){
		result += set%2;
		set /= 2;
	}
	return result;
}

int NthVertex(long long int set, int n){
	//funkcja zwraca indeks N-tego wierzcho�ka w zestawie "set"
	int result = 1;
	while(true){
		if(set%2 == 1) n--;
		if(n == -1) return result;
		result++;
		set /= 2;
	}
}

int findNOfVertex(long long int set, int vertex){
	//funkcja zwraca kt�rym wierzcho�kiem w zestawie "set" jest wierzcho�ek "vertex"
	int result = 0;
	vertex--;
	while(vertex>0){
		if(set%2==1) result++;
		set /= 2;
		vertex--;
	}
	return result;
}

bool main_algorithm(){
	bool safe = true;
	long long int setsNo = 1<<(vertexNo-1);
	//G[A][B][C], gdzie
	//A - zakodowany zestaw wierzcho�k�w (licz�c od wierzcho�ka 1 na najmniej znacz�cym bicie)
	//B - kt�ry z kolei wierzcho�ek w zakodowanym zestawie jest ostatnim odwiedzonym
	//C=0 oznacza d�ugo�� najkr�tszej �cie�ki przechodz�cej przez zestaw a, ko�cz�c na wierzcho�ku b
	//C=1 oznacza przedostatni wierzcho�ek takiej �cie�ki (potrzebne do wyprowadzenia pe�nej �cie�ki na koniec algorytmu)
	
	int*** G = (int***) malloc(setsNo*sizeof(int**));
	if(G==NULL) safe = false; //[1] - zako�cz algorytm je�eli w kt�rymkolwiek momencie zabraknie pami�ci
	for(long long int i = 0; i<setsNo && safe; i++){
		G[i] = (int**) malloc(sizeOfSet(i)*sizeof(int*));
		if(G[i]==NULL) safe = false; //[1]
		for(int j = 0; j<sizeOfSet(i) && safe; j++){
			G[i][j] = (int*) malloc(2*sizeof(int*));
			if(G[i][j]==NULL) safe = false; //[1]
		}
	}
	
	if(safe){
		
		//main loop
		long long int pSet;
		int previousVertex, targetVertex;
		int newDistance;
		for(long long int i = 0; i<setsNo; i++){
			//rozpatrujemy najkr�tsz� �cie�k� kt�ra prowadzi do zestawu i
			if(sizeOfSet(i)==1){
				//Jeden wierzcho�ek mo�na odwiedzi� tylko w jeden spos�b:
				//bezpo�rednio wchodz�c do niego z wierzcho�ka startowego
				G[i][0][0] = edgeLen[NthVertex(i, 0)];
				G[i][0][1] = 0;
			}
			else{
				for(int j = 0; j<sizeOfSet(i); j++){
					//targetVertex to wierzcho�ek o kt�ry rozszerzamy zestaw wierzcho�k�w "i"
					targetVertex = NthVertex(i, j);
					
					//pSet (previousSet) to zestaw "i" minus wierzcho�ek targetVertex
					pSet = i - (1<<(targetVertex-1));
					
					//dla ka�dego wierzcho�ka ko�cowego w pSet
					previousVertex = NthVertex(pSet, 0);
					G[i][j][0] = G[pSet][0][0] + edgeLen[vertexNo*previousVertex + targetVertex];
					G[i][j][1] = previousVertex;
					for(int k = 1; k < sizeOfSet(pSet); k++){
						previousVertex = NthVertex(pSet, k);
						newDistance = G[pSet][k][0] + edgeLen[vertexNo*previousVertex + targetVertex];
						if(newDistance < G[i][j][0]){
							G[i][j][0] = newDistance;
							G[i][j][1] = previousVertex;
						}
					}
				}
			}
		}
		
		//last step
		bestPathLen = G[setsNo-1][0][0] + edgeLen[vertexNo];
		bestPath[vertexNo-1] = 1;
		for(int k = 1; k < vertexNo-1; k++){
			previousVertex = k+1;
			newDistance = G[setsNo-1][k][0] + edgeLen[vertexNo*previousVertex];
			if(newDistance < bestPathLen){
				bestPathLen = newDistance;
				bestPath[vertexNo-1] = previousVertex;
			}
		}
		
		//find path
		long long int pathLeft = setsNo-1;
		int x = vertexNo-2;
		while(pathLeft>0){
			bestPath[x] = G[pathLeft][findNOfVertex(pathLeft,bestPath[x+1])][1];
			pathLeft -= 1<<(bestPath[x+1]-1);
			x--;
		}
	}
	
	//cleanup
	if (G!=NULL) for(long long int i = 0; i<setsNo; i++){
		if(G[i]==NULL) continue;
		for(int j = 0; j<sizeOfSet(i); j++){
			free(G[i][j]);
		}
		free(G[i]);
	}
	free(G);
	
	return safe;
}

int main(){
	
	//otworz .ini
	FILE* fileINI = fopen ("settings.ini","r");
	if(fileINI == NULL){
		printf("Nie wykryto pliku .ini\n");
		return 0;
	}
	
	//otworz plik wyjsciowy
	char fileName[100];
	fscanf(fileINI, "%s", fileName);
	FILE* fileCSV = fopen (fileName,"w");
	
	FILE* fileTXT;
	
	while(true){
		fscanf(fileINI, "%s%d", fileName, &iterations);
		if(feof(fileINI)) break; //koniec pliku .ini
		
		fileTXT = fopen (fileName,"r");
		printf("Instancja %s", fileName);
		
		//input
		fscanf(fileTXT, "%d", &vertexNo);
		edgeLen = (int*) malloc(sizeof(int)*vertexNo*vertexNo);
		for(int i = 0; i < vertexNo*vertexNo; i++){
			fscanf(fileTXT, "%d", &edgeLen[i]);
		}
		
		fclose(fileTXT);
		
		bestPath = (int*) malloc(vertexNo*sizeof(int));
		bool complete = true;
		
		for(int loop = 0; loop < iterations; loop++){
			//start czasu
			clock_t start = clock();
			
			//algorytm
			complete = main_algorithm();
			
			//je�li wyst�pi� b��d allokacji pami�ci, zako�cz instancj�
			if(!complete){
				printf("! Zabraklo pamieci do obliczenia instancji, przerywam - ");
				break;
			}
			else printf(".");
			
			//stop czasu
			clock_t end = clock();
			double elapsed = double(end - start)/CLOCKS_PER_SEC;
			
			//output
			fprintf(fileCSV, "%s,%.3f,%d", fileName, elapsed, bestPathLen);
			bestPathLen = 0;
			for(int i = 0; i < vertexNo; i++){
				fprintf(fileCSV, ",%d", bestPath[i]);
				bestPath[i] = -1;
			}
			fprintf(fileCSV, "\n");
		}
		
		//koniec rozpatrywania danej instancji
		printf("koniec\n");
		free(edgeLen);
	}
	
	fclose(fileINI);
	fclose(fileCSV);
	return 0;
}
