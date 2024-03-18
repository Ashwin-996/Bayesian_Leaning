#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <tuple>


// Format checker just assumes you have Alarm.bif and Solved_Alarm.bif (your file) in current directory
using namespace std;

// Our graph consists of a list of nodes where each node is represented as follows:
class Graph_Node{

private:
	string Node_Name;  // Variable name 
	vector<int> Children; // Children of a particular node - these are index of nodes in graph.
	vector<string> Parents; // Parents of a particular node- note these are names of parents
	int nvalues;  // Number of categories a variable represented by this node can take
	vector<string> values; // Categories of possible values
	vector<float> CPT; // conditional probability table as a 1-d array . Look for BIF format to understand its meaning
public:
	// Constructor- a node is initialised with its name and its categories
    Graph_Node(string name,int n,vector<string> vals)
	{
		Node_Name=name;
	
		nvalues=n;
		values=vals;
		

	}
	string get_name()
	{
		return Node_Name;
	}
	vector<int> get_children()
	{
		return Children;
	}
	vector<string> get_Parents()
	{
		return Parents;
	}
	vector<float> get_CPT()
	{
		return CPT;
	}
	int get_nvalues()
	{
		return nvalues;
	}
	vector<string> get_values()
	{
		return values;
	}
	void set_CPT(vector<float> new_CPT)
	{
		CPT.clear();
		CPT=new_CPT;
	}
    void set_Parents(vector<string> Parent_Nodes)
    {
        Parents.clear();
        Parents=Parent_Nodes;
    }
    // add another node in a graph as a child of this node
    int add_child(int new_child_index )
    {
        for(int i=0;i<(int)Children.size();i++)
        {
            if(Children[i]==new_child_index)
                return 0;
        }
        Children.push_back(new_child_index);
        return 1;
    }



};


 // The whole network represted as a list of nodes
class network{

	list <Graph_Node> Pres_Graph;

public:
	int addNode(Graph_Node node)
	{
		Pres_Graph.push_back(node);
		return 0;
	}
    
    
	int netSize()
	{
		return Pres_Graph.size();
	}
    // get the index of node with a given name
    int get_index(string val_name)
    {
        list<Graph_Node>::iterator listIt;
        int count=0;
        for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        {
            if(listIt->get_name().compare(val_name)==0)
                return count;
            count++;
        }
        return -1;
    }
// get the node at nth index
    list<Graph_Node>::iterator get_nth_node(int n)
    {
       list<Graph_Node>::iterator listIt;
        int count=0;
        for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        {
            if(count==n)
                return listIt;
            count++;
        }
        return listIt; 
    }
    //get the iterator of a node with a given name
    list<Graph_Node>::iterator search_node(string val_name)
    {
        list<Graph_Node>::iterator listIt;
        for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        {
            if(listIt->get_name().compare(val_name)==0)
                return listIt;
        }
    
            cout<<"node not found\n";
        return listIt;
    }
	

};

network read_network()
{
	network Alarm;
	string line;
	int find=0;
  	ifstream myfile("alarm.bif"); 
  	string temp;
	string temp1;
  	string name;
  	vector<string> values;
  	
    if (myfile.is_open())
    {
    	while (! myfile.eof() )
    	{
    		stringstream ss;
      		getline (myfile,line);
      		
      		
      		ss.str(line);
     		ss>>temp;
		
     		
     		if(temp.compare("variable")==0)
     		{
                    
     				ss>>name;
     				getline (myfile,line);
                   
     				stringstream ss2;
     				ss2.str(line);
     				for(int i=0;i<4;i++)
     				{
     					
     					ss2>>temp;
     					
     					
     				}
     				values.clear();
     				while(temp.compare("};")!=0)
     				{
     					values.push_back(temp);
     					
     					ss2>>temp;
    				}
     				Graph_Node new_node(name,values.size(),values);
     				int pos=Alarm.addNode(new_node);

     				
     		}
     		else if(temp.compare("probability")==0)
     		{
                    
     				ss>>temp;
     				ss>>temp;
     				
                    list<Graph_Node>::iterator listIt;
                    list<Graph_Node>::iterator listIt1;
     				listIt=Alarm.search_node(temp);
                    int index=Alarm.get_index(temp);
                    ss>>temp;
                    values.clear();
     				while(temp.compare(")")!=0)
     				{
                        listIt1=Alarm.search_node(temp);
                        listIt1->add_child(index);
     					values.push_back(temp);
     					
     					ss>>temp;

    				}
                    listIt->set_Parents(values);
    				getline (myfile,line);
     				stringstream ss2;
					stringstream ss3;
                    
     				ss2.str(line);
     				ss2>> temp;
                    
     				ss2>> temp;
                    
                    string::size_type sz;

					int count = 0;
					while(temp.compare(";")!=0)
					{
						count++;
						ss2>>temp;
					}
     				vector<float> curr_CPT(count);
					for(int i=0; i<count; i++)
     				{
						vector<string> pars;
						pars = listIt->get_Parents();
						int flag = 1;
						for(int k = (int)pars.size()-1; k>=0; k--)
						{
							list<Graph_Node>::iterator It;
							It = Alarm.search_node(pars[k]);
							int len = It->get_nvalues();
							flag *= len;
						}

						int len = listIt->get_nvalues();
						for(int j=0; j < flag; j++)
						{
							float total = 0;
							for(int k=0; k<len; k++) 
							{
								float div = 10000;
								if(k!=(len-1))
								{
									float prob;
									if((int)total==9993) prob = 1; 
									else prob = rand()%(9993-(int)total) + 1;
									curr_CPT[j + k*flag] = prob/div;
									total += prob;
								}else curr_CPT[j + k*flag] = 1-(total/div);
							}
						}
								

    				}
                    
                    listIt->set_CPT(curr_CPT);

     		}
            else
            {
                
            }
     		
     		

    		
    		
    	}
    	
    	if(find==1)
    	myfile.close();
  	}
  	
  	return Alarm;
}

