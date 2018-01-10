#pragma once
#include "treeNode.h"
typedef unsigned char byte;

typedef struct
{
	// 正数为字节，负数(-1)为非叶子节点
	int value;
	// 结点数量总和
	int count;
} record;

class compressor
{
public:
	// 统计byte种类数，返回一个长度为256的数组
	treeNode<record>** countByte(byte* src, int lens);
	// 生成哈夫曼树，返回根节点
	treeNode<record>* genHuffmanTree(treeNode<record>** src);
public:
	compressor();
	~compressor();
};
