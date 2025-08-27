#pragma once
#include <SFML/Graphics.hpp>

#include <string>

enum class DrawType
{
	single,
	allTiles,
};

using namespace sf;
using namespace std;
class Objects
{
public:
	Objects(Vector2f center, float radius, string id);
	void setRadius(float newRadius);
	void setMass(const float mass);
	float getMass() const;
	float getRadius() const;
	Vector2f getCenter() const;
	virtual void setCenter(Vector2f newCenter) = 0;
	virtual void draw(RenderWindow& window) const = 0;
	virtual void TimeElapsed(int diff);
	virtual ~Objects()=0;
	virtual void shiftPos(float offsetX = 0, float offsetY = 0) = 0;
	string getID() const;
	bool m_IsLive;
	bool m_isMarked;
protected:
	float _radius;
	float m_mass;
	string id;
	Vector2f Center;
};

