#pragma once;
#include<iostream>
#include<vector>
#include"heap.h"
//#include"FileCompress.h"

using namespace std;

template<class T>
struct HuffmanTreeNode
{
	HuffmanTreeNode<T>* _left;
	HuffmanTreeNode<T>* _right;
	T _weight;

	HuffmanTreeNode<T>(const T& w)
		:_left(NULL)
		, _right(NULL)
		, _weight(w)
	{}
};
//仿函数比较每个节点的height值
template<class T>
struct NodeCompare
{
	bool operator()(HuffmanTreeNode<T>* l, HuffmanTreeNode<T>* r)
	{
		return l->_weight < r->_weight;
	}
};
template<class T>
class HuffmanTree
{
	typedef HuffmanTreeNode<T> Node;
public:
	HuffmanTree()
	{}
	//构建Huffman树
	HuffmanTree(const T* arr, size_t size,const T& invalid)
	{
		_root = _CreateHuffmanTree(arr, size, invalid);
	}
	Node* GetRoot()
	{
		return _root;
	}
protected:
	Node* _CreateHuffmanTree(const T* arr, size_t size, const T& invaild)
	{
		assert(arr);
		Heap<Node*, NodeCompare<T>> minHeap;
		//将所有的数放入小堆中，小堆的堆顶是最小的元素
		for (size_t i = 0; i < size; i++)
		{
			if (arr[i] != invaild)
			{
				Node* tmp = new Node(arr[i]);
				minHeap.Push(tmp);

			}
		}
		//构建哈夫曼树
		while (!minHeap.Empty())
		{
			Node* left = minHeap.Top();
			minHeap.Pop();
			Node* right = minHeap.Top();
			minHeap.Pop();

			Node* parent = new Node(left->_weight + right->_weight);
			parent->_left = left;
			parent->_right = right;

			if (minHeap.Empty())
				return parent;

			minHeap.Push(parent);

		}
		return NULL;
	}
private:
	Node* _root;
};
void TestHuffmanTree()
{
	int arr[] = { 6, 3, 9, 1, 0, 4, 8, 2, 7, 5 };
	HuffmanTree<int> treeHuff(arr,10,-1);
}