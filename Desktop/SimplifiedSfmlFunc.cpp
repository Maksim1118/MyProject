#include "SimplifiedSfmlFunc.h"

namespace SFML_SIMPLIFIED
{
	using namespace sf;
	
	void initSprite(Sprite& outSprite, const Texture& texture, const Vector2f& origin, const Vector2f& pos, const Vector2f& size)
	{
		outSprite.setTexture(texture);
		float scaleX = size.x / texture.getSize().x;
		float scaleY = size.y / texture.getSize().y;
		outSprite.setScale(scaleX, scaleY);
		outSprite.setOrigin(origin);
		outSprite.setPosition(pos);
	}

	Color lerpColor(const Color& a, const Color& b, const float t)
	{
		return Color(
		static_cast<Uint8>(a.r + t * (b.r - a.r)),
		static_cast<Uint8>(a.g + t * (b.g - a.g)),
		static_cast<Uint8>(a.b + t * (b.b - a.b)),
		static_cast<Uint8>(a.a+ t * (b.a - a.a))
		);
	}

	float approachWithClamp(float cur, float target, int diff, float speed, float minVal, float maxVal)
	{
		cur += (target - cur) * (float)diff * speed;

		if (cur > maxVal) {
			cur = maxVal;
		}
		else if (cur < minVal) {
			cur = minVal;
		}
		return cur;
	}

}
