  #include <iostream>
  #include <fstream>
  #include <string>
  #include <cstdlib>
  #include <map>
  #include <vector>
  #include <tuple>
  #include <queue>
  #include <stack>
  #include <functional>
  #include <regex>
  #include <cmath>
  #include <numeric>
  #include <random>
  #include <ctime>
using namespace std;

// ^^Part1
typedef tuple<std::string, int , std::string> order;
typedef vector<order> ordersVector;
typedef void (*callback_function)(ordersVector &unsortedOrders,int nOptional); // definition type for conciseness
double measureExecutionTime(callback_function sortFunction, ordersVector &unsortedOrders, int nOptional=INT_MIN) {
  int size = nOptional > 0 ? nOptional: unsortedOrders.size();
  clock_t start = clock();
  sortFunction(unsortedOrders,nOptional);
  clock_t end = clock();
  double elapsed = double(end - start) / CLOCKS_PER_SEC;
  std::cout << "Time taken for n = " << size << ": " << elapsed << " seconds" << std::endl;
  return elapsed;
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

//extracts the number 
int parseOrderNumber(string orderName){
  string number = orderName.substr(3);
  int orderNumber = 0;
  int size = number.size();
  int power = size - 1;
  //not a pretty solution but this takes string size as the upper bound for powers of ten
  //ie 432 has size()==3 so power is 2 as in 4*1^2 + 3*10^1 + 2*10^0
  for(char num : number){
    orderNumber += atoi(&num) * pow(10, power);
    // cout<<"Number =" << orderNumber << endl;
    power -= 1;
  };
  return orderNumber;
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
bool coinFlip(){
  double mean = 0.0;        // Mean of the distribution
  double stddev = 1.0;      // Standard deviation of the distribution
  random_device seed;  // Obtain a seed from the system
  mt19937 gen(seed()); // Standard mersenne_twister_engine seeded with rd()
  std::normal_distribution<> distrib(mean, stddev);
  double randomNumber = distrib(gen);
  return randomNumber >0.5;
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
  for (int i = 1; i < size; i+=1) {
    int key = getPriority(unsortedOrders.at(i));
    order keyOrder = unsortedOrders.at(i);
    int j = i - 1;
    while (j >= 0 && getPriority(unsortedOrders.at(j)) > key) {
      unsortedOrders.at(j+1) = unsortedOrders.at(j);
      j-=1;
    }
    unsortedOrders.at(j+1) = keyOrder;
  }
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

double average(vector<double> &vect){
  double sum = accumulate(vect.begin(),vect.end(),0.0);
  double size  =  static_cast<double>(vect.size());
  return sum / size;
};

