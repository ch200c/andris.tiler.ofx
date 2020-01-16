#pragma once

namespace tiler {
#ifdef __cpp_inline_variables
inline constexpr auto param_proportional{"Proportional"};
inline constexpr auto param_horizontal_scale{"HorizontalScale"};
inline constexpr auto param_vertical_scale{"VerticalScale"};
inline constexpr auto param_center{"Center"};

inline constexpr auto param_horizontal_scale_default{0.5};
inline constexpr auto param_vertical_scale_default{0.5};
inline constexpr auto param_center_x_default{0.5};
inline constexpr auto param_center_y_default{0.5};
#else
constexpr static auto param_proportional{"Proportional"};
constexpr static auto param_horizontal_scale{"HorizontalScale"};
constexpr static auto param_vertical_scale{"VerticalScale"};
constexpr static auto param_center{"Center"};

constexpr static auto param_horizontal_scale_default{0.5};
constexpr static auto param_vertical_scale_default{0.5};
constexpr static auto param_center_x_default{0.5};
constexpr static auto param_center_y_default{0.5};
#endif
} // namespace tiler