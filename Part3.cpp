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
  order Delete(order orderToDelete){
    TreeNode* nodeToDelete = Search(root,orderToDelete);
    if(nodeToDelete!=nullptr){
      TreeNode* localRoot = nodeToDelete->GetRoot();
      if(nodeToDelete->IsLeaf()){
        if(localRoot->GetLeft() == nodeToDelete){localRoot->SetLeft(nullptr);}
        else{localRoot->SetRight(nullptr);};
      }//next checks if only one is a nullptr. this works because this condition is only reached
      // if BOTH branches weren't nullptr; 
      else if( nodeToDelete ->GetLeft() ==nullptr || nodeToDelete ->GetRight() == nullptr){
        if(nodeToDelete ->GetLeft()!= nullptr){localRoot->SetLeft(nodeToDelete->GetLeft());}
        else{localRoot->SetRight(nodeToDelete->GetRight());};
      }//this condition is only reached when both branches are defined 
      // the most complex scenario for deletion
      else {
        
      }
      delete[] nodeToDelete;
    } 
    return orderToDelete;
  };
  private:
  TreeNode* root;
};