vector<tuple<int,string,int, int>> unknown_data;

void parser_data(string &record_file, network &Alarm)
{
	ifstream inp;
	inp.open(record_file);

	string line;
	while(getline(inp,line))
	{
		string record;
		stringstream ss;
		ss.str(line);
		for(int i=0; i<Alarm.netSize(); i++)
		{
			ss>>record;

			if(record=="\"?\"")
			{
				vector<string> curr_records;
				stringstream ss1;
				ss1.str(line);
				string temp;
				for(int j=0; j<Alarm.netSize(); j++)
				{
					ss1>>temp;
					curr_records.push_back(temp);
				}

				list<Graph_Node>::iterator listIt;
				listIt = Alarm.get_nth_node(i);
				vector<string> pars;
				pars = listIt->get_Parents();

				list<Graph_Node>::iterator It;
				int flag = 1;
				int idx_in_CPT = 0;
				for(int k = (int)pars.size()-1; k>=0; k--)
				{
					It = Alarm.search_node(pars[k]);

					int idx = Alarm.get_index(pars[k]);

					vector<string> vals;
					vals = It->get_values();
					int len = It->get_nvalues();

					int check=0;
					while(curr_records[idx] != vals[check]) check++;
					
					idx_in_CPT += flag*check;

					flag *= len;
				}

				vector<float> temp_CPT;
				temp_CPT = listIt->get_CPT();

				int idx = idx_in_CPT;
				
				int cmp = rand()%9999 + 1;
				int counter = 0;
				int res = temp_CPT[idx]*10000;
				while(cmp > res) 
				{
					idx += flag;
					res += temp_CPT[idx]*10000;
					counter++;
				}

				vector<string> categories;
				categories = listIt->get_values();
				string curr_val = categories[counter];

				unknown_data.push_back(make_tuple(i, curr_val, idx_in_CPT, flag));

				break;
			}
		}
	}

	inp.close();
}

void show_CPT(network &Alarm)
{
	for(int i=0; i<(int)Alarm.netSize(); i++)
	{
		list<Graph_Node>::iterator listIt;
		listIt = Alarm.get_nth_node(i);

		string name = listIt->get_name();
		cout<< name << "   :   ";
		vector<float> curr_CPT;
		curr_CPT = listIt->get_CPT();
		for(auto u: curr_CPT) cout<< u << "  ";
		cout<<endl;
	}
}

string update_data(int &a, int &b, int &c, network &Alarm)
{
	list<Graph_Node>::iterator listIt;
	listIt = Alarm.get_nth_node(a);
	vector<float> temp_CPT;
	temp_CPT = listIt->get_CPT();

	int idx = b;
	
	int cmp = rand()%9950 + 1;
	int counter = 0;
	int res = temp_CPT[idx]*10000;
	while(cmp > res) 
	{
		idx += c;
		res += temp_CPT[idx]*10000;
		counter++;
	}

	vector<string> categories;
	categories = listIt->get_values();
	string curr_val = categories[counter];

	return curr_val;
}


void M_step(string &record_file, network &Alarm);


void E_step(string &record_file, network &Alarm)
{
	int convergence = 0;
	for(auto &u: unknown_data)
	{
		int a,b,c;
		string s;
		tie(a, s, b, c) = u;

		string new_val = update_data(a, b, c, Alarm);
		if(new_val!=s) u = make_tuple(a, new_val, b, c);
		else convergence++;
	}
	if(convergence < (int)unknown_data.size()-1350) M_step(record_file, Alarm);
}

