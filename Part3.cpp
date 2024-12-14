#ifndef Functions_h
#define Functions_h
  #include "Functions.h"
#endif
using namespace std;

// multipurpose TreeNode for BST and AVL
class TreeNode {
  public:
    TreeNode(){height=INT_MIN;orderDetails=make_tuple("",INT_MIN,"");localRoot=nullptr;left=nullptr;right=nullptr;};
    TreeNode(order newOrder,TreeNode* newLeft=nullptr,TreeNode* newRight=nullptr){orderDetails=newOrder; localRoot=nullptr;left=newLeft;right=newRight;};
    ~TreeNode(){SetLeft(nullptr);SetRight(nullptr);};
    TreeNode* GetLeft(){return left;};
    void SetLeft(TreeNode* newLeft){left=newLeft;};
    TreeNode* GetRight(){return right;};
    void SetRight(TreeNode* newRight){right=newRight;};
    TreeNode* GetRoot(){return localRoot;};
    void SetRoot(TreeNode* newRoot){localRoot=newRoot;};
    void SetOrder(order newOrder){orderDetails=newOrder;};
    order GetOrder(){return orderDetails;};
    int GetHeight(){return height;};
    void SetHeight(int newHeight){height = newHeight;};
    bool IsLeaf(){bool isLeaf = left == nullptr && right ==nullptr? true : false; return isLeaf;};
    void PrintOrderDetails(){printOrderTuple(orderDetails);};
  private:
    int height;
    order orderDetails;
    TreeNode* localRoot;
    TreeNode* left;
    TreeNode* right;
};
class BST {
  public:
  BST(){root = nullptr;};
  TreeNode* GetRoot(){return root;};
  void SetRoot(TreeNode* newRoot){root = newRoot;};
  
  /*BST implementation does nothing but return the node it was given
  basically serves as a stub for AVL to override the definition later.
  allows code reuse by stubbing the exact moment AVL's definition for Insert diverges from BST
  an empty call isn't very elegant I'll admit but it does save a lot of recoding of the same behavior*/
  TreeNode* RotateInserted(TreeNode* localRoot, int orderNumber){return localRoot;};
  //BST implementation is again empty. Stubbing for the moment AVL will diverge from BST's Delete Definition
  TreeNode* Rebalance(TreeNode* localRoot, order newOrder){return localRoot;};
  TreeNode* Insert(TreeNode* localRoot,order newOrder){
    //wrapper for code reuse and avoiding later redefinition by AVL
    //AVL definition will also call BSTInsert but extende fuctionality to include rotations
    if(root != nullptr){ // the actual root of our tree
      if(localRoot == nullptr){return new TreeNode(newOrder);};
      int orderNumber = parseOrderNumber(get<0>(newOrder));
      // int newOrderPriority = get<1>(newOrder);
      int rootOrderNumber = parseOrderNumber(get<0>(localRoot->GetOrder()));
      // int rootPriority = get<1>(localRoot->GetOrder());
      if(orderNumber < rootOrderNumber){
        localRoot->SetLeft(Insert(localRoot->GetLeft(),newOrder));
      } else if(orderNumber > rootOrderNumber){
        localRoot->SetRight(Insert(localRoot->GetRight(),newOrder));
      }
      else {return localRoot;};
      return RotateInserted(localRoot, orderNumber);
    } else{
      SetRoot(new TreeNode(newOrder));
      return root;
    }
    return localRoot;
  };
  void InOrder(TreeNode* localRoot){
    if(localRoot == nullptr){return;};
    InOrder(localRoot->GetLeft());
    localRoot->PrintOrderDetails();
    InOrder(localRoot->GetRight());
  };
  // second definition for convenience. The recursive nature of the orignal definition requires an argument of TreeNode*
  // however most of the time we want to start from the root of the BST anyway
  // it would be convenient to not have to feed the root as an argument every time we want to begin at the top
  // an empty argument list has the intuitive behavior of traversing beginning from the root
  // unfortunately the argument cannot be initialized to the data member of root
  // initiliazition of unfed arguments have to be literal values
  void InOrder(){InOrder(root);};
  TreeNode* Search(TreeNode* localRoot, int candidateNumber){
    if(localRoot == nullptr) {
      cout << "ORDER NOT FOUND" << endl;
      order nullOrder = make_tuple("No Order", INT_MIN, "No Location");
      return new TreeNode(nullOrder);
    };
    auto [rootName, rootPriority, rootLocation] = localRoot ->GetOrder();
    int rootNumber = parseOrderNumber(rootName);
    if(rootNumber == candidateNumber){return localRoot;} 
    else if (candidateNumber < rootNumber){
      return Search(localRoot->GetLeft(), candidateNumber);
    }
    else {
      return Search(localRoot->GetRight(),candidateNumber);
    }
  };
  // simplifies the call for Search. Again we often want to start at the root
  // both of these will call the recursive search defined earlier with the root as argument
  TreeNode* Search(int candidateNumber){
    return Search(root, candidateNumber);
  }
  TreeNode* Search(string candidateName){
    int orderNumber = parseOrderNumber(candidateName);
    return Search(root,orderNumber);
  };


