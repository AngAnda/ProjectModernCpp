#pragma once
#include <qstring.h>

using GameHistory = std::pair<std::string, std::string>;

class User
{
public: 

    User();
    void setUsername(const std::string& username);
    void setLobbyCode(const std::string& lobbyCode);
    void setAdmin();

    QString getUsername() const;
    std::optional<QString> getLobbyCode();

    void SetGameHistory(const std::vector<GameHistory>& history);

    std::vector<GameHistory> GetGameHistory();

private:
    QString m_username;
    bool m_isAdmin;
    std::optional<QString> m_lobbyCode;
    std::vector<GameHistory> m_gamesHistory;
};

