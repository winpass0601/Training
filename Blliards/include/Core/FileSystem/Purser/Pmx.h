#pragma once

#include <./Core/FileSystem/Purser.h>
#include <./Component/Model/PmxModel.h>

#include <iostream>

//プロトタイプ宣言[./Component/Model/PmxModel.cpp]より
int ReadIndex(std::istream *stream, int size);
std::wstring ReadString(std::istream *stream, uint8_t encoding);

namespace rinfw {
namespace core {
void ReadMesh(std::istream *_stream, PmxMesh* _mesh, PmxSetting* setting);

void ReadCollision(std::istream *_stream, PmxCollision* _col, PmxSetting* setting);

template<>
class ImPurser<PmxModel> {
public:
	//仮
	static bool Purse(std::wstring filepass, boost::shared_ptr<PmxModel> &obj, ImportFlg flg = ImportFlg::Read) {

		//ぬるぽなら生成
		if (obj.get() == nullptr) {
			boost::shared_ptr<PmxModel> tmp(new PmxModel());
			obj = tmp;
		}

		//ファイルを開く
		std::ifstream stream = std::ifstream(filepass.c_str(), std::ios_base::binary);
		if (stream.fail())
			return false;

		//アドレスを取得して初期化
		PmxModel *data = obj.get();
		data->Init();

		// マジック
		char magic[4];
		stream.read((char*) magic, sizeof(char) * 4);
		if (magic[0] != 0x50 || magic[1] != 0x4d || magic[2] != 0x58 || magic[3] != 0x20)
		{
			//std::cerr << "invalid magic number." << std::endl;
			return false;
		}
		// バージョン
		stream.read((char*) &data->version, sizeof(float));
		if (data->version != 2.0f && data->version != 2.1f)
		{
			//std::cerr << "this is not ver2.0 or ver2.1 but " << version << "." << std::endl;
			return false;
		}


		// ファイル設定取得
		PmxSetting *setting = new PmxSetting();
		setting->Read(&stream);

		// モデル情報取得
		data->model_name.swap(ReadString(&stream, setting->encoding));
		data->model_english_name.swap(ReadString(&stream, setting->encoding));
		data->model_comment.swap(ReadString(&stream, setting->encoding));
		data->model_english_commnet.swap(ReadString(&stream, setting->encoding));


		//メッシュ情報取得
		ReadMesh(&stream, &data->mesh, setting);

		//モーフ情報取得
		stream.read((char*) &data->morph_count, sizeof(int));
		data->morphs = std::make_unique<PmxMorph []>(data->morph_count);
		for (int i = 0; i < data->morph_count; i++)
		{
			data->morphs[i].Read(&stream, setting);
		}

		// 表示枠情報取得
		stream.read((char*) &data->frame_count, sizeof(int));
		data->frames = std::make_unique<PmxFrame []>(data->frame_count);
		for (int i = 0; i < data->frame_count; i++)
		{
			data->frames[i].Read(&stream, setting);
		}

		//コリジョン情報取得
		ReadCollision(&stream, &data->collision, setting);

		int path_i = filepass.find_last_of(L"\\") + 1;
		if (path_i - 1 == std::string::npos) {
			path_i = filepass.find_last_of(L"/") + 1;
		}
		std::wstring path = filepass.substr(0, path_i);
		for (int i = 0; i < data->mesh.textureCount; i++) {
			Texture2D tex;
			tex.LoadFile(path + data->mesh.textureName[i]);
			data->mesh.texture.push_back(tex);
		}
		data->mesh.textureNum = data->mesh.texture.size();


		stream.close();

		delete setting;

		return true;
	}
};
}
}