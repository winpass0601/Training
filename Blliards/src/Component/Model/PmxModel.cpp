#include <./Component/Model/PmxModel.h>
#include <./Utility/EncodingHelper.h>

using namespace rinfw::core;

// インデックス値を読み込む
int ReadIndex(std::istream *stream, int size)
{
	switch (size)
	{
	case 1:
	uint8_t tmp8;
	stream->read((char*) &tmp8, sizeof(uint8_t));
	if (255 == tmp8)
	{
		return -1;
	}
	else {
		return (int) tmp8;
	}
	case 2:
	uint16_t tmp16;
	stream->read((char*) &tmp16, sizeof(uint16_t));
	if (65535 == tmp16)
	{
		return -1;
	}
	else {
		return (int) tmp16;
	}
	case 4:
	int tmp32;
	stream->read((char*) &tmp32, sizeof(int));
	return tmp32;
	default:
	return -1;
	}
}

// 文字列を読み込む
std::wstring ReadString(std::istream *stream, uint8_t encoding)
{
	oguna::EncodingConverter converter = oguna::EncodingConverter();
	int size;
	stream->read((char*) &size, sizeof(int));
	std::vector<char> buffer;
	if (size == 0)
	{
		return std::wstring(L"");
	}
	buffer.resize(size);
	stream->read((char*) buffer.data(), size);
	if (encoding == 0)
	{
		// UTF16
		return std::wstring((wchar_t*) buffer.data(), size / 2);
	}
	else
	{
		// UTF8
		std::wstring result;
		converter.Utf8ToUtf16(buffer.data(), size, &result);
		return result;
	}
}


void PmxSetting::Read(std::istream *_stream) {
	uint8_t count;
	int size = sizeof(uint8_t);
	_stream->read((char*) &count, size);
	if (count < 8)
		return;
	_stream->read((char*) &encoding, size);
	_stream->read((char*) &uv, size);
	_stream->read((char*) &vertex_index_size, size);
	_stream->read((char*) &texture_index_size, size);
	_stream->read((char*) &material_index_size, size);
	_stream->read((char*) &bone_index_size, size);
	_stream->read((char*) &morph_index_size, size);
	_stream->read((char*) &rigidbody_index_size, size);

	uint8_t temp;
	for (int i = 8; i < count; i++)
		_stream->read((char*) &temp, size);
}

#pragma region 頂点スキニング
void PmxVertexSkinningBDEF1::Read(std::istream *stream, PmxSetting *setting)
{
	this->bone_index = ReadIndex(stream, setting->bone_index_size);
}

void PmxVertexSkinningBDEF2::Read(std::istream *stream, PmxSetting *setting)
{
	this->bone_index1 = ReadIndex(stream, setting->bone_index_size);
	this->bone_index2 = ReadIndex(stream, setting->bone_index_size);
	stream->read((char*) &this->bone_weight, sizeof(float));
}

void PmxVertexSkinningBDEF4::Read(std::istream *stream, PmxSetting *setting)
{
	this->bone_index1 = ReadIndex(stream, setting->bone_index_size);
	this->bone_index2 = ReadIndex(stream, setting->bone_index_size);
	this->bone_index3 = ReadIndex(stream, setting->bone_index_size);
	this->bone_index4 = ReadIndex(stream, setting->bone_index_size);
	stream->read((char*) &this->bone_weight1, sizeof(float));
	stream->read((char*) &this->bone_weight2, sizeof(float));
	stream->read((char*) &this->bone_weight3, sizeof(float));
	stream->read((char*) &this->bone_weight4, sizeof(float));
}

void PmxVertexSkinningSDEF::Read(std::istream *stream, PmxSetting *setting)
{
	this->bone_index1 = ReadIndex(stream, setting->bone_index_size);
	this->bone_index2 = ReadIndex(stream, setting->bone_index_size);
	stream->read((char*) &this->bone_weight, sizeof(float));
	stream->read((char*) this->sdef_c, sizeof(float) * 3);
	stream->read((char*) this->sdef_r0, sizeof(float) * 3);
	stream->read((char*) this->sdef_r1, sizeof(float) * 3);
}

