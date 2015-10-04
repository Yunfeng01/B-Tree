/**
* Name: Yunfeng(Wilson) Zhao
* ID: 301215576
* sfu email: yza215@sfu.ca
*/
#include "BpTree.h"
#include <math.h>    
#include <iostream>
#include <vector>
#include <utility>  
#include <queue>

using namespace std;

//helper function for constructor
vector< BpTree::Pair* >* BpTree::creatNode(int pairCount)
{
	vector<Pair*>* node = new vector<Pair*>();

	for( int i= 0; i < pairCount; i++ )
	{
		Pair* pair = new Pair;
		pair->keyPair.first=0;
		pair->keyPair.second = "";
		pair->nextNode = NULL;
		node->push_back( pair );
	}
	Pair* parentPair = new Pair;
	parentPair->keyPair.first = -1;
	parentPair->nextNode = NULL;
	Pair* childPair = new Pair;
	childPair->keyPair.first = -2;
	childPair->nextNode = NULL;
	node->push_back( parentPair );
	node->push_back( childPair );
	return node;
}
//helper function for find
string BpTree::findNode( int key, vector<Pair*>* root)
{
	if (root == NULL)
		return "not found";

	for(vector<Pair*>::iterator i = (*root).begin(); i!= (*root).end()-2;i++)
	{
		if(key == (*i)->keyPair.first && (*i)->nextNode == NULL )
			return (*i)->keyPair.second;
		
		if(key < (*i)->keyPair.first )	
			return findNode(key, (*i)->nextNode);

		if(key >= (*i)->keyPair.first && ( (*next(i))->keyPair.first <=0 || (*next(i))->keyPair.first>key) )
		   return findNode(key, (*next(i))->nextNode );
	}
	return "not found";
}
//get number of stored keys in the node
int BpTree::numOfKey(vector<Pair*>* root){
	int num=0;
	if(root==NULL)
		return num;
	for(vector<Pair*>::iterator i = (*root).begin(); i!= (*root).end()-2;i++,num++){
		if((*i)->keyPair.first <= 0)
			return num;
	}
	return num;
}
// get number of pointers in the node
int BpTree::numOfPointer(vector<Pair*>* root){
	int num=0;
	for(vector<Pair*>::iterator i = (*root).begin(); i!= (*root).end()-1;i++,num++){
		if((*i)->nextNode == NULL)
			return num;
	}
	return num;
}
//check if the node is full
bool BpTree::isFull(vector<Pair*>*& root){
	vector<Pair*>::iterator i = (*root).end();
	i--;i--;i--;
	
	if((*i)->keyPair.first > 0){
		return true;
	}
	else
		return false;
}
// check if the node is less tha half full
bool BpTree::lessThanHalf(vector<Pair*>*& root){
	vector<Pair*>::iterator i = (*root).begin();
	int j=0;
	for(; i!= (*root).end()-2;i++,j++){
		if((*i)->keyPair.first <= 0)
			break;
	}
	if(j<ceil(float(m_numkey)/2)){
		return true;
	}
	else{
		return false;
	}
}
// get the aviable sibling and check whether it's right sibling or left
vector<BpTree::Pair*>* BpTree::getSibling(bool& right, vector<Pair*>*& parentNode, vector<Pair*>*& root){
	vector<Pair*>::iterator i = (*parentNode).begin();
	for(; i!= (*parentNode).end()-1;i++){
		if((*i)->nextNode == root){
			break;
		}
	}
	vector<BpTree::Pair*>* rightSibling = (*next(i))->nextNode;
	vector<BpTree::Pair*>* leftSibling = NULL;
	if(i != (*parentNode).begin()){
		leftSibling = (*prev(i))->nextNode;
	}
	if(rightSibling == NULL){
		right = false;
		(*prev(i))->keyPair.first = 0;//delete the search key
		return leftSibling;
	}
	if(leftSibling == NULL){
		right = true;
		(*(i))->keyPair.first = 0;//delete the search key
		return rightSibling;
	}
	if(numOfKey(rightSibling) > numOfKey(leftSibling)){
		right = true;
		(*(i))->keyPair.first = 0;//delete the search key
		return rightSibling;
	}
	else{
		right = false;
		(*prev(i))->keyPair.first = 0;//delete the search key
		return leftSibling;
	}
}
// get the aviable sibling and check whether it's right sibling or left
vector<BpTree::Pair*>* BpTree::findSiblingInter(bool& left, vector<Pair*>*& parentNode, vector<Pair*>*& root, int& borrowedKey){
	vector<Pair*>::iterator i = (*parentNode).begin();
	for(; i!= (*parentNode).end()-1;i++){
		if((*i)->nextNode == root){
			break;
		}
	}
	vector<BpTree::Pair*>* rightSibling = (*next(i))->nextNode;
	vector<BpTree::Pair*>* leftSibling = NULL;
	if(i != (*parentNode).begin()){
		leftSibling = (*prev(i))->nextNode;
	}
	if(rightSibling == NULL || numOfKey(rightSibling) < numOfKey(leftSibling)){
		left = true;
		borrowedKey = (*prev(i))->keyPair.first;
		(*prev(i))->keyPair.first = 0;//delete the search key
		return leftSibling;
	}
	else{
		left = false;
		borrowedKey = (*i)->keyPair.first;
		(*i)->keyPair.first = 0;//delete the search key
		return rightSibling;
	}
}
// move one key from sibling to the removed node
void BpTree::moveOneFromSib(bool right, vector<Pair*>*& siblingNode, vector<Pair*>*& root){
	if(right){
		vector<Pair*>::iterator i = (*siblingNode).begin();
		addToLeaf(root, (*i)->keyPair);
		for(; i!= (*siblingNode).end()-3;i++){
			(*i)->keyPair = (*next(i))->keyPair;
		}
		(*i)->keyPair.first = 0;
		(*i)->keyPair.second = "";
	}
	else{
		vector<Pair*>::iterator i = (*siblingNode).end()-1;
		for(;(*i)->keyPair.first <= 0;i--){
		}
		int key = (*i)->keyPair.first;
		string value = (*i)->keyPair.second;
		(*i)->keyPair.first = 0;
		(*i)->keyPair.second = "";
		//shift the removed node
		vector<Pair*>::iterator j = (*root).end()-3;
		for(;j!=(*root).begin();j--){
			(*j)->keyPair = (*prev(j))->keyPair;
		}
		(*j)->keyPair.first = key;
		(*j)->keyPair.second = value;
	}
}
// move one key from sibling to the removed node
void BpTree::moveOneFromSibInter(bool left, vector<Pair*>*& siblingNode, vector<Pair*>*& root, int key){
	vector<Pair*>* motherNode = (*((*root).end()-1))->nextNode;
	if(left){
		vector<Pair*>::iterator i = (*siblingNode).end()-1;
		for(;(*i)->keyPair.first <= 0;i--){
		}
		int movedkey = (*i)->keyPair.first;
		string value = (*i)->keyPair.second;
		vector<Pair*>* movedNextNode = (*next(i))->nextNode;
		(*next(i))->nextNode = NULL;
		(*i)->keyPair.first = 0;
		(*i)->keyPair.second = "";
		//insert key to mother's node
		for(vector<Pair*>::iterator j = (*motherNode).begin();j!=(*motherNode).end()-2;j++){
			if((*j)->keyPair.first == 0 ){
				(*j)->keyPair.first = movedkey;
				break;
			}
		}
		//insert to root
		Pair* current = new Pair; 
		current->keyPair.first = key;
		current->nextNode = movedNextNode;
		vector<Pair*>::iterator begin = (*root).begin();
		(*root).insert(begin,current);
		(*root).erase((*root).end()-3);
		//link the child
		vector<Pair*>* child = (*begin)->nextNode;
		(*((*child).end()-1))->nextNode = root;
	}
	else{
		vector<Pair*>::iterator i = (*siblingNode).begin();
		int movedkey = (*i)->keyPair.first;
		string value = (*i)->keyPair.second;
		vector<Pair*>* movedNextNode = (*i)->nextNode;
		(*i)->nextNode = NULL;
		(*i)->keyPair.first = 0;
		(*i)->keyPair.second = "";
		//shift the sibling node
		for(;i!=(*siblingNode).end()-3;i++){
			(*i)->keyPair = (*next(i))->keyPair;
			(*i)->nextNode = (*next(i))->nextNode;
		}
		(*i)->nextNode = (*next(i))->nextNode;
		//insert key to mother's node
		for(vector<Pair*>::iterator j = (*motherNode).begin();j!=(*motherNode).end()-2;j++){
			if((*j)->keyPair.first == 0 ){
				(*j)->keyPair.first = movedkey;
				break;
			}
		}
		//insert to root
		vector<Pair*>::iterator k = (*root).begin();
		for(;k!=(*root).end()-2;k++){
			if((*k)->keyPair.first == 0 ){
				(*k)->keyPair.first = key;
				break;
			}
		}
		(*next(k))->nextNode = movedNextNode;
		//link the child
		vector<Pair*>* child = (*next(k))->nextNode;
		(*((*child).end()-1))->nextNode = root;
	}
}
// add a key to a intermediate node by asending ordered
void BpTree::addToInter(vector<Pair*>*& motherNode,vector<Pair*>*& newNode){
	int key = (*((*newNode).begin()))->keyPair.first;
	(*((*newNode).end()-1))->nextNode = motherNode;
	vector<Pair*>::iterator i = (*motherNode).begin();
	for(; i!= (*motherNode).end()-2;i++){
		if((*i)->keyPair.first == 0 || (*i)->keyPair.first > key)
		{
			break;
		}		   
   }
   if((*i)->keyPair.first == 0){
	   (*i)->keyPair.first = key;
	   i++;
	   (*i)->nextNode = newNode;
	   return;
   }
   if((*i)->keyPair.first > key){
	   vector<Pair*>::iterator newPlace = i;
	   vector<Pair*>::iterator j= (*motherNode).end()-2;
	   (*j)->nextNode = (*prev(j))->nextNode;j--;
	   for(; j!= next(i);j--){//shift all nodes
		   (*j)->keyPair.first = (*prev(j))->keyPair.first;
		   (*j)->nextNode = (*prev(j))->nextNode;
	   }
	   (*j)->keyPair.first = (*prev(j))->keyPair.first;
	   (*j)->nextNode = newNode;
	   (*newPlace)->keyPair.first = key;
	   return;
   }
}
// add a key to a intermediate node by asending ordered
void BpTree::addToInter(vector<Pair*>*& newNode, pair<int, std::string> removedItem, vector<Pair*>*& removedNext){
	(*((*removedNext).end()-1))->nextNode = newNode;
	Pair* current = new Pair; 
	current->keyPair = removedItem;
	current->nextNode = removedNext;
	vector<Pair*>::iterator i = (*newNode).begin();
	for(; i!= (*newNode).end()-2;i++){
		if((*i)->keyPair.first == 0 || (*i)->keyPair.first > removedItem.first)
		{
			break;
		}		   
   }
	(*newNode).insert(i,current);
	(*newNode).erase((*newNode).end()-3);
	(*((*removedNext).end()-1))->nextNode = newNode;
}
//add a key and value to a leaf node by asending ordered
void BpTree::addToLeaf(vector<Pair*>*& root, pair<int, std::string> newItem ) {
	Pair* current = new Pair; 
	current->keyPair = newItem;
	current->nextNode = NULL;
	vector<Pair*>::iterator i = (*root).begin();
	for(; i!= (*root).end()-2;i++){
		if((*i)->keyPair.first == 0 || (*i)->keyPair.first > newItem.first)
		{
			break;
		}		   
   }
   (*root).insert(i,current);
   (*root).erase((*root).end()-3);
}
// remove a key and value to a leaf node by asending ordered
void BpTree::deleteFromLeaf(vector<Pair*>*& root, pair<int, string> newItem){
	vector<Pair*>::iterator i = (*root).begin();
	for(; i!= (*root).end()-2;i++){
		if((*i)->keyPair.first == newItem.first)
		{
			for(; i!= (*root).end()-3;i++){
				(*i)->keyPair = (*next(i))->keyPair;
			}
			(*i)->keyPair.first = 0;
			(*i)->keyPair.second = "";
			break;
		}		   
   }
}
// // delete the last key and last pointer from intermidiate node
// void BpTree::deleteLastItem(vector<Pair*>*& root){
// 	for(vector<Pair*>::iterator i = (*root).end()-2; (*i)->keyPair.first>0;i++){
// 		if((*i)->nextNode != NULL){
// 			(*i)->nextNode == NULL;
// 			(*prev(i))->keyPair.first = 0;
// 			break;
// 		}
// 	}
// }
// combine two leaf nodes
void BpTree::combineLeafNode(bool right, vector<Pair*>*& sibling,vector<Pair*>*& root){
	for(vector<Pair*>::iterator i = (*root).begin(); (*i)->keyPair.first>0;i++){
		addToLeaf(sibling, (*i)->keyPair);
	}
	if(!right){
		(*((*sibling).end()-2))->nextNode = (*((*root).end()-2))->nextNode;
	}
	else{
		//find previous leaf node
		vector<Pair*>* i = m_root;
		for(;(*(*i).begin())->nextNode != NULL;i = (*(*i).begin())->nextNode){
		}
		for(;(*((*i).end()-2))->nextNode != root;i = (*((*i).end()-2))->nextNode){
		}
		(*((*i).end()-2))->nextNode = sibling;
	}
	//destroy(root);
	(*root).erase((*root).begin(),(*root).end());
	root = NULL;
}
// combine two inter nodes
void BpTree::combineInterNode(bool left, vector<Pair*>*& siblingNode, vector<Pair*>*& root, int key){
	vector<Pair*>* motherNode = (*((*root).end()-1))->nextNode;
	//shift mother node  after 
	vector<Pair*>::iterator i=(*motherNode).begin();
	for(;i!=(*motherNode).end()-3;i++){
		if((*i)->keyPair.first == 0){
			break;
		}
	}
	
	if(left){
		for(;i!=(*motherNode).end()-3;i++){
			(*i)->keyPair = (*next(i))->keyPair;
			(*next(i))->nextNode = (*next(next(i)))->nextNode;
		}
		(*i)->keyPair.first = 0;
		//insert root to sibling
		vector<Pair*>::iterator sibi = (*siblingNode).end()-3;
		for(;(*sibi)->keyPair.first <= 0;sibi--){
		}
		(*sibi)->keyPair.first = key;
		(*next(sibi))->nextNode = (*(*root).begin())->nextNode;
		vector<Pair*>* child = (*(*root).begin())->nextNode;
		(*((*child).end()-1))->nextNode = siblingNode;
		sibi++;
		for(vector<Pair*>::iterator j = (*root).begin();(*j)->keyPair.first != 0;j++,sibi++){
			(*sibi)->keyPair = (*j)->keyPair;
			(*next(sibi))->nextNode = (*next(j))->nextNode;
			vector<Pair*>* child = (*next(j))->nextNode;
			(*((*child).end()-1))->nextNode = siblingNode;
		}
		(*root).erase((*root).begin(),(*root).end());
		root = NULL;
	}
	else{
		//shift mother node
		for(;i!=(*motherNode).end()-3;i++){
			(*i)->keyPair = (*next(i))->keyPair;
			(*i)->nextNode = (*next(i))->nextNode;
		}
		(*i)->keyPair.first = 0;
		(*i)->nextNode = (*next(i))->nextNode;
		//insert root to sibling
		//int numofpoint = numOfPointer(root);
		vector<Pair*>::iterator j = (*root).end()-2;
		for(;j!=(*root).begin();j--){
			if((*j)->nextNode != NULL){
				break;
			}
		}
		Pair* current = new Pair; 
		current->keyPair.first = key;
		current->nextNode = (*j)->nextNode;
		siblingNode->insert((*siblingNode).begin(),current);
		siblingNode->erase((*siblingNode).end()-3);
		//deleteLastItem(siblingNode);
		if(j!=(*root).begin()){
			j--;
			for(;j!=(*root).begin();j--){
				Pair* tmp = new Pair; 
				tmp->keyPair = (*j)->keyPair;
				tmp->nextNode = (*j)->nextNode;
				siblingNode->insert((*siblingNode).begin(),tmp);
				siblingNode->erase((*siblingNode).end()-3);
				//deleteLastItem(siblingNode);
			}
			Pair* tmp = new Pair; 
			tmp->keyPair = (*j)->keyPair;
			tmp->nextNode = (*j)->nextNode;
			siblingNode->insert((*siblingNode).begin(),tmp);
			siblingNode->erase((*siblingNode).end()-3);
			//deleteLastItem(siblingNode);
		}
		(*root).erase((*root).begin(),(*root).end());
		root = NULL;
	}
}
// redistribute the intermidate nodes
void BpTree::distributeInterNode(vector<Pair*>*& parentNode){
	int num = numOfPointer(parentNode);
	if(num < ceil(float(m_numkey+1)/2)){
		//borrow or combine
		vector<Pair*>* grandPaNode = (*((*parentNode).end()-1))->nextNode;
		if(grandPaNode == NULL){//if no more mother node
			if(num == 1){
				m_root = (*(*parentNode).begin())->nextNode;
				(*parentNode).erase((*parentNode).begin(),(*parentNode).end());
			}
		}
		else{
			int key = 0;
			bool left = true;
			vector<Pair*>* siblingNode = findSiblingInter(left, grandPaNode, parentNode, key);
			int numSibPointer = numOfPointer(siblingNode);
			if(numSibPointer > ceil(float(m_numkey+1)/2)){//borrow
				moveOneFromSibInter(left, siblingNode, parentNode, key);
			}
			else{//combine
				combineInterNode(left, siblingNode, parentNode, key);
				distributeInterNode(grandPaNode);
			}
		}
	}
	else{
		return;
	}
}
// // remove new node key from intermidiate node
void BpTree::deleteSearchKeyFromInter(bool right, vector<Pair*>*& parentNode, int key){
	if(key == 0){//no new key need to insert
		//shift all nodes after 0
		vector<Pair*>::iterator i = (*parentNode).begin();
		for(;i!=(*parentNode).end()-3;i++){
			if((*i)->keyPair.first == 0){
				break;
			}
		}
		if(right){
			for(;i!=(*parentNode).end()-3;i++){
				(*i)->keyPair = (*next(i))->keyPair;
				(*i)->nextNode = (*next(i))->nextNode;
			}
			(*i)->keyPair.first = 0;
			(*i)->nextNode = (*next(i))->nextNode;
		}
		else{
			for(;i!=(*parentNode).end()-3;i++){
				(*i)->keyPair = (*next(i))->keyPair;
				(*next(i))->nextNode = (*next(next(i)))->nextNode;
			}
			(*i)->keyPair.first = 0;
		}
		//check if the mother node is less than half or not
		distributeInterNode(parentNode);
	}
	else{
		for(vector<Pair*>::iterator i = (*parentNode).begin(); i!=(*parentNode).end()-2;i++){
			if((*i)->keyPair.first == 0){
				(*i)->keyPair.first = key;
				break;
			}
		}
	}
}
// add new node to intermidiate node
void BpTree::insertIntermediateNode(vector<Pair*>*& motherNode,vector<Pair*>*& newNode){
	if(!isFull(motherNode)){
		addToInter(motherNode,newNode);
	}
	else{
		vector< BpTree::Pair* >* newMotherNode = creatNode(m_numkey);
		//get the maximum of left node
		vector<Pair*>::iterator maxMother = (*motherNode).end()-3;
		vector<Pair*>::iterator begin = (*newMotherNode).begin();
		//insert max element to new node
		(*begin)->keyPair = (*maxMother)->keyPair;
		(*begin)->nextNode = (*next(maxMother))->nextNode;
		vector<Pair*>* child = (*begin)->nextNode;
		if(child != NULL){
			(*((*child).end()-1))->nextNode = newMotherNode;
		}

		(*next(maxMother))->nextNode = NULL;
		(*maxMother)->keyPair.first = 0;
		(*maxMother)->keyPair.second = "";
		//insert new element to old node
		addToInter(motherNode, newNode);
		//move half keys to new node
		int j=m_numkey;
		for(vector<Pair*>::iterator i = (*motherNode).end()-3; j>floor(float(m_numkey+1)/2);i--,j--){
			pair<int, std::string> removedItem = (*i)->keyPair;
			addToInter(newMotherNode, removedItem, (*next(i))->nextNode);
			(*next(i))->nextNode = NULL;
			(*i)->keyPair.first = 0;
			(*i)->keyPair.second = "";
		}

		vector<Pair*>::iterator topKey = (*newMotherNode).begin();
		if((*((*motherNode).end()-1))->nextNode == NULL){//no upper level node
			vector<BpTree::Pair*>* topMother = creatNode(m_numkey);
			int k = (*topKey)->keyPair.first;
			vector<Pair*>::iterator beginRoot = (*topMother).begin();
			(*beginRoot)->keyPair.first = k;
			(*beginRoot)->nextNode = motherNode;
			(*next(beginRoot))->nextNode = newMotherNode;
			(*((*motherNode).end()-1))->nextNode = topMother;
			(*((*newMotherNode).end()-1))->nextNode = topMother;
			//remove the top key from new node and shift
			vector<Pair*>::iterator a = (*newMotherNode).begin();
			for(; a!= (*newMotherNode).end()-3;a++){
				(*a)->keyPair = (*next(a))->keyPair;
			}
			(*a)->keyPair.first = 0;
			m_root = topMother;
		}
		else{//already a root node or a upper level node
			vector<BpTree::Pair*>* topMother = (*((*motherNode).end()-1))->nextNode;
			insertIntermediateNode(topMother, newMotherNode);
			vector<Pair*>::iterator a = (*newMotherNode).begin();
			for(; a!= (*newMotherNode).end()-3;a++){
				(*a)->keyPair = (*next(a))->keyPair;
			}
			(*a)->keyPair.first = 0;
		}
	}
}
//helper function for insert
void BpTree::insert(pair<int, std::string> newItem, vector<Pair*>*& root)
{
	if (root == NULL)
		return;
	//check if leaf
	if((*((*root).begin()))->nextNode == NULL){
		if(!isFull(root)){
			addToLeaf(root, newItem);
		}
		else{
			vector< BpTree::Pair* >* newNode = creatNode(m_numkey);
			//get the maximum of left node
			vector<Pair*>::iterator maxI = (*root).end()-3;
			pair<int, std::string> max = (*maxI)->keyPair;

			if(m_numkey <= 2 && newItem.first > max.first){
				addToLeaf(newNode, newItem);
				vector<Pair*>::iterator next = (*root).end()-2;
				(*((*newNode).end()-2))->nextNode = (*next)->nextNode;
				(*next)->nextNode = newNode;
			}
			else{
			
				(*maxI)->nextNode = NULL;
				(*maxI)->keyPair.first = 0;
				(*maxI)->keyPair.second = "";
				addToLeaf(newNode, max);
				addToLeaf(root, newItem);
				int j=m_numkey;
				for(vector<Pair*>::iterator i = (*root).end()-3; j>ceil(float(m_numkey+1)/2);i--,j--){
					pair<int, std::string> removedItem = (*i)->keyPair;
					addToLeaf(newNode, removedItem);
					(*i)->nextNode = NULL;
					(*i)->keyPair.first = 0;
					(*i)->keyPair.second = "";
				}
				vector<Pair*>::iterator next = (*root).end()-2;
				(*((*newNode).end()-2))->nextNode = (*next)->nextNode;
				(*next)->nextNode = newNode;
			}

			vector<Pair*>::iterator parent = (*root).end()-1;
			if((*parent)->nextNode == NULL){//only leaf node yet
				vector< BpTree::Pair* >* motherNode = creatNode(m_numkey);
				vector<Pair*>::iterator first = (*motherNode).begin();
				(*first)->keyPair.first = (*(*newNode).begin())->keyPair.first;
				(*first)->nextNode = root;
				(*((*root).end()-1))->nextNode = motherNode;
				first++;
				(*first)->nextNode = newNode;
				(*((*newNode).end()-1))->nextNode = motherNode;
				root = motherNode;
			}
			else{//already have a mother node, //move key above and shift pointers
				vector< BpTree::Pair* >* motherNode = (*parent)->nextNode;
				insertIntermediateNode(motherNode,newNode);
			}
		}
	}
	else{//intermediate node
		for(vector<Pair*>::iterator i = (*root).begin(); i!= (*root).end()-2;i++){
			if(newItem.first < (*i)->keyPair.first ){	
				insert(newItem, (*i)->nextNode);
				break;
			}

			if(newItem.first >= (*i)->keyPair.first && ((*next(i))->keyPair.first <=0 || (*next(i))->keyPair.first>newItem.first)){
				insert(newItem, (*next(i))->nextNode);
				break;
			}
		}	
	}
}
// helper function for remove
void BpTree::remove(pair<int, string> newItem, vector<Pair*>*& root){
	if (root == NULL)
		return;
	//check if leaf
	if((*((*root).begin()))->nextNode == NULL){
		deleteFromLeaf(root, newItem);
		vector<BpTree::Pair*>* parentNode = (*((*root).end()-1))->nextNode;
		if(parentNode != NULL){
			if(lessThanHalf(root)){
				//get the left sibling
				bool right = true;
				vector<BpTree::Pair*>* siblingNode = getSibling(right, parentNode, root);
				int numSibKey = numOfKey(siblingNode);
				if(numSibKey-1 < ceil(float(m_numkey)/2)){
					//combine
					combineLeafNode(right, siblingNode, root);
					deleteSearchKeyFromInter(right, parentNode, 0);//shift all nodes after 0
				}
				else{
					moveOneFromSib(right, siblingNode, root);
					deleteSearchKeyFromInter(right, parentNode,(*(*root).begin())->keyPair.first);
				}
			}
		}
	}
	else{//intermediate node
		for(vector<Pair*>::iterator i = (*root).begin(); i!= (*root).end()-2;i++){
			if(newItem.first < (*i)->keyPair.first ){	
				remove(newItem, (*i)->nextNode);
				break;
			}

			if(newItem.first >= (*i)->keyPair.first && ((*next(i))->keyPair.first <=0 || (*next(i))->keyPair.first>newItem.first)){
				remove(newItem, (*next(i))->nextNode);
				break;
			}
		}	
	}
}
//helper function for destroying tree
void BpTree::destroy(vector<Pair*>*& root)
{
	if(root==NULL)
		return;
	for(vector<Pair*>::iterator i= (*root).begin(); i != (*root).end()-2; i++ )
	{
		destroy((*i)->nextNode);
	}
	(*root).erase( (*root).begin(), (*root).end() );
	delete root;	
	root = NULL;
}
//function for printing a node's key
void BpTree::printNode(vector<Pair*>*& node)
{
	if(node == NULL)
		return;
	vector<Pair*>::iterator i = (*node).begin();
	cout<<"[ ";
	if((*i)->keyPair.first>0)
	{
		cout<<(*i)->keyPair.first;
	}
	for(  i = (*node).begin()+1; i != (*node).end()-2; i++ )
	{
		if((*i)->keyPair.first <= 0 )
		{
			break;
		}
		cout<<","<<(*i)->keyPair.first;
	}
	cout<<" ]";
}
//function for printing a node's value
void BpTree::printNodeValue(vector<Pair*>*& node)
{
	if(node == NULL)
		return;
	vector<Pair*>::iterator i;
	for(  i = (*node).begin(); i != (*node).end()-2; i++ )
	{
		if((*i)->keyPair.first <= 0 )
		{
			break;
		}
		cout<<(*i)->keyPair.second<<"\n";
	}
}
//helper function for print key
void BpTree::print(vector<Pair*>*& root)
{
    if(root==NULL){
        cout<<"[ ]\n";
        return;
    } 
    queue< pair<int, NodeType*>* > q;
	pair<int, NodeType*>* current = new pair<int, NodeType*>(0, root);
    q.push(current);
	int level = 1;
    while(!q.empty()){
        pair<int, NodeType*>* tmp = q.front();
        q.pop();
		printNode(tmp->second );
		level=tmp->first;
		level+=1;
		if((*(*tmp->second).begin())->nextNode != NULL){
			for(vector<Pair*>::iterator i = (*tmp->second).begin(); i != (*tmp->second).end()-1; i++ )
			{
				if((*i)->nextNode != NULL)
				{
					pair<int, NodeType*>* here = new pair<int, NodeType*>(level, (*i)->nextNode);
					q.push( here );
				}
			}
		}
		if(q.empty()){
			cout<<"\n";
			return;
		}
		pair<int, NodeType*>* next = q.front();
		if(next == NULL || next->first > tmp->first){
			cout<<"\n";
		}
	} 
}
//helper function for print value
void BpTree::printV(vector<Pair*>*& root)
{
	if(root==NULL){
		cout<<"\n";
        return;
    }
	vector<Pair*>* tmp = root;
	while((*((*tmp).begin()))->nextNode != NULL){
		tmp = (*((*tmp).begin()))->nextNode ;
	}
	printNodeValue(tmp);
	while((*tmp)[m_numkey]->nextNode != NULL){
		tmp = (*tmp)[m_numkey]->nextNode ;
		printNodeValue(tmp);
	}
}
//helper function for copy constructor
vector<BpTree::Pair*>* BpTree::copy( vector<Pair*>* root)
{
	if(root==NULL)
		return NULL;
	vector<BpTree::Pair*>* newT= creatNode( (*root).size()-2 );
	vector<BpTree::Pair*>::iterator i =  (*root).begin();
	vector<BpTree::Pair*>::iterator j=  (*newT).begin();
	for(; j!= (*newT).end() || i!= (*root).end(); j++, i++)
	{
		(*j)->keyPair = (*i)->keyPair;
		(*j)->nextNode = copy((*i)->nextNode);
    }
	return newT;
}


