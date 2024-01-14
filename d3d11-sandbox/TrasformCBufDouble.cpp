#include "TrasformCBufDouble.h"

Bind::TransformCBufDouble::TransformCBufDouble(Graphics& gfx, const Drawable& parent, UINT slotV, UINT slotP)
	:
	TransformCbuf(gfx, parent, slotV)
{
	if (!pPcbf)
	{
		pPcbf = std::make_unique<PixelConstantBuffer<Transforms>>(gfx, slotP);
	}
}

void Bind::TransformCBufDouble::Bind(Graphics& gfx) noexcept
{
	const auto tf = GetTransforms(gfx);
	TransformCbuf::UpdateBindImpl(gfx, tf);
	UpdateBindImpl(gfx, tf);
}

void Bind::TransformCBufDouble::UpdateBindImpl(Graphics& gfx, const Transforms& tf) noexcept
{
	pPcbf->Update(gfx, tf);
	pPcbf->Bind(gfx);
}

std::unique_ptr<Bind::PixelConstantBuffer<Bind::TransformCbuf::Transforms>> Bind::TransformCBufDouble::pPcbf;