void PmxVertexSkinningQDEF::Read(std::istream *stream, PmxSetting *setting)
{
	this->bone_index1 = ReadIndex(stream, setting->bone_index_size);
	this->bone_index2 = ReadIndex(stream, setting->bone_index_size);
	this->bone_index3 = ReadIndex(stream, setting->bone_index_size);
	this->bone_index4 = ReadIndex(stream, setting->bone_index_size);
	stream->read((char*) &this->bone_weight1, sizeof(float));
	stream->read((char*) &this->bone_weight2, sizeof(float));
	stream->read((char*) &this->bone_weight3, sizeof(float));
	stream->read((char*) &this->bone_weight4, sizeof(float));
}

#pragma endregion

void PmxVertex::Read(std::istream *stream, PmxSetting *setting)
{
	stream->read((char*) this->positon, sizeof(float) * 3);
	stream->read((char*) this->normal, sizeof(float) * 3);
	stream->read((char*) this->uv, sizeof(float) * 2);
	for (int i = 0; i < setting->uv; ++i)
	{
		stream->read((char*) this->uva[i], sizeof(float) * 4);
	}
	stream->read((char*) &this->skinning_type, sizeof(PmxVertexSkinningType));
	switch (this->skinning_type)
	{
	case PmxVertexSkinningType::BDEF1:
	this->skinning = std::make_unique<PmxVertexSkinningBDEF1>();
	break;
	case PmxVertexSkinningType::BDEF2:
	this->skinning = std::make_unique<PmxVertexSkinningBDEF2>();
	break;
	case PmxVertexSkinningType::BDEF4:
	this->skinning = std::make_unique<PmxVertexSkinningBDEF4>();
	break;
	case PmxVertexSkinningType::SDEF:
	this->skinning = std::make_unique<PmxVertexSkinningSDEF>();
	break;
	case PmxVertexSkinningType::QDEF:
	this->skinning = std::make_unique<PmxVertexSkinningQDEF>();
	break;
	default:
	throw "invalid skinning type";
	}
	this->skinning->Read(stream, setting);
	stream->read((char*) &this->edge, sizeof(float));
}

void PmxMaterial::Read(std::istream *stream, PmxSetting *setting)
{
	this->material_name.swap(ReadString(stream, setting->encoding));
	this->material_english_name.swap(ReadString(stream, setting->encoding));
	stream->read((char*) this->diffuse, sizeof(float) * 4);
	stream->read((char*) this->specular, sizeof(float) * 3);
	stream->read((char*) &this->specularlity, sizeof(float));
	stream->read((char*) this->ambient, sizeof(float) * 3);
	stream->read((char*) &this->flag, sizeof(uint8_t));
	stream->read((char*) this->edge_color, sizeof(float) * 4);
	stream->read((char*) &this->edge_size, sizeof(float));
	this->diffuse_texture_index = ReadIndex(stream, setting->texture_index_size);
	this->sphere_texture_index = ReadIndex(stream, setting->texture_index_size);
	stream->read((char*) &this->sphere_op_mode, sizeof(uint8_t));
	stream->read((char*) &this->common_toon_flag, sizeof(uint8_t));
	if (this->common_toon_flag)
	{
		stream->read((char*) &this->toon_texture_index, sizeof(uint8_t));
	}
	else {
		this->toon_texture_index = ReadIndex(stream, setting->texture_index_size);
	}
	this->memo.swap(ReadString(stream, setting->encoding));
	stream->read((char*) &this->index_count, sizeof(int));
}

void PmxIkLink::Read(std::istream *stream, PmxSetting *setting)
{
	this->link_target = ReadIndex(stream, setting->bone_index_size);
	stream->read((char*) &this->angle_lock, sizeof(uint8_t));
	if (angle_lock == 1)
	{
		stream->read((char*) this->max_radian, sizeof(float) * 3);
		stream->read((char*) this->min_radian, sizeof(float) * 3);
	}
}

