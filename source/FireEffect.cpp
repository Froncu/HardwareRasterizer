#include "pch.h"
#include "FireEffect.h"

#pragma region Constructors/Destructor
FireEffect::FireEffect(ID3D11Device* const pDevice) :
	DefaultEffect(pDevice, "Resources/Fire.fx")
{
}
#pragma endregion Constructors/Destructor