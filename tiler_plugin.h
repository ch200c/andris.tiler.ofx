#pragma once

#include <string>				// std::string
#include "ofxsImageEffect.h"	// OFX::ImageEffect, OFX::RenderArguments, OFX::IsIdentityArguments, OFX::Clip, OFX::InstanceChangedArgs
#include "ofxImageEffect.h"		// OfxImageEffectHandle
#include "constants.h"

namespace OFX
{
	class BooleanParam;
	class DoubleParam;
	class Double2DParam;
}

class tiler_plugin : public OFX::ImageEffect
{
public:
	tiler_plugin(
		OfxImageEffectHandle handle) noexcept;

	void render(
		OFX::RenderArguments const& args) noexcept override;

	bool isIdentity(
		OFX::IsIdentityArguments const& args,
		OFX::Clip*& identityClip,
		double& identityTime) noexcept override;

	void changedParam(
		OFX::InstanceChangedArgs const& args,
		std::string const& paramName) noexcept override;

private:
	OFX::Clip* source{ fetchClip(kOfxImageEffectSimpleSourceClipName) };
	OFX::Clip* output{ fetchClip(kOfxImageEffectOutputClipName) };
	OFX::BooleanParam* proportional{ fetchBooleanParam(tiler::param_proportional) };
	OFX::DoubleParam* horizontal_scale{ fetchDoubleParam(tiler::param_horizontal_scale) };
	OFX::DoubleParam* vertical_scale{ fetchDoubleParam(tiler::param_vertical_scale) };
	OFX::Double2DParam* center{ fetchDouble2DParam(tiler::param_center) };
	bool is_last_proportional{ proportional->getValue() };
	double last_horizontal_scale{ horizontal_scale->getValue() };
	double last_vertical_scale{ vertical_scale->getValue() };
};



