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
  void SutureNodes(){};
  TreeNode* GetSuccessor(TreeNode* curr){
    curr = curr->GetRight();
    while (curr != nullptr && curr->GetLeft() != nullptr)
        curr = curr->GetLeft();
    return curr;
  }
  TreeNode* DeleteNode(TreeNode* localRoot, order orderToDelete){
    auto [name, priority, location] = orderToDelete;
    auto [rootName, rootPriority, rootLocation] = localRoot->GetOrder();
    // Base case
    if (localRoot == nullptr)
        return localRoot;

    // If key to be searched is in a subtree
    if(rootPriority > priority){localRoot->SetLeft(DeleteNode(localRoot->GetLeft(), orderToDelete));}
    else if(rootPriority < priority){localRoot->SetRight(DeleteNode(localRoot->GetRight(),orderToDelete));}
    // If root matches with the given key
    else {

        // Cases when root has 0 children
        // or only right child
        if ( localRoot ->GetLeft() == nullptr){
            TreeNode* temp = localRoot->GetRight();
            delete[] localRoot;
            return temp;
        }

        // When root has only left child
        if (localRoot->GetRight() == nullptr) {
            TreeNode* temp = localRoot->GetLeft();
            delete[] localRoot;
            return temp;
        }

        // When both children are present
        TreeNode* successor = GetSuccessor(localRoot);
        localRoot->SetOrder(successor->GetOrder());
        localRoot->SetRight(DeleteNode(localRoot->GetRight(), successor->GetOrder()));
    }
    return localRoot;
  };
  private:
  TreeNode* root;
};