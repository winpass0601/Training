#pragma once
#include <string>
#include <./boost/shared_ptr.hpp>
#include <./boost/filesystem.hpp>

namespace rinfw {
namespace core {
enum class ImportFlg {
	Read,
	Lock,
};
enum class ExportFlg{
	Add,
	Write,
	New,
};
template<typename T>
class ImPurser{
public:
	static bool Purse(std::string,boost::shared_ptr<T>&,ImportFlg = ImportFlg::Read) {
		return false;
	}
protected:
private:
};
template<typename T>
class ExPurser {
public:
	static bool Purse(std::string,boost::shared_ptr<T>&,ExportFlg) {
		return false;
	}
protected:
private:
};
}
}

#include <./Core/FileSystem/Purser/txt.h>
#include <./Core/FileSystem/Purser/pmx.h>