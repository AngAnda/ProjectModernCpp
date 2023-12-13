#include "Database.h"

/*skribbl::Database::Database(Storage& db)
	: m_db{ db }
{}*/

bool skribbl::Database::Initialize(const std::string& fileName)
{
	m_db.sync_schema();
	auto initWordsCount = m_db.count<Words>();
	if (initWordsCount == 0)
		PopulateStorage(fileName);

	auto wordsCount = m_db.count<Words>();
	return wordsCount != 0;
}

bool skribbl::Database::CheckUserExists(const std::string& username)
{
	auto user = m_db.get_all<User>(sql::where(sql::c(&User::m_username) == username));
	return !user.empty();
}

void skribbl::Database::CreateNewUser(const std::string& username, const std::string& password)
{
	m_db.insert(User{ -1, username, password, 0 });
}

std::optional<User> skribbl::Database::AuthenticateUser(const std::string& username, const std::string& password)
{
    auto users = m_db.get_all<User>(sql::where(sql::c(&User::m_username) == username));
    if (!users.empty())
    {
        const auto& user = users.front();
        if (user.m_password == password)
        {
            return user;
        }
    }
    return std::nullopt;
}

void skribbl::Database::AddGameHistory(int playerId, int gameId, int points)
{
	m_db.insert(GameHistory{ -1, playerId, gameId, points });
}

std::vector<int> skribbl::Database::GetIdWords()
{
	return m_db.select(&Words::m_id);
}

void skribbl::Database::PopulateStorage(const std::string& fileName)
{
	std::vector <Words> words;
	std::ifstream fin(fileName);
	if (!fin.is_open()) 
	{
		throw std::exception("File was unable to be oppened"); // TODO: treat this exception
	}
	else 
	{
		std::string word;
		while (fin >> word)
		{
			words.push_back({ -1, word });
		}
		fin.close();
		m_db.insert_range(words.begin(), words.end());
	}
}
