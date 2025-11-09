#pragma once

#include "nlohmann\json.hpp"

namespace Server
{
	class Piece;
	class Feed;
	class Bot;
	class Hero;
	class Thorn;
	class Objects;

	class JsonSerialize
	{
	public:
		JsonSerialize() = default;
		nlohmann::json toJson(Objects& obj) const;
	private:
		nlohmann::json oneTimeDataInJson(Bot& bot) const;
		nlohmann::json staticDataInJson(Bot& bot) const;
		nlohmann::json persistentDataInJson(Bot& bot) const;

		nlohmann::json oneTimeDataInJson(Hero& hero) const;
		nlohmann::json staticDataInJson(Hero& hero) const;
		nlohmann::json persistentDataInJson(Hero& hero) const;

		nlohmann::json oneTimeDataInJson(Piece& piece) const;
		nlohmann::json staticDataInJson(Piece& piece) const;
		nlohmann::json persistentDataInJson(Piece& piece) const;

		nlohmann::json oneTimeDataInJson(Feed& feed) const;
		nlohmann::json staticDataInJson(Feed& feed) const;
		nlohmann::json persistentDataInJson(Feed& feed) const;

		nlohmann::json oneTimeDataInJson(Thorn& thorn) const;
		nlohmann::json staticDataInJson(Thorn& thorn) const;
		nlohmann::json persistentDataInJson(Thorn& thorn) const;

	};
}