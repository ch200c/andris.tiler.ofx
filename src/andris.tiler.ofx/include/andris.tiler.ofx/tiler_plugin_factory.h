#pragma once

#include "ofxImageEffect.h" // OfxImageEffectHandle
#include "ofxsImageEffect.h" // OFX::PluginFactoryHelper, OFX::ImageEffectDescriptor, OFX::ContextEnum

class tiler_plugin_factory
    : public OFX::PluginFactoryHelper<tiler_plugin_factory> {
public:
  tiler_plugin_factory() noexcept;

  void describe(OFX::ImageEffectDescriptor &desc) noexcept override;

  void describeInContext(OFX::ImageEffectDescriptor &desc,
                         OFX::ContextEnum context) noexcept override;

  OFX::ImageEffect *createInstance(OfxImageEffectHandle handle,
                                   OFX::ContextEnum context) noexcept override;
};
