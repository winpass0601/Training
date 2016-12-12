#pragma once
#include <./Core/FileSystem/IO.h>
#include <./Component/Model/PmxModel.h>
#include <list>

namespace rinfw {
namespace core {
namespace filesystem {
template<>
class IO <Model> {
public:
	static bool Import(std::wstring filepass, boost::shared_ptr<Model> &obj, ImportFlg flg = ImportFlg::Read) {
		//拡張子取得
		int ext_i = filepass.find_last_of(L".");
		std::wstring extName = filepass.substr(ext_i, filepass.size() - ext_i);

		bool ret = false;
		boost::shared_ptr<PmxModel> tmp(new PmxModel());
		if (extName == L".pmx") {
			ret = ImPurser<PmxModel>::Purse(filepass, tmp, flg);
		}
		
		tmp->CreateBuffer();

		obj = std::move(tmp);

		//TODO リソースマネージャーに登録
		return ret;
	}
	template<typename T>
	static bool Export(std::string filepass, boost::shared_ptr<Model> &obj, ExportFlg flg = ExportFlg::New) {
		bool ret = ExPurser<Model>::Purse(filepass, obj, flg);
		return ret;
	}
};


}
}
}