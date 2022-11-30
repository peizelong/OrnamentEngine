#include <optional>
#include "vulkan/vulkan.h"
namespace OE {

	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;

		bool isComplete() {
			return graphicsFamily.has_value();
		}
	};


}