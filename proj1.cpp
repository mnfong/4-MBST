#include <climits>
#include <algorithm>
#include <queue>
#include <math.h>
#include <vector>
#include <iostream>
#include <string>
#include <utility>
#include <list>

using namespace std;

#ifndef TIMESTAMP_H
#define TIMESTAMP_H

class TimeStamp{
  public:
    TimeStamp(): d(0), f(0){};
    int d;
    int f;
};
#endif

#ifndef UGRAPH_H
#define UGRAPH_H

struct edge{
  int source;
  int neighbor;
  int w;
  int u_oname;
  int v_oname;

  edge()
  {
    neighbor = 0;
    w = 0;
    u_oname = 0;
    v_oname = 0;
  };

  edge(int from, int to, int aweight, int u_name, int v_name)
  {
    source = from;
    neighbor = to;
    w = aweight;
    u_oname = u_name;
    v_oname = v_name;
  };
};

class Ugraph{
  public:
    Ugraph();
    int getSize(){return size;};

    //void addNode();
    void addEdge(int u, int v, int aweight, int u_oname, int v_oname);
    //void removeEdge(int u, int v);
    void printGraph(vector< list<edge> > adj);
    void printGraphW();

    int select(vector<int> &v, int start, int fin, int k);
    void MBST(vector< list<edge> > &adjA);

    void collect_weights(vector< list<edge> > adjA, vector<int> &weights);
    void select_smallest(vector< list<edge> > &adjA, vector< list<edge> > &adjB, int median_weight);
    int connected(vector< list<edge> > adjB, vector<int> &cc);

    void dfsCC(vector< list<edge> > adjA, vector< list<edge> >adjB, vector< list<edge> > &adjC, int &ID, vector<int> &cc, int median_weight);
    void dfsCCVisit(vector< list<edge> > adjB, int u, int &t, int ID, vector<int> &cc);

    vector< list<edge> > Adj;

  private:
    vector<int> parents;
    vector<char> colors;
    int size;
};
#endif

Ugraph::Ugraph()
{
  int N = 0;
  int M = 0;
  cin >> N >> M;

  size = N;
  Adj.resize(size);
  parents.resize(size);
  colors.resize(size);

  int u = 0;
  int v = 0;
  int w = 0;

  for (int i = 0; i < M; i++)
  {
    cin >> u >> v >> w;

    Adj[u].push_back(edge(u, v, w, u, v));
    Adj[v].push_back(edge(v, u, w, v, u));
  }
}//Ugraph

void Ugraph::printGraph(vector< list<edge> > adj)
{
  list<edge>::iterator i;
  for (int u = 0; u < size; u++)
  {
    cout<<u<<": ";
    for (auto i = adj[u].begin(); i != adj[u].end(); i++)
      cout << i -> neighbor << " ";

    cout << endl;
  }
}//printGraph


void Ugraph::printGraphW()
{
  for (int u = 0; u < size; u++)
  {
    for (auto i = Adj[u].begin(); i != Adj[u].end(); i++)
      cout << i -> neighbor << "/" << i -> w << "\t";

    cout << endl;
  }
}//printGraphW
/*
void Ugraph::addNode()
{
  size++;
  Adj.resize(size);
}//addNode
*/

void Ugraph::addEdge(int u, int v, int aweight, int u_oname, int v_oname)
{
  edge one(u, v, aweight, u_oname, v_oname);
  edge two(v, u, aweight, v_oname, u_oname);
  Adj[u].push_back(one);
  Adj[v].push_back(two);
}//addEdge

/*
void Ugraph::removeEdge(int u, int v)
{
  int uSize = (int)Adj[u].size();
  int vSize = (int)Adj[v].size();

  for (int i = 0; i < uSize; i++)
  {
    int child = Adj[u][i].neighbor;

    if (child == v)
    {
      Adj[u][i].neighbor = Adj[u][uSize - 1].neighbor;
      Adj[u][i].w = Adj[u][uSize - 1].w;
      Adj[u].resize(uSize - 1);
    }
  }

  for (int i = 0; i < vSize; i++)
  {
    int child = Adj[v][i].neighbor;

    if (child == u)
    {
      Adj[v][i].neighbor = Adj[v][vSize - 1].neighbor;
      Adj[v][i].w = Adj[v][vSize - 1].w;
      Adj[v].resize(vSize - 1);
    }
  }
}//removeEdge
*/

