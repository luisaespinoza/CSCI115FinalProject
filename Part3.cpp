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
  TreeNode* GetSuccessor(TreeNode* currentNode){
    currentNode = currentNode->GetRight();
    while (currentNode != nullptr && currentNode->GetLeft() != nullptr)
        currentNode = currentNode->GetLeft();
    return currentNode;
  }
  TreeNode* DeleteNode(TreeNode* localRoot, order orderToDelete){
    auto [name, priority, location] = orderToDelete;
    auto [rootName, rootPriority, rootLocation] = localRoot->GetOrder();
    // Base case
    if (localRoot == nullptr)
        return localRoot;

    // If order is in a subtree
    if(rootPriority > priority){localRoot->SetLeft(DeleteNode(localRoot->GetLeft(), orderToDelete));}
    else if(rootPriority < priority){localRoot->SetRight(DeleteNode(localRoot->GetRight(),orderToDelete));}
    // If root matches order
    else {

        // Cases when root has 0 branches
        // or only right branch
        if ( localRoot ->GetLeft() == nullptr){
            TreeNode* temp = localRoot->GetRight();
            delete[] localRoot;
            return temp;
        }

        // When root has only left branch
        if (localRoot->GetRight() == nullptr) {
            TreeNode* temp = localRoot->GetLeft();
            delete[] localRoot;
            return temp;
        }

        // When both branches exist
        TreeNode* successor = GetSuccessor(localRoot);
        localRoot->SetOrder(successor->GetOrder());
        localRoot->SetRight(DeleteNode(localRoot->GetRight(), successor->GetOrder()));
    }
    return localRoot;
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
  TreeNode* RotateInserted(TreeNode*localRoot, int priorityInserted){
    int balance = GetBalance(localRoot);
    int rootLeftBranchPriority = get<1>(localRoot->GetLeft()->GetOrder());
    int rootRightBranchPriority =get<1>(localRoot->GetRight()->GetOrder());
    // booleans to make if statements readable later
    // too many variable declarations??? Readability vs variable bloat..... hmmmmm
    bool isLeftHeavy = balance > -1;
    bool isRightHeavy = balance > 1;
    bool isLessThanLeft = priorityInserted < rootLeftBranchPriority;
    bool isLessThanRight = priorityInserted < rootRightBranchPriority;
    bool isGreaterThanLeft = priorityInserted > rootLeftBranchPriority;
    bool isGreaterThanRight = priorityInserted > rootRightBranchPriority;
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
  TreeNode* Insert(order newOrder,TreeNode* localRoot=nullptr) {
    auto [name , priority, location ] = newOrder;
    //if empty
    if (!localRoot) {return new TreeNode(newOrder);};
    auto [rootName, rootPriority , rootLocation] = localRoot->GetOrder();

    if (priority < rootPriority) {localRoot->SetLeft( Insert(newOrder, localRoot->GetLeft()));}
    else if (priority > rootPriority) {localRoot->SetRight( Insert(newOrder, localRoot->GetRight()));}
    else {return localRoot;};
    localRoot->SetHeight(1 + max(GetHeight(localRoot->GetLeft()), GetHeight(localRoot->GetRight())));
    //abstracted away all logic for rotation. Cases are checked by RotateInserted and handled there.
    return RotateInserted(localRoot, priority);
  };
};

void runPart3(){

};