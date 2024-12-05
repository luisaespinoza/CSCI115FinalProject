  #include <iostream>
  #include <fstream>
  #include <string>
  #include <cstdlib>
  #include <map>
  #include <vector>
  #include <tuple>
  #include <regex>
using namespace std;
typedef tuple<std::string, int , std::string> order;
typedef vector<order> ordersVector;
bool validateOrderPattern(string orderName){
  string orderPrefix = "ORD";
  string orderNumber = "[0-9]+";
  regex orderPattern("^"+orderPrefix+orderNumber+"$");
  bool isValid = regex_search(orderName,orderPattern);
  return isValid;
};

bool validateLocationPattern(string location){
  string locationPrefix = "Location ";
  string locationPostfix = "[A-Z]{1}";
  regex locationPattern("^"+locationPrefix+locationPostfix+"$");
  bool isValid = regex_search(location,locationPattern);
  return isValid;
}

bool isValidOrder(order const &newOrder){
  auto [orderName, priority,location] = newOrder;
  bool isValidFormat = typeid(priority)==typeid(int);
  // printOrderTuple(newOrder);
  // cout << priority << " " << isValidFormat << endl;
  isValidFormat = validateOrderPattern(orderName);
  isValidFormat = validateLocationPattern(location);
  return isValidFormat;
};
void printOrderTuple(order const &singleOrder){
  auto [detail1, detail2, detail3] = singleOrder;
  cout << detail1 << endl << "Priority: " << detail2 << endl << "Destination: " << detail3 << endl;
};
//single reuseable node for all versions linkedlist
class Node {
  public:
  Node(){orderDetails = make_tuple("",INT_MIN,"");next=nullptr;};
  Node(order newOrder,Node* newNext=nullptr,Node* newPrevious=nullptr, Node* newDown=nullptr){
    orderDetails=newOrder;next=newNext;previous=newPrevious;down=newDown;};
  order GetOrder(){return orderDetails;};
  void SetOrder(order newOrder){orderDetails=newOrder;};
  Node* GetNext(){return next;};
  void SetNext(Node* newNext){next=newNext;};
  void PrintOrderDetails(){printOrderTuple(orderDetails);};
  Node* GetPrevious(){return previous;};
  void SetPrevious(Node* newPrevious){previous = newPrevious;};
  Node* GetDown(){return down;};
  protected:
  order orderDetails;
  Node* next;
  Node* previous;
  Node* down;
};

//graveyard for every previous Node class....
// class SLLNode {
//   public:
//   SLLNode(){orderDetails = make_tuple("",INT_MIN,"");next=nullptr;};
//   SLLNode(order newOrder,SLLNode* newNext=nullptr){orderDetails=newOrder;next=newNext;};
//   order GetOrder(){return orderDetails;};
//   void SetOrder(order newOrder){orderDetails=newOrder;};
//   SLLNode* GetNext(){return next;};
//   void SetNext(SLLNode* newNext){next=newNext;};
//   void PrintOrderDetails(){printOrderTuple(orderDetails);};
//   protected:
//   order orderDetails;
//   SLLNode* next;
// };
// class DLLNode : public SLLNode{
//   public:
//   DLLNode(){orderDetails = make_tuple("",INT_MIN,"");next=nullptr;previous=nullptr;};
//   DLLNode(order newOrder,DLLNode* newNext=nullptr, DLLNode* newPrevious=nullptr){orderDetails=newOrder;next=newNext;previous=newPrevious;};
//   SLLNode* GetPrevious(){return previous;};
//   void SetPrevious(SLLNode* newPrevious){previous = newPrevious;};
//   protected:
//   SLLNode* previous;
// };
// class SKLNode : public SLLNode {
//   SKLNode* GetDown(){return down;};
//   protected:
//   SKLNode* down;
// };

