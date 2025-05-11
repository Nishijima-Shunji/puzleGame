#include "Object.h"

void Object::Init(const wchar_t* imgname, int sx, int sy, int dir) {
	splitX = sx;
	splitY = sy;
	if (dir == 0) {
	/*	vertexList[1].u = 1.0f / splitX;
		vertexList[2].v = 1.0f / splitY;
		vertexList[3].u = 1.0f / splitX;
		vertexList[3].v = 1.0f / splitY;*/

		vertexList[0].u = 0.0f;
		vertexList[0].v = 0.0f;
		vertexList[1].u = 1.0f / splitX;
		vertexList[1].v = 0.0f;
		vertexList[2].u = 0.0f;
		vertexList[2].v = 1.0f / splitY;
		vertexList[3].u = 1.0f / splitX;
		vertexList[3].v = 1.0f / splitY;
	}
	else if (dir == 1) {
		vertexList[0].u = 1.0f / splitX;
		vertexList[0].v = 0.0f;
		vertexList[1].u = 0.0f;
		vertexList[1].v = 0.0f;
		vertexList[2].u = 1.0f / splitX;
		vertexList[2].v = 1.0f / splitY;
		vertexList[3].u = 0.0f;
		vertexList[3].v = 1.0f / splitY;
	}

	// ���_�o�b�t�@���쐬����
	// �����_�o�b�t�@��VRAM�ɒ��_�f�[�^��u�����߂̋@�\
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeof(vertexList);// �m�ۂ���o�b�t�@�T�C�Y���w��
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;// ���_�o�b�t�@�쐬���w��
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subResourceData;
	subResourceData.pSysMem = vertexList;// VRAM�ɑ���f�[�^���w��
	subResourceData.SysMemPitch = 0;
	subResourceData.SysMemSlicePitch = 0;

	HRESULT hr = g_pDevice->CreateBuffer(&bufferDesc, &subResourceData, &m_pVertexBuffer);

	//�e�N�X�`���ǂݍ���
	hr = DirectX::CreateWICTextureFromFile(g_pDevice, imgname, NULL, &m_pTextureView);
	if (FAILED(hr)) {
		MessageBoxA(NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[", MB_ICONERROR | MB_OK);
		return;
	}
}

void Object::Draw() {
	UINT strides = sizeof(Vertex);
	UINT offsets = 0;
	g_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &strides, &offsets);

	//�e�N�X�`�����s�N�Z���V�F�[�_�[�ɓn��
	g_pDeviceContext->PSSetShaderResources(0, 1, &m_pTextureView);

	//�萔�o�b�t�@���X�V
	ConstBuffer cb;

	//�v���W�F�N�V�����ϊ��s����쐬
	cb.matrixProj = DirectX::XMMatrixOrthographicLH(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 3.0f);
	cb.matrixProj - DirectX::XMMatrixTranspose(cb.matrixProj);

	//���[���h�ϊ��s��̍쐬
	//���I�u�W�F�N�g�̈ʒu�E�傫���E�������w��
	cb.matrixWorld = DirectX::XMMatrixScaling(size.x, size.y, size.z);
	cb.matrixWorld *= DirectX::XMMatrixRotationZ(angle * 3.14f / 180);
	cb.matrixWorld *= DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	cb.matrixWorld = DirectX::XMMatrixTranspose(cb.matrixWorld);

	//UV�A�j���[�V�����̍s��쐬
	float u = (float)numU / splitX;
	float v = (float)numV / splitY;
	cb.matrixTex = DirectX::XMMatrixTranslation(u, v, 0.0f);
	cb.matrixTex = DirectX::XMMatrixTranspose(cb.matrixTex);

	//���_�J���[�̃f�[�^���쐬
	cb.color = color;

	//�s����V�F�[�_�[�ɓn��
	g_pDeviceContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);

	g_pDeviceContext->Draw(4, 0); // �`�施��
}

void Object::Uninit() {
	SAFE_RELEASE(m_pTextureView);
	SAFE_RELEASE(m_pVertexBuffer);
}

void Object::SetPos(float x, float y, float z) {
	//���W���Z�b�g����
	pos.x = x;
	pos.y = y;
	pos.z = z;
}

void Object::SetSize(float x, float y, float z) {
	//�傫�����Z�b�g����
	size.x = x;
	size.y = y;
	size.z = z;
}

void Object::SetAngle(float a) {
	//�p�x���Z�b�g����
	angle = a;
}

void Object::SetColor(float r, float g, float b, float a) {
	//�F���Z�b�g����
	color.x = r;
	color.y = g;
	color.z = b;
	color.w = a;
}

void Object::SetAlpha(float a){
	color.w = a;
}


DirectX::XMFLOAT3 Object::GetPos(void) {
	return pos;
}

DirectX::XMFLOAT3 Object::GetSize(void) {
	return size;
}

float Object::GetAngle(void) {
	return angle;
}

DirectX::XMFLOAT4 Object::GetColor(void) {
	return color;
}

