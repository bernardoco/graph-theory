// Bernardo Carneiro Oradovschi
// Victor Rafael Breves Santos Ferreira

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <list>
#include <cstring>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>
#include <time.h>
#include <iomanip>

#define MAX_INT 2147483647

class Graph{
	private:
		unsigned int vertices;
		unsigned int num_edges=0;
		std::vector<std::vector<int>> structure;
		std::string type;
		bool is_vector;

		void create_matrix(std::ifstream &input){
			unsigned int n = this->vertices;
			unsigned int a,b;
			std::string line;

			for(int i = 0; i < n; i++){
				std::vector<int> temp(this->vertices,0);
				this->structure.push_back(temp);
			}
			
			while(std::getline(input,line)){
				std::istringstream iss(line);

				if(!(iss>>a>>b)){break;} // error

				this->structure[a-1][b-1] = 1;
				this->structure[b-1][a-1] = 1;

				this->num_edges++;
			}

			/*
			//print final matrix
			std::cout << "Matriz de adjacencia:\n";

			for (int i  = 0; i < n; i++){
				for (int j = 0; j < n; j++){
					std::cout << this->structure[i][j]<<" ";
				}
				std::cout << "\n";
			}
			*/
		}

		void create_vector(std::ifstream &input){
			unsigned int a,b;
			unsigned int n = this->vertices;
			std::string line;

			for(int i = 0; i < n; i++){
				std::vector<int> temp;
				this->structure.push_back(temp);
			}

			while(std::getline(input,line)){
				std::istringstream iss(line);

				if(!(iss >> a >> b)){break;} //error

				this->structure[a-1].push_back(b);
				this->structure[b-1].push_back(a);

			}

			//std::cout << "Vetor de adjancencia\n";

			for(std::vector<int> row: this->structure){
				std::sort(row.begin(),row.end());
				/*
				for(int val:row){
					std::cout<<val<<" ";
				}
				std::cout<<"\n";
				*/
			}

		}

		int* bfs_(int first, std::string file_name=""){
			//desmarca todos os vertices
			int *marked = new int[this->vertices];
			std::memset(marked,-1,this->vertices*sizeof(int));

			marked[first-1]=0;

			int v;
			int *parent = new int[this->vertices];

			//raiz
			parent[first-1]=0;
			//fila vazia
			std::queue <int> q;

			q.push(first-1);


			if(this->is_vector){
			while (!q.empty()){
				v = q.front();
				q.pop();



				for (int w : this->structure[v]){	
						if(marked[w-1]==-1){
							marked[w-1]=marked[v]+1;
							q.push(w-1);
							parent[w-1]=v+1;
						}
					
				}
			}
			}
			else{
			while (!q.empty()){
				v = q.front();
				q.pop();

				for (int w =0 ; w < this->vertices; w++){
					if (this->structure[v][w]==1){
						if(marked[w]==-1){
							marked[w]=marked[v]+1;
							q.push(w);
							parent[w]=v+1;
						}
					}
				}
			}
			}
			
			if(file_name!=""){
					std::ofstream output;
					output.open(file_name);

					for (int i=0; i<this->vertices;i++)
						output<<"v:"<<i+1<<", pai:"<<parent[i]<<", nivel:"<<marked[i]<<"\n";
					
					output.close();
				}

			delete[] parent;
			return marked;
		}

		void dfs_(int first, std::string file_name=""){
			//desmarca todos os vertices
			int* marked = new int[this->vertices];
			std::memset(marked,-1,this->vertices*sizeof(int));
			marked[first-1]=0;

			int v;
			int* parent = new int[this->vertices];
			std::vector<int> vect;

			//raiz
			parent[first-1]=0;

			//fila vazia
			std::stack <int> s;

			s.push(first-1);

			while (!s.empty()){

				v = s.top();

				vect = structure[v];

				std::sort(vect.begin(),vect.end());

				short i = 0;
				for (int w : vect){	
						if(marked[w-1]==-1){
							i++;
							marked[w-1]=marked[v]+1;
							s.push(w-1);
							parent[w-1]=v+1;
							break;
						}
				}

				if(i==0)
					s.pop();

			}
			if(!(file_name!="")){
				std::ofstream output;
				output.open(file_name);

				for (int i=0; i<this->vertices;i++){
					output<<"v:"<<i+1<<", pai:"<<parent[i]<<", nivel:"<<marked[i]<<"\n";
				} 
				output.close();
			}

			delete[] marked;
			delete[] parent;

		}


