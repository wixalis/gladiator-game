#pragma once
template <typename T1, typename T2>
struct PairRef
{
	T1& first;
	T2& second;

	PairRef(T1& first_, T2& second_) : first(first_), second(second_) { }
};
