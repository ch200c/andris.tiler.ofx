#include "pch.h"
#include "tiler_plugin.h"
#include "tiler_processor.h"

tiler_plugin::tiler_plugin(
	OfxImageEffectHandle handle) noexcept :
	OFX::ImageEffect(handle)
{
	if (is_last_proportional)
	{
		vertical_scale->setValue(last_horizontal_scale);
	}
}

void tiler_plugin::render(
	OFX::RenderArguments const& args) noexcept
{
	auto output_image{ std::unique_ptr<OFX::Image>(
		output->fetchImage(args.time)) };

	if (!output_image)
	{
		return;
	}

	auto source_image{ std::unique_ptr<OFX::Image>(
		source->fetchImage(args.time)) };

	if (!source_image)
	{
		return;
	}

	auto processor{ tiler_processor(*this) };

	processor.setRenderWindow(args.renderWindow);
	processor.set_source(source_image.get());
	processor.setDstImg(output_image.get());

	auto center_x{ 0.0 };
	auto center_y{ 0.0 };
	center->getValueAtTime(args.time, center_x, center_y);
	processor.set_center({center_x, center_y});
	processor.set_horizontal_scale(horizontal_scale->getValueAtTime(args.time));
	processor.set_vertical_scale(vertical_scale->getValueAtTime(args.time));

	if (host_description->hostName.find("com.sonycreativesoftware.vegas") != std::string::npos)
	{
		processor.set_y_flipped(true);
	}

	processor.process();
}

bool tiler_plugin::isIdentity(
	OFX::IsIdentityArguments const& args,
	OFX::Clip*& identityClip,
	double& identityTime) noexcept
{
	auto _horizontal_scale{ horizontal_scale->getValueAtTime(args.time) };
	auto _vertical_scale{ vertical_scale->getValueAtTime(args.time) };
	auto center_x{ 0.0 };
	auto center_y{ 0.0 };
	center->getValueAtTime(args.time, center_x, center_y);

	auto result { 
		_horizontal_scale == 1.0 &&
		_vertical_scale == 1.0 &&
		center_x == 0.5 &&
		center_y == 0.5 };

	if (result)
	{
		identityClip = source;
		identityTime = args.time;
	}

	return result;
}

void tiler_plugin::changedParam(
	OFX::InstanceChangedArgs const& args,
	std::string const& paramName) noexcept
{
	switch (args.reason)
	{
		case OFX::eChangeUserEdit:
		{
			auto is_proportional{ proportional->getValueAtTime(args.time) };
			auto _horizontal_scale{ horizontal_scale->getValueAtTime(args.time) };
			auto _vertical_scale{ vertical_scale->getValueAtTime(args.time) };

			if (is_proportional == is_last_proportional &&
				_horizontal_scale == last_horizontal_scale &&
				_vertical_scale == last_vertical_scale)
			{
				break;
			}

			if (paramName == tiler::param_proportional)
			{
				if (is_proportional)
				{
					vertical_scale->setValue(_horizontal_scale);
				}
			}

			else if (paramName == tiler::param_horizontal_scale)
			{
				if (is_proportional)
				{
					if (horizontal_scale->getIsAnimating())
					{
						// Once you enable animating, a keyframe will be created at 
						// the start of the effect. It will have a value of the paramter 
						// before enabling the animation. This is done to have a reference
						// value to animate from. If we have the proportional flag set,
						// we need to create a keyframe at the start for the other scaling.

						auto start_horizontal_scale{ horizontal_scale->getValueAtTime(0) };
						vertical_scale->setValueAtTime(0, start_horizontal_scale);

						if (last_horizontal_scale != _horizontal_scale)
						{
							vertical_scale->setValueAtTime(args.time, _horizontal_scale);
						}
					}

					vertical_scale->setValue(_horizontal_scale);
				}
			}

			else if (paramName == tiler::param_vertical_scale)
			{
				if (is_proportional)
				{
					if (vertical_scale->getIsAnimating())
					{
						auto start_vertical_scale{ vertical_scale->getValueAtTime(0) };
						horizontal_scale->setValueAtTime(0, start_vertical_scale);

						if (last_vertical_scale != _vertical_scale)
						{
							horizontal_scale->setValueAtTime(args.time, _vertical_scale);
						}
					}

					horizontal_scale->setValue(_vertical_scale);
				}
			}

			is_last_proportional = is_proportional;
			last_horizontal_scale = _horizontal_scale;
			last_vertical_scale = _vertical_scale;

			break;
		}
		case OFX::eChangePluginEdit:
		{
			break;
		}
		case OFX::eChangeTime:
		{
			break;
		}
		default:
		{
			break;
		}
	}
}
