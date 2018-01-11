#pragma once

// �ļ��洢������ƣ�
//	��һ���ֽ��ǰ汾�ţ��Ӷ����ֽڿ�ʼ��ÿ�ĸ��ֽڴ���byteΪnʱ���ֵĴ����������ؽ�����������֮��Ϊ��Ӧ���ļ�������������Ϣ

// ���������⣬���̲���ȫ��һ��������һ��


#include "treeNode.h"

#define END_NODE -10
#define STRUCTURE_NODE -1

typedef unsigned char byte;

typedef struct RECORD
{
	// ����Ϊ�ֽڣ�����(-1)Ϊ��Ҷ�ӽڵ㣬-10Ϊ���ڵ�
	int value;
	// ��������ܺ�
	int count;
} record;

class compressor
{
private:

	// ���뼯����256λ�ǽ�ֹ����λ
	std::string encode[257];

	// ���ڱ����ķ������õ��ݹ飬�����Ϊ�ݹ�ʹ��
	void recordEncode(treeNode<record>* r, char* beforeVal = "");

	// ��������Ƶ�8n��8n+7λ��ֵ���ɵ�λ����λ����0��ʼ
	byte calcNVal(int value, int  n);

public:

	// ͳ��byte������������һ������Ϊ256������
	treeNode<record>** countByte(byte* src, int lens);

	// ���ɹ������������ظ��ڵ�
	treeNode<record>* genHuffmanTree(treeNode<record>** count);

	// ͳ�Ʋ����ɹ�������
	//treeNode<record>* trans2Huffman(byte* src, int lens);

	// ����Ƶ���ת��Ϊ�����ƣ�256*4Bytes��
	byte* transCountResult2Bytes(treeNode<record>** result);

	// �������ƣ�256*4Bytes��ת��Ϊ��Ƶ���
	treeNode<record>** transBytes2CountResult(byte * bytes);

	// ��Huffman�����ļ�ת��Ϊ����
	byte* encodeByHuffmanTree(byte* input, int inLens, int* outLens, treeNode<record>* hTree);

	// ��Huffman��������ת��Ϊ�ļ�
	byte* decodeByHuffmanTree(byte* input, int* outLens, treeNode<record>* hTree);

	// ��bytesд���ļ�
	void write2File(std::string filename, byte* input, int lens);

	// ���ļ�����bytes
	byte* readFromFile(std::string filename, unsigned int* lens);

	// ƴ������byte��
	byte* bytecat(byte* byte1, int lens1, byte* byte2, int lens2);

	compressor();
	~compressor();
};
