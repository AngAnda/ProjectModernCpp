#include <vector>
#include <string>
#include <memory>
#include "Database.h"
import uniquerandom;

namespace skribbl
{
	class WordHandler
	{
	public:
		using WordHandlerPtr = std::unique_ptr<WordHandler>;

	public:
		WordHandler(skribbl::Database& db);
		~WordHandler() = default; 
		WordHandler(const WordHandler& wg)=delete; 
		WordHandler& operator=(const WordHandler&) = delete;
		WordHandler(WordHandler&& wg)noexcept = default;
		WordHandler& operator=(WordHandler&&)noexcept = default;

		void Reset();
		void ResetToInitialState();
		
		std::string GetWord();
		std::vector<uint8_t> GenerateHint();

		void AddCustomWord(const std::string& word);
		void UpdateCustomWordScore(int16_t score);
		uint8_t GetNumberCustomWord() const;
		bool AreCustomWordsLeft() const;
		void AddCustomWordsToDatabase(uint8_t playersNumber);


	private:
		std::string m_currentWord;
		UniqueRandom<int>::UniqueRandomPtr m_ur;
		skribbl::Database& m_db;
		std::vector<std::pair<std::string, int16_t>> m_customWords;
		std::vector<uint8_t> m_hint;
		int m_customWordPos;
	};
}