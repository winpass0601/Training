#include <./Core/FileSystem/Purser/Pmx.h>


namespace rinfw {
namespace core {

void ReadMesh(std::istream *_stream, PmxMesh* _mesh, PmxSetting* setting) {
	// 頂点
	_stream->read((char*) &_mesh->vertexCount, sizeof(int));
	_mesh->vertices = std::make_unique<PmxVertex []>(_mesh->vertexCount);
	for (int i = 0; i < _mesh->vertexCount; i++)
	{
		_mesh->vertices[i].Read(_stream, setting);
		_mesh->vertices[i].positon[0] *= 0.1f;
		_mesh->vertices[i].positon[1] *= 0.1f;
		_mesh->vertices[i].positon[2] *= 0.1f;
	}

	// 面
	_stream->read((char*) &_mesh->indexCount, sizeof(int));
	_mesh->indices = std::make_unique<int []>(_mesh->indexCount);
	for (int i = 0; i < _mesh->indexCount; i++)
	{
		_mesh->indices[i] = ReadIndex(_stream, setting->vertex_index_size);
	}

	// テクスチャ
	_stream->read((char*) &_mesh->textureCount, sizeof(int));
	_mesh->textureName = std::make_unique<std::wstring []>(_mesh->textureCount);
	for (int i = 0; i < _mesh->textureCount; i++)
	{
		_mesh->textureName[i] = ReadString(_stream, setting->encoding);
	}

	// マテリアル
	_stream->read((char*) &_mesh->materialCount, sizeof(int));
	_mesh->materials = std::make_unique<PmxMaterial []>(_mesh->materialCount);
	for (int i = 0; i < _mesh->materialCount; i++)
	{
		_mesh->materials[i].Read(_stream, setting);
	}

	// ボーン
	_stream->read((char*) &_mesh->boneCount, sizeof(int));
	_mesh->bones = std::make_unique<PmxBone []>(_mesh->boneCount);
	for (int i = 0; i < _mesh->boneCount; i++)
	{
		_mesh->bones[i].Read(_stream, setting);
	}
}

void ReadCollision(std::istream *_stream, PmxCollision* _col, PmxSetting* setting) {
	// 剛体
	_stream->read((char*) &_col->rigidBodyCount, sizeof(int));
	_col->rigidBodies = std::make_unique<PmxRigidBody []>(_col->rigidBodyCount);
	for (int i = 0; i < _col->rigidBodyCount; i++)
	{
		_col->rigidBodies[i].Read(_stream, setting);
	}

	// ジョイント
	_stream->read((char*) &_col->jointCount, sizeof(int));
	_col->joints = std::make_unique<PmxJoint []>(_col->jointCount);
	for (int i = 0; i < _col->jointCount; i++)
	{
		_col->joints[i].Read(_stream, setting);
	}
}

}
}