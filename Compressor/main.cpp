#include <iostream>
#include "compressor.h"

//#define TEST
#define VERSON 1

using namespace std;


void help() {
	cout << "-h �� --help\t\t\t\t\t\t��ʾ����" << endl;
	cout << "-z �� --zip [Դ�ļ�·��] [ѹ���ļ�����·��] \t\t��Դ�ļ�ѹ��" << endl;
	cout << "-u �� --unzip [ѹ���ļ�·��] [��ѹ���ļ�����·��] \t��Դ�ļ�ѹ��" << endl;
}


void zip(string src, string dst) {
	compressor c;
	unsigned int srcLens;
	byte* srcb = c.readFromFile(src, &srcLens);
	treeNode<record>** count = c.countByte(srcb, srcLens);
	treeNode<record>* hTree = c.genHuffmanTree(count);
	int comLens;
	byte* com = c.encodeByHuffmanTree(srcb, srcLens, &comLens, hTree);
	byte* countb = c.transCountResult2Bytes(count);
	byte version[1] = { VERSON };
	byte* head = c.bytecat(version, 1, countb, 1024);
	byte* out = c.bytecat(head, 1 + 1024, com, comLens);
	c.write2File(dst, out, 1 + 1024 + comLens);
}

void unzip(string src, string dst) {
	compressor c;
	unsigned int srcLens;
	byte* srcb = c.readFromFile(src, &srcLens);
	if (srcb[0] != VERSON) {
		cout << "�汾��������ǰ�汾Ϊ" << VERSON << "�ļ��汾Ϊ" << (int)src[0];
	}
	treeNode<record>** count = c.transBytes2CountResult(srcb + 1);
	treeNode<record>* hTree = c.genHuffmanTree(count);
	int uncomLens;
	byte* uncom = c.decodeByHuffmanTree(srcb + 1024 + 1, &uncomLens, hTree);
	c.write2File(dst, uncom, uncomLens);
}

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
	byte* decode = c.decodeByHuffmanTree(encode, &decodeLens, hTree);

	c.write2File("test.bin", test, 10);
	unsigned int readLens;
	byte* read = c.readFromFile("test.bin", &readLens);

	zip("test1.bin", "test1.zzz");
	unzip("test1.zzz", "test1.bin.result");
}

int main(int argc, char* argv[]) {
#ifdef TEST
	test();
#else
	
	if (argc <= 1) {
		cout << "�����������-h�鿴������" << endl;
		return 1;
}

	//��ʾ����
	if (!strcmp(argv[1], "-h")) { help(); return 0; }
	else if (!strcmp(argv[1], "-H")) { help(); return 0; }
	else if (!strcmp(argv[1], "--help")) { help(); return 0; }
	else if (!strcmp(argv[1], "-help")) { help(); return 0; }
	else if (!strcmp(argv[1], "?")) { help(); return 0; }

	if (argc < 4) {
		cout << "�����������-h�鿴������" << endl;
		return 1;
	}
	bool isZip = false;
	std::string fileName1, fileName2;
	for (int a = 1; a < argc; a++) {
		//����Ƚ��ļ���
		if ((!strcmp(argv[a], "-z")) || (!strcmp(argv[a], "--zip"))) {
			if (a + 2 < argc) {
				fileName1 = argv[a + 1];
				fileName2 = argv[a + 2];
				a += 2;
				isZip = true;
			}
			else {
				cout << "�����������-h�鿴������" << endl;
				return 1;
			}
		}
	}
	for (int a = 1; a < argc; a++) {
		//����Ƚ��ļ���
		if ((!strcmp(argv[a], "-u")) || (!strcmp(argv[a], "--unzip"))) {
			if (a + 2 < argc) {
				fileName1 = argv[a + 1];
				fileName2 = argv[a + 2];
				a += 2;
				isZip = false;
			}
			else {
				cout << "�����������-h�鿴������" << endl;
				return 1;
			}
		}
	}
	if (isZip) {
		zip(fileName1, fileName2);
	}
	else
	{
		unzip(fileName1, fileName2);
	}
	return 0;
#endif // TEST


}