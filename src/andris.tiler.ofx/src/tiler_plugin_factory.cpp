#include "andris.tiler.ofx/tiler_plugin_factory.h"
#include "andris.tiler.ofx/tiler_plugin.h" // tiler_plugin
#include "andris.tiler.ofx/constants.h"

tiler_plugin_factory::tiler_plugin_factory() noexcept
    : OFX::PluginFactoryHelper<tiler_plugin_factory>("andris.tiler", 1,
                                                     1) {}

void tiler_plugin_factory::describe(OFX::ImageEffectDescriptor &desc) noexcept {
  desc.setLabels("Tiler", "Tiler", "Tiler");
  desc.setPluginGrouping("AN DRIS");
  desc.addSupportedContext(OFX::eContextFilter);
  desc.addSupportedBitDepth(OFX::eBitDepthUByte);

  desc.setRenderTwiceAlways(false);
}

void tiler_plugin_factory::describeInContext(
    OFX::ImageEffectDescriptor &desc, OFX::ContextEnum context) noexcept {
  switch (context) {
  case OFX::eContextFilter: {
    auto source{desc.defineClip(kOfxImageEffectSimpleSourceClipName)};
    source->addSupportedComponent(OFX::ePixelComponentRGBA);

    auto output{desc.defineClip(kOfxImageEffectOutputClipName)};
    output->addSupportedComponent(OFX::ePixelComponentRGBA);

    break;
  }
  default: { break; }
  }

  auto page{desc.definePageParam("MainPage")};
  auto scale_group{desc.defineGroupParam("ScaleGroup")};
  scale_group->setLabel("Scale");
  scale_group->setOpen(true);

  {
    auto parameter{desc.defineBooleanParam(tiler::param_proportional)};
    parameter->setLabel("Proportional");
    parameter->setDefault(true);
    parameter->setParent(*scale_group);
  }

  {
    auto parameter{desc.defineDoubleParam(tiler::param_horizontal_scale)};
    parameter->setLabel("Horizontal");
    parameter->setRange(0.001, 1.0);
    parameter->setDisplayRange(0.001, 1.0);
    parameter->setIncrement(0.02);
    parameter->setDigits(3);
    parameter->setDefault(tiler::param_horizontal_scale_default);
    parameter->setParent(*scale_group);
  }

  {
    auto parameter{desc.defineDoubleParam(tiler::param_vertical_scale)};
    parameter->setLabel("Vertical");
    parameter->setRange(0.001, 1.0);
    parameter->setDisplayRange(0.001, 1.0);
    parameter->setIncrement(0.02);
    parameter->setDigits(3);
    parameter->setDefault(tiler::param_vertical_scale_default);
    parameter->setParent(*scale_group);
  }

  page->addChild(*scale_group);

  {
    auto parameter{desc.defineDouble2DParam(tiler::param_center)};

    parameter->setLabel("Center");
    parameter->setDefault(tiler::param_center_x_default,
                          tiler::param_center_x_default);
    parameter->setDigits(3);

    page->addChild(*parameter);
  }
}

OFX::ImageEffect *
tiler_plugin_factory::createInstance(OfxImageEffectHandle handle,
                                     OFX::ContextEnum context) noexcept {
  return new tiler_plugin(handle);
}

void OFX::Plugin::getPluginIDs(OFX::PluginFactoryArray &factories) {
  static tiler_plugin_factory factory;
  factories.push_back(&factory);
}