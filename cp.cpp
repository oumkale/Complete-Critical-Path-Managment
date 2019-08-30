#include <cstdio>
#include <memory>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <stack>
#define DBG 1  
#define PLOT_GRAPH 0 
using namespace std;
struct activity {
	string name;
	int duration;
	int es, ef, ls, lf, st;  
							 
};
std::string exec(const char* cmd) {
    char buffer[128];
    std::string result = "";
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != NULL)
            result += buffer;
    }
    return result;
}
std::vector<int> ReadNumbers()
{
    std::vector<int> numbers ;

    do
    {
        int input ;
        if ( std::cin >> input )
            numbers.push_back(input) ;
    }while ( std::cin && std::cin.peek() != '\n' );

    return numbers ;
}
void topologicalSortUtil(int v, vector<bool> &visited,  stack<int> &Stack, vector< vector<int> > &adj)
{
    visited[v] = true;
 
    vector<int>::iterator i;
    for (i = adj[v].begin(); i != adj[v].end(); ++i)
        if (!visited[*i])
            topologicalSortUtil(*i, visited, Stack, adj);
 
    Stack.push(v);
}


int main() {

	
	int i,n_tasks,top,j;
	// if(PLOT_GRAPH) {
		ofstream f;
		f.open("plot_graph.plt");

	// }
	cout<<"Prof : Jagannath_Sir "<<endl;
    cout<<"Project : Critical Path system"<<endl;
    cout<<"Team : Oum Kale"<<endl;
    cout<<"Roll Noms : 1705022"<<endl;
     
	cout<<"-------------------------- Critical Path system ---------------\n\n";
	cout<<"Enter the number of tasks : ";
	cin>>n_tasks; // n_tasks is the number of tasks
	
	if(PLOT_GRAPH) {
		f<<n_tasks<<endl;
	}

	struct activity nodes[n_tasks+2]; 

	nodes[0].name = "Start";
	nodes[0].duration = 0;
	nodes[n_tasks+1].name = "Finish";
	nodes[n_tasks+1].duration = 0;

	if(PLOT_GRAPH) {
		f<<"Start"<<" "<<"0"<<endl;
	}
	
	for(i = 1 ; i <= n_tasks; i++) {
		cout<<"\n\nEnter task #"<<i<<" : ";
		cin>>nodes[i].name;
		//getline(cin, nodes[i].name);
		cout<<"Enter duration for "<<i<<" : ";
		cin>>nodes[i].duration;
		if(PLOT_GRAPH) {
			f<<nodes[i].name<<" "<<nodes[i].duration<<endl;
		}
	}
	if(PLOT_GRAPH) {
		f<<"Finish"<<" "<<"0"<<endl;
	}
	cout<<"\n\n\t\tTasks entered :\n";
	for(i = 0 ; i <= n_tasks+1; i++) {
		cout<<"\t\t"<<i<<". "<<nodes[i].name<<" "<<nodes[i].duration<<endl;
	}


	vector< vector<int> > adj;  
	vector< vector<int> > pred;
	for(i = 0 ; i <= n_tasks; i++) {
		vector<int> temp;
		adj.push_back(temp);
		pred.push_back(temp);
	}
	cout<<"\n User need to input all the tasks with no predecessors as the successor of \"Start\"";
	for(i = 0 ; i <= n_tasks; i++) {
		cout<<"\n\nEnter successors for task "<<nodes[i].name<<" : ";
		vector<int> temp = ReadNumbers();
		if(temp.size()==0){
			adj[i].push_back(n_tasks);
			pred[n_tasks].push_back(i);
			if(PLOT_GRAPH) {
				f<<i<<" "<<n_tasks<<endl;
			}
		}
		else {
			for(int j=0; j<temp.size(); j++)
				adj[i].push_back(temp[j]);
			for(int j=0;j < temp.size(); j++)
				pred[temp[j]].push_back(i);
			if(PLOT_GRAPH) {
				for(int j=0;j<temp.size();j++){
					f<<i<<" "<<temp[j]<<endl;
				}
			}
		}

	}
	f<<"quit"<<endl;
	if(DBG) {
		//debugging
		cout<<"\nSuccessor matrix :\n";
		for(i = 0 ; i < n_tasks+2; i++) {
			cout<<i<<" : ";
			for(j = 0 ; j < adj[i].size(); j++) {
				cout<<adj[i][j]<<"->";
			}
			cout<<endl;
		}

		cout<<"Predecessor matrix :\n";
		for(i = 0 ; i < n_tasks+2; i++) {
			cout<<i<<" : ";
			for(j = 0 ; j < pred[i].size(); j++) {
				cout<<pred[i][j]<<"->";
			}
			cout<<endl;
		}
	}

	stack<int> Stack;
	vector<bool> visit(n_tasks+2, false);
	topologicalSortUtil(0,visit, Stack, adj);
	
	nodes[0].es = 0;
	nodes[0].ef = 0;
	Stack.pop();

	while(!Stack.empty()) {
		top = Stack.top();
		int max_f = -1;
		for(i = 0; i < pred[top].size(); i++) {
			if(max_f < nodes[pred[top][i]].ef) {
				max_f = nodes[pred[top][i]].ef;
			} 
		}
		nodes[top].es = max_f;
		nodes[top].ef = max_f + nodes[top].duration;
		Stack.pop();
	}


	if(DBG) {
		cout<<"Es and Ef : \n";
		for(i = 0 ; i < n_tasks+2; i++) {
			cout<<i<<" "<<nodes[i].name<<" "<<nodes[i].es<<" "<<nodes[i].ef<<endl;
		}
	}

	// calculating latest start and finish time for each task

	stack<int> Stack2;
	vector<bool> visit2(n_tasks+2, false);

	topologicalSortUtil(n_tasks+1, visit2, Stack2, pred);

	nodes[n_tasks+1].ls = nodes[n_tasks+1].es;
	nodes[n_tasks+1].lf = nodes[n_tasks+1].ef;
	Stack2.pop();
	while(!Stack2.empty()) {
		top = Stack2.top();
		int min_s = 99999;
		for(i = 0; i < adj[top].size(); i++) {
			if(min_s > nodes[adj[top][i]].ls) {
				min_s = nodes[adj[top][i]].ls;
			} 
		}
		nodes[top].lf = min_s;
		nodes[top].ls = min_s - nodes[top].duration;
		Stack2.pop();
	
	if(DBG) {
		cout<<"Ls and Lf : \n";
		for(i = 0 ; i < n_tasks+2; i++) {
			cout<<i<<" "<<nodes[i].name<<" "<<nodes[i].ls<<" "<<nodes[i].lf<<endl;
		}
	}


	// display of results

	cout<<"RESULTS : \n\n";
	cout<<"\t#\tTask\tDur.\tEs\tEf\tLs\tLf\tST\n\n";
	for(i = 0 ; i < n_tasks+2 ; i++) {
		nodes[i].st = nodes[i].ls - nodes[i].es;
		cout<<"\t"<<i<<"\t"<<nodes[i].name<<"\t"<<nodes[i].duration<<"\t"<<nodes[i].es<<"\t"<<nodes[i].ef<<"\t"<<nodes[i].ls<<"\t"<<nodes[i].lf<<"\t"<<nodes[i].st<<"\n\n";
	}
	queue<int> q3;
	vector<int> visited3(n_tasks+2,0);
	vector<int> critical_path(n_tasks+2,0);
	q3.push(0);
	critical_path[0] = 1;
	while(!q3.empty()) {
		top = q3.front();
		q3.pop();
		if(nodes[top].es == nodes[top].ls) {
			critical_path[top] = 1;
		}
		for(i = 0 ; i < adj[top].size(); i++) {
			if(visited3[adj[top][i]] == 0){
				q3.push(adj[top][i]);
			}
		}
	}

	cout<<"Critical Path : ";
	for(i = 0 ; i < critical_path.size(); i++) {
		if(critical_path[i]==1){
		 cout<<nodes[i].name<<"->";
		 f<<i<<" ";
		}
	}
	cout<<endl;
	f<<endl;
	f.close();
	//exec("python3 plot_graph2.py < plot_graph.plt");

	return 0;
  }
  }