void PmxBone::Read(std::istream *stream, PmxSetting *setting)
{
	this->bone_name.swap(ReadString(stream, setting->encoding));
	this->bone_english_name.swap(ReadString(stream, setting->encoding));
	stream->read((char*) this->position, sizeof(float) * 3);
	this->parent_index = ReadIndex(stream, setting->bone_index_size);
	stream->read((char*) &this->level, sizeof(int));
	stream->read((char*) &this->bone_flag, sizeof(uint16_t));
	if (this->bone_flag & 0x0001) {
		this->target_index = ReadIndex(stream, setting->bone_index_size);
	}
	else {
		stream->read((char*)this->offset, sizeof(float) * 3);
	}
	if (this->bone_flag & (0x0100 | 0x0200)) {
		this->grant_parent_index = ReadIndex(stream, setting->bone_index_size);
		stream->read((char*) &this->grant_weight, sizeof(float));
	}
	if (this->bone_flag & 0x0400) {
		stream->read((char*)this->lock_axis_orientation, sizeof(float) * 3);
	}
	if (this->bone_flag & 0x0800) {
		stream->read((char*)this->local_axis_x_orientation, sizeof(float) * 3);
		stream->read((char*)this->local_axis_y_orientation, sizeof(float) * 3);
	}
	if (this->bone_flag & 0x2000) {
		stream->read((char*)this->key, sizeof(int));
	}
	if (this->bone_flag & 0x0020) {
		this->ik_target_bone_index = ReadIndex(stream, setting->bone_index_size);
		stream->read((char*) &ik_loop, sizeof(int));
		stream->read((char*) &ik_loop_angle_limit, sizeof(float));
		stream->read((char*) &ik_link_count, sizeof(int));
		this->ik_links = std::make_unique<PmxIkLink []>(ik_link_count);
		for (int i = 0; i < ik_link_count; i++) {
			ik_links[i].Read(stream, setting);
		}
	}
}

#pragma region モーフ
void PmxMorphVertexOffset::Read(std::istream *stream, PmxSetting *setting)
{
	this->vertex_index = ReadIndex(stream, setting->vertex_index_size);
	stream->read((char*)this->position_offset, sizeof(float) * 3);
}

void PmxMorphUVOffset::Read(std::istream *stream, PmxSetting *setting)
{
	this->vertex_index = ReadIndex(stream, setting->vertex_index_size);
	stream->read((char*)this->uv_offset, sizeof(float) * 4);
}

void PmxMorphBoneOffset::Read(std::istream *stream, PmxSetting *setting)
{
	this->bone_index = ReadIndex(stream, setting->bone_index_size);
	stream->read((char*)this->translation, sizeof(float) * 3);
	stream->read((char*)this->rotation, sizeof(float) * 4);
}

void PmxMorphMaterialOffset::Read(std::istream *stream, PmxSetting *setting)
{
	this->material_index = ReadIndex(stream, setting->material_index_size);
	stream->read((char*) &this->offset_operation, sizeof(uint8_t));
	stream->read((char*)this->diffuse, sizeof(float) * 4);
	stream->read((char*)this->specular, sizeof(float) * 3);
	stream->read((char*) &this->specularity, sizeof(float));
	stream->read((char*)this->ambient, sizeof(float) * 3);
	stream->read((char*)this->edge_color, sizeof(float) * 4);
	stream->read((char*) &this->edge_size, sizeof(float));
	stream->read((char*)this->texture_argb, sizeof(float) * 4);
	stream->read((char*)this->sphere_texture_argb, sizeof(float) * 4);
	stream->read((char*)this->toon_texture_argb, sizeof(float) * 4);
}

void PmxMorphGroupOffset::Read(std::istream *stream, PmxSetting *setting)
{
	this->morph_index = ReadIndex(stream, setting->morph_index_size);
	stream->read((char*) &this->morph_weight, sizeof(float));
}

void PmxMorphFlipOffset::Read(std::istream *stream, PmxSetting *setting)
{
	this->morph_index = ReadIndex(stream, setting->morph_index_size);
	stream->read((char*) &this->morph_value, sizeof(float));
}

