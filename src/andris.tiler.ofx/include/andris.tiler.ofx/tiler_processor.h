#pragma once
#include "constants.h"
#include "ofxCore.h"        // OfxRectI, OfxPointD
#include "ofxsProcessing.h" // OFX::ImageProcessor

namespace OFX {
class ImageEffect;
class Image;
} // namespace OFX

namespace cv {
class Mat;
}

class tiler_processor : public OFX::ImageProcessor {
public:
  tiler_processor(OFX::ImageEffect &effect) noexcept;

  /*
  tiler_processor& operator=(tiler_processor const&) = delete;
  tiler_processor& operator=(tiler_processor&&) = delete;
  tiler_processor(tiler_processor const&) = delete;
  */

  void multiThreadProcessImages(OfxRectI window) override;

  void set_source(OFX::Image *value) noexcept;

  void set_center(OfxPointD const &value) noexcept;

  void set_horizontal_scale(double value) noexcept;

  void set_vertical_scale(double value) noexcept;

private:
  void tile(OfxRectI const &window, cv::Mat const &input, cv::Mat &output);

  OFX::Image *source{nullptr};
  double horizontal_scale{tiler::param_horizontal_scale_default};
  double vertical_scale{tiler::param_vertical_scale_default};
  OfxPointD center{tiler::param_center_x_default,
                   tiler::param_center_y_default};
};
