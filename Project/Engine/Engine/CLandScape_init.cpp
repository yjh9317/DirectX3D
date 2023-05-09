#include "pch.h"
#include "CLandScape.h"

#include "CSceneMgr.h"
#include "CResMgr.h"
#include "CEventMgr.h"

void CLandScape::Create()
{
	CreateMesh();

	CreateShaderMaterial();

	CreateTexture();
}



void CLandScape::CreateMesh()
{
	// ���� �޽� ����
	Ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"LandscapeMesh");

	// �޽� �����
	// ������ �����ϴ� �޽��� ����
	if (nullptr != pMesh)
	{
		// ����
		CResMgr::GetInst()->ForceDeleteRes<CTexture>(L"LandscapeMesh");
		pMesh = nullptr;
	}

	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;

	Vtx v;

	// ���� ��ġ
	for (UINT row = 0; row < m_iZFaceCount + 1; ++row)
	{
		for (UINT col = 0; col < m_iXFaceCount + 1; ++col)
		{
			v.vPos = Vec3((float)col, 0.f, (float)row);
			v.vUV = Vec2(col, m_iZFaceCount - row);

			v.vNormal = Vec3(0.f, 1.f, 0.f);
			v.vTangent = Vec3(1.f, 0.f, 0.f);
			v.vBinormal = Vec3(0.f, 0.f, -1.f);

			v.vColor = Vec4(1.f, 0.f, 1.f, 1.f);

			vecVtx.push_back(v);
		}
	}

	// �ε���
	for (UINT row = 0; row < m_iZFaceCount; ++row)
	{
		for (UINT col = 0; col < m_iXFaceCount; ++col)
		{
			// 0
			// | \
			// 2- 1
			vecIdx.push_back(row * (m_iXFaceCount + 1) + col + m_iXFaceCount + 1);
			vecIdx.push_back(row * (m_iXFaceCount + 1) + col + 1);
			vecIdx.push_back(row * (m_iXFaceCount + 1) + col);

			// 1- 2
			//  \ |
			//    0
			vecIdx.push_back(row * (m_iXFaceCount + 1) + col + 1);
			vecIdx.push_back(row * (m_iXFaceCount + 1) + col + m_iXFaceCount + 1);
			vecIdx.push_back(row * (m_iXFaceCount + 1) + col + m_iXFaceCount + 1 + 1);
		}
	}

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	CResMgr::GetInst()->AddRes<CMesh>(L"LandscapeMesh", pMesh.Get(), true);
	SetMesh(pMesh);
}


void CLandScape::CreateShaderMaterial()
{
	// ======================
	// ���� ���̴� �� ���� ����
	// ======================		
	Ptr<CGraphicsShader> pShader = CResMgr::GetInst()->FindRes<CGraphicsShader>(L"LandScapeShader");

	if (nullptr == pShader)
	{
		pShader = new CGraphicsShader;
		pShader->CreateVertexShader(L"shader\\LandScape.fx", "VS_LandScape");
		pShader->CreateHullShader(L"shader\\LandScape.fx", "HS_LandScape");
		pShader->CreateDomainShader(L"shader\\LandScape.fx", "DS_LandScape");
		pShader->CreatePixelShader(L"shader\\LandScape.fx", "PS_LandScape");
		pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

		pShader->SetShaderDomain(SHADER_DOMAIN::DOMAIN_DEFERRED);
		pShader->SetRSType(RS_TYPE::CULL_BACK);
		//pShader->SetRSType(RS_TYPE::WIRE_FRAME);
		pShader->SetBSType(BS_TYPE::DEFAULT);
		pShader->SetDSType(DS_TYPE::LESS);

		CResMgr::GetInst()->AddRes<CGraphicsShader>(L"LandScapeShader", pShader.Get(), true);
	}

	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\LandScapeMtrl.mtrl");

	if (nullptr == pMtrl)
	{
		pMtrl = new CMaterial;
		pMtrl->SetShader(pShader);
		CResMgr::GetInst()->AddRes<CMaterial>(L"material\\LandScapeMtrl.mtrl", pMtrl.Get(), true);
	}

	SetSharedMaterial(pMtrl, 0);


	// =====================
	// ���� ��ŷ ��ǻƮ ���̴�
	// =====================
	m_pCSRaycast = (CRaycastShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"RaycastShader").Get();
	if (nullptr == m_pCSRaycast)
	{
		m_pCSRaycast = new CRaycastShader;
		m_pCSRaycast->CreateComputeShader(L"shader\\raycast.fx", "CS_Raycast");
		CResMgr::GetInst()->AddRes<CComputeShader>(L"RaycastShader", m_pCSRaycast.Get(), true);
	}	

	// ======================
	// ���� ���� ��ǻƮ ���̴�
	// ======================
	m_pCSHeightMap = (CHeightMapShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"HeightMapShader").Get();
	if (nullptr == m_pHeightMap)
	{
		m_pCSHeightMap = new CHeightMapShader;
		m_pCSHeightMap->CreateComputeShader(L"shader\\heightmap.fx", "CS_HeightMap");
		CResMgr::GetInst()->AddRes<CComputeShader>(L"HeightMapShader", m_pCSHeightMap.Get(), true);
	}
	

	// =======================
	// ����ġ ���� ��ǻƮ ���̴�
	// =======================
	m_pCSWeightMap = (CWeightMapShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"WeightMapShader").Get();
	if (nullptr == m_pCSWeightMap)
	{
		m_pCSWeightMap = new CWeightMapShader;
		m_pCSWeightMap->CreateComputeShader(L"shader\\weightmap.fx", "CS_WeightMap");
		CResMgr::GetInst()->AddRes<CComputeShader>(L"WeightMapShader", m_pCSWeightMap.Get(), true);
	}

	m_pCSPreviewMap = (CPreivewMapShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"PreviewMapShader").Get();
	if (nullptr == m_pHeightMap)
	{
		m_pCSPreviewMap = new CPreivewMapShader;
		m_pCSPreviewMap->CreateComputeShader(L"shader\\previewmap.fx", "CS_PreviewMap");
		CResMgr::GetInst()->AddRes<CComputeShader>(L"PreviewMapShader", m_pCSPreviewMap.Get(), true);
	}
}

