#pragma once

template<typename T>
class treeNode
{
public:
	T data;
	treeNode<T>* lChild;
	treeNode<T>* rChild;
	treeNode();
	~treeNode();
	void preorder(void(*func)(T));
};

template<typename T>
treeNode<T>::treeNode()
{
	lChild = 0;
	rChild = 0;
}

template<typename T>
treeNode<T>::~treeNode()
{
}

template<typename T>
void treeNode<T>::preorder(void(*func)(T))
{
	func(data);
	this->lChild->preorder(func);
	this->rChild->preorder(func);
}

