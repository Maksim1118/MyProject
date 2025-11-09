#pragma once

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <stdexcept>

namespace Server
{
	class Objects;

	class IRegistrator
	{
	public:
		virtual bool spawn(float radius, sf::Vector2f& futurePosition) = 0;
		virtual void registerAuxiliary(std::shared_ptr<Objects> obj) = 0; //rTree insert;
		virtual void unregisterAuxiliary(std::shared_ptr<Objects> obj) = 0; //rTree delete;
		virtual void moveObj(std::shared_ptr<Objects> obj) = 0; //rTree update;
		virtual std::vector<std::shared_ptr<Objects>> getNearObjects(const sf::FloatRect& area) const = 0;
	};

	inline void checkRegistrator(const IRegistrator* reg)
	{
#ifndef NDEBUG
		if (!reg)
		{
			throw std::runtime_error("iRegistrator cannot be nullptr");
		}
#endif
	}
}