#include <iostream>
#include "compressor.h"
using namespace std;
int main() {
	byte test[10] = { 0, 3, 4, 2, 3, 3, 0 , 5, 4, 4 };
	compressor c;
	treeNode<record>** r = c.countByte(test, 10);
	treeNode<record>* re = c.genHuffmanTree(r);
}