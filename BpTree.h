/**
* Name: Yunfeng(Wilson) Zhao
* ID: 301215576
* sfu email: yza215@sfu.ca
*/
#include <iostream>
#include <vector>
#include <utility>      
#include <string>
#ifndef BPTREE_H
#define BPTREE_H
using namespace std;

class BpTree{
   public:
      /**
      * construct a BpTree
      * @param number of key in a node
      */
      BpTree( int keyCount );
      /**
      * destruct the BpTree
      */
       ~BpTree();
      /**
        * copy constrcutor 
        * @pre none
        * @post the BpTree copied from a other BpTree
        * @param other other BpTree
        */
       BpTree(const BpTree& other);
      /**
		* assign operator
		* @pre the BpTree has created
		* @post the BpTree equals to other 
		* @param other other BpTree
		* @return new BpTree as same as other
		*/
       BpTree& operator=(const BpTree& other);
	  /**
	  * print all the keys of the tree
	  **/
	  void printKeys();
	  /**
	  * print all the values in leaf node
	  **/
	  void printValues();
	  /**
	  * find the records by its key
      * @param the key of the record
	  * @return the value of the record
	  **/
	  string find( int key );
	  /**
	  * insert new record (key,value) to the tree
      * @param key of the record
	  * @param value of the record
	  **/
	  void insert(int key, string value);
	  /**
	  * remove a record from the tree
      * @param key of the record
	  **/
	  void remove(int key);
     
   private:
	  
      struct Pair{
         pair<int, string> keyPair;
		 vector<Pair*>* nextNode;
      };
	  typedef vector<BpTree::Pair*> NodeType;
	  //instance variable number of key in a node
	  int m_numkey;
	  //instance variable root
	  vector<Pair*>* m_root;
	  /**
	  * helper function for constructor
      * @param number of keys in a node
	  * @return a tree root
	  **/
	  static std::vector<Pair*>* creatNode(int pairCount);
	  /**
	  * helper function for find
      * @param tree root node
	  * @param key of the record
	  * @return the value of the record
	  **/
	  string findNode( int key, vector<Pair*>* root);
	  /**
	  * helper function for insert
      * @param tree root node
	  * @param pair of key and value
	  **/
	  void insert(pair<int, string> newItem, vector<Pair*>*& root);
	  /**
	  * helper function for remove
      * @param tree root node
	  * @param pair of key and value
	  **/
	  void remove(pair<int, string> newItem, vector<Pair*>*& root);
	  /**
	  * remove a key and value to a leaf node by asending ordered
      * @param tree leaf node
	  * @param pair of key and value
	  **/
	  void deleteFromLeaf(vector<Pair*>*& root, pair<int, string> newItem);
	  /**
	  * combine two leaf nodes
      * @param sibling node
	  * @param original node
	  **/
	  void combineLeafNode(bool right, vector<Pair*>*& sibling,vector<Pair*>*& root);
	  /**
	  * combine two inter nodes
      * @param sibling node
	  * @param parent node
	  * @param left to be aware if it's left sibling or right sibling
	  * @param key, removed key from mother node
	  **/
	  void combineInterNode(bool left, vector<Pair*>*& siblingNode, vector<Pair*>*& parentNode, int key);
	  /**
	  * add a key and value to a leaf node by asending ordered
      * @param tree leaf node
	  * @param pair of key and value
	  **/
	  void addToLeaf(vector<Pair*>*& root, pair<int, string> newItem );
	  /**
	  * add a key to a intermediate node by asending ordered
      * @param mother node
	  * @param new child node
	  **/
	  void addToInter(vector<Pair*>*& motherNode,vector<Pair*>*& newNode);
	  /**
	  * add a key to a intermediate node by asending ordered
	  * @param mother node
	  * @param removed item from old node
	  * @param next node of the removed Item
	  **/
	  void addToInter(vector<Pair*>*& newNode, pair<int, std::string> removedItem, vector<Pair*>*& removedNext);
	  /**
	  * add new node to intermidiate node
	  * @param mother node
	  * @param new node
	  **/
	  void insertIntermediateNode(vector<Pair*>*& motherNode,vector<Pair*>*& newNode);
	  // *
	  // * remove new node key from intermidiate node
	  // * @param mother node
	  // * @param new node
	  // *
	  void deleteSearchKeyFromInter(bool right, vector<Pair*>*& parentNode, int key);
	  /**
	  * redistribute the intermidate nodes
	  * @param mother node
	  * @param modified node
	  **/
	  void distributeInterNode(vector<Pair*>*& parentNode);
	  /**
	  * helper function for destroying tree
      * @param tree root node
	  **/
      static void destroy(vector<Pair*>*& list);
	  /**
	  * helper function for print key
      * @param tree root node
	  **/
	  void print(vector<Pair*>*& list);
	  /**
	  * helper function for print value
      * @param tree root node
	  **/
	  void printV(vector<Pair*>*& list);
	  /**
	  * function for printing a node's key
      * @param tree node
	  **/
	  static void printNode(vector<Pair*>*& list);
	  /**
	  * function for printing a node's value
      * @param tree leaf node
	  **/
	  static void printNodeValue(vector<Pair*>*& list);
	  /**
	  * check if the node is full
      * @param tree node
	  * @return true if the node is full
	  *			false if the node is not full
	  **/
	  bool isFull(vector<Pair*>*& list);
	  /**
	  * check if the node is less tha half full
      * @param tree node
	  * @return true if the node is less than half
	  *			false if the node is not less than half
	  **/
	  bool lessThanHalf(vector<Pair*>*& root);
	  /**
	  * get the aviable sibling and check whether it's right sibling or left
	  * @param bool value if true then means sibling is right. If not, then left.
      * @param tree node
	  * @param parent node
	  * @return the avaliable sibling node
	  **/
	  vector<Pair*>* getSibling(bool& right, vector<Pair*>*& parentNode, vector<Pair*>*& root);
	  /**
	  * get the aviable sibling and check whether it's right sibling or left
	  * @param bool value if true then means sibling is right. If not, then left.
      * @param tree node
	  * @param parent node
	  * @param the key borrowed from mother
	  * @return the avaliable sibling node
	  **/
	  vector<Pair*>* findSiblingInter(bool& left, vector<Pair*>*& grandPaNode, vector<Pair*>*& parentNode, int& borrowedKey);
	  /**
	  * get number of stored keys in the node
      * @param tree node
	  * @return the number of stored keys 
	  **/
	  int numOfKey(vector<Pair*>* root);
	  /**
	  * get number of pointers in the node
      * @param tree node
	  * @return the number of pointers
	  **/
	  int numOfPointer(vector<Pair*>* root);
	  /**
	  * move one key from sibling to the removed node
	  * @param bool value if true then means sibling is right. If not, then left.
      * @param sibling node
	  * @param removed node
	  **/
	  void moveOneFromSib(bool right, vector<Pair*>*& siblingNode, vector<Pair*>*& root);
	  /**
	  * move one key from sibling to the removed node
	  * @param bool value if true then means sibling is right. If not, then left.
      * @param sibling node
	  * @param removed node
	  * @param the key put on the mother node
	  **/
	  void moveOneFromSibInter(bool left, vector<Pair*>*& siblingNode, vector<Pair*>*& root, int key);
	  /**
	  * delete the last key and last pointer from intermidiate node
	  * @param tree node
	  */
	  void deleteLastItem(vector<Pair*>*& siblingNode);
	  /**
	  * helper function for copy constructor
	  * @param tree node
	  */
	  vector<Pair*>* copy( vector<Pair*>* root);
};
#endif //BPTREE_H
