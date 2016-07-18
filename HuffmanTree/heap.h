#include<iostream>
using namespace std;
#include<assert.h>
#include<vector>

//С�ڣ�������С��
template<class T>
struct Less
{
	bool operator() (const T& l, const T& r)
	{
		return l < r;
	}
};
//���ڣ����������
template<class T>
struct Greater
{
	bool operator() (const T& l, const T& r)
	{
		return l > r;
	}
};

//���÷º���ʵ�ִ�С��
template<class T, class compare>
class Heap
{
public:
	Heap()
	{}
	Heap(const T* arr, size_t size)
	{
		assert(arr);
		//������������������ַ���vector��
		for (size_t i = 0; i < size; i++)
		{
			_arr.push_back(arr[i]);
		}
		//����(�����һ����Ҷ�ӽڵ㿪ʼ���µ���)
		for (int i = size/2-1; i >= 0; i--)
		{
			_AdjustDown(i);
		}
	}
	//��ȡ�Ѷ�Ԫ��
	T& Top()
	{
		return _arr[0];
	}
	//��ȡ�ѽ�����
	size_t Size()
	{
		return _arr.size();
	}
	//�ж϶��Ƿ�Ϊ��
	bool Empty()
	{
		return _arr.empty();
	}
	//��������һ�����
	void Push(const T& x)
	{
		_arr.push_back(x);
		//for (int begin = _arr.size() - 1; begin >= 0; begin--)
		//{
			_AdjustUp(_arr.size()-1);
		//}
	}
	//�Ӷ���ɾ��һ�����
	void Pop()
	{
		//�����һ����㸲�ǶѶ�
		_arr[0] = _arr[_arr.size() - 1];
		//ɾ�����һ�����
		_arr.pop_back();
		//�Ӹ���㿪ʼ���µ���
		//for (int begin = 0; begin < _arr.size(); begin++)
		//{
			_AdjustDown(0);
		//}
	}
protected:
	//���µ����㷨
	void _AdjustDown(int parent)
	{
		compare com;
		//child���������Һ���
		int child = parent * 2 + 1;
		while (child < _arr.size())
		{
			//ѡ�����Һ����нϴ��
			if (child + 1<_arr.size() && com(_arr[child + 1], _arr[child]))
			{
				child = child + 1;
			}
			//�жϸ��������Һ����нϴ�ֵ�ô�С
			if (com(_arr[child], _arr[parent]))
			{
				//���child��parent�󣬽���������ֵ
				swap(_arr[child], _arr[parent]);
				//�����жϸ��º�����Һ����Ƿ������ѻ���С��
				parent = child;
				child = parent * 2 + 1;
			}
			else
			{
				break;
			}
		}
	}
	//���ϵ����㷨
	void _AdjustUp(int child)
	{
		compare com;
		int parent = (child - 1) / 2;
		while (child > 0)
		{
			//ֻ��ȽϺ��Ӻ͸��ڵ�
			if (com(_arr[child], _arr[parent]))
			{
				swap(_arr[child], _arr[parent]);
				child = parent;
				parent = (child - 1) / 2;
			}
			else
			{
				break;
			}
		}
	}
private:
	vector<T> _arr;
};
void Test()
{
	int a[] = { 10, 11, 13, 12, 16, 18, 15, 17, 14, 19 };
	Heap<int, Less<int>> hp(a, 10);
	hp.Push(20);
	hp.Pop();
}