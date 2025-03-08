#include "Bot.h"
#include "ResourñeManager.h"

Color botsArrColor[] =
{ Color(250, 255, 0),
	Color(255, 0, 0),
	Color(0, 199, 255),
	Color(100, 250, 50),
	Color(165, 72, 38),
	Color(168, 17, 255)
};



Bot::Bot(Vector2f center, float radius, int Col, string text, int id) : MoveObject(Vector2f(0, 0), radius, id)
{
	name.setString(text);
	name.setColor(Color::White);
	name.setFont(*rec.font);
	name.setCharacterSize(20 + radius / 5);

	name.setPosition(center.x - name.getLocalBounds().width / 2, center.y - name.getLocalBounds().height / 2);
	Center = center;
	_shape.setRadius(radius);
	_shape.setFillColor(botsArrColor[Col]);
	_shape.setPosition(center.x - radius, center.y - radius);
	splitted = false;
}

void Bot::TimeElapsed(int diff)
{
	if (splitted == true)
	{
		pieceToSides();
	}
	MoveObject::TimeElapsed(diff);
}

void Bot::pieceToSides()
{
	for (auto& piece : pieces)
	{
		Vector2f center = piece->getCenter();

		for (auto& piece2 : pieces)
		{
			float lenPiece2 = GetLen(piece->getCenter(), piece2->getCenter());
			float sumRadius = piece->getRadius() + piece2->getRadius();
			if (lenPiece2 < sumRadius)
			{
				Vector2f toPiece21 = getIdentityVector(piece2->getCenter() - center);
				piece2->setCenter(piece2->getCenter() + toPiece21 * (sumRadius - lenPiece2) / (float)2);
				piece->setCenter(piece->getCenter() - toPiece21 * (sumRadius - lenPiece2) / (float)2);
			}

		}
	}
}


void Bot::setPosMouse(float x, float y)
{
	Mouse = Vector2f(x, y);
}

void Bot::isSplitted(bool splite)
{
	splitted = splite;
}

void Bot::draw(RenderWindow& window)
{
	_shape.setRadius(_radius);
	if (!splitted)
	{
		_shape.setPosition(Center.x - _shape.getRadius(), Center.y - _shape.getRadius());
		window.draw(_shape);
	}
	name.setPosition(Center.x - name.getLocalBounds().width / 2, Center.y - name.getLocalBounds().height / 2);
	window.draw(name);
}