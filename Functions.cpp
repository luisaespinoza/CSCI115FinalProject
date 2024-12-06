  #include <iostream>
  #include <fstream>
  #include <string>
  #include <cstdlib>
  #include <map>
  #include <vector>
  #include <tuple>
  #include <regex>
  #include <cmath>
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

vector<int> generateRandomList(int sizeN){
  vector<int> randomList;
  for(int i = 0; i < sizeN; i += 1){
    randomList.push_back(generateRandomNumber(5));
  }
  return randomList;
};

ordersVector generateNRandomOrders( int sizeN){
  vector<int> randomList = generateRandomList(sizeN);
  ordersVector newOrders;
  for(int i = 0; i < sizeN; i += 1){
    string orderPrefix = "ORD";
    string locationPrefix = "Location";
    string orderNumber = to_string(i);
    string location = locationPrefix + generateRandomLetterAZ();
    string orderName = orderPrefix+orderNumber;
    order newOrder = make_tuple(orderName,randomList.at(i),location);
    newOrders.push_back(newOrder);
  }
};
