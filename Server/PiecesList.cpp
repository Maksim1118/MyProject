#include "PiecesList.h"

#include "IRegistrator.h"

#include "Constants.h"
#include "BotServer.h"

#include <unordered_set>

using namespace std;
using namespace sf;

namespace Server
{

	std::unordered_map<std::string, std::shared_ptr<Piece>>& PiecesList::getList()
	{
		return pieces;
	}

	size_t PiecesList::getSize()
	{
		return pieces.size();
	}

	void PiecesList::update(int diff, const sf::Vector2f& targetP)
	{
		Vector2f centroid(0.f, 0.f);
		for (auto& piece : pieces)
		{
			piece.second->setTarget(targetP);
			piece.second->TimeElapsed(diff);
			centroid += piece.second->getCenter();
		}
		centroid /= (float)pieces.size();

		m_parent->setCenter(centroid);

		mergePices();
		pieceToSides();
	}

	PiecesList::PiecesList(Bot* parent, IRegistrator* iRegistrator)
		:m_parent(parent), m_registrator(iRegistrator)
	{
		checkRegistrator(m_registrator);
	}

	void PiecesList::pieceToSides()
	{
		unordered_map<Piece*, sf::Vector2f> movements;

		for (auto& piece : pieces)
		{
			Vector2f center = piece.second->getCenter();
			
			for (auto & piece2 : pieces)
			{
				if (&piece == &piece2)
				{
					continue;
				}
				
				PieceState ab1 = piece.second->getAbility();
				PieceState ab2 = piece2.second->getAbility();

				bool canHustle1 = (ab1 == PieceState::None || ab1 == PieceState::Merge);
				bool canHustle2 = (ab2 == PieceState::None || ab2 == PieceState::Merge);
				bool atLeastOnNone = (ab1 == PieceState::None || ab2 == PieceState::None);

				if (canHustle1 && canHustle2 && atLeastOnNone)
				{
					float dist = GetCyclicDist(piece.second->getCenter(), piece2.second->getCenter(),MapConstants::mapWidth, MapConstants::mapHeight);
					float sumRadius = piece.second->getRadius() + piece2.second->getRadius();
					if (dist < sumRadius)
					{
						Vector2f dir = getIdentityVector(piece2.second->getCenter() - center);
						float overlap = (sumRadius - dist) / 2.f;

						movements[piece.second.get()] += -dir * overlap;
						movements[piece2.second.get()] += dir * overlap;
					}
				}
			}
		}
		for (auto& move : movements)
		{
			move.first->Move(move.second);
		}
	}
	void PiecesList::mergePices()
	{

		bool mergedThisRound = false;

		do
		{
			mergedThisRound = false;
			unordered_set<string> toRemove;
			vector<shared_ptr<Piece>> toInsert;

			for (auto& piece : pieces)
			{
				if (toRemove.count(piece.first)) continue;

				for (auto& piece2 : pieces)
				{
					if (toRemove.count(piece2.first)) continue;

					if (&piece == &piece2)
					{
						continue;
					}

					auto mergedPiece = merge2Pieces(piece.second, piece2.second);

					if (mergedPiece)
					{
						toInsert.push_back(mergedPiece);

						toRemove.insert(piece.first);
						toRemove.insert(piece2.first);
						mergedThisRound = true;
						break;
					}
				}
			}

			for (const auto& id : toRemove)
			{
				remove(id);
			}

			for (auto& newPiece : toInsert)
			{
				pieces[newPiece->getID()] = newPiece;
			}

		} while (mergedThisRound);

	}

	shared_ptr<Piece> PiecesList::merge2Pieces(std::shared_ptr<Piece>& p1, std::shared_ptr<Piece>& p2)
	{
		if (p1->getAbility() != PieceState::Merge || p2->getAbility() != PieceState::Merge) return nullptr;
		if (!canBeMerged(p1, p2)) return nullptr;

		vector<Vector2f> centers{ p1->getCenter(), p2->getCenter() };
		Vector2f generalCenter = findCyclicCenter(centers);
		float m1 = p1->getMass();
		float m2 = p2->getMass();
		float generalMass = m1 + m2;
		Vector2f generalV = (p1->getSpeed() * m1 + p2->getSpeed() * m2) / generalMass;

		return 	make_shared<Piece>(m_registrator, m_parent,generalMass, generalV, generalCenter, PieceState::Merge);
	}

	bool PiecesList::canBeMerged(std::shared_ptr<Piece>& p1, std::shared_ptr<Piece>& p2)
	{
		float dist = GetCyclicDist(p1->getCenter(), p2->getCenter(),MapConstants::mapWidth, MapConstants::mapHeight);
		return dist <= min(p1->getRadius(), p2->getRadius());
	}
	void PiecesList::remove(const std::string& id)
	{
		auto it = pieces.find(id);
		if (it != pieces.end())
		{
			m_registrator->unregisterAuxiliary(it->second);
			pieces.erase(it);
		}
	}
	void PiecesList::add(const std::string& id, std::shared_ptr<Piece> piece)
	{
		m_registrator->registerAuxiliary(piece);
		pieces[id] = piece;
	}

}