  TreeNode* GetSuccessor(TreeNode* currentNode){
    currentNode = currentNode->GetRight();
    while (currentNode != nullptr && currentNode->GetLeft() != nullptr)
        currentNode = currentNode->GetLeft();
    return currentNode;
  }
  TreeNode* DeleteNode(TreeNode* localRoot, order orderToDelete){
    auto [name, priority, location] = orderToDelete;
    auto [rootName, rootPriority, rootLocation] = localRoot->GetOrder();
    int rootOrderNumber = parseOrderNumber(rootName);
    int orderNumber = parseOrderNumber(name);
    // Base case
    if (localRoot == nullptr){return localRoot;};

    // If order is in a subtree
    if(rootOrderNumber > orderNumber){localRoot->SetLeft(DeleteNode(localRoot->GetLeft(), orderToDelete));}
    else if(rootOrderNumber < orderNumber){localRoot->SetRight(DeleteNode(localRoot->GetRight(),orderToDelete));}
    // If root matches order
    else {
        // Cases when root has 0 branches
        // or only right branch
        if ( localRoot ->GetLeft() == nullptr){
            TreeNode* temp = localRoot->GetRight();
            delete localRoot;
            return temp;
        }
        // When root has only left branch
        if (localRoot->GetRight() == nullptr) {
            TreeNode* temp = localRoot->GetLeft();
            delete localRoot;
            return temp;
        }

        // When both branches exist
        TreeNode* successor = GetSuccessor(localRoot);
        localRoot->SetOrder(successor->GetOrder());
        localRoot->SetRight(DeleteNode(localRoot->GetRight(), successor->GetOrder()));
    }
    if(localRoot == nullptr){return localRoot;};
    return Rebalance(localRoot, orderToDelete);
    // return localRoot;
  };
  // a simpler delete operation where the only argument is the ordernumber
  // searches first to see if order exists, then calls the recursive version
  TreeNode* DeleteNode(int orderNumber){
    TreeNode* candidateNode = Search(orderNumber);
    // if the numbers don't match, search failed and there's nothing to delete
    if(get<1>(candidateNode->GetOrder())==INT_MIN){
      return nullptr;
    }
    return DeleteNode(root, candidateNode->GetOrder() );
  };
  protected:
  TreeNode* root;
};
class AVLTree : public BST {
  public:
  AVLTree(){root=nullptr;};
  int GetHeight(TreeNode* targetNode) { return targetNode ? targetNode->GetHeight() : 0; }
  int GetBalance(TreeNode* targetNode) { return targetNode ? GetHeight(targetNode->GetLeft()) - GetHeight(targetNode->GetRight()) : 0; }
  TreeNode* RotateRight(TreeNode* localRoot) {
    TreeNode* leftBranch = localRoot->GetLeft();
    TreeNode* leftTree= leftBranch->GetRight();
    leftBranch->SetRight(localRoot);
    localRoot->SetLeft(leftTree);
    localRoot->SetHeight( max(GetHeight(localRoot->GetLeft()), GetHeight(localRoot->GetRight())) + 1);
    leftBranch->SetHeight( max(GetHeight(leftBranch->GetLeft()), GetHeight(leftBranch->GetRight())) + 1);
    return leftBranch;
  };
  TreeNode* RotateLeft(TreeNode* localRoot) {
    TreeNode* rightBranch = localRoot->GetRight();
    TreeNode* leftTree = rightBranch->GetLeft();
    rightBranch->SetLeft(localRoot);
    localRoot->SetRight(leftTree);
    localRoot->SetHeight(max(GetHeight(localRoot->GetLeft()), GetHeight(localRoot->GetRight())) + 1);
    rightBranch->SetHeight(max(GetHeight(rightBranch->GetLeft()), GetHeight(rightBranch->GetRight())) + 1);
    return rightBranch;
  };
  TreeNode* RotateInserted(TreeNode*localRoot, int orderNumberInserted){
    localRoot->SetHeight(1 + max(GetHeight(localRoot->GetLeft()), GetHeight(localRoot->GetRight())));
    int balance = GetBalance(localRoot);
    int rootLeftBranch = parseOrderNumber(get<0>(localRoot->GetLeft()->GetOrder()));
    int rootRightBranch = parseOrderNumber(get<0>(localRoot->GetRight()->GetOrder()));
    // booleans to make if statements readable later
    // too many variable declarations??? Readability vs variable bloat hmmmmm......
    bool isLeftHeavy = balance < -1;
    bool isRightHeavy = balance > 1;
    bool isLessThanLeft = orderNumberInserted < rootLeftBranch;
    bool isLessThanRight = orderNumberInserted < rootRightBranch;
    bool isGreaterThanLeft = orderNumberInserted > rootLeftBranch;
    bool isGreaterThanRight = orderNumberInserted > rootRightBranch;
    // R rotation
    if (isRightHeavy && isLessThanLeft) {return RotateRight(localRoot);};
    //L rotation
    if (isLeftHeavy && isGreaterThanRight) {return RotateLeft(localRoot);};
    //LR rotation
    if (isRightHeavy && isGreaterThanLeft) {
      localRoot->SetLeft( RotateLeft(localRoot->GetLeft()));
      return RotateRight(localRoot);
    }//RL rotation
    if (isLeftHeavy && isLessThanRight) {
      localRoot->SetRight( RotateRight(localRoot->GetRight()));
      return RotateLeft(localRoot);
    }
    //no rotation
    return localRoot;
  };
  TreeNode* Rebalance(TreeNode* localRoot){
    localRoot->SetHeight(1 + max(GetHeight(localRoot->GetLeft()),GetHeight(localRoot->GetRight())));
    int balance = GetBalance(localRoot);
    int rightBranchBalance = GetBalance(localRoot->GetRight());
    int leftBranchBalance = GetBalance(localRoot->GetLeft());
    bool isLeftHeavy = balance < -1;
    bool isRightHeavy = balance > 1;
    if(isRightHeavy && leftBranchBalance >=0){return RotateRight(localRoot);};
    if(isRightHeavy && leftBranchBalance < 0){localRoot->SetLeft(RotateLeft(localRoot->GetLeft()));return RotateRight(localRoot);};
    if(isLeftHeavy && rightBranchBalance <=0){return RotateLeft(localRoot);};
    if(isLeftHeavy && rightBranchBalance > 0){localRoot->SetRight(RotateRight(localRoot->GetRight()));return RotateLeft(localRoot);};
    //if no rotations return unchanged
    return localRoot;
  };
};

