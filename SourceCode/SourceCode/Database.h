#pragma once
#include <string>
#include <sqlite_orm/sqlite_orm.h>
#include <optional>
#include <vector>
#include <fstream>

namespace sql = sqlite_orm;

struct User {
	int m_id;
	std::string m_username;
	std::string m_password;
	int m_nrGamesPlayed;
};

struct GameHistory {
	int m_id;
	int m_id_player;
	int m_id_game;
	int m_points;
};

struct Games {
	int m_id;
	std::string m_date; //TODO: change to date
};

struct Words {
	int m_id;
	std::string m_word;
};


namespace skribbl
{
	inline auto CreateDatabase(const std::string& filename) 
	{
		return sql::make_storage(
			filename,
			sql::make_table(
				"Users",
				sql::make_column("id", &User::m_id, sql::primary_key().autoincrement()),
				sql::make_column("username", &User::m_username),
				sql::make_column("password", &User::m_password),
				sql::make_column("nrGamesPlayed", &User::m_nrGamesPlayed)
			),
			sql::make_table(
				"Game",
				sql::make_column("id", &Games::m_id, sql::primary_key().autoincrement()),
				sql::make_column("date", &Games::m_date)
			),
			sql::make_table(
				"GamesHistory",
				sql::make_column("id", &GameHistory::m_id, sql::primary_key().autoincrement()),
				sql::make_column("user", &GameHistory::m_id_player),
				sql::make_column("game", &GameHistory::m_id_game),
				sql::make_column("points", &GameHistory::m_points),
				sql::foreign_key(&GameHistory::m_id_player).references(&User::m_id),
				sql::foreign_key(&GameHistory::m_id_game).references(&Games::m_id)
			),
			sql::make_table(
				"Words",
				sql::make_column("id", &Words::m_id, sql::primary_key().autoincrement()),
				sql::make_column("word", &Words::m_word)
			)
		);
	}

	using Storage = decltype(CreateDatabase(""));

	class Database
	{
	public:
		//Database();
		//~Database();

		bool Initialize(const std::string& fileName);

		bool CheckUserExists(const std::string& username);
		void CreateNewUser(const std::string& username, const std::string& password);

		std::optional<User> AuthenticateUser(const std::string& username, const std::string& password);
		void AddGameHistory(int playerId, int gameId, int points);

		std::vector<int> GetIdWords();
		std::string GetWord(int id);

	private:
		void PopulateStorage(const std::string& fileName);

		const std::string kDbFile{ "database.sqlite" };

		Storage m_db = CreateDatabase(kDbFile);
	};
}
