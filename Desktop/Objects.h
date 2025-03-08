#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;
class Objects
{
public:
	Objects(Vector2f center, float radius, int id);
	Objects();
	void setRadius(float newRadius);
	float getRadius() const;
	Vector2f getCenter() const;
	void setCenter(Vector2f newCenter);
	virtual void draw(RenderWindow& window) = 0;
	virtual void TimeElapsed(int diff);
	virtual ~Objects()=0;
	int getID();
protected:
	float _radius;
	int id;
	Vector2f Center;
private:
	static int lastId;
};