void CLandScape::CreateTexture()
{
	// ���̸� �ؽ���	
	//m_pHeightMap = CResMgr::GetInst()->Load<CTexture>(L"HeightMap", L"texture\\landscape\\HeightMap_01.jpg");
	m_pHeightMap = CResMgr::GetInst()->CreateTexture( L"HeightMap"
													, 2048, 2048
													, DXGI_FORMAT_R32_FLOAT
													, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS, true);


	m_pPreviewTex = CResMgr::GetInst()->CreateTexture(L"PreviewMap"
														,2048, 2048
														,DXGI_FORMAT_R32_FLOAT
														, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS, true);

	Ptr<CMaterial> pMtrl = GetSharedMaterial(0);
	pMtrl->SetScalarParam(SCALAR_PARAM::INT_0, &m_iXFaceCount);
	pMtrl->SetScalarParam(SCALAR_PARAM::INT_1, &m_iZFaceCount);

	Vec2 vResolution = Vec2(m_pHeightMap->Width(), m_pHeightMap->Height());
	pMtrl->SetScalarParam(SCALAR_PARAM::VEC2_0, &vResolution);
	pMtrl->SetTexParam(TEX_PARAM::TEX_0, m_pHeightMap);
	pMtrl->SetTexParam(TEX_PARAM::TEX_1, m_pPreviewTex);
	

	Ptr<CTexture> pppp = CResMgr::GetInst()->Load<CTexture>(L"texture\\tile\\tilea.dds", L"texture\\tile\\tilea.dds");

	// Ÿ�� �ؽ���(Color, Normal ȥ��, �� 6��)
	//Texture1;
	m_pTileArrTex = CResMgr::GetInst()->Load<CTexture>(L"texture\\tile\\TILE_ARR.dds", L"texture\\tile\\TILE_ARR.dds");


	// �귯�� �ؽ���
	m_pBrushArrTex = CResMgr::GetInst()->Load<CTexture>(L"BrushTex", L"texture\\brush\\BrushArr.dds");	
	//m_pBrushArrTex = CResMgr::GetInst()->Load<CTexture>(L"BrushTex", L"texture\\brush\\Brush_01.png");
	//m_pBrushArrTex = CResMgr::GetInst()->Load<CTexture>(L"BrushTex", L"texture\\brush\\Brush_02.png");


	

	// ����ġ ����
	m_iWeightWidth = 1024;
	m_iWeightHeight = 1024;

	m_pWeightMapBuffer = new CStructuredBuffer;
	m_pWeightMapBuffer->Create(sizeof(tWeight_4), m_iWeightWidth * m_iWeightHeight, SB_TYPE::READ_WRITE, false, nullptr);

	m_pWeightTex = CResMgr::GetInst()->CreateTexture(L"WeightMap"
															  ,m_iWeightWidth
															  ,m_iWeightHeight
														      , DXGI_FORMAT_R8G8B8A8_UNORM
														      , D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS, true);


}

