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
bool operator<(const pair<char,int> &vertexAndWeight1, const pair<char,int> &vertexAndWeight2){
  return vertexAndWeight1.second < vertexAndWeight2.second;
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
class Edges {
  public:
  Edges(){isDirected=true;};
      // technically unneccesary for the project assignment
      // but it was fun to think about how to handle symmetric relationships
      // handles the symmetric relations of an undirectedGraph
      // by toggling behavior in methods to handle inverse assignments
  Edges(EdgeMap &preconstructedEdgeMap,bool isDirectedGraph=true){edges = preconstructedEdgeMap;isDirected=isDirectedGraph;};
  // this insert handles symmetric mapping
    EdgeMap GetEdges(){return edges;};
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
        if(!isVisited){vertexStack.push(neighbor);parent[neighbor]=front;};//fixed
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
      char front = vertexStack.top();//fixed
      neighbors = GetNeighbors(front);
      visited.push_back(front);
      if(front==destinationVertex){
        ReconstructPath();
        return path;
      };
      //will eventually stop populating the stack and break the loop
      vertexStack.pop();
      PushNeighbors(front);
    };
    return path;

  };
  // vvv Dijsktra's algorithm vvv
  auto ShortestPaths(char start){
    priority_queue<pair<char,int>,vector<pair<char,int>>,greater<pair<char,int>>>vertexQueue; //it's a lot I know
    //basically creates priority queue of char,int pairs contained within a vector ordering on pairs 
    //by the second element as defined earlier on line 26
    map<char,int> shortestDistance;
    vector<char> visited, neighbors, path;
    map<char,vector<char>> paths;
    map<char,char> parent;
    pair<char,int> vertexCurrent;
    map<pair<char,int>,bool> inQueue;
    //might seem strange  but the reasoning is the same as BFS and DFS. It improves readability to see
    // entire blocks of code have name and organized by their function. Lambdas keep them localized.
    auto PushToQueue = [&vertexQueue, &inQueue](pair<char,int> newVertex){
      // if newVertex is not in queue push it to the queue and make entry for inQueue map
      if(!inQueue[newVertex]){vertexQueue.push(newVertex); inQueue[newVertex]=true;};
    };
    // pops and updates inQueue map
    auto PopFromQueue = [&vertexQueue, &inQueue](){inQueue[vertexQueue.top()]=false;vertexQueue.pop();};

    auto QueueNeighbors= [&neighbors,&visited,&PushToQueue] (Edges &edges,pair<char,int>vertexCurrent) {
      for(char neighbor : neighbors){
        bool isVisited = false;
        for(char visitedVertex: visited){if(neighbor == visitedVertex){isVisited = true;};}
        if(!isVisited){
          pair<char,char> edgeName = make_pair(vertexCurrent.first,neighbor);
          int edgeWeight = edges.GetEdges()[edgeName];
          pair<char,int>neighborPair = make_pair(neighbor,edgeWeight);
          PushToQueue(neighborPair);
        };
      };
    };
    auto UpdateShortestDistance = [&neighbors,&parent,&shortestDistance](char currentVertex,Edges &edges){
      for(char neighbor:neighbors){
        pair<char,char> edgeName = make_pair(currentVertex,neighbor);
        int edgeWeight = edges.GetEdges()[edgeName];
        if(shortestDistance[currentVertex]+edgeWeight < shortestDistance[neighbor]){
          shortestDistance[neighbor]=shortestDistance[currentVertex]+edgeWeight;
          parent[neighbor]=currentVertex;
        };
      }
    };
    auto ReconstructPath = [&start, &parent, &path, &paths](char destination){
      stack<char> temp;
      // temp.push(destinationVertex);
      char tempVertex = destination;
      bool isStart = false;
      while(!isStart){
        temp.push(tempVertex);
        if(tempVertex==start){
          isStart =true;
        }
        tempVertex=parent[tempVertex];
      }
      while(!temp.empty()){
        path.push_back(temp.top());
        temp.pop();
      }
      paths[destination]=path;
      //this crucial line fixed all the issues from last night. Reinitialize the path to empty
      // for the next pass. The algorithm was fine otherwise
      // Software Engineering in a nutshell. One line changes everything
      // What was the "study" about ghost engineers making "trivial" changes to code?
      // Guess I'm a ghost engineer now...
      path = vector<char>();
    };
    // set all distances to infinity
    for(char vertex : Vertices){shortestDistance[vertex]=INT_MAX;};
    vertexCurrent= make_pair(start,0);
    shortestDistance[start]=0;
    PushToQueue(vertexCurrent);
    // neighbors = GetNeighbors(vertexCurrent.first);

    while(!vertexQueue.empty()){
      bool isVisited;
      do {
        isVisited=false;
        vertexCurrent=vertexQueue.top();
        PopFromQueue();
        for(char visitedVertex:visited){
          if(vertexCurrent.first==visitedVertex){
            isVisited=true;
          }
        }
      } while(isVisited);
      neighbors = GetNeighbors(vertexCurrent.first);
      visited.push_back(vertexCurrent.first);
      // cout <<"Visited Nodes" << endl;
      // PrintPath(visited);
      // if(vertexCurrent.first==destination){
      //   ReconstructPath();
      //   return path;
      // };
      UpdateShortestDistance(vertexCurrent.first,edgeMap);
      QueueNeighbors(edgeMap,vertexCurrent);
    };
    for(char destination:Vertices){
      ReconstructPath(destination);
    }
    return paths;
  };

  private:
  Edges edgeMap;
  AdjacencyList vertices;
  bool isDirected;
};
double measureRunTime(DeliveryGraph &obj, char startVertex, char destinationVertex,  map<char,vector<char>> &destinationPath, int mode=0 ) {
  clock_t start = clock();
  if(mode == 0){PrintPath(obj.BFS(startVertex,destinationVertex));};
  if(mode == 1){PrintPath(obj.DFS(startVertex,destinationVertex));};
  if(mode == 2){
      destinationPath = obj.ShortestPaths(startVertex);
      for( char const destinationDijkstra: Vertices){
        cout << "final path from "<<startVertex<<" to "<<destinationDijkstra<<" : "<< endl;
        PrintPath(destinationPath[destinationDijkstra]);
      };
  };
  clock_t end = clock();
  double elapsed = double(end - start) / CLOCKS_PER_SEC;
  string searchName;
  if(mode == 0){searchName = "BFS ";};if(mode==1){searchName = "DFS ";};if(mode==2){searchName= "Dijkstra's ";
  std::cout << "Time taken for "<< searchName << startVertex<< " to every destination: " << elapsed << " seconds" << std::endl;}
  else{
    std::cout << "Time taken for "<< searchName << startVertex<< " to "<<destinationVertex << ": " << elapsed << " seconds" << std::endl;
  };
  return elapsed;
};

void runPart4(){
  DeliveryGraph graph;
  map<char,vector<char>> destinationPath;
  map<char,map<char,vector<char>>> path;
  for( int j =0; j < 2 ; j += 1){
    measureRunTime(graph,'A','J',destinationPath,j);
    measureRunTime(graph,'A','D',destinationPath,j);
    measureRunTime(graph,'B','A',destinationPath,j);
    measureRunTime(graph,'C','B',destinationPath,j);
  };
  measureRunTime(graph,'A','J',destinationPath,2);
  measureRunTime(graph,'B','J',destinationPath,2);
  measureRunTime(graph,'D','J',destinationPath,2);
};

// main for testing part3 unused in production file
// int main () {
//   runPart4();
//   return 0;
// };