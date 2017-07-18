#pragma once
/*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������FBXMeshLoader
*	�衡������FBXģ���ļ���ģ�ͼ�����
*
*/

#include "MeshLoader.h"
#include "Mesh.h"
#include <fbxsdk.h>

class FBXModelLoader : public MeshLoader
{
private:
	FbxScene *m_scene;						//FBX����ָ��
	SkeletonModel *m_model;					//��̬Mesh


public:
	FBXModelLoader(string path);

	//Override
	virtual Model* LoadModel();


public:
	class FBXMeshLoaderException
	{
	private:
		string m_err;

	public:
		FBXMeshLoaderException(string err) :m_err(err) {}
	};

private:
	//ģ����Ϣ
	void LoadModelInfo(const FbxGlobalSettings &pGlobalSettings);

	//������Ϣ
	void LoadMesh(FbxNode *node);

	//������Ϣ
	void LoadBone(void);
	void LoadBone(FbxNode *node, Bone *bone);

	//������Ϣת��
	void MeshTransform(FbxMesh * mesh, SkeletonMesh &skeletonMesh);
	void GetVertexWeight(FbxGeometry* pGeometry, VertexWeight *vertexBoneInfo);		//���������Ϣ������������Ȩ����Ϣ��
	void GetNormal(FbxMesh *mesh, vector<SkeletonVertex> &vertexes);				//������Ϣ
	vec3 GetColor(FbxMesh *mesh, int crlPointIndex, int polyPointIndex);			//��ɫ��Ϣ
	void GetUV(FbxMesh *mesh, vector<SkeletonVertex> &vertexes);					//UV������Ϣ
	void LoadTexture(FbxMesh *mesh, string type, SkeletonMesh &skeletonMesh);		//������ͼ

	//������Ϣ
	void LoadBoneAnimation(void);
	void LoadAnimationLayerInfo(FbxAnimLayer* pAnimLayer, FbxNode* pNode);
	void LoadChannels(FbxNode* pNode, FbxAnimLayer* pAnimLayer);
	void LoadTranslate(FbxNode *node, FbxAnimLayer* animLayer, BoneAnimation &bone);
	void LoadRotate(FbxNode *node, FbxAnimLayer* animLayer, BoneAnimation &bone);
	void LoadScale(FbxNode *node, FbxAnimLayer* animLayer, BoneAnimation &bone);
	FbxAMatrix GetGeometryTransformation(FbxNode* inNode);
	vector<Key>*  LoadCurveKeys(FbxAnimCurve* pCurve);

public:
	//ת������
	static void FbxMatrixToGLM(FbxAMatrix& fbxMatrix, mat4& glmMatrix);
	static FbxAMatrix GLMToFbxMatrix(mat4& glmMatrix);
};