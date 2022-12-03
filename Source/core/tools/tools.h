#include <iostream>
#include "log.h"


#define VKRESULT(function)  if (function != VK_SUCCESS) { std::runtime_error( #function); } 
