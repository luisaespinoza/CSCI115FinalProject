#ifndef Functions_h
#define Functions_h
  #include "Functions.h"
#endif
using namespace std;

// single multipurpose Node
// sacrifices lightweight characteristic for flexibility in accomodating variants of LinkedList structure
class Node {
  public:
  Node(){orderDetails = make_tuple("",INT_MIN,"");next=nullptr;previous=nullptr;down=nullptr;lvl=INT_MIN;};
  Node(order newOrder, Node* newNext=nullptr, Node* newPrevious=nullptr, Node* newDown=nullptr, int newLvl=INT_MIN){orderDetails=newOrder;next=newNext;previous=newPrevious;down=newDown; lvl=newLvl;};
  Node(int newLvl){lvl=newLvl;orderDetails = make_tuple("",INT_MIN,"");next=nullptr;previous=nullptr;down=nullptr;}
  ~Node(){next = nullptr, previous = nullptr; down = nullptr;};
  order GetOrder(){return orderDetails;};
  void SetOrder(order newOrder){orderDetails=newOrder;};
  Node* GetNext(){return next;};
  void SetNext(Node* newNext){next=newNext;};
  void PrintOrderDetails(){printOrderTuple(orderDetails);};
  Node* GetPrevious(){return previous;};
  void SetPrevious(Node* newPrevious){previous = newPrevious;};
  Node* GetDown(){return down;};
  void SetDown(Node* newDown){down=newDown;};
  int GetLvl(){return lvl;};
  protected:
  order orderDetails;
  Node* next;
  Node* previous;
  Node* down;
  int lvl;
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
  // LinkedList(Node* newHead){head = newHead;};
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
    string orderID = "ORD"+to_string(orderIDNo);
    return Search(orderID);
  };
  protected:
  Node* head;
};

class DoubleLinkedList : public LinkedList {
  public: 
  DoubleLinkedList();
    Node* GetPrevious(){return previous;};
    void SetPrevious(Node* newPrevious){previous = newPrevious;};
    void SutureNodes(Node* targetNode1, Node* targetNode2){targetNode1->SetNext(targetNode2);targetNode2->SetPrevious(targetNode1);};
    order RemoveNode(Node* targetNode){
      order deletedOrder = targetNode ->GetOrder();Node* before = targetNode->GetPrevious(); 
      Node* after = targetNode->GetNext(); 
      SutureNodes(before,after); 
      delete targetNode;
      return deletedOrder;
    };

    void DisplayInReverse(){
      Node* currentNode = GetLast();
      currentNode->PrintOrderDetails();
      while(currentNode ->GetPrevious() != nullptr){
        currentNode = currentNode->GetPrevious();
        currentNode->PrintOrderDetails();
      }
    }
    order UpdatePriority(Node* targetNode,int newPriority){
      auto[orderName, priority, location] =targetNode ->GetOrder();
      order newOrder = make_tuple(orderName,newPriority,location);
      targetNode->SetOrder(newOrder);
      return newOrder;
    };

  protected:
  Node* previous;
};
class SkipList : public DoubleLinkedList {
  public:
  SkipList();
  Node* GetHead(){
  }
  void Insert(Node* newNode){
    bool isInsertedSuccesfully = false;
    string newID = get<0>(newNode->GetOrder());
    Node* currentNode = GetHead();
    auto currentID = [](Node* current){return get<0>(current->GetOrder());};
    if(currentID(currentNode) < newID){
      SutureNodes(newNode,currentNode);
      isInsertedSuccesfully = true;
    } else {
      while(currentNode->GetNext()!=nullptr){
        currentNode = currentNode->GetNext();
        if(currentID(currentNode)<newID){
          SutureNodes(newNode,currentNode);
          isInsertedSuccesfully = true;
          break;
        }
      }
    }
    if(!isInsertedSuccesfully){
      if(currentNode != GetLast()){
        currentNode = GetLast();
      }
      SutureNodes(currentNode,newNode);
    }
  };
  void SutureNodes(Node* targetNode1, Node* targetNode2){

  };
  order RemoveNode(Node* targetNode){

  };
  Node* Search(){

  };

  protected:
  Node* nullHead;
  int maxLevel;
};

void vectorToLinkedList(ordersVector vect, LinkedList &list){
  for(order elem:vect){
    list.AppendNode(new Node(elem));
  }
};

void runPart1(){
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
};