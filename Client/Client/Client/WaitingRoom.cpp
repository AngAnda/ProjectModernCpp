﻿#include "WaitingRoom.h"

WaitingRoom::WaitingRoom(QWidget* parent, Routing* m_rt)
	: QWidget(parent)
	, ui(new Ui::WaitingRoomClass())
	, m_refreshCount{ 0 }
	, m_rt{ m_rt }
	,m_admin{ User() }
{
	ui->setupUi(this);
	connect(ui->startGame, &QPushButton::clicked, this, &WaitingRoom::OnStartBtnPushed);
	connect(ui->pushButton, &QPushButton::clicked, this, &WaitingRoom::AddCustomWord);
}


void WaitingRoom::OnStartBtnPushed() 
{
	if (m_rt->GetPlayers().size() <= 1)
		QMessageBox::warning(this, "Waiting Room", "Not enough players to start the game.");
	else
		if (CheckAllCustomWordsAdded() == false)
			QMessageBox::warning(this, "Custom Word", "Not all players have added their custom word yet. Please wait until everyone has finished their input before starting the game.");
	else
		emit goToGame();
}

WaitingRoom::~WaitingRoom()
{
	delete ui;
}

void WaitingRoom::SetRoomCode(QString lobbyCode)
{
	ui->label_2->setText(lobbyCode);
}

void WaitingRoom::addUserToRoom(User& user)
{
	UserWidget* userWidget = new UserWidget(user.getUsername(), this);

	QListWidgetItem* item = new QListWidgetItem();
	item->setSizeHint(userWidget->sizeHint());
	ui->listWidget->addItem(item);
	ui->listWidget->setItemWidget(item, userWidget);
}

void WaitingRoom::UpdatePlayerList(std::vector<User>& players)
{
	ui->listWidget->clear();
	bool admin = false;
	for (auto& player : players) 
	{
		if (admin == false)
		{
			admin = true;
			player.setAdmin();
			m_admin = player;
		}
		addUserToRoom(player);
		if (QString::fromLatin1(m_rt->GetPlayerName().data()) == m_admin.getUsername())
			ui->startGame->setVisible(true);
		else ui->startGame->setVisible(false);
	}
	
}

void WaitingRoom::FetchPlayers()
{
	std::vector<User> players = m_rt->GetPlayers();
	UpdatePlayerList(players);
}

void WaitingRoom::paintEvent(QPaintEvent* e)
{
	if (m_refreshCount % kRefreshRate == 0)
	{
		FetchPlayers();
		if(m_rt->GetRound() != kWaitingState)
			emit goToGame();
	}
	m_refreshCount++;
}

void WaitingRoom::AddCustomWord()
{
	QString word = ui->lineEdit->text();
	if (!word.isEmpty()) 
	{
		QMessageBox::information(this, "Custom Word", word);
		ui->lineEdit->setDisabled(true);
		ui->pushButton->setDisabled(true);
		m_rt->SendCustomWord(word.toLatin1().data());
		std::string username = m_rt->GetPlayerName();
		m_customWords[username] = true;
	}
}

bool WaitingRoom::CheckAllCustomWordsAdded() {
	for (const auto& pair : m_customWords) {
		if (!pair.second) {
			return false;
		}
	}
	return true;
}