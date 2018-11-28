#include <RayTracing/GenData.h>

#include "GenData_HV.h"
#include "GenData_MV.h"
#include "GenData_TV.h"

#include <RayTracing/Group.h>

using namespace RayTracing;
using namespace CppUtility::Other;
using namespace std;

GenData::GenData(const CppUtility::Other::CPtr<Hitable> & scene)
	: hitableVisitor(ToPtr(new GenData_HV)),
	matVisitor(ToPtr(new GenData_MV)),
	texVisitor(ToPtr(new GenData_TV))
{
	auto group = new Group;
	group->push_back(scene);
	root = ToCPtr(group);

	root->Accept(hitableVisitor);
	hitableVisitor->Accept(matVisitor);
	matVisitor->Accept(texVisitor);
}


const vector<float> GenData::GetSceneData() const {
	return hitableVisitor->GetSceneData();
}

const vector<float> GenData::GetMatData() const {
	return matVisitor->GetMatData();
}

const vector<float> GenData::GetTexData() const {
	return texVisitor->GetTexData();
}

const GenData::ImgIdxMap GenData::GetImgIdxMap() const {
	return texVisitor->GetImgIdxMap();
}