void PmxMorphImplusOffset::Read(std::istream *stream, PmxSetting *setting)
{
	this->rigid_body_index = ReadIndex(stream, setting->rigidbody_index_size);
	stream->read((char*) &this->is_local, sizeof(uint8_t));
	stream->read((char*)this->velocity, sizeof(float) * 3);
	stream->read((char*)this->angular_torque, sizeof(float) * 3);
}

void PmxMorph::Read(std::istream *stream, PmxSetting *setting)
{
	this->morph_name = ReadString(stream, setting->encoding);
	this->morph_english_name = ReadString(stream, setting->encoding);
	stream->read((char*) &category, sizeof(MorphCategory));
	stream->read((char*) &morph_type, sizeof(MorphType));
	stream->read((char*) &this->offset_count, sizeof(int));
	switch (this->morph_type)
	{
	case MorphType::Group:
	group_offsets = std::make_unique<PmxMorphGroupOffset []>(this->offset_count);
	for (int i = 0; i < offset_count; i++)
	{
		group_offsets[i].Read(stream, setting);
	}
	break;
	case MorphType::Vertex:
	vertex_offsets = std::make_unique<PmxMorphVertexOffset []>(this->offset_count);
	for (int i = 0; i < offset_count; i++)
	{
		vertex_offsets[i].Read(stream, setting);
	}
	break;
	case MorphType::Bone:
	bone_offsets = std::make_unique<PmxMorphBoneOffset []>(this->offset_count);
	for (int i = 0; i < offset_count; i++)
	{
		bone_offsets[i].Read(stream, setting);
	}
	break;
	case MorphType::Matrial:
	material_offsets = std::make_unique<PmxMorphMaterialOffset []>(this->offset_count);
	for (int i = 0; i < offset_count; i++)
	{
		material_offsets[i].Read(stream, setting);
	}
	break;
	case MorphType::UV:
	case MorphType::AdditionalUV1:
	case MorphType::AdditionalUV2:
	case MorphType::AdditionalUV3:
	case MorphType::AdditionalUV4:
	uv_offsets = std::make_unique<PmxMorphUVOffset []>(this->offset_count);
	for (int i = 0; i < offset_count; i++)
	{
		uv_offsets[i].Read(stream, setting);
	}
	break;
	default:
	throw;
	}
}
#pragma endregion

#pragma region フレーム
void PmxFrameElement::Read(std::istream *stream, PmxSetting *setting)
{
	stream->read((char*) &this->element_target, sizeof(uint8_t));
	if (this->element_target == 0x00)
	{
		this->index = ReadIndex(stream, setting->bone_index_size);
	}
	else {
		this->index = ReadIndex(stream, setting->morph_index_size);
	}
}

void PmxFrame::Read(std::istream *stream, PmxSetting *setting)
{
	this->frame_name = ReadString(stream, setting->encoding);
	this->frame_english_name = ReadString(stream, setting->encoding);
	stream->read((char*) &this->frame_flag, sizeof(uint8_t));
	stream->read((char*) &this->element_count, sizeof(int));
	this->elements = std::make_unique<PmxFrameElement []>(this->element_count);
	for (int i = 0; i < this->element_count; i++)
	{
		this->elements[i].Read(stream, setting);
	}
}
#pragma endregion

void PmxRigidBody::Read(std::istream *stream, PmxSetting *setting)
{
	this->girid_body_name = ReadString(stream, setting->encoding);
	this->girid_body_english_name = ReadString(stream, setting->encoding);
	this->target_bone = ReadIndex(stream, setting->bone_index_size);
	stream->read((char*) &this->group, sizeof(uint8_t));
	stream->read((char*) &this->mask, sizeof(uint16_t));
	stream->read((char*) &this->shape, sizeof(uint8_t));
	stream->read((char*) this->size, sizeof(float) * 3);
	stream->read((char*) this->position, sizeof(float) * 3);
	stream->read((char*) this->orientation, sizeof(float) * 3);
	stream->read((char*) &this->mass, sizeof(float));
	stream->read((char*) &this->move_attenuation, sizeof(float));
	stream->read((char*) &this->rotation_attenuation, sizeof(float));
	stream->read((char*) &this->repulsion, sizeof(float));
	stream->read((char*) &this->friction, sizeof(float));
	stream->read((char*) &this->physics_calc_type, sizeof(uint8_t));
}

