#pragma once

// 二つの値を入れ替える
template<class T> void Swap(T* num1, T* num2)
{
	T tmp = *num1;
	*num1 = *num2;
	*num2 = tmp;
}
