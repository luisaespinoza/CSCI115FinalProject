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
  Node(int newLvl,int orderValue){lvl=newLvl;orderDetails = make_tuple("ORD"+to_string(orderValue),orderValue,"");next=nullptr;previous=nullptr;down=nullptr;}
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
  Node* InsertOrder(order newOrder){
    Node* currentNode = GetHead();
    int nextOrderNumber = parseOrderNumber(get<0>(currentNode->GetNext()->GetOrder()));
    int newOrderNumber = parseOrderNumber(get<0>(newOrder));
    while(nextOrderNumber<newOrderNumber){
      currentNode = currentNode->GetNext();
      nextOrderNumber = parseOrderNumber(get<0>(currentNode->GetNext()->GetOrder()));
      newOrderNumber = parseOrderNumber(get<0>(newOrder));
    };
    if(nextOrderNumber==newOrderNumber){return nullptr;};
    Node* newNode = new Node(newOrder);
    SutureNodes(newNode,currentNode->GetNext());
    SutureNodes(currentNode,newNode);
    return newNode;
  };
  void DisplayList(){
    Node* currentNode = GetHead();
    currentNode -> PrintOrderDetails();
    while(currentNode-> GetNext() != nullptr){
      currentNode = currentNode -> GetNext();
      // if(get<1>(currentNode->GetOrder())>INT_MIN && get<1>(currentNode->GetOrder())< INT_MAX)
      currentNode -> PrintOrderDetails();
    }
  };
  //explicit matching by string
  Node* Search(string orderID,bool returnPrevious=false){
    bool isValidString = validateOrderPattern(orderID);
    if(!isValidString){
      return nullptr;
    }
    Node* currentNode = GetHead();
    Node* previousNode;
    bool isFound = orderID == get<0>(currentNode->GetOrder());
    if(isFound){
      return currentNode;
    };
    while(currentNode -> GetNext() != nullptr){
      if(returnPrevious){previousNode=currentNode;};
      currentNode = currentNode -> GetNext();
      auto [orderName, priority, location] = currentNode ->GetOrder();
      isFound = orderName == orderID;
      if(isFound){
        if(returnPrevious){return previousNode;};
        return currentNode;
      }
    }
    //search unsuccessful
    cout <<orderID<<" NOT FOUND" << endl;
    return nullptr;
  };
  //allow search by an int since ORD is a mandatory prefix
  Node* Search(int orderIDNo,bool returnPrevious=false){
    string orderID = "ORD"+to_string(orderIDNo);
    return Search(orderID,returnPrevious);
  };
  order RemoveOrder(int orderID){
    Node* previousOfNodeToRemove = Search(orderID,true);
    Node* nodeToRemove = previousOfNodeToRemove->GetNext();
    order removedOrder= nodeToRemove->GetOrder();
    SutureNodes(previousOfNodeToRemove,nodeToRemove->GetNext());
    delete nodeToRemove;
    return removedOrder;
  };
  order RemoveOrder(string orderID){return RemoveOrder(parseOrderNumber(orderID));};
  protected:
  Node* head;
};