void initialize_CPT(string &record_file, network &Alarm)
{
	for(int i=0; i < (int)Alarm.netSize(); i++)
	{
		list<Graph_Node>::iterator listIt;
		listIt = Alarm.get_nth_node(i);

		vector<float> temp;
		temp = listIt->get_CPT();
		for(int j=0; j < (int)temp.size(); j++) temp[j] = 1;

		listIt->set_CPT(temp);
	}

	ifstream inp;
	inp.open(record_file);

	string line;
	vector<string> curr_records((int)Alarm.netSize());
	int ud_idx=0;
	while(getline(inp,line))
	{
		bool ques_found = false;

		string record;
		stringstream ss;
		ss.str(line);
		for(int i=0; i < (int)Alarm.netSize(); i++)
		{
			ss>>record;
			curr_records[i] = record;
			if(record=="\"?\"") ques_found = true;
		}
		for(int i=0; i < (int)Alarm.netSize(); i++)
		{
			list<Graph_Node>::iterator listIt;
			listIt = Alarm.get_nth_node(i);

			vector<string> pars;
			pars = listIt->get_Parents();
			int flag = 1;
			int idx_in_CPT = 0;
			for(int k = (int)pars.size()-1; k>=0; k--)
			{
				list<Graph_Node>::iterator It;
				It = Alarm.search_node(pars[k]);

				int idx = Alarm.get_index(pars[k]);

				vector<string> vals;
				vals = It->get_values();
				int len = It->get_nvalues();

				int check=0;
				string ref;
				if(curr_records[idx]=="\"?\"")
				{
					int a,b,c;
					string s;
					tie(a, s, b, c) = unknown_data[ud_idx];
					ref = s;
				}
				else ref = curr_records[idx];
				while(ref != vals[check]) check++;
				
				idx_in_CPT += flag*check;

				flag *= len;
			}
			vector<string> vals;
			vals = listIt->get_values();
			int check=0;
			string ref;
			if(curr_records[i]=="\"?\"")
			{
				int a,b,c;
				string s;
				tie(a, s, b, c) = unknown_data[ud_idx];
				ref = s;
			}
			else ref = curr_records[i];
			while(ref != vals[check]) check++;
			idx_in_CPT += flag*check;

			vector<float> temp;
			temp = listIt->get_CPT();
			temp[idx_in_CPT]++;

			listIt->set_CPT(temp);
		}
		if(ques_found) ud_idx++;
	}

	inp.close();
}

void update_CPT(network &Alarm)
{
	for(int i=0; i < (int)Alarm.netSize(); i++)
	{
		list<Graph_Node>::iterator listIt;
		listIt = Alarm.get_nth_node(i);
		vector<float> new_CPT;
		new_CPT = listIt->get_CPT();

		vector<string> pars;
		pars = listIt->get_Parents();
		int flag = 1;
		for(int k = (int)pars.size()-1; k>=0; k--)
		{
			list<Graph_Node>::iterator It;
			It = Alarm.search_node(pars[k]);
			int len = It->get_nvalues();
			flag *= len;
		}

		int len = listIt->get_nvalues();
		for(int j=0; j < flag; j++)
		{
			float total = 0;
			for(int k=0; k<len; k++) total += new_CPT[j + k*flag];
			for(int k=0; k<len; k++) new_CPT[j + k*flag] /= total; 
		}

		listIt->set_CPT(new_CPT);
	}

}

void M_step(string &record_file, network &Alarm)
{
	initialize_CPT(record_file, Alarm);

	update_CPT(Alarm);

	E_step(record_file, Alarm);
}

void compute_output(string input_file, string output_file, network Alarm)
{
	ofstream op;
	op.open(output_file);
	ifstream inp;
	inp.open(input_file);

	string line;
	while(getline(inp,line))
	{
		string temp;
		stringstream ss;
		ss.str(line);

		ss>>temp;
		if(temp.compare("variable")==0)
		{
			op<< line <<endl;
			for(int i=0; i<3; i++)
			{
				getline(inp,line);
				op<< line <<endl;
			}
		}else if(temp.compare("probability")==0)
		{
			ss>>temp;
			ss>>temp;

			list<Graph_Node>::iterator listIt;
			listIt = Alarm.search_node(temp);
			op<< line <<endl;

			getline(inp,line);
			int i=0;
			while(line[i]!='e') 
			{
				op<< line[i];
				i++;
			}
			op<< "e ";

			vector<float> curr_CPT;
			curr_CPT = listIt->get_CPT();
			for(auto u: curr_CPT) op<< u << " ";
			op<< ';';
			op<< endl;

			getline(inp,line);
			op<< line <<endl;
		}else op<< line <<endl;
	}

	inp.close();
	op.close();
}

int main(int argc, char** argv)
{
	if(argc < 3)
	{
		cout<< "Wrong Format!" <<endl;
		exit(0);
	}

	string input_file = argv[1];
	string record_file = argv[2];
	
	network Alarm;
	Alarm=read_network();

	parser_data(record_file, Alarm);
	E_step(record_file, Alarm);
    compute_output(input_file, "solved_alarm.bif", Alarm);
	
	return 0;
}





