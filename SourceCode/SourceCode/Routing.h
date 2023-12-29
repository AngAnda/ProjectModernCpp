#pragma once
#include <crow.h>
#include <random>
#include <vector>
#include <unordered_map>
#include "IGame.h"
#include "Database.h"
#include <mutex>

import uniquerandom;

namespace skribbl
{
	class Routing
	{
	public:
		//Routing() = default;
		Routing(std::shared_ptr<skribbl::Database> db);
		~Routing() = default;
		Routing(const Routing& other) = delete;
		Routing& operator=(const Routing& other) = delete;
		Routing(Routing&& other) noexcept = delete;
		Routing& operator=(Routing&& other) noexcept = delete;
		
		void Run();

		static constexpr uint8_t klobbyCodeLength = 6;
		static constexpr uint8_t kmaxGamesSupported = 8;

	private:
		crow::response JoinLobby(const crow::request& req);
		crow::response StartGame(const crow::request& req);
		crow::response CreateLobby(const crow::request& req);
		crow::response GetGameLeaderboard(const crow::request& req);
		crow::response RemovePlayer(const crow::request& req);
		crow::response ProcessAnswer(const crow::request& req);
		crow::response ProcessDrawing(const crow::request& req);
		

		crow::response GetWord(const crow::request& req);
		crow::response GetDrawing(const crow::request& req);
		crow::response GetAnswers(const crow::request& req);
		crow::response GetDrawingPlayer(const crow::request& req);
		crow::response GetGameState(const crow::request& req);
		crow::response Login(const crow::request& req);
		crow::response Register(const crow::request& req);
		crow::response GetTime(const crow::request& req);
		crow::response GetHint(const crow::request& req);
		crow::response GetGamePlayers(const crow::request& req);

	private:
		crow::SimpleApp m_app;
		std::unordered_map<std::string, IGame::IGamePtr> m_games;
		std::shared_ptr<skribbl::Database> m_db;
		UniqueRandom<std::string> m_ur;
		std::mutex m_mutex;
	};
}