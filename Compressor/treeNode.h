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

