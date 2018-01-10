#pragma once
#include "treeNode.h"
typedef unsigned char byte;

typedef struct
{
	// ����Ϊ�ֽڣ�����(-1)Ϊ��Ҷ�ӽڵ�
	int value;
	// ��������ܺ�
	int count;
} record;

class compressor
{
public:
	// ͳ��byte������������һ������Ϊ256������
	treeNode<record>** countByte(byte* src, int lens);
	// ���ɹ������������ظ��ڵ�
	treeNode<record>* genHuffmanTree(treeNode<record>** src);
public:
	compressor();
	~compressor();
};