//---member function----//
//construct a BpTree
BpTree::BpTree( int keyCount )
{
	m_numkey = keyCount;
	m_root = creatNode( m_numkey );
}
//destruct a BpTree
BpTree::~BpTree()
{
	destroy(m_root);
	m_root = NULL;
	m_numkey = 0;
}
//assign operator
BpTree& BpTree::operator=(const BpTree& other)
{
   if(this==&other)
        return *this;
    destroy(m_root);
    m_root= new vector<BpTree::Pair*> ( *(other.m_root) );
	m_numkey = other.m_numkey;
    return *this;
}
//copy constructor
BpTree::BpTree(const BpTree& other)
{
   m_root= new vector<BpTree::Pair*> ( *(other.m_root) ) ;
   m_numkey = other.m_numkey;
}
//void BpTree::destroy()
//{
//	destroy( m_root );
//}
//print all the keys of the tree
void BpTree::printKeys()
{
	print( m_root );
}
//print all the values in leaf node
void BpTree::printValues()
{
	printV( m_root );
}
//find the records by its key
string BpTree::find( int key )
{
	return findNode(key, m_root);
	/*std::vector<Pair*>::iterator result = searchNode(key, m_root);
	return (*result)->keyPair.second;*/
}
//insert new record (key,value) to the tree
void BpTree::insert(int key, string value){
	pair<int, string>* newItem =  new pair<int, string>(key, value);
	insert(*newItem, m_root);
}
//remove a record from the tree
void BpTree::remove(int key){
	pair<int, string>* newItem =  new pair<int, string>(key, "");
	remove(*newItem, m_root);
}

