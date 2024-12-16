#ifndef Functions_h
#define Functions_h
  #include "Functions.h"
#endif
using namespace std;
typedef pair<char,char> Edge;
typedef map<Edge,int> EdgeMap;
typedef map<char,char> Adjacency;
static const char Vertices[10]={'A','B','C','D','E','F','G','H','I','J'};
EdgeMap prebuiltMap = {
  {make_pair('A','B'),10},
  {make_pair('A','C'),15},
  {make_pair('B','D'),12},
  {make_pair('B','I'),11},
  {make_pair('C','E'),10},
  {make_pair('D','F'),2},
  {make_pair('D','H'),14},
  {make_pair('E','F'),5},
  {make_pair('E','G'),4},
  {make_pair('F','G'),3},
  {make_pair('G','H'),8},
  {make_pair('H','I'),7},
  {make_pair('I','J'),6},
  {make_pair('J','A'),6},
};

class Edges {
  public:
  Edges(){isDirected=true;};
      // technically unneccesary for the project assignment
      // but it was fun to think about how to handle symmetric relationships
      // handles the symmetric relations of an undirectedGraph
      // by toggling behavior in methods to handle inverse assignments
  Edges(EdgeMap &preconstructedEdgeMap,bool isDirectedGraph=true){edges = preconstructedEdgeMap;isDirected=isDirectedGraph;};
  // this insert handles symmetric mapping
    Edge StandardizeOrder(Edge &newEdge){
      if(!isDirected){
        if(newEdge.first > newEdge.second){
          char temp = newEdge.first;
          newEdge.first = newEdge.second;
          newEdge.second = temp;
        };
      };
      // otherwise order matters and we preserve ordering as a unique element
        return newEdge;
    };
    void insert(Edge &newEdge, int weight){
      // if undirected
      // always ensure pair gets rearranged in order
      // effectively no duplicate entries: (a,b) is (b,a)
      newEdge = StandardizeOrder(newEdge);
      edges[newEdge]=weight;
    };
    optional<Edge> FindValue(Edge &candidateEdge) {
      //ensure standard order of pairs
      candidateEdge = StandardizeOrder(candidateEdge);
      // then check value
      if(edges[candidateEdge]){
        return candidateEdge;
      }
      return nullopt;
    };
  private:
    EdgeMap edges;
    bool isDirected;
};

class AdjacencyList {
  public:
  AdjacencyList(){isDirected=true;};
  AdjacencyList(EdgeMap edges, bool isDirectedGraph=true){
    isDirected=isDirectedGraph;
    for(const auto& edge: edges){
      vertices[edge.first.first].push_back(edge.first.second);
      // technically unneccesary for the assignment
      // handles the symmetric relations of an undirectedGraph
      if(!isDirectedGraph){
        vertices[edge.first.second].push_back(edge.first.first);
      };
    };
  };
  map<char, vector<char>> GetList(){return vertices;};
  vector<char> GetNeighbors(char targetVertex){return vertices[targetVertex];};
  bool Find(char targetVertex, char neighborVertex){
    bool isFound = false;
    for( char neighbor:vertices[targetVertex]){
      if(neighbor==neighborVertex){isFound=true;};
    };
    return isFound;
  };
  void Insert(char targetVertex, char neighborVertex){
    if(!Find(targetVertex, neighborVertex)){
      vertices[targetVertex].push_back(neighborVertex);
  };}
  private:
  map<char, vector<char>> vertices;
  bool isDirected;
};


