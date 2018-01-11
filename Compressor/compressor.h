#pragma once

// 文件存储构造设计：
//	第一个字节是版本号，从二个字节开始，每四个字节代表byte为n时出现的次数，用于重建哈夫曼树。之后为对应的文件哈夫曼编码信息

// 现在这玩意，进程不安全！一个对象处理一个


#include "treeNode.h"

#define END_NODE -10
#define STRUCTURE_NODE -1

typedef unsigned char byte;

typedef struct RECORD
{
	// 正数为字节，负数(-1)为非叶子节点，-10为完结节点
	int value;
	// 结点数量总和
	int count;
} record;

class compressor
{
private:

	// 编码集，第256位是截止编码位
	std::string encode[257];

	// 用于遍历的方法。用到递归，后变量为递归使用
	void recordEncode(treeNode<record>* r, char* beforeVal = "");

	// 计算二进制第8n到8n+7位的值。由低位到高位，从0开始
	byte calcNVal(int value, int  n);

public:

	// 统计byte种类数，返回一个长度为256的数组
	treeNode<record>** countByte(byte* src, int lens);

	// 生成哈夫曼树，返回根节点
	treeNode<record>* genHuffmanTree(treeNode<record>** count);

	// 统计并生成哈夫曼树
	//treeNode<record>* trans2Huffman(byte* src, int lens);

	// 将词频结果转化为二进制（256*4Bytes）
	byte* transCountResult2Bytes(treeNode<record>** result);

	// 将二进制（256*4Bytes）转化为词频结果
	treeNode<record>** transBytes2CountResult(byte * bytes);

	// 按Huffman树将文件转换为编码
	byte* encodeByHuffmanTree(byte* input, int inLens, int* outLens, treeNode<record>* hTree);

	// 按Huffman树将编码转换为文件
	byte* decodeByHuffmanTree(byte* input, int* outLens, treeNode<record>* hTree);

	// 将bytes写入文件
	void write2File(std::string filename, byte* input, int lens);

	// 从文件读入bytes
	byte* readFromFile(std::string filename, unsigned int* lens);

	// 拼接两个byte留
	byte* bytecat(byte* byte1, int lens1, byte* byte2, int lens2);

	compressor();
	~compressor();
};
