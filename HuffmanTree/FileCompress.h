#include<iostream>
#include<assert.h>
#include<string>
#include<stdlib.h>
#include"HuffmanTree.h"
using namespace std;

typedef unsigned long LongType;
struct charInfo
{
	unsigned char _ch;      //统计出现的字符
	LongType _count;		//统计字符出现的次数
	string _code;			//统计字符对应的Huffman编码

	charInfo(LongType count = 0)
		:_count(count)
	{}

	//重载operator<
	bool operator<(const charInfo& _info)const
	{
		return _count < _info._count;
	}
	//重载operator+
	charInfo operator+(const charInfo& _info)const
	{
		return charInfo(_count + _info._count);
	}
	//重载operator!=
	bool operator!=(const charInfo& _info)const
	{
		return _count != _info._count;
	}
};

class FileCompress
{
public:
	//构造函数
	FileCompress()
	{
		for (size_t i = 0; i < 256; i++)
		{
			_info[i]._ch = i;
			_info[i]._count = 0;
		}
	}
	//压缩
	bool Compress(const char* filename)
	{
		FILE* fp = fopen(filename, "r");
		assert(fp);
		//1.统计文件中字符出现的次数
		char ch = fgetc(fp);
		long long chSize = 0;
		while (ch != EOF)
		{
			chSize++;
			_info[(unsigned char)ch]._count++;
			ch = fgetc(fp);
		}

		//2.构建哈夫曼树
		charInfo invalid;//过滤没出现的字符，没出现的字符不需要构建哈夫曼树
		HuffmanTree<charInfo> Tree(_info, 256, invalid);

		//3.生成哈夫曼编码
		string code;
		GenerateHuffmanCode(Tree.GetRoot(), code);

		//4.写入压缩文件
		string compressfile = filename;
		compressfile += ".huffman";

		FILE* fileIn = fopen(compressfile.c_str(), "wb");
		assert(fileIn);

		fseek(fp, 0, SEEK_SET);

		ch = fgetc(fp);
		unsigned char value = 0;
		int size = 0;
		while (ch != EOF)
		{
			string& code = _info[(unsigned char)ch]._code;
			for (size_t i = 0; i < code.size(); i++)
			{
				if (code[i] == '1')
					value |= 1;

				++size;
				if (size == 8)
				{
					fputc(value, fileIn);
					size = 0;
					value = 0;
				}

				value <<= 1;
			}
			ch = fgetc(fp);
		}
		//最后一个字符没有够8位，需要进行补位操作
		if (size > 0)
		{
			value <<= (7 - size);
			fputc(value, fileIn);
		}

		//写配置文件，以便解压缩
		string configcompress = filename;
		configcompress += ".config";

		FILE* fileCon = fopen(configcompress.c_str(), "wb");
		assert(fileCon);

		char str[128];
		itoa(chSize, str, 10);
		fputs(str, fileCon);
		fputc('\n', fileCon);

		for (size_t i = 0; i < 256; i++)
		{
			string Inconfig;
			if (_info[i]._count>0)
			{
				Inconfig += _info[i]._ch;
				Inconfig += ',';
				Inconfig += itoa(_info[i]._count, str, 10);
				Inconfig += '\n';
			}
			fputs(Inconfig.c_str(), fileCon);
			Inconfig.clear();
		}
		fclose(fp);
		fclose(fileIn);
		fclose(fileCon);
		return true;
	}
	//解压缩
	bool Uncompress(const char* filename)
	{
		//读取配置文件，得到字符及字符出现的次数
		string configFile = filename;
		configFile += ".config";
		FILE* foutconfig = fopen(configFile.c_str(), "r");
		assert(foutconfig);

		string line;
		long long chSize = 0;
		//读取配置文件中的一行，得到一个字符及其出现的次数
		ReadLine(foutconfig, line);
		chSize += atoi(line.c_str());
		line.clear();

		while (ReadLine(foutconfig, line))
		{
			if (!line.empty())
			{
				unsigned char ch = line[0];
				_info[ch]._count = atoi(line.substr(2).c_str());
				line.clear();
			}
			else
				line += '\n';
		}

		//重构哈夫曼树
		charInfo invalid;
		HuffmanTree<charInfo> Tree(_info, 256, invalid);

		//读取压缩文件，解析字符及出现的次数
		string compressfile = filename;
		compressfile += ".huffman";
		FILE* filecom = fopen(compressfile.c_str(), "rb");
		assert(filecom);

		//解压缩文件
		string uncompressfile = filename;
		uncompressfile += ".uncompress";
		FILE* fUncom = fopen(uncompressfile.c_str(), "wb");
		assert(fUncom);

		char ch = fgetc(filecom);
		HuffmanTreeNode<charInfo>* root = Tree.GetRoot();
		HuffmanTreeNode<charInfo>* cur = root;
		int pos = 8;
		while (1)
		{
			if (cur->_left == NULL && cur->_right == NULL)
			{
				fputc(cur->_weight._ch, fUncom);
				cur = root;
				if (--chSize == 0)
					break;
			}
			--pos;
			if (ch & (1 << pos))
				cur = cur->_right;
			else
				cur = cur->_left;
			if (pos == 0)
			{
				ch = fgetc(filecom);
				pos = 8;
			}
			//ch = fgetc(filecom);
		}
		fclose(foutconfig);
		fclose(filecom);

		fclose(fUncom);
		return true;
	}
protected:
	//生成哈夫曼编码
	void GenerateHuffmanCode(HuffmanTreeNode<charInfo>* root, string& code)
	{
		if (root == NULL)
			return;
		if (root->_left == NULL && root->_right == NULL)
		{
			_info[root->_weight._ch]._code = code;
			return;
		}
		GenerateHuffmanCode(root->_left, code + '0');
		GenerateHuffmanCode(root->_right, code + '1');
	}
	//读取文件中的一行
	bool ReadLine(FILE* fp,string& line)
	{
		char ch = fgetc(fp);
		if (ch == EOF)
			return false;
		while (ch!=EOF && ch!='\n')
		{
			line += ch;
			ch = fgetc(fp);
		}
		return true;
	}
private:
	charInfo _info[256];
};
void TestFileCompress()
{
	FileCompress fc;
	fc.Compress("input.BIG");
}
void TestUnCompress()
{
	FileCompress fc;
	fc.Uncompress("input.BIG");
}