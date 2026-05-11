/*!
@file BinaryFile.h
@brief バイナリファイル読み書き用クラス
@copyright 菅野 皐
*/

#pragma once
#include <filesystem>
#include <fstream>
#include <type_traits>
#include <stdexcept>

template <typename T>
inline static void BinaryWrite(const std::string& path, const T& r)
{
	static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable");

	std::filesystem::path filePath(path);

	std::ofstream ofs(filePath, std::ios::binary);
	if (!ofs) throw std::runtime_error("open failed");
			
	ofs.write(reinterpret_cast<const char*>(&r), sizeof(r));
	if (!ofs) throw std::runtime_error("write failed");
			
	ofs.close();
	if (!ofs) throw std::runtime_error("close failed");
}

template <typename T>
inline static void BinaryRead(const std::string& path, T& r)
{
	std::filesystem::path filePath(path);

	std::ifstream ifs(filePath, std::ios::binary);
	if (!ifs) throw std::runtime_error("open failed");
		
	ifs.read(reinterpret_cast<char*>(&r), sizeof(r));
	if (!ifs) throw std::runtime_error("read failed");
}
