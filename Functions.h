  #include <iostream>
  #include <fstream>
  #include <string>
  #include <cstdlib>
  #include <map>
  #include <vector>
  #include <tuple>
  #include <regex>
  #include <cmath>
  #include <ctime>
using namespace std;

// ^^Part1
typedef tuple<std::string, int , std::string> order;
typedef vector<order> ordersVector;
typedef void (*callback_function)(ordersVector &unsortedOrders,int nOptional); // definition type for conciseness


void measureExecutionTime(callback_function sortFunction, ordersVector &unsortedOrders, int nOptional=INT_MIN) {
  int size = unsortedOrders.size();
  clock_t start = clock();
  sortFunction(unsortedOrders,nOptional);
  clock_t end = clock();
  double elapsed = double(end - start) / CLOCKS_PER_SEC;
  std::cout << "Time taken for n = " << size << ": " << elapsed << " seconds" << std::endl;
};

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
  isValidFormat = validateOrderPattern(orderName);
  isValidFormat = validateLocationPattern(location);
  return isValidFormat;
};

void printOrderTuple(order const &singleOrder){
  auto [detail1, detail2, detail3] = singleOrder;
  cout << detail1 << endl << "Priority: " << detail2 << endl << "Destination: " << detail3 << endl;
};
//single reuseable node for all versions linkedlist
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

// ^^Part2
string generateRandomLetterAZ(){
  srand(0);
  //65-90 is the ASCII decimal value for [A-Z]
  string randomLetter = to_string( static_cast<char>( floor( 65 + rand() * 26)));
  return randomLetter;
};

//range is zero inclusive [0-range]
int generateRandomNumber(int range){
  srand(time(0));
  int randomInt = floor( rand()*range);
  return randomInt;
};

ordersVector generateNRandomOrders( int sizeN){
  ordersVector newOrders;
  for(int i = 0; i < sizeN; i += 1){
    string orderPrefix = "ORD";
    string locationPrefix = "Location";
    string orderNumber = to_string(i);
    string location = locationPrefix + generateRandomLetterAZ();
    string orderName = orderPrefix+orderNumber;
    int maxPriority = 5;
    int priority =generateRandomNumber(maxPriority);
    order newOrder = make_tuple(orderName,priority,location);
    newOrders.push_back(newOrder);
  }
  return newOrders;
};
void swap(order &order1,order &order2){
  order tempOrder = order1;
  order1=order2;
  order2=tempOrder;
};

int getPriority(order currentOrder){
  return get<1>(currentOrder);
};

void bubbleSort(ordersVector &unsortedOrders,int nOptional = INT_MIN) {
  //Sort first N if N is given as argument : otherwise sort the whole vector
  int size = nOptional > 1? nOptional : unsortedOrders.size();
  for (int i = 0; i < size - 1; i+=1) {
    for (int j = 0; j < size - i - 1; j+=1) {
        if ( getPriority(unsortedOrders.at(j)) > getPriority(unsortedOrders.at(j+1))) {
          swap(unsortedOrders.at(j),unsortedOrders.at(j+1));
        };
    };
  };
};

void selectionSort(ordersVector &unsortedOrders, int nOptional = INT_MIN) {
  //Sort first N if N is given as argument : otherwise sort the whole vector
  int size = nOptional > 1 ?nOptional : unsortedOrders.size();
  for (int i = 0; i < size - 1; i+=1) {
    int lowerBound = i;
    for (int j = i + 1; j < size; j+=1) {
      if (getPriority(unsortedOrders.at(j)) < getPriority(unsortedOrders.at(lowerBound)))
      lowerBound = j;
    }
    swap(unsortedOrders.at(i), unsortedOrders.at(lowerBound));
  }
};

