#include "WaitingRoom.h"

WaitingRoom::WaitingRoom(QWidget* parent, Routing* m_rt)
	: QWidget(parent)
	, ui(new Ui::WaitingRoomClass())
	, m_refreshCount{ 0 }
	, m_rt{ m_rt }
{
	ui->setupUi(this);

	connect(ui->startGame, &QPushButton::clicked, this, &WaitingRoom::ChangeToGamePage);
}


void WaitingRoom::ChangeToGamePage() {
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

void WaitingRoom::addUserToRoom(const QString& user)
{
	UserWidget* userWidget = new UserWidget(user, this);

	QListWidgetItem* item = new QListWidgetItem();
	item->setSizeHint(userWidget->sizeHint());
	ui->listWidget->addItem(item);
	ui->listWidget->setItemWidget(item, userWidget);
}

void WaitingRoom::UpdatePlayerList(const std::vector<QString>& players)
{
	ui->listWidget->clear();
	for (const auto& player : players) {
		addUserToRoom(player);
	}
}

void WaitingRoom::FetchPlayers()
{
	std::vector<QString> players = m_rt->GetPlayers();
	UpdatePlayerList(players);
}

void WaitingRoom::paintEvent(QPaintEvent* e)
{
	if (m_refreshCount % kRefreshRate == 0)
	{
		FetchPlayers();
	}
	m_refreshCount++;
}