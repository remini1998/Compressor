#include <iostream>
#include "compressor.h"

#define TEST

using namespace std;
void test() {
	byte test[10] = { 0, 3, 4, 2, 3, 3, 0 , 5, 4, 4 };
	compressor c;
	treeNode<record>** count = c.countByte(test, 10);
	treeNode<record>* hTree = c.genHuffmanTree(count);
	byte* countb = c.transCountResult2Bytes(count);
	treeNode<record>** countNew = c.transBytes2CountResult(countb);
	treeNode<record>* hTreeNew = c.genHuffmanTree(countNew);

	int encodeLens, decodeLens;
	byte* encode = c.encodeByHuffmanTree(test, 10, &encodeLens, hTree);
	byte* decode = c.decodeByHuffmanTree(encode, encodeLens, &decodeLens, hTree);
}

int main() {
#ifdef TEST
	test();
#else

#endif // TEST


}