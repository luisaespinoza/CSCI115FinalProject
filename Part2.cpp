#ifndef Functions_h
#define Functions_h
  #include "Functions.h"
#endif
using namespace std;

void runPart2(){
  // assign names to ptrs of our algorithms
  callback_function bubblePtr = bubbleSort;
  callback_function selectPtr = selectionSort;
  callback_function insertPtr = insertionSort;
  callback_function shellPtr = shellSort;
  callback_function callbacks[4] = {bubblePtr,selectPtr,insertPtr,shellPtr};

  ordersVector ordersTable = gatherData();
  for( int i =0 ; i < 10 ; i += 1){
  // generate a random list 10 times and test it against all algorithms in order each time
    ordersVector newOrders = generateNRandomOrders(10000);
    for(callback_function callbackFunction:callbacks){
      // two copies of the same random array for use with this algorithm
      // once for first 1000 and again for all elements
      // every iteration will change algorithm and use the SAME list of orders
      ordersVector newOrdersCopy1(newOrders.begin(),newOrders.end());
      ordersVector newOrdersCopy2(newOrders.begin(),newOrders.end());
      int nOptional=1000;
      measureExecutionTime(callbackFunction,newOrdersCopy2, nOptional);
      measureExecutionTime(callbackFunction,newOrdersCopy2);

      // now the original table of orders
      ordersVector ordersTableCopy(ordersTable.begin(),ordersTable.end());
      measureExecutionTime(callbackFunction,ordersTableCopy);
    }
  }
};