int Ugraph::select(vector<int> &v, int start, int fin, int k){

  int vsize = fin - start + 1;

  if(start == fin)
    return v[start];

  else if(start > fin)
    return -1;

  if(vsize <= 5){
    sort(v.begin() + start, v.begin() + fin + 1);

    return v[k];
  }

  int msize = ceil((double)vsize/5);

  vector<int> medians(msize, 0);

  int med_ind = 0;

  for(int i = start; i <= fin; i = i + 5){
    vector<int> temp(5, 0);
    int j = 0;

    for(; j < 5 && (i + j)<= fin; j++){
      temp[j] = v[i+j];
    }//for j

    if(j < 5)
      temp.resize(j);
    
    sort(temp.begin(), temp.end());

    if(j == 5)
      medians[med_ind++] = temp[2];

    else{
      int jmid = j >> 1;//divide by 2
      
      if(j % 2 == 0)
        jmid--;

      medians[med_ind++] = temp[jmid];

    }
  }//for i

  //so far we selected medians of n/5 groups of 5 elem
  int mid = msize >> 1; //divide by 2

  if((msize % 2) == 0)
    mid--;

  int x = select(medians, 0, msize - 1, mid);
  //partition v's elements around x

  //find x
  int indx = -1;

  for(int i = start; i <= fin; i++){
    if(v[i] == x){
      indx = i;
      break;
    }
  }//for i

  if(indx == -1)
    return -2;

  int t = v[start];
  v[start] = x;
  v[indx] = t;

  int i = start + 1, j = fin;

  while(i < j)
  {
    if(v[i] > x && v[j] <= x){
      t = v[j];
      v[j] = v[i];
      v[i] = t;
      i++;
      j--;
    }//if

    else if(v[i] <= x){
      i++;

      if(i > fin){
        i--;
        break;
      }
    }

    else{
      j--;

      if(j == 0){
        j++;
        break;
      }
    }
  }//while partition
  
  int last = i;

  if(v[i] > x)
    last--;//last index that has less than or equal elemenents than x

  //count how many elements are equal to x
  int count_equal = 0;

  for(int u = start; u <= last; u++){
    if(v[u] == x)
      count_equal++;
  }//for u

  if(k > (last - count_equal) && k <= last)
    return x;

  else if(last < k)
    return select(v, last + 1, fin, k);

  else
    return select(v, start, last, k);
}//select

void Ugraph::MBST( vector<list<edge> > &adjA){
  //Count the number of edges in adjA
  int m = 0;
  int size = adjA.size();

  for(int i = 0; i < size; i++){
    m += adjA[i].size();
  }

  m = m >> 1;//divide by 2, each edge occurs twice in Adj
  //termination case, if the number of edges is 1, print out the output an    d return
  
  if(m == 1){
    //print out the output
    list<edge>::iterator i = adjA[0].begin();

    if (i -> v_oname < i -> u_oname)
      cout << i -> v_oname << " " << i -> u_oname << " " << i -> w << endl;

    else
      cout << i -> u_oname << " " << i -> v_oname << " " << i -> w << endl;
    return;
  }

  /***********   Collect weights of edges **********/
  vector<int> weights;
  //You need to write this function:
  //We need to add only one weight for each edge (u, v)
  //Since each edge occurs in adjA twice, use this condition to add the we    ight only once:
  //if u < v, then add weight
  collect_weights(adjA, weights);


  /*******   Find the median of weights *********/
  int k = m >> 1;//divide by 2
  if(m % 2 == 0)
    k--;
  int median_weight = select(weights, 0, m-1,k);
  //Function "select" is provided above

  /*****  Collect edges with weights <= median_weight
    from adjA into adjB
    Erase an edge from adjA:
    it takes O(1) to erase (use erase) from
    a list
   *************************************************/
  vector< list<edge> > adjB(adjA.size());//smallest edges
  vector< list<edge> > adjC;//smallest edges

  //You need to write this function
  //when adding the smallest half edges to adjB, erase them from adjA:
  select_smallest(adjA, adjB, median_weight);

  /******* Find connected components in adjB 
    in linear time 
   ***************************************/
  vector<int> cc(adjB.size(), 0);
  int ID = 0;

  dfsCC(adjA, adjB, adjC, ID, cc, median_weight);
  int total_cc = connected(adjB, cc);//number of comp is returned

  /**********     If connected (only one component)
    we don't need edges of A anymore
    just call recursively MBST and return 
    after the recursive call returns 
    i.e. do nothing after recursive call returns
   ****************************************/

  if(total_cc == 1){
    MBST(adjB);
    return;
  }

  else{ //If not connected

    /******   You need to find a function that  
      constructs adjC (a new graph)
      with contracted connected components
      Construct adjC:
      each vertex of C corresponds to a single connected component of adjB,
      add each edge from adjA if this edge is between different connected components.

      Rename edges of adjC according to the names
      of their new vertices (connected components) as 0, 1,.., total_cc-1.
     **************************************************/

    //call recursively MBST on adjC: 

    //vector< list<edge> > adjC;
    MBST(adjC); 

    /*** In original Camerini's algorithm, at this place of the program,
      we would add edges from adjB (use original names)
      into the resulting graph MBST, but
      in this project we don't need to do anything here.
     *****************************************/

    return;
  }//else not connected
}//MBST recursive