class DeliveryGraph {
  public:
  DeliveryGraph(){isDirected=true; edgeMap=Edges(prebuiltMap);vertices=AdjacencyList(prebuiltMap);};
  // DeliveryGraph()
  // might seem a bit roundabout here. But GetNeighbors is much more readable than inferring through the code that we're fetching neighbors
  vector<char> GetNeighbors(char targetVertex){vector<char> neighbors = vertices.GetNeighbors(targetVertex);return neighbors;};
  // using queue/stack from c++ library
  vector<char> BFS(char startingVertex, char destinationVertex){
    // forward declaring all variables. It gets complicated later...
    queue<char> vertexQueue;
    vector<char> visited;
    vector<char> neighbors;
    map<char,vector<char>> paths;
    vector<char> path;
    map<char,char> parent;
    // named here for readability. Invoked later...
    auto QueueNeighbors= [&vertexQueue,&neighbors,&visited,&parent] () {
      for(char neighbor : neighbors){
        bool isVisited = false;
        for(char visitedVertex: visited){
          if(neighbor == visitedVertex){isVisited = true;};
        }
        if(!isVisited){vertexQueue.push(neighbor);parent[neighbor]=vertexQueue.front();};
      };
    };
    auto ReconstructPath = [&parent,&path,&destinationVertex](){
      stack<char> temp;
      // temp.push(destinationVertex);
      char tempVertex = destinationVertex;
      while(tempVertex){
        temp.push(tempVertex);
        tempVertex=parent[tempVertex];
        // temp.push(parent[tempVertex]);
      }
      while(!temp.empty()){
        path.push_back(temp.top());
        temp.pop();
      }
    };

    // now we can start our search...
    vertexQueue.push(startingVertex);

    // this is our loop
    while(!vertexQueue.empty()){
      char front = vertexQueue.front();
      neighbors = GetNeighbors(front);
      visited.push_back(front);
      if(front==destinationVertex){
        ReconstructPath();
        return path;
      };
      //will eventually stop populating the queue and break the loop
      QueueNeighbors();
      vertexQueue.pop();
    };
    return path;
  };

  // similarity in design leads me to believe there's a way to define ONE search
  // using templates. Basically swapping queue for stack 
  // however I'm running out of time to look into that option
  vector<char> DFS(char startingVertex, char destinationVertex){
    stack<char> vertexStack;
    vector<char> visited;
    vector<char> neighbors;
    map<char,vector<char>> paths;
    vector<char> path;
    map<char,char> parent;
    // named here for readability. Invoked later...
    auto PushNeighbors= [&vertexStack,&neighbors,&visited,&parent] (char front) {
      for(char neighbor : neighbors){
        bool isVisited = false;
        for(char visitedVertex: visited){
          if(neighbor == visitedVertex){isVisited = true;};
        }
        if(!isVisited){vertexStack.push(neighbor);parent[neighbor]=front;};//fixthis
      };
    };
    auto ReconstructPath = [&parent,&path,&destinationVertex](){
      stack<char> temp;
      // temp.push(destinationVertex);
      char tempVertex = destinationVertex;
      while(tempVertex){
        temp.push(tempVertex);
        tempVertex=parent[tempVertex];
        // temp.push(parent[tempVertex]);
      }
      while(!temp.empty()){
        path.push_back(temp.top());
        temp.pop();
      }
    };

    // now we can start our search...
    vertexStack.push(startingVertex);

    // this is our loop
    while(!vertexStack.empty()){
      char front = vertexStack.top();//fixthis
      neighbors = GetNeighbors(front);
      visited.push_back(front);
      if(front==destinationVertex){
        ReconstructPath();
        return path;
      };
      //will eventually stop populating the queue and break the loop
      vertexStack.pop();
      PushNeighbors(front);
    };
    return path;

  };
  // // vvv Dijsktra's algorithm vvv
  // vector<char> ShortestPath(char start, char destination){
  //   ;
  // };

  private:
  Edges edgeMap;
  AdjacencyList vertices;
  bool isDirected;
};

void PrintPath(vector<char> path){
  int size = path.size();
  for(int i = 0 ; i < size; i += 1){
    cout << path.at(i);
    if(size - 1 != i){
      cout << "==>" ;
    };
  };
  cout << endl;
;}
void runPart3(){
  DeliveryGraph graph;
  PrintPath(graph.BFS('A','J'));
  PrintPath(graph.BFS('A','D'));
  PrintPath(graph.BFS('B','A'));
  PrintPath(graph.BFS('C','B'));
  PrintPath(graph.DFS('A','J'));
  PrintPath(graph.DFS('A','D'));
  PrintPath(graph.DFS('B','A'));
  PrintPath(graph.DFS('C','B'));
  };

//main for testing part3 unused in production file
int main () {
  runPart3();
  return 0;
};