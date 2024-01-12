#include "TestPlane.h"
#include "Plane.h"
#include "BindableCommon.h"

TestPlane::TestPlane(Graphics& gfx, float size)
{
	using namespace Bind;
	namespace dx = DirectX;

	auto model = Plane::Make();
	model.Transform(dx::XMMatrixScaling(size, size, 1.0f));
	const auto geometryTag = "$plane." + std::to_string(size);

	AddBind(VertexBuffer::Resolve(gfx, geometryTag, model.vertices));
	AddBind(IndexBuffer::Resolve(gfx, geometryTag, model.indices));

	AddBind(Texture::Resolve(gfx, "images\\brickwall.jpg"));

	auto pvs = VertexShader::Resolve(gfx, "PhongVS.cso");
	auto pvsbv = pvs->GetBytecode();
	AddBind(std::move(pvs));

	AddBind(PixelShader::Resolve(gfx, "PhongPS.cso"));

	struct PMaterialConst
	{
		float specularIntensity = 0.1f;
		float speculatPower = 20.0f;
		float padding[2];
	} pmc;

	AddBind(PixelConstantBuffer<PMaterialConst>::Resolve(gfx, pmc, 1u));
	
	AddBind(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbv));

	AddBind(Topology::Resolve(gfx, D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_shared<TransformCbuf>(gfx, *this));
}

void TestPlane::SetPos(DirectX::XMFLOAT3 pos) noexcept
{
	this->pos = pos;
}

void TestPlane::SetRotation(float roll, float pitch, float yaw) noexcept
{
	this->roll = roll;
	this->pitch = pitch;
	this->yaw = yaw;
}

DirectX::XMMATRIX TestPlane::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) * DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
}