void measureExecutionTime(BST &obj, int candidate) {
  clock_t start = clock();
  obj.Search(candidate)->PrintOrderDetails();
  clock_t end = clock();
  double elapsed = double(end - start) / CLOCKS_PER_SEC;
  std::cout << "Time taken for BST search : " << elapsed << " seconds" << std::endl;
};
void measureExecutionTime(AVLTree &obj, int candidate) {
  clock_t start = clock();
  obj.Search(candidate)->PrintOrderDetails();
  clock_t end = clock();
  double elapsed = double(end - start) / CLOCKS_PER_SEC;
  std::cout << "Time taken for AVL search : " << elapsed << " seconds" << std::endl;
};


// Test binary search with at least 3 Order IDs (existent and non-existent)
// o Insert all delivery orders into the BST and AVL Tree, then perform searches for 5
// specific Order IDs
// o Delete a few orders from the BST and AVL Tree, then verify the structures
void runPart3(){
  ordersVector ordersTable = gatherData();
  BST ordersBST = BST();
  AVLTree ordersAVL = AVLTree();
  vector<int> searchList = {1, 31 ,82, 26, 51, 4, 47};
  for(order currentOrder: ordersTable){
    ordersBST.Insert(ordersBST.GetRoot(),currentOrder);
    ordersAVL.Insert(ordersAVL.GetRoot(),currentOrder);
  }
  ordersBST.InOrder();
  ordersAVL.InOrder();
  for(int candidate:searchList){
    measureExecutionTime(ordersBST,candidate);
    measureExecutionTime(ordersAVL,candidate);
  }
  ordersBST.DeleteNode(31);
  ordersBST.DeleteNode(12);
  ordersBST.DeleteNode(45);
  ordersBST.InOrder();
  ordersAVL.DeleteNode(31);
  ordersAVL.DeleteNode(12);
  ordersAVL.DeleteNode(45);
  ordersAVL.InOrder();
};


// modular main used in testing. Not needed in production file
// int main () {
//   runPart3();
//   return 0;
// }