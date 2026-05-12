/*!
@file Conversion.h
@brief 文字列変換用クラス
@copyright 菅野 皐
*/

#pragma once
#include <stdexcept>

inline static std::string to_string(const std::wstring& wString)
{
	std::string str;
	str.reserve(wString.size());
	for (wchar_t wc : wString)
	{
		str.push_back(static_cast<char>(wc));
	}
	return str;
}
