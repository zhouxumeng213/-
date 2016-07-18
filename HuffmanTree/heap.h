#include<iostream>
using namespace std;
#include<assert.h>
#include<vector>

//小于，即建立小堆
template<class T>
struct Less
{
	bool operator() (const T& l, const T& r)
	{
		return l < r;
	}
};
//大于，即建立大堆
template<class T>
struct Greater
{
	bool operator() (const T& l, const T& r)
	{
		return l > r;
	}
};

//利用仿函数实现大小堆
template<class T, class compare>
class Heap
{
public:
	Heap()
	{}
	Heap(const T* arr, size_t size)
	{
		assert(arr);
		//将所给的数组里的数字放入vector中
		for (size_t i = 0; i < size; i++)
		{
			_arr.push_back(arr[i]);
		}
		//建堆(从最后一个非叶子节点开始向下调整)
		for (int i = size/2-1; i >= 0; i--)
		{
			_AdjustDown(i);
		}
	}
	//获取堆顶元素
	T& Top()
	{
		return _arr[0];
	}
	//获取堆结点个数
	size_t Size()
	{
		return _arr.size();
	}
	//判断堆是否为空
	bool Empty()
	{
		return _arr.empty();
	}
	//向堆中添加一个结点
	void Push(const T& x)
	{
		_arr.push_back(x);
		//for (int begin = _arr.size() - 1; begin >= 0; begin--)
		//{
			_AdjustUp(_arr.size()-1);
		//}
	}
	//从堆中删除一个结点
	void Pop()
	{
		//将最后一个结点覆盖堆顶
		_arr[0] = _arr[_arr.size() - 1];
		//删除最后一个结点
		_arr.pop_back();
		//从根结点开始向下调整
		//for (int begin = 0; begin < _arr.size(); begin++)
		//{
			_AdjustDown(0);
		//}
	}
protected:
	//向下调整算法
	void _AdjustDown(int parent)
	{
		compare com;
		//child：保存左右孩子
		int child = parent * 2 + 1;
		while (child < _arr.size())
		{
			//选出左右孩子中较大的
			if (child + 1<_arr.size() && com(_arr[child + 1], _arr[child]))
			{
				child = child + 1;
			}
			//判断根结点和左右孩子中较大值得大小
			if (com(_arr[child], _arr[parent]))
			{
				//如果child比parent大，交换两个的值
				swap(_arr[child], _arr[parent]);
				//继续判断更新后的左右孩子是否满足大堆或者小堆
				parent = child;
				child = parent * 2 + 1;
			}
			else
			{
				break;
			}
		}
	}
	//向上调整算法
	void _AdjustUp(int child)
	{
		compare com;
		int parent = (child - 1) / 2;
		while (child > 0)
		{
			//只需比较孩子和父节点
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