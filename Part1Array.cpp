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
    word = tempString.at(0); //ideally, all that's left now is a single character with location id
    parsedStrings.push_back(word);
  }
  return parsedStrings;
};

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
            returnVector.push_back(make_tuple(parsed.at(0),priority, location));
          };
        };
    };
    return returnVector;
};

void printOrderTuple(order const &singleOrder){
  auto [detail1, detail2, detail3] = singleOrder;
  cout << detail1 << endl << "Priority: " << detail2 << endl << "Destination: " << detail3 << endl;
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

bool isValidOrder(order const &newOrder){
  string orderPrefix = "ORD";
  string orderNumber = "[0-9]+";
  string locationPrefix = "Location ";
  string locationPostfix = "[A-Z]{1}";
  regex orderPattern("^"+orderPrefix+orderNumber+"$");
  regex locationPattern("^"+locationPrefix+locationPostfix+"$");
  auto [number, priority,location] = newOrder;
  bool isValidFormat = typeid(priority)==typeid(int);
  printOrderTuple(newOrder);
  // cout << priority << " " << isValidFormat << endl;
  isValidFormat = regex_search(number,orderPattern);
  // cout << isValidFormat << endl;
  isValidFormat = regex_search(location,locationPattern);
  // cout << isValidFormat << endl;
  return isValidFormat;
};

int addDeliveryOrder(order const &newOrder, ordersVector &orders){
  if(isValidOrder(newOrder)){
    orders.push_back(newOrder);
    return orders.size()-1;
  }
  cout << "INVALID ORDER FORMAT. ORDER NOT ADDED." << endl;
  return INT_MIN;
};

int main () {
  ordersVector data = gatherData();
  //printOrdersVector(data); //print orders vector test;
  printOrderTuple(searchOrdersVector("Ord101", data));
  printOrderTuple(searchOrdersVector("ORD36", data));
  addDeliveryOrder(make_tuple("ORD51",4,"Location R"),data);
  addDeliveryOrder(make_tuple("ORDer533",4,"LocationR"),data);
  return 0;
};