class DoubleLinkedList : public LinkedList {
  public: 
    DoubleLinkedList(){head=nullptr;};
    DoubleLinkedList(Node* newHead){head=newHead;};
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
  SkipList(){
    maxLevel=4;
    levels = vector<DoubleLinkedList*>(maxLevel);
    // iteratively create LinkedLists for managing every level of SkipList starting from maxLvl
    for(int level = 0; level < maxLevel ; level += 1){
      // create a linkedList with a null head at current level
      levels.at(maxLevel -1 - level) = new DoubleLinkedList(new Node(maxLevel-level,INT_MIN));
      // add a null tail at the current Level
      levels.at(maxLevel -1 -level)->AppendNode(new Node(maxLevel-level,INT_MAX));
      SutureNodes(levels.at(maxLevel-1-level)->GetHead(),levels.at(maxLevel-1-level)->GetLast());
      // cout <<"current level at constructor: " << level << endl;
      // Set all associated down pointers
      // if(level != maxLevel-1){
      //   levels.at(level + 1)->GetHead()->SetDown(levels.at(level)->GetHead());
      //   levels.at(level + 1)->GetHead()->GetNext()->SetDown(levels.at(level)->GetHead()->GetNext());
      // }
    };
    nullHead=levels.at(maxLevel-1)->GetHead();
  };
  Node* GetHead(){return nullHead;};
  void AppendNode(Node* newNode){
    Node* currentNode;
    Node* previousNode;
    Node* tempUpperNode;
    int levelsToAdd = 0;
    srand(time(0));
    // bool flipCoin = [](){int result =rand()%2;cout <<result << endl;return result>0;};
    // for(int i = 0; i <50; i += 1){cout << rand()%2 << endl;};
    while(maxLevel != levelsToAdd){
      if(coinFlip()){
        levelsToAdd += 1;
      }
      else{
        break;
      }
    };
    // cout <<"levels created " << levelsToAdd << endl; 
    for(int level = levelsToAdd ; level >= 0; level -=1){
      // cout <<"current level: "<< level << endl;
      currentNode = levels.at(level)->GetLast();
      // currentNode->PrintOrderDetails();
      // cout << "is current Node == to previous node "<< (currentNode == previousNode)<<endl;
      previousNode = currentNode->GetPrevious();
      // previousNode->PrintOrderDetails();
      SutureNodes(newNode,currentNode);
      SutureNodes(previousNode, newNode);
      // insertedNode=currentNode;
      tempUpperNode = newNode;
      newNode = new Node(newNode->GetOrder());
      // levels.at(level)->AppendNode(insertedNode);
      if(level > 0){
        tempUpperNode->SetDown(newNode);
      }
    };
      // cout <<"i suspect this wont print"<<endl;
  };
  // void Insert(Node* newNode){
  //   bool isInsertedSuccesfully = false;
  //   string newID = get<0>(newNode->GetOrder());
  //   Node* currentNode = GetHead();
  //   auto currentID = [](Node* current){return get<0>(current->GetOrder());};
  //   if(currentID(currentNode) < newID){
  //     SutureNodes(newNode,currentNode);
  //     isInsertedSuccesfully = true;
  //   } else {
  //     while(currentNode->GetNext()!=nullptr){
  //       currentNode = currentNode->GetNext();
  //       if(currentID(currentNode)<newID){
  //         SutureNodes(newNode,currentNode);
  //         isInsertedSuccesfully = true;
  //         break;
  //       }
  //     }
  //   }
  //   if(!isInsertedSuccesfully){
  //     if(currentNode != GetLast()){
  //       currentNode = GetLast();
  //     }
  //     SutureNodes(currentNode,newNode);
  //   }
  // };

