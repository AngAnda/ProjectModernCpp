#pragma once

#include <crow.h>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <ranges>
#include "IGame.h"

import player;
import turn;

namespace skribbl
{

	class Game : public IGame
	{
	public:
		enum class State
		{
			WAITING,
			FIRST_ROUND,
			SECOND_ROUND,
			THIRD_ROUND,
			FOURTH_ROUND,
			GAME_OVER


		};

	public:
		static const uint8_t kMaxPlayersNumber = 8;

	public:
		Game();
		~Game() = default;
		Game(const Game& otherGame) = delete;
		Game& operator=(const Game& otherGame) = delete;
		Game(Game&& otherGame) noexcept = default;
		Game& operator=(Game&& otherGame) noexcept = default;

		void start(crow::SimpleApp& app) override;
		void setUrl(std::string lobbyCode) override;
		void removePlayer(const std::string& name) override;

		bool addPlayer(const std::string& name) override;
		std::vector<std::shared_ptr<Player>> leaderboard() override;
		std::string getState() const override;

		State getNextState(State currentState);

	private:
		std::vector<std::shared_ptr<Player>> m_players;
		std::shared_ptr<Turn> m_turn;
		State m_state;
		std::string m_url;
	};
}