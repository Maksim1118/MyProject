#pragma once

#include "PieceServer.h"

#include <unordered_map>
#include <memory>
#include <string>

namespace Server
{
	class PiecesList
	{
	public:
		PiecesList(Bot* parent, IRegistrator* iRegistrator);
		std::unordered_map<std::string, std::shared_ptr<Piece>>& getList();
		size_t getSize();
		void update(int diff, const sf::Vector2f & targetP);
		void pieceToSides();
		void mergePices();
		void remove(const std::string& id);
		void add(const std::string& id, std::shared_ptr<Piece> piece);
	private:
		std::shared_ptr<Piece> merge2Pieces(std::shared_ptr<Piece>& p1, std::shared_ptr<Piece>& p2);
		bool canBeMerged(std::shared_ptr<Piece>& p1, std::shared_ptr<Piece>& p2);
	private:
		std::unordered_map<std::string, std::shared_ptr<Piece>> pieces;
		Bot* m_parent;
		IRegistrator* m_registrator;
	};
}