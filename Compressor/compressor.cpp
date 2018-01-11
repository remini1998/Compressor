#include <vector>
#include <algorithm>  
#include <string>
#include <math.h>
#include <string.h>
#include "compressor.h"
#include "treeNode.h"

void compressor::recordEncode(treeNode<record>* r, char* beforeVal)
{
	if (r->data.value >= 0) {
		this->encode[r->data.value] = beforeVal;
	}
	else if (r->data.value == END_NODE) {
		this->encode[256] = beforeVal;
	}
	else {
		char bsl[256];
		strcpy(bsl, beforeVal);
		recordEncode(r->lChild, strcat(bsl, "0")); 
		char bsr[256];
		strcpy(bsr, beforeVal);
		recordEncode(r->rChild, strcat(bsr, "1"));
	}
}

byte compressor::calcNVal(int value, int n)
{
	int low = 1<< (8 * n);//����λ��8*nλ����1*2^(8*n)
	// ������λӰ��
	value /= low;
	return value % 256;
}

treeNode<record>** compressor::countByte(byte * src, int lens)
{
	// INIT
	treeNode<record>** results = new treeNode<record>*[256];
	for (int a = 0; a < 256; a++) {
		results[a] = new treeNode<record>();
		results[a]->data.value = a;
		results[a]->data.count = 0;
	}

	// COUNT
	for (int a = 0; a < lens; a++) 
		results[src[a]]->data.count++;
	
	return results;
}
treeNode<record>* compressor::genHuffmanTree(treeNode<record>** count) {
	// ��������
	std::vector<treeNode<record>*> list;
	for (int a = 0; a < 256; a++) {
		if(count[a]->data.count)
			list.push_back(count[a]);
	}
	
	// ���������
	treeNode<record>* end = new treeNode<record>();
	end->data.count = 1;
	end->data.value = END_NODE;
	list.push_back(end);


	treeNode<record> *father;
	treeNode<record>* min1 = list[0];
	treeNode<record>* min2;
	while (list.size() > 1) { // ��������ǿռ��С С��
		// �ҵ���С������
		min1 = list[0];
		min2 = list[1];
		list.erase(list.begin());
		list.erase(list.begin());
		for (unsigned int a = 0; a < (list.size());) {
			if (min1->data.count > list[a]->data.count) {
				list.push_back(min2);
				min2 = min1;
				min1 = list[a];
				list.erase(list.begin() + a);
			}
			else if (min2->data.count > list[a]->data.count) {
				list.push_back(min2);
				min2 = list[a];
				list.erase(list.begin() + a);
			}
			else
			{
				a++;
			}
		}

		// ƴ���������
		father = new treeNode<record>();
		father->data.value = STRUCTURE_NODE;
		father->data.count = min1->data.count + min2->data.count;
		father->lChild = min1;
		father->rChild = min2;
		
		list.push_back(father);

	}
	return list[0];
}
//treeNode<record>* compressor::trans2Huffman(byte * src, int lens)
//{
//	treeNode<record>** r = this->countByte(src, lens);
//	treeNode<record>* re = this->genHuffmanTree(r);
//	return re;
//}

byte* compressor::transCountResult2Bytes(treeNode<record>** result)
{
	byte* r = new byte[4*256];
	for (int a = 0; a < 256; a++) 
		for (int b = 0; b < 4; b++)
			r[4 * a + b] = calcNVal(result[a]->data.count, 3 - b);
	return r;
}

treeNode<record>** compressor::transBytes2CountResult(byte * bytes)
{
	treeNode<record>** r = new treeNode<record>*[256];
	for (int a = 0; a < 256; a++) {
		r[a] = new treeNode<record>();
		r[a]->data.count =
			(bytes[4 * a] << (8 * 3))
			+ (bytes[4 * a + 1] << (8 * 2))
			+ (bytes[4 * a + 2] << (8 * 1))
			+ (bytes[4 * a + 3]);
		r[a]->data.value = a;
	}
	return r;
}

byte * compressor::encodeByHuffmanTree(byte * input, int inLens, int* outLens, treeNode<record>* hTree)
{
	std::vector<byte> resultv;
	recordEncode(hTree);
	bool data[8];
	int boolIndex = 0;
	for (int a = 0; a <= inLens; a++) {
		std::string encodeNow;
		if (a == inLens)
			encodeNow = encode[256];
		else
			encodeNow = encode[input[a]];
		int lens = encodeNow.size();
		// ����¼ֵһһ����
		for (int b = 0; b < lens; b++) {
			char t = encodeNow[b];
			data[boolIndex++] = (t == '1');
			if (boolIndex >= 8) {
				boolIndex = 0;
				byte tb = (data[0] << 7)
					+ (data[1] << 6)
					+ (data[2] << 5)
					+ (data[3] << 4)
					+ (data[4] << 3)
					+ (data[5] << 2)
					+ (data[6] << 1)
					+ data[7];
				resultv.push_back(tb);
			}
		}
	}
	// ����βʣ��ֵ����
	byte tb = 0;
	for (int a = 0; a < boolIndex; a++)
		tb += data[a] << (7 - a); //����������λ��7-a
	resultv.push_back(tb);

	// ��vectorת��Ϊ����
	*outLens = resultv.size();
	byte* resultb = new byte[*outLens];
	for (int a = 0; a < *outLens; a++) {
		resultb[a] = resultv[a];
	}

	return resultb;
}

byte * compressor::decodeByHuffmanTree(byte * input, int inLens, int* outLens, treeNode<record>* hTree)
{
	std::vector<byte> resultv;
	bool data[8];
	int boolIndex = 0;
	treeNode<record>* nowNode = hTree;
	for (int nowInput = 0; nowInput < inLens; nowInput++) {
		byte tb = input[nowInput];
		//data[0]Ϊ��ͷλ
		for (int a = 0; a < 8; a++) {
			data[7 - a] = tb % 2;
			tb /= 2;
		}
		//����Huffman��
		for (int a = 0; a < 8; a++) {
			if (data[a]) 
				nowNode = nowNode->rChild;
			else
				nowNode = nowNode->lChild;

			if (nowNode->data.value == END_NODE) 
				goto FOR_END;
			else if (nowNode->data.value >= 0)
			{
				resultv.push_back(nowNode->data.value);
				nowNode = hTree;
			}
		}
	}
FOR_END:
	// ��vectorת��Ϊ����
	*outLens = resultv.size();
	byte* resultb = new byte[*outLens];
	for (unsigned int a = 0; a < resultv.size(); a++) {
		resultb[a] = resultv[a];
	}

	return resultb;
}

void compressor::write2File(char * filename, byte * input, int lens)
{

}

compressor::compressor()
{
}


compressor::~compressor()
{
}
