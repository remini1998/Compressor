#include <vector>
#include<algorithm>  
#include "compressor.h"
#include "treeNode.h"

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
treeNode<record>* compressor::genHuffmanTree(treeNode<record>** src) {
	// �������飬����Ҫ������ֹ����
	std::vector<treeNode<record>*> list;
	for (int a = 0; a < 256; a++) {
		list.push_back(src[a]);
	}
	treeNode<record> *father;
	std::vector<treeNode<record>*>::iterator min1 = list.begin();
	std::vector<treeNode<record>*>::iterator min2 = list.begin();
	while (list.size() > 1) { // ��������ǿռ��С С��
		// �ҵ���С������
		min1 = list.begin();
		min2 = list.begin();
		min2++;
		for (std::vector<treeNode<record>*>::iterator now = list.begin()+1; now != list.end(); now++) {
			if ((*min1)->data.count > (*now)->data.count) {
				min2 = min1;
				min1 = now;
			}
			else if ((*min2)->data.count > (*now)->data.count)
				min2 = now;
		}

		// ƴ���������
		father = new treeNode<record>();
		father->data.value = -1;
		father->data.count = (*min1)->data.count + (*min2)->data.count;
		father->lChild = *min1;
		father->rChild = *min2;

		list.erase(min2);
		list.erase(min1);
		list.push_back(father);

	}
	return list[0];
}

compressor::compressor()
{
}


compressor::~compressor()
{
}
