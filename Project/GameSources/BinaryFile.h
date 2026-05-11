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
inline static void BinaryWriteAdd(const std::string& path, const T& r)
{
	std::filesystem::path filePath(path);
	
	std::ofstream ofs(filePath, std::ios::binary | std::ios::app);
	if (!ofs) throw std::runtime_error("open failed");
			
	ofs.write(reinterpret_cast<const char*>(&r), sizeof(r));
	if (!ofs) throw std::runtime_error("write failed");
			
	ofs.close();
	if (!ofs) throw std::runtime_error("close failed");
}

template <typename T>
inline static void BinaryAllRead(const std::string& path, T& r)
{
	std::filesystem::path filePath(path);

	std::ifstream ifs(filePath, std::ios::binary);
	if (!ifs) throw std::runtime_error("open failed");
		
	ifs.read(reinterpret_cast<char*>(&r), sizeof(r));
	if (!ifs) throw std::runtime_error("read failed");

	ifs.close();
	if (!ifs) throw std::runtime_error("close failed");
}

template <typename T>
inline static void BinaryAllReadDataUnit(const std::string& path, std::vector<T>& datas)
{
	std::ifstream readFile(path, std::ios::binary | std::ios::ate);
	if (!readFile) throw std::runtime_error("open failed");
	
	std::streamsize size = readFile.tellg();
	readFile.seekg(0, std::ios::beg);

	std::vector<T> objects(size / sizeof(datas));

	readFile.read(reinterpret_cast<char*>(objects.data()), size);
	if (!readFile) throw std::runtime_error("read failed");

	datas = objects;

	readFile.close();
	if (!readFile) throw std::runtime_error("close failed");
}

inline static void BinaryClear(const std::string& path)
{
	std::filesystem::path filePath(path);

	std::ofstream ofs(filePath, std::ios::binary | std::ios::trunc);
	if (!ofs) throw std::runtime_error("open failed");

	ofs.close();
	if (!ofs) throw std::runtime_error("close failed");
}