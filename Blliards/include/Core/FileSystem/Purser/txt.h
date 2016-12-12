#pragma once
#include <./Core/FileSystem/Purser.h>
#include <iostream>

namespace rinfw {
namespace core {
template<>
class ImPurser<std::string>{
public:
	static bool Purse(std::string filepass,boost::shared_ptr<std::string> &obj,ImportFlg flg) {
		if (obj.get() == nullptr) {
			boost::shared_ptr<std::string> newobj(new std::string);
			obj = newobj;
		}
		if (flg == ImportFlg::Lock) {
			boost::filesystem::ifstream file( filepass,std::ios::in);
			if (file.fail()) {
				return false;
			}
			file >> *obj.get();
			file.close();
			return true;
		}
		boost::filesystem::ifstream file( filepass);
		if (file.fail()) {
			return false;
		}
		file >> *obj.get();
		file.close();
		return false;
	}
protected:
private:
};
template<>
class ExPurser<std::string>{
public:
	static bool Purse(std::string filepass,boost::shared_ptr<std::string> &obj,ExportFlg flg) {
		//’Ç‹L‚à‚µ‚­‚Íã‘‚«
		if(flg == ExportFlg::Add){
			boost::filesystem::ofstream file( filepass,std::ios::app);
			if (file.fail()){
				return false;
			}
			file << *obj.get() << std::endl;
			file.close();
			return true;
		}else if(flg == ExportFlg::Write){
			boost::filesystem::ofstream file( filepass,std::ios::trunc);
			if (file.fail()){
				return false;
			}
			file << *obj.get() << std::endl;
			file.close();
			return true;
		}
		//V‹K’Ç‰Á
		boost::filesystem::ofstream file( filepass);
		file << *obj.get() << std::endl;
		file.close();
		return false;
	}
protected:
private:
};
}
}