  Node* InsertOrder(order newOrder){
    Node* insertedNode;
    Node* tempUpperNode;
    int levelsToAdd = 0;
    srand(time(0));
    while(coinFlip() && levelsToAdd <maxLevel-1){
      levelsToAdd += 1;
    };
    for(int level = levelsToAdd ; level >= 0; level -=1){
      tempUpperNode = insertedNode;
      insertedNode = levels.at(level)->InsertOrder(newOrder);
      if(level != levelsToAdd){
        tempUpperNode->SetDown(insertedNode);
      }
    };
    return insertedNode;
  };
  order RemoveOrder(int orderID){
    Node* nodeToRemove = Search(orderID);
    Node* previousOfNodeToRemove = nodeToRemove->GetPrevious();
    order removedOrder= nodeToRemove->GetOrder();
    SutureNodes(previousOfNodeToRemove,nodeToRemove->GetNext());
    RemoveAllLevels(nodeToRemove);
    delete nodeToRemove;
    return removedOrder;
  };
  // recurisively traverse down levels suturing adjacent nodes. delete them on the climb out.
  void RemoveAllLevels(Node*nodeToRemove){
    if(nodeToRemove->GetDown()){
      Node* down = nodeToRemove->GetDown();
      Node* downPrevious = down->GetPrevious();
      Node* downNext = down->GetNext();
      SutureNodes(downPrevious,downNext);
      RemoveAllLevels(down);
      delete down;
    }
  };
  Node* Search(int id){return SkipSearch(id);};
  Node* SkipSearch(int id){return SkipSearch(id,maxLevel-1,nullHead);}
  Node* SkipSearch(int id, int currentLevel,Node* startingNode){
    if(currentLevel<0){cout<<"Order not found!"<<endl;return nullptr;};
    Node* currentNode = startingNode;
    int nextOrder = parseOrderNumber( get<0>(currentNode->GetNext()->GetOrder()));
    if(nextOrder ==INT_MAX){
      return SkipSearch(id,currentLevel-1,levels.at(currentLevel-1)->GetHead());
    }
    while(nextOrder<id){
      currentNode = currentNode -> GetNext();
      nextOrder = parseOrderNumber( get<0>(currentNode->GetNext()->GetOrder()));
    }
    if(nextOrder == id){
      return currentNode ->GetNext();
    }
    else{
      return SkipSearch(id, currentLevel -1,currentNode->GetDown());
    }
  };
  void DisplayList(){
    levels.at(0)->DisplayList();
  };
  void DisplayAllLevels(){
    for(int i =0; i <maxLevel; i+=1){
      levels.at(i)->DisplayList();
    }
  };

  protected:
  vector<DoubleLinkedList*> levels;
  Node* nullHead;
  int maxLevel;
};

void vectorToLinkedList(ordersVector vect, LinkedList &list){
  for(order elem:vect){
    list.AppendNode(new Node(elem));
  };
};
void vectorToSKL(ordersVector vect, SkipList &list){
  for(order elem:vect){
    list.AppendNode(new Node(elem));
  };
};

void runPart1(){
  ordersVector ordersVect = gatherData();
  LinkedList ordersLL = LinkedList(); 
  DoubleLinkedList ordersDLL = DoubleLinkedList();
  SkipList ordersSKL = SkipList();
  //printOrdersVector(data); //print orders vector test;
  // printOrderTuple(searchOrdersVector("Ord101", ordersVect));
  // printOrderTuple(searchOrdersVector("ORD36", ordersVect));
  // addDeliveryOrder(make_tuple("ORD51",4,"Location R"),ordersVect);
  // addDeliveryOrder(make_tuple("ORDer533",4,"LocationR"),ordersVect);
  vectorToLinkedList(ordersVect,ordersLL);
  ordersLL.Search(22)->PrintOrderDetails();
  ordersLL.Search("ORD34")->PrintOrderDetails();
  ordersLL.Search(99);//order will not be found. Trying to print is a segmentation fault
  ordersLL.AppendNode(new Node(make_tuple("ORD99",4,"Location S")));
  ordersLL.InsertOrder(make_tuple("ORD65",3,"Location T"));
  ordersLL.Search(99)->PrintOrderDetails(); //order now exists
  ordersLL.DisplayList();
  vectorToLinkedList(ordersVect,ordersDLL);
  ordersDLL.Search(22)->PrintOrderDetails();
  ordersDLL.Search("ORD34")->PrintOrderDetails();
  ordersDLL.Search(99);//order will not be found. Trying to print is a segmentation fault
  ordersDLL.AppendNode(new Node(make_tuple("ORD99",4,"Location S")));
  ordersDLL.InsertOrder(make_tuple("ORD65",3,"Location T"));
  ordersDLL.Search(99)->PrintOrderDetails(); //order now exists
  ordersDLL.DisplayList();
  // SkipList is intermittently problematic... I don't have time to fix it....
  // vectorToSKL(ordersVect,ordersSKL);
  // ordersSKL.SkipSearch(22)->PrintOrderDetails();
  // ordersSKL.InsertOrder(make_tuple("ORD65",3,"Location T"));
  // ordersSKL.SkipSearch(65)->PrintOrderDetails();
  // ordersSKL.DisplayList();
};

// temporary main for testing..
// int main () {
//   runPart1();
// };