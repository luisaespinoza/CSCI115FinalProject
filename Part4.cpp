#ifndef Functions_h
#define Functions_h
  #include "Functions.h"
#endif
using namespace std;
typedef pair<char,char> Edge;
typedef map<pair<char,char>,int> EdgeMap;
// typedef map<char,vector<map<char,int>*>> EdgeMap;

class Edges {
  public:
  // this insert handles symmetric mapping
    Edge StandardizeOrder(Edge &newEdge){
        if(newEdge.first > newEdge.second){
          char temp = newEdge.first;
          newEdge.first = newEdge.second;
          newEdge.second = temp;
        };
        return newEdge;
    };
    void insert(Edge &newEdge, int weight){
      // always ensure pair gets rearranged in order
      // effectively no duplicate entries: (a,b) is (b,a)
      newEdge = StandardizeOrder(newEdge);
      // associated weight is unique to entry
      edges[newEdge]=weight;
    };
    optional<Edge> FindValue(Edge &candidateEdge) {
      //ensure standard order of pairs
      candidateEdge = StandardizeOrder(candidateEdge);
      // then check value
      if(edges[candidateEdge]){
        return edges[candidateEdge];
      }
      return nullopt;
    };
  private:
    EdgeMap edges;
};

class DeliveryGraph {
  public:

  private:
  EdgeMap edgeMap;
};