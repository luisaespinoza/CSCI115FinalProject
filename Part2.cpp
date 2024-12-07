#ifndef Functions_cpp
  #include "Functions.cpp"
#endif
using namespace std;

void runPart2(){
  callback_function bubblePtr = bubbleSort;
  callback_function selectPtr = selectionSort;
  callback_function insertPtr = insertionSort;
  callback_function shellPtr = shellSort;
  ordersVector newOrders = generateNRandomOrders(10000);

};