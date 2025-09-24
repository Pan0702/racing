#include "Ground.h"

Ground::Ground()
{
	mesh = new CFbxMesh();
	mesh->Load("Data/Map/map50Field1.mesh");

	meshCol = new MeshCollider();
	meshCol->MakeFromMesh(mesh);
}

Ground::~Ground()
{
	SAFE_DELETE(meshCol);
	SAFE_DELETE(mesh);
}

bool Ground::HitCheckSphere(const VECTOR3& center, float radius, VECTOR3* hit)
{
	std::list<MeshCollider::CollInfo　> meshes = meshCol->CheckCollisionSphereList(XMMatrixIdentity(), center, radius);
	if (meshes.size() == 0)
		return false;
	if (hit != nullptr) { // ���W���K�v�Ȃ̂ł����
		VECTOR3 pushVec = VECTOR3(0, 0, 0); // �ŏI�I�ɉ����x�N�g��
		for (const MeshCollider::CollInfo& m : meshes) { // �������Ă��邷�ׂẴ|���S����
			VECTOR3 move = center - m.hitPosition;
			float len = move.Length(); // ���������_���璆�S�ւ̋���
			move = move * ((radius - len) / len);
			VECTOR3 push = m.normal * Dot(move, m.normal); // �����Ԃ������x�N�g��
			// ����pushVec�ƍ�������
			VECTOR3 pushVecNorm = XMVector3Normalize(pushVec); // �����ς݃x�N�g���̌���
			float dot = Dot(push, pushVecNorm);	// ���̐����̒���
			if (dot < pushVec.Length()) {
				pushVec += push - pushVecNorm * dot; // ���̐��������炵�Ă���
			}
			else {
				pushVec = push;
			}
		}
		float f = pushVec.Length();
		*hit = center + pushVec;
	}
	return true;
}

bool Ground::HitCheckLine(const VECTOR3& from, const VECTOR3& to, VECTOR3* hit)
{
	MeshCollider::CollInfo coll;
	bool ret = meshCol->CheckCollisionLine(XMMatrixIdentity(), from, to, &coll);
	if (ret) {
		if (hit != nullptr) {
			*hit = coll.hitPosition;
		}
		return true;
	}
	return false;
}
