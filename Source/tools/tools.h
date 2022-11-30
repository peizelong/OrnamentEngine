#include <iostream>
#include "log.h"

#define OE_VALIDATION_DEBUG  //开启vulkan验证层开关

#define VKRESULT(function)    if (function != VK_SUCCESS) { std::runtime_error( #function); }