#pragma once
#include<iostream>
#include"instance.h"
namespace OE {
	class RHI
	{
		
	public:
		~RHI();



	private:
		VulkanInstance _instance{};
	
	};


}