	public:
		Graph(std::string file_name,std::string type):type(type){
			unsigned int v1,v2,n;
			std::string line;

			std::ifstream input(file_name);

			std::getline(input,line);
			std::istringstream iss(line);

			iss >> n;
			this->vertices = n;

			if(type=="matriz"){
				this->create_matrix(input);
				this->is_vector = false;
			}else{
				this->is_vector = true;
				this->create_vector(input);
			}

			input.close();

		}

		void get_output(std::string file_name){
			std::ofstream output;
			output.open(file_name);

			double mean=0;
			unsigned int median;
			unsigned int min_degree = MAX_INT;
			unsigned int max_degree = 0;
			unsigned int degree;
			unsigned int degree_vector[this->vertices];

			for(int i = 0; i < this->vertices; i++){
				degree = 0;
				
				for (int j = 0; j < this->vertices; j++){
					if (this->structure[i][j]==1)
						degree++;
				}
				
				if(degree>max_degree)
					max_degree=degree;
				
				if(degree<min_degree)
					min_degree=degree;

				degree_vector[i]=degree;

				mean+=(double) degree;
			}

			mean/=(double) this->vertices;

			int n = sizeof(degree_vector)/sizeof(degree_vector[0]);
			std::sort(degree_vector,degree_vector+n);

			median = degree_vector[this->vertices/2];

			output<<"Numero de vertices:"<<this->vertices<<"\n";
			output<<"Numero de arestas:"<<this->num_edges<<"\n";
			output<<"Grau minimo:"<< min_degree<<"\n";
			output<<"Grau maximo:"<< max_degree<<"\n";
			output<<"Media dos graus:"<< mean<<"\n";
			output<<"Mediana dos graus:"<< median<<"\n";

			output.close();
		}

		void close(){
			this->structure.clear();
		}

		void bfs(int first_vertex,std::string file_name){
			int * dist;
			dist = this->bfs_(first_vertex,file_name);
			delete[] dist;
			
		}

		void dfs(int first_vertex,std::string file_name){
			this->dfs_(first_vertex,file_name);
		}

		int distance(int a, int b){
			int* distances;
			distances = this->bfs_(a);

			return distances[b-1];
		}


		int diameter(){
			int current_diameter=0;
			int* dist ;
			int i,j;

			#pragma omp parallel for num_threads(4) schedule(dynamic) shared(current_diameter) private(i,j,dist)
			for(i =1; i<=this->vertices;i++){

				dist = this->bfs_(i);
				
				for (j=i;j<this->vertices;j++){
					if(dist[j]>current_diameter)
						current_diameter = dist[j];	
				}
				delete[] dist;
			}	
			

			return current_diameter;
		
		}


		void component(std::string file_name){
			int* tested = new int[this->vertices];
			std::memset(tested,-1,this->vertices*sizeof(int));
			int* dist;
			int group=0;
			int total_vertices=0;
			clock_t t;
			double time;
			int i,j;

			for(i =0; i<this->vertices;i++){
				if (tested[i]!=-1)
					continue;
				group++;
				
				dist = this->bfs_(i+1);
				

				for(j = 0; j < this->vertices; j++){
					if(dist[j]!=-1 and tested[j]==-1)
						tested[j]=group;
				}
				delete[] dist;
			}

			std::ofstream output;
			output.open(file_name);

			output<<"Total de componentes conexas:"<<group<<"\n\n";
			for (int i = 1 ; i<=group;i++){
				total_vertices=0;
				output<<"Componente:"<<i<<"\n";

				output<<"Membros:"<<"\n";

				for(int j=0; j<this->vertices;j++){
					if(tested[j]==i){
						output<<j+1<<" ";
						total_vertices++;
					}
				}
				output<<"\n";
				
				output<<"Total de vertices:"<<total_vertices<<"\n\n";
				
			}
			output.close();
			
		}		
};