void Ugraph::collect_weights(vector< list<edge> >adjA, vector<int> &weights)
{
  list<edge>::iterator i;

  for (int u = 0; u < (int)adjA.size(); u++)
  {
    for (auto i = adjA[u].begin(); i != adjA[u].end(); i++)
    {
      int v = i -> neighbor;

      if (u < v)
        weights.push_back(i -> w);
    }
  }
}//collect_weights

void Ugraph::select_smallest(vector< list<edge> > &adjA, vector< list<edge> > &adjB, int median_weight)
{
  list<edge>::iterator i;

  for (int u = 0; u < (int)adjA.size(); u++)
  {
    for (auto i = adjA[u].begin(); i != adjA[u].end(); i++)
    {
      if (i -> w <= median_weight)
      {
        adjB[u].push_back(edge(u, i -> neighbor, i -> w, i -> u_oname, i -> v_oname));
        //adjA[u].erase(i);
      }
    }
  }

  return;
}//select_smallest

int Ugraph::connected(vector< list<edge> > adjB, vector<int> &cc)
{
  int count = 1;
  sort(cc.begin(), cc.end());

  for (int i = 1; i < (int) cc.size(); i++)
  {
    if (cc[i] != cc[i - 1])
    {
      count++;
    }
  }

  return count;
}//connected

void Ugraph::dfsCC(vector< list<edge> > adjA, vector< list<edge> >adjB, vector< list<edge> > &adjC, int &ID, vector<int> &cc, int median_weight)
{
  parents.resize(adjB.size(), 0);
  colors.resize(adjB.size(), 'W');

  for (unsigned int i = 0; i < adjB.size(); i++)
  {
    parents[i] = i;
    colors[i] = 'W';
  }
  
  //int ID = 0;
  int time = 0;

  for (int i = 0; i < (int) adjB.size(); i++)
  {
    if (colors[i] == 'W')
    {
      dfsCCVisit(adjB, i, time, ID, cc);
      ID++;
    }
  }

  adjC.resize(ID);

  list<edge>::iterator i;

  for (int u = 0; u < (int) adjA.size(); u++)
  {
    for (auto i = adjA[u].begin(); i != adjA[u].end(); i++)
    {
      if (i -> w > median_weight)
      {
        if (cc[u] != cc[i -> neighbor])
        {
          adjC[cc[u]].push_back(edge(u, cc[i -> neighbor], i -> w, i -> u_oname, i -> v_oname));
        }
      }
    }
  }
}//dfsCC

void Ugraph::dfsCCVisit(vector< list<edge> > adjB, int u, int &t, int ID, vector<int> &cc)
{
  colors[u] = 'G';
  cc[u] = ID;
  t++;

  list<edge>::iterator i;

  for (i = adjB[u].begin(); i != adjB[u].end(); i++)
  {
    if (colors[i -> neighbor] == 'W')
    {
      colors[i -> neighbor] = 'G';
      dfsCCVisit(adjB, i -> neighbor, t, ID, cc);
    }
  }

  colors[u] = 'B';
  t++;
}//dfsCCVisit

int main()
{
  Ugraph ag;

  ag.MBST(ag.Adj);
}
