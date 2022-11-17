#include <iostream>
#include "log.h"

#define OE_VALIDATION_DEBUG  //开启验证层开关

#define VKRESULT(function)  { const VkResult result = function; if (result != VK_SUCCESS) { std::runtime_error( #function); }}