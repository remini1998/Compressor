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
	// 构造数组，可能要加入中止符号
	std::vector<treeNode<record>*> list;
	for (int a = 0; a < 256; a++) {
		list.push_back(src[a]);
	}
	treeNode<record> *father;
	treeNode<record>* min1 = list[0];
	treeNode<record>* min2;
	while (list.size() > 1) { // 这个可能是空间大小 小心
		// 找到最小的两个
		min1 = list[0];
		min2 = list[1];
		list.erase(list.begin());
		list.erase(list.begin());
		for (int a = 0; a < (list.size());) {
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

		// 拼接两个结点
		father = new treeNode<record>();
		father->data.value = -1;
		father->data.count = min1->data.count + min2->data.count;
		father->lChild = min1;
		father->rChild = min2;
		
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
