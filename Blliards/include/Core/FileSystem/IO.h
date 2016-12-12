#pragma once
#include <./Core/ResourceManager.h>
#include <./Core/FileSystem/Purser.h>
#include <list>

namespace rinfw {
namespace core {
namespace filesystem {
template<typename T>
class IO {
public:
	static bool Import(std::string filepass,boost::shared_ptr<T> &obj,ImportFlg flg = ImportFlg::Read) {
		bool ret = ImPurser<T>::Purse(filepass, obj,flg);
		//TODO リソースマネージャーに登録
		return ret;
	}
	static bool Export(std::string filepass,boost::shared_ptr<T> &obj,ExportFlg flg = ExportFlg::New) {
		bool ret = ExPurser<T>::Purse(filepass, obj,flg);
		return ret;
	}
};
}
}
}
#include <./Core/FileSystem/IO/ModelIO.h>