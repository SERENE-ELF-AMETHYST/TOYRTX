#include "GenData_HV.h"
#include "GenData_MV.h"

#include <RayTracing/Group.h>
#include <RayTracing/Sphere.h>
#include <RayTracing/BVH_Node.h>

using namespace RayTracing;
using namespace glm;
using namespace std;

//HT : Hitable Type
const float HT_Sphere   = 0.0f;
const float HT_Group    = 1.0f;
const float HT_BVH_Node = 2.0f;

void GenData_HV::Visit(const Hitable::CPtr & hitable) {
	if (hitable == NULL)
		return;

	if(hitable->GetMat())
		mat2idxVec[hitable->GetMat()].push_back(sceneData.size());
	sceneData.push_back(-1);

	sceneData.push_back(hitable->IsMatCoverable());

	const AABB box = hitable->GetBoundingBox();
	sceneData.push_back(box.GetMinP().x);
	sceneData.push_back(box.GetMinP().y);
	sceneData.push_back(box.GetMinP().z);
	sceneData.push_back(box.GetMaxP().x);
	sceneData.push_back(box.GetMaxP().y);
	sceneData.push_back(box.GetMaxP().z);
}

void GenData_HV::Visit(const Sphere::CPtr & sphere) {
	if (sphere == NULL)
		return;

	auto targetPair = hitable2idx.find(sphere);
	if (targetPair != hitable2idx.end())
		return;

	hitable2idx[sphere] = sceneData.size();

	sceneData.push_back(HT_Sphere);

	Visit(static_cast<const Hitable::CPtr>(sphere));

	vec3 center = sphere->GetCenter();
	sceneData.push_back(center.x);
	sceneData.push_back(center.y);
	sceneData.push_back(center.z);

	sceneData.push_back(sphere->GetRadius());
}

void GenData_HV::Visit(const Group::CPtr & group) {
	if (group == NULL)
		return;

	auto targetPair = hitable2idx.find(group);
	if (targetPair != hitable2idx.end())
		return;

	hitable2idx[group] = sceneData.size();

	sceneData.push_back(HT_Group);

	Visit(static_cast<const Hitable::CPtr>(group));

	size_t childrenSize = group->GetChildren().size();
	//sceneData.push_back(childrenSize);

	size_t curChildIt = sceneData.size();
	for (size_t i = 0; i < childrenSize; i++)
		sceneData.push_back(-1);
	
	// childrenEnd ���Ա�ʶ��β
	sceneData.push_back(-1);

	for (auto const & child : group->GetChildren()) {
		if (child == NULL)
			continue;

		auto targetHitableIdx = hitable2idx.find(child);
		if (targetHitableIdx != hitable2idx.end()) {
			sceneData[curChildIt++] = targetHitableIdx->second;
			continue;
		}

		sceneData[curChildIt++] = sceneData.size();
		child->Accept(This());
	}
}

void GenData_HV::Visit(const BVH_Node::CPtr & bvhNode) {
	if (bvhNode == NULL)
		return;

	auto targetPair = hitable2idx.find(bvhNode);
	if (targetPair != hitable2idx.end())
		return;

	hitable2idx[bvhNode] = sceneData.size();

	sceneData.push_back(HT_BVH_Node);
	Visit(static_cast<const Hitable::CPtr>(bvhNode));

	size_t curChildIt = sceneData.size();
	// left
	if(bvhNode->GetLeft())
		sceneData.push_back(-1);
	// right
	if(bvhNode->GetRight())
		sceneData.push_back(-1);
	
	// childrenEnd ���Ա�ʶ��β
	sceneData.push_back(-1);

	if (bvhNode->GetLeft()) {
		auto targetLeftIdx = hitable2idx.find(bvhNode->GetLeft());
		if (targetLeftIdx == hitable2idx.end()) {
			sceneData[curChildIt++] = sceneData.size();
			bvhNode->GetLeft()->Accept(This());
		}else
			sceneData[curChildIt++] = targetLeftIdx->second;
	}

	if (bvhNode->GetRight()) {
		auto targetRightIdx = hitable2idx.find(bvhNode->GetRight());
		if (targetRightIdx == hitable2idx.end()) {
			sceneData[curChildIt++] = sceneData.size();
			bvhNode->GetRight()->Accept(This());
		}
		else
			sceneData[curChildIt++] = targetRightIdx->second;
	}
}

void GenData_HV::SetMat(const MatIdxMap & mat2idx) {
	for (auto const & pair : mat2idx) {
		auto target = mat2idxVec.find(pair.first);
		if (target != mat2idxVec.end()) {
			for (auto const & idx : target->second)
				sceneData[idx] = pair.second;
		}
	}
}

void GenData_HV::Accept(const GenData_MV::Ptr & genFS_MV) {
	for (auto const & pair : mat2idxVec)
		pair.first->Accept(genFS_MV);

	SetMat(genFS_MV->GetMatIdxMap());
}