class LinkedList {
  public:
  LinkedList(){head=nullptr;};
  LinkedList(Node* newHead){head = newHead;};
  Node* GetHead(){return head;};
  void SetHead(Node* newHead){
    head=newHead;
    };
  Node* GetLast(){
    if(GetHead() == nullptr){
      return nullptr;
    }
    Node* currentNode = GetHead(); 
    while(currentNode->GetNext() != nullptr){
      currentNode = currentNode -> GetNext();
    }
    return currentNode;
  };
  void SutureNodes(Node*targetNode1,Node*targetNode2){
    targetNode1->SetNext(targetNode2);
  };
  void AppendNode(Node* newNode){
    if(GetHead()==nullptr){
      SetHead(newNode);
    }
    else {
      Node* currentNode = GetLast();
      SutureNodes(currentNode,newNode);
    }
  };
  void DisplayList(){
    Node* currentNode = GetHead();
    bool isNotEmpty = currentNode == nullptr;
    cout << isNotEmpty << endl;
    currentNode -> PrintOrderDetails();
    while(currentNode-> GetNext() != nullptr){
      currentNode = currentNode -> GetNext();
      currentNode -> PrintOrderDetails();
    }
  };
  //explicit matching by string
  Node* Search(string orderID){
    bool isValidString = validateOrderPattern(orderID);
    if(!isValidString){
      return nullptr;
    }
    Node* currentNode = GetHead();
    bool isFound = orderID == get<0>(currentNode->GetOrder());
    if(isFound){
      return currentNode;
    };
    while(currentNode -> GetNext() != nullptr){
      currentNode = currentNode -> GetNext();
      auto [orderName, priority, location] = currentNode ->GetOrder();
      isFound = orderName == orderID;
      if(isFound){
        return currentNode;
      }
    }
    //search unsuccessful
    cout <<orderID<<" NOT FOUND" << endl;
    return nullptr;
  };
  //allow search explicitly by an int since ORD is a mandatory prefix
  Node* Search(int orderIDNo){
    string orderID= "ORD"+to_string(orderIDNo);
    return Search(orderID);
  };
  private:
  Node* head;
};

class DoubleLinkedList : public LinkedList {

};
class SkipList : public DoubleLinkedList {

};
vector<std::string> parseString(std::string const &originalString, std::string delimiter){
  size_t pos = 0; // holding our position index in the string
  std::string tempString = originalString; //intended to leave original untouched
  int size = tempString.size();
  vector<string> parsedStrings;
  std::string word;
  if( size > 0 ){
    while((pos = tempString.find(delimiter)) != std::string::npos){ //remove words at delimiter repeatedly
      word = tempString.substr(0, tempString.find(delimiter));
      tempString = tempString.substr(pos + delimiter.length());
      parsedStrings.push_back(word);
    }
    word = tempString.at(0); // all that's left now is a single character with location id
    parsedStrings.push_back(word);
  }
  return parsedStrings;
};

//read orders from text file
ordersVector gatherData(){
  ordersVector returnVector;
    std::ifstream inputFile;
    inputFile.open("data.txt");
    std::string inputString;
    std::string stringTemp;
    std::string splitTemp;
    if (inputFile.is_open()) {
        while (!inputFile.eof()) {
          getline(inputFile,stringTemp);
          vector<std::string>parsed = parseString(stringTemp, " ");
          int size = parsed.size();
          if( size > 0 ){
            int priority = atoi(&parsed.at(1).at(0));
            std::string location = parsed.at(2) + " " + parsed.at(3);
            std::string orderID = parsed.at(0);
            returnVector.push_back(make_tuple(orderID, priority, location));
          };
        };
    };
    return returnVector;
};


void printOrdersVector(ordersVector orders){
  for( const order& orderElem : orders ){
    printOrderTuple(orderElem);
  }
};

order searchOrdersVector(string orderName, ordersVector const &orders){
  for(order orderElem : orders){
    if(get<0>(orderElem)==orderName){
      return orderElem;
    }
  }
  return make_tuple("NOT FOUND", INT_MIN, "NOT FOUND");
};


int addDeliveryOrder(order const &newOrder, ordersVector &orders){
  if(isValidOrder(newOrder)){
    orders.push_back(newOrder);
    return orders.size()-1;
  }
  cout << "INVALID ORDER FORMAT. ORDER NOT ADDED." << endl;
  return INT_MIN;
};

void vectorToLinkedList(ordersVector vect, LinkedList &list){
  for(order elem:vect){
    list.AppendNode(new Node(elem));
  }
};

// #MAIN starts here........                                                            
int main () {
  ordersVector ordersVect = gatherData();
  LinkedList ordersLL = LinkedList(); 
  //printOrdersVector(data); //print orders vector test;
  printOrderTuple(searchOrdersVector("Ord101", ordersVect));
  printOrderTuple(searchOrdersVector("ORD36", ordersVect));
  addDeliveryOrder(make_tuple("ORD51",4,"Location R"),ordersVect);
  addDeliveryOrder(make_tuple("ORDer533",4,"LocationR"),ordersVect);
  vectorToLinkedList(ordersVect,ordersLL);
  ordersLL.Search(22)->PrintOrderDetails();
  ordersLL.Search("ORD34")->PrintOrderDetails();
  ordersLL.Search(99);
  ordersLL.AppendNode(new Node(make_tuple("ORD99",4,"Location S")));
  ordersLL.Search(99)->PrintOrderDetails();
  ordersLL.DisplayList();
  return 0;
};