#pragma region ジョイント
void PmxJointParam::Read(std::istream *stream, PmxSetting *setting)
{
	this->rigid_body1 = ReadIndex(stream, setting->rigidbody_index_size);
	this->rigid_body2 = ReadIndex(stream, setting->rigidbody_index_size);
	stream->read((char*) this->position, sizeof(float) * 3);
	stream->read((char*) this->orientaiton, sizeof(float) * 3);
	stream->read((char*) this->move_limitation_min, sizeof(float) * 3);
	stream->read((char*) this->move_limitation_max, sizeof(float) * 3);
	stream->read((char*) this->rotation_limitation_min, sizeof(float) * 3);
	stream->read((char*) this->rotation_limitation_max, sizeof(float) * 3);
	stream->read((char*) this->spring_move_coefficient, sizeof(float) * 3);
	stream->read((char*) this->spring_rotation_coefficient, sizeof(float) * 3);
}

void PmxJoint::Read(std::istream *stream, PmxSetting *setting)
{
	this->joint_name = ReadString(stream, setting->encoding);
	this->joint_english_name = ReadString(stream, setting->encoding);
	stream->read((char*) &this->joint_type, sizeof(uint8_t));
	this->param.Read(stream, setting);
}
#pragma endregion

void PmxAncherRigidBody::Read(std::istream *stream, PmxSetting *setting)
{
	this->related_rigid_body = ReadIndex(stream, setting->rigidbody_index_size);
	this->related_vertex = ReadIndex(stream, setting->vertex_index_size);
	stream->read((char*) &this->is_near, sizeof(uint8_t));
}

void PmxSoftBody::Read(std::istream *stream, PmxSetting *setting)
{
	// 未実装
	//std::cerr << "Not Implemented Exception" << std::endl;
	throw;
}

//モデル
void PmxModel::Init()
{
	this->version = 0.0f;
	this->model_name.clear();
	this->model_english_name.clear();
	this->model_comment.clear();
	this->model_english_commnet.clear();
	/*this->vertex_count = 0;
	this->vertices = nullptr;
	this->index_count = 0;
	this->indices = nullptr;
	this->texture_count = 0;
	this->textures = nullptr;
	this->material_count = 0;
	this->materials = nullptr;
	this->bone_count = 0;
	this->bones = nullptr;*/
	this->morph_count = 0;
	this->morphs = nullptr;
	this->frame_count = 0;
	this->frames = nullptr;
	this->soft_body_count = 0;
	this->soft_bodies = nullptr;
}

void PmxModel::CreateBuffer()
{
	DX11Device &device = DX11Device::getInstance();

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(ConstantVertex) * this->mesh.vertexCount;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	device.getDevice()->CreateBuffer(&desc, nullptr, &this->vertexBuffer);

	desc.ByteWidth = sizeof(int)* this->mesh.indexCount;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	device.getDevice()->CreateBuffer(&desc, nullptr, &this->indexBuffer);

	desc.ByteWidth = sizeof(ConstantMaterial);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	device.getDevice()->CreateBuffer(&desc, nullptr, &this->materialBuffer);


	//頂点情報を格納していく
	D3D11_MAPPED_SUBRESOURCE ms;
	device.getContext()->Map(this->vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	ConstantVertex *pvertex = (ConstantVertex*) ms.pData;
	for (int i = 0; i < this->mesh.vertexCount; ++i) {
		memcpy(&pvertex[i], &this->mesh.vertices[i], 12 + 12 + 8 + 16 + 16 + 16 + 16);
	}
	device.getContext()->Unmap(vertexBuffer, NULL);

	//index情報を格納
	device.getContext()->Map(indexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, this->mesh.indices.get(), sizeof(int) * this->mesh.indexCount);
	device.getContext()->Unmap(indexBuffer, NULL);
}