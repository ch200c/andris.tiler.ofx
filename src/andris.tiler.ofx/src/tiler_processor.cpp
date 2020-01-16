
#include "andris.tiler.ofx/tiler_processor.h"
#include "andris.ofx.util/util.h"
#include "opencv2/core.hpp"    // cv::flip
#include "opencv2/imgproc.hpp" // cv::resize
#include <cmath>               // std::ceil, std::lround

tiler_processor::tiler_processor(OFX::ImageEffect &effect) noexcept
    : OFX::ImageProcessor(effect) {}

void tiler_processor::set_source(OFX::Image *value) noexcept { source = value; }

void tiler_processor::multiThreadProcessImages(OfxRectI window) {
  auto source_data{source->getPixelData()};
  if (!source_data) {
    return;
  }

  auto source_pixel_depth{source->getPixelDepth()};
  auto source_pixel_components{source->getPixelComponents()};
  auto source_type{util::mat_type(source_pixel_components, source_pixel_depth)};

  if (source_type == -1) {
    return;
  }

  auto source_bounds{source->getBounds()};
  auto source_row_bytes{static_cast<size_t>(source->getRowBytes())};
  auto source_mat{cv::Mat(source_bounds.y2 - source_bounds.y1,
                          source_bounds.x2 - source_bounds.x1, source_type,
                          source_data, source_row_bytes)};

  auto output_data{_dstImg->getPixelData()};
  if (!output_data) {
    return;
  }

  auto output_pixel_depth{_dstImg->getPixelDepth()};
  auto output_pixel_components{_dstImg->getPixelComponents()};
  auto output_type{util::mat_type(output_pixel_components, output_pixel_depth)};

  if (output_type == -1) {
    return;
  }

  auto output_bounds{_dstImg->getBounds()};
  auto output_row_bytes{static_cast<size_t>(_dstImg->getRowBytes())};
  auto output_mat{cv::Mat{output_bounds.y2 - output_bounds.y1,
                          output_bounds.x2 - output_bounds.x1, output_type,
                          output_data, output_row_bytes}};

  auto resized_mat{cv::Mat{}};

  cv::resize(source_mat.clone(), resized_mat, cv::Size{}, horizontal_scale,
             vertical_scale);

  tile(window, resized_mat, output_mat);

  cv::flip(output_mat, output_mat, 0);
}

void tiler_processor::set_center(OfxPointD const &value) noexcept {
  center = value;
}

void tiler_processor::set_horizontal_scale(double value) noexcept {
  horizontal_scale = value;
}

void tiler_processor::set_vertical_scale(double value) noexcept {
  vertical_scale = value;
}

void tiler_processor::tile(OfxRectI const &window, cv::Mat const &input,
                           cv::Mat &output) {
  auto window_width{window.x2 - window.x1};
  auto window_height{window.y2 - window.y1};

  auto x_offset{std::lround(center.x * window_width)};
  auto y_offset{std::lround((1.0 - center.y) * window_height)};

  auto x_diff{x_offset - util::nearest_even(input.cols / 2.0) - window.x1};
  auto x_iterations_count{
      static_cast<int>(std::ceil(x_diff / static_cast<double>(input.cols)))};
  auto x{x_diff - (x_iterations_count * input.cols)};

  auto y_diff{y_offset - util::nearest_even(input.rows / 2.0) - window.y1};
  auto y_iterations_count{
      static_cast<int>(std::ceil(y_diff / static_cast<double>(input.rows)))};
  auto y{y_diff - (y_iterations_count * input.rows)};

  auto window_rect{cv::Rect{window.x1, window.y1, window_width, window_height}};

  for (auto i = y; i < window.y2; i += input.rows) {
    if (_effect.abort()) {
      return;
    }

    for (auto j = x; j < window.x2; j += input.cols) {
      auto tile_rect{cv::Rect{j, i, input.cols, input.rows}};

      auto output_roi = tile_rect & window_rect;

      auto width_diff{tile_rect.width - output_roi.width};
      auto height_diff{tile_rect.height - output_roi.height};

      auto clipped_input_roi{cv::Rect{0, 0, input.cols, input.rows}};

      if (j < window.x1) {
        clipped_input_roi.x = width_diff;
        clipped_input_roi.width -= width_diff;
      }

      if (j + input.cols > window.x2) {
        clipped_input_roi.width = output_roi.width;
      }

      if (i < window.y1) {
        clipped_input_roi.y = height_diff;
        clipped_input_roi.height -= height_diff;
      }

      if (i + input.rows > window.y2) {
        clipped_input_roi.height = output_roi.height;
      }

      input(clipped_input_roi).copyTo(output(output_roi));
    }
  }
}