void insertionSort(ordersVector &unsortedOrders, int nOptional=INT_MIN) {
  //Sort first N if N is given as argument : otherwise sort the whole vector
  int size = nOptional > 1 ? nOptional : unsortedOrders.size();

  // int shifts = 0;
  for (int i = 1; i < size; i+=1) {
    int key = getPriority(unsortedOrders.at(i));
    order keyOrder = unsortedOrders.at(i);
    int j = i - 1;
    while (j >= 0 && getPriority(unsortedOrders.at(j)) > key) {
      unsortedOrders.at(j+1) = unsortedOrders.at(j);
      j-=1;
  // shifts += 1;
    }
    unsortedOrders.at(j+1) = keyOrder;
  }
// cout<< "Shifts: " << shifts << endl;
};

void shellSort(ordersVector &unsortedOrders, int nOptional=INT_MIN) {
  //Sort first N if N is given as argument : otherwise sort the whole vector
  int size = nOptional > 1?nOptional : unsortedOrders.size();
  for (int gap = size / 2; gap > 0; gap /= 2) {
    for (int i = gap; i < size; i+=1) {
      int temp = getPriority(unsortedOrders.at(i));
      order tempOrder = unsortedOrders.at(i);
      int j;
      for (j = i; j >= gap && getPriority(unsortedOrders.at(j-gap))> temp; j -= gap) {
        unsortedOrders.at(j) =  unsortedOrders.at(j - gap);
      }
      unsortedOrders.at(j) = tempOrder;
    }
  }
};

// ^^ Part3
class TreeNode {
  public:
    TreeNode(){orderDetails=make_tuple("",INT_MIN,"");left=nullptr;right=nullptr;};
    TreeNode(order newOrder,TreeNode* newLeft=nullptr,TreeNode* newRight=nullptr){orderDetails=newOrder;left=newLeft;right=newRight;};
    ~TreeNode(){SetLeft(nullptr);SetRight(nullptr);};
    TreeNode* GetLeft(){return left;};
    void SetLeft(TreeNode* newLeft){left=newLeft;};
    TreeNode* GetRight(){return right;};
    void SetRight(TreeNode* newRight){right=newRight;};
    void SetOrder(order newOrder){orderDetails=newOrder;};
    order GetOrder(){return orderDetails;};
    void PrintOrderDetails(){printOrderTuple(orderDetails);};
  private:
    order orderDetails;
    TreeNode* left;
    TreeNode* right;
};

class BST {
  public:
  BST(){};
  TreeNode* GetRoot(){return root;};
  void SetRoot(TreeNode* newRoot){root = newRoot;};
  TreeNode* Insert(TreeNode* localRoot,order newOrder){
    if(localRoot ==nullptr){return new TreeNode(newOrder);};
    int newOrderPriority = get<1>(newOrder);
    int rootPriority = get<1>(localRoot->GetOrder());
    if(newOrderPriority < rootPriority){
      localRoot->SetLeft(Insert(localRoot->GetLeft(),newOrder));
    } else if(newOrderPriority > rootPriority){
      localRoot->SetRight(Insert(localRoot->GetLeft(),newOrder));
    }
    return localRoot;
  };
  void InOrder(TreeNode* localRoot){
    if(localRoot == nullptr){return;};
    InOrder(localRoot->GetLeft());
    localRoot->PrintOrderDetails();
    InOrder(localRoot->GetRight());
  };
  TreeNode* Search(TreeNode* localRoot, order candidateOrder){
    auto [rootName, rootPriority, rootLocation] = localRoot ->GetOrder();
    auto [candidateName, candidatePriority, candidateLocation] = candidateOrder;
    if(localRoot == nullptr) {
      cout << "ORDER NOT FOUND" << endl;
      return nullptr;};
    if(rootName == candidateName){return localRoot;} 
    else if (candidatePriority < rootPriority){
      return Search(localRoot->GetLeft(), candidateOrder);
    }
    else {
      return Search(localRoot->GetRight(),candidateOrder);
    }
  };
  order Delete(order orderToDelete){
    TreeNode* nodeToDelete = Search(root,orderToDelete);
    delete[] nodeToDelete;
  };
  private:
  TreeNode* root;
};
