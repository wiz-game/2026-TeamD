/*!
@file Utils.h
@brief 汎用的な関数や構造体を定義するヘッダーファイル
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

//--------------------------------------------------------------------------------------
// Timer : 時間計測用構造体
//--------------------------------------------------------------------------------------
struct Timer
{
private:
	float Counter = 0.0f;
	float Time = 1.0f;
public:
	Timer(const float& T) : Time(T) {}
		
	// アクセサ
	void SetCounter(const float& C = 0.0f) { Counter = C; }
	void SetTime(const float& T) { Time = T; };
	float GetCounter() { return Counter; }
	float GetTime() { return Time; };

	//----------------------------------------------------------------------------------
	/*
		設定した時間を超えたら自動で真偽値を返す
		@param ElapsedTime 経過時間
		@param AutoCounterReset 自動でカウンターをリセットするか
		@return 超えたかどうか
	*/
	bool TimeCount(const float& ElapsedTime, bool AutoCounterReset = true)
	{
		Counter += ElapsedTime;
		if (Counter >= Time)
		{
			if (AutoCounterReset) SetCounter();
			return true;
		}
		return false;
	}

	//----------------------------------------------------------------------------------
	/*
		割合以上か
		@param Ratio 割合
		@return 以上か
	*/
	bool IsRatioOrMore(const float& Ratio) { return (Counter >= Time * (Ratio / 100.0f)); }
};

