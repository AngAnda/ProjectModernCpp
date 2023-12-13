#include "Game.h"

using skribbl::IGame;
using skribbl::Game;
using skribbl::Player;
using skribbl::Turn;

Game::Game()
	: m_turn{ nullptr },
	m_playerGuessCount{0}
{
	m_players.reserve(kMaxPlayersNumber);
}

IGame::IGamePtr IGame::Factory()
{
	return std::make_unique<Game>();
}

std::vector<std::pair<std::string, int16_t>> Game::GetLeaderboard()
{
	std::vector<std::pair<std::string, int16_t>> leaderboard;
	leaderboard.reserve(m_players.size());

	for (const auto& player : m_players)
	{
		leaderboard.push_back({ player->GetUsername(), player->GetScore() });
	}

	std::ranges::sort(leaderboard, [](const std::pair<std::string, int16_t>& firstPlayer, const std::pair<std::string, int16_t>& secondPlayer)
		{
			return firstPlayer.second > secondPlayer.second;
		}
	);

	return leaderboard;
}

Game::State Game::GetNextState(State currentState)
{
	return static_cast<State>(static_cast<int>(currentState) + 1);
}

void Game::Start()
{
	m_turn = std::make_unique<Turn>();

	m_state = Game::State::FIRST_ROUND;

	while (m_state != Game::State::GAME_OVER)
	{
		for (Player::PlayerPtr& player : m_players)
		{
			m_playerGuessCount = 0;
			m_turn->Reset();
			while (!m_turn->IsOver())
			{
				if (m_players.size() - 1 == m_playerGuessCount)
					m_turn->AllPlayersGuessed();
			}
			player->UpdateScore(m_turn->ScoreDrawingPlayer());
			ResetPlayersGuessed();
		}

		m_state = GetNextState(m_state);

		if (m_players.size() == 0)
			m_state = Game::State::GAME_OVER;
	}
}

bool Game::AddPlayer(const std::string& name)
{
	if (m_players.size() < kMaxPlayersNumber)
	{
		m_players.push_back(std::make_unique<Player>(name));
		return true;
	}
	return false;
}

std::string Game::GetState() const
{
	switch (m_state)
	{
	case Game::State::WAITING:
		return "Waiting";

	case Game::State::FIRST_ROUND:
		return "First Round";

	case Game::State::SECOND_ROUND:
		return "Second Round";

	case Game::State::THIRD_ROUND:
		return "Third Round";

	case Game::State::FOURTH_ROUND:
		return "Fourth Round";

	case Game::State::GAME_OVER:
		return "Game Over";
	}
}

void Game::RemovePlayer(const std::string& name)
{
	std::erase_if(m_players, [&name, this](const Player::PlayerPtr& player)
		{
			if (player->GetUsername() == name)
			{
				if (player->HasGuessed())
					m_playerGuessCount--;
				return true;
			}
			return false;
		});
}

void skribbl::Game::ResetPlayersGuessed()
{
	for (auto& player : m_players)
	{
		player->setGuessed(false);
	}
}

bool skribbl::Game::VerifyAnswer(const std::string& name, const std::string& answer)
{
	if (m_turn->VerifyGuess(answer)) 
	{
		for (auto& player : m_players)
		{
			if (player->GetUsername() == name) 
			{
				player->UpdateScore(m_turn->ScoreGuessingPlayer());
				m_playerGuessCount++;
				return true;
			}
		}
	}
	return false;
}
