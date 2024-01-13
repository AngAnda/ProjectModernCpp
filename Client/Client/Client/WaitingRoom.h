#pragma once

#include <QWidget>
#include <QTimer.h>
#include <QListWidgetItem>
#include <QPaintEvent>
#include <QMessageBox>

#include "Routing.h"
#include "ui_WaitingRoom.h"
#include "User.h"
#include "UserWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class WaitingRoomClass; };
QT_END_NAMESPACE

class WaitingRoom : public QWidget
{
	Q_OBJECT

public:
	WaitingRoom(QWidget *parent = nullptr, Routing* m_rt = nullptr);
	~WaitingRoom();
	void SetRoomCode(QString lobbyCode);
	void addUserToRoom(User& user);
	void UpdatePlayerList(std::vector<User>& players);
	void FetchPlayers();
	void paintEvent(QPaintEvent* e) override;

private slots:
	void OnStartBtnPushed();
	void AddCustomWord();

signals:
	void goToGame();

private:
	Ui::WaitingRoomClass *ui;
	Routing* m_rt;

	static const uint8_t kRefreshRate{ 2 };
	uint m_refreshCount;

	User m_admin;
	const QString kWaitingState{ "Waiting" };

	std::unordered_map<std::string, bool> m_customWords;

private:
	bool ValidateInput();
	bool CheckAllCustomWordsAdded();
};