int main(){

	BpTree* tree = new BpTree( 3 );
	tree->insert(2, "wilson2");
	tree->insert(21, "wilson21");
	tree->insert(11, "wilson11");
	tree->insert(8, "wilson8");
	tree->insert(64, "wilson64");
	tree->insert(5, "wilson5");
	tree->insert(23, "wilson23");
	tree->insert(6, "wilson6");
	tree->insert(19, "wilson19");
	tree->insert(9, "wilson9");
	tree->insert(7, "wilson7");
	tree->insert(60, "wilson60");
	tree->insert(51, "wilson51");
	tree->insert(45, "wilson45");
	tree->insert(31, "wilson31");
	tree->insert(39, "wilson39");
	tree->insert(93, "wilson93");
	tree->insert(77, "wilson77");
	tree->printKeys();

	BpTree* tree1 = tree;
	tree1->printKeys();

	BpTree* tree2 = new BpTree( *tree);
	tree2->printKeys();


	tree->remove(9);
	tree->printKeys();
	tree->printValues();
	tree->remove(6);
	tree->printKeys();
	tree->printValues();
	tree->remove(8);
	tree->printKeys();
	tree->printValues();
	tree->remove(23);
	tree->printKeys();
	tree->printValues();
	tree->remove(31);
	tree->printKeys();
	tree->remove(7);
	tree->printKeys();
	tree->remove(5);
	tree->printKeys();
	tree->remove(77);
	tree->printKeys();
	tree->remove(19);
	tree->printKeys();
	tree->remove(2);
	tree->printKeys();
	tree->remove(51);
	tree->printKeys();
	tree->remove(45);
	tree->printKeys();
	// tree->printValues();
	// string a = tree->find(45);
	// cout<<a;

	// delete tree;
	// tree->printKeys();
}
