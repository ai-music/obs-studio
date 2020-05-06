#include <QMessageBox>
#include <QPainter>
#include "window-basic-main.hpp"

#ifdef _WIN32
#include "win-update/win-update.hpp"
#include "windows.h"
#endif

const int minBtnWidth = 100;
const int minBtnHeight = 100;

void OBSBasic::aiMusicSetupUI() {
	ui->btnConnect->setMinimumWidth(minBtnWidth);
	ui->btnDisconnect->setMinimumWidth(minBtnWidth);

	setupBtn(ui->btnGenre1, "ROCK", ":res/images/1.png");
	setupBtn(ui->btnGenre2, "HIP_HOP", ":res/images/2.png");
	setupBtn(ui->btnGenre3, "JAZZ", ":res/images/6.png");

	setupBtn(ui->btnGenre4, "POP", ":res/images/4.png");
	setupBtn(ui->btnGenre5, "HOUSE", ":res/images/5.png");
	setupBtn(ui->btnGenre6, "AMBIENT", ":res/images/3.png");

	connect(ui->btnGenre1, SIGNAL(clicked()), this, SLOT(DoGenre1()));
	connect(ui->btnGenre2, SIGNAL(clicked()), this, SLOT(DoGenre2()));
	connect(ui->btnGenre3, SIGNAL(clicked()), this, SLOT(DoGenre3()));
	connect(ui->btnGenre4, SIGNAL(clicked()), this, SLOT(DoGenre4()));
	connect(ui->btnGenre5, SIGNAL(clicked()), this, SLOT(DoGenre5()));
	connect(ui->btnGenre6, SIGNAL(clicked()), this, SLOT(DoGenre6()));

	connect(&aiMusicRoom, SIGNAL(signalGotRoom(QString, QString, QString)),
		this, SLOT(OnGotRoom(QString, QString, QString)));

	connect(&aiMusicRoom,
		SIGNAL(signalChangedRoomStyle(QString, QString, QString)), this,
		SLOT(OnChangedRoomStyle(QString, QString, QString)));

	connect(&aiMusicRoom,
		SIGNAL(signalDeletedRoom(QString, QString, QString)), this,
		SLOT(OnDeletedRoom(QString, QString, QString)));

	connect(&aiMusicRoom, SIGNAL(signal_httpError(int, QString, QString)),
		this, SLOT(OnHttpError(int, QString, QString)));

	connect(&aiMusicRoom,
		SIGNAL(signal_failed_Step1_createApplication(QString)), this,
		SLOT(onFailed_Step1_createApplication(QString)));
	connect(&aiMusicRoom,
		SIGNAL(signal_failed_Step2_authenticateApplication(QString)),
		this, SLOT(onFailed_Step2_authenticateApplication(QString)));
	connect(&aiMusicRoom,
		SIGNAL(signal_failed_Step3_createRoomStartPlayback(QString)),
		this,
		SLOT(void onFailed_Step3_createRoomStartPlayback(QString)));
	connect(&aiMusicRoom,
		SIGNAL(signal_failed_Step4_updateRoomChangeStyle(QString)),
		this, SLOT(onFailed_Step4_updateRoomChangeStyle(QString)));
	connect(&aiMusicRoom, SIGNAL(signal_failed_Step5_deleteRoom(QString)),
		this, SLOT(onFailed_Step5_deleteRoom(QString)));

	// File Menu
	auto menuFile = ui->menu_File;
	auto actionShowRecordings = ui->actionShow_Recordings;
	auto actionRemux = ui->actionRemux;
	auto actionSettings = ui->action_Settings;
	auto actionShowSettingsFolder = ui->actionShowSettingsFolder;
	auto actionShowProfileFolder = ui->actionShowProfileFolder;
	auto actionAlwaysOnTop = ui->actionAlwaysOnTop;
	auto actionExit = ui->actionE_xit;
	actionShowRecordings->setVisible(false);
	actionRemux->setVisible(false);
	actionSettings->setVisible(false);
	actionShowSettingsFolder->setVisible(false);
	actionShowProfileFolder->setVisible(false);

	// Edit Menu
	auto menuEdit = ui->menuBasic_MainMenu_Edit;
	auto actionCopySource = ui->actionCopySource;
	auto actionPasteRef = ui->actionPasteRef;
	auto actionPasteDup = ui->actionPasteDup;
	auto actionCopyFilters = ui->actionCopyFilters;
	auto actionPasteFilters = ui->actionPasteFilters;

	menuEdit->menuAction()->setVisible(false);

	// View Menu
	auto menuView = ui->viewMenu;
	auto actionFullScreenInterface = ui->actionFullscreenInterface;
	auto menuViewMenuDosk = ui->viewMenuDocks;
	auto menuViewMenuToolbats = ui->viewMenuToolbars;
	auto actionToggleSourceIcons = ui->toggleSourceIcons;
	auto actionToggleStatusBar = ui->toggleStatusBar;
	auto actionStats = ui->stats;

	menuView->menuAction()->setVisible(false);
	actionFullScreenInterface->setVisible(false);
	menuViewMenuDosk->menuAction()->setVisible(false);
	menuViewMenuToolbats->menuAction()->setVisible(false);
	actionToggleSourceIcons->setVisible(false);
	actionToggleStatusBar->setVisible(false);
	actionStats->setVisible(false);

	// Profile Menu
	auto menuProfile = ui->profileMenu;
	menuProfile->menuAction()->setVisible(false);

	// Scene Menu
	auto menuScene = ui->sceneCollectionMenu;
	menuScene->menuAction()->setVisible(false);

	// Tools Menu
	auto menuTools = ui->menuTools;
	menuTools->menuAction()->setVisible(false);

	// Help Menu
	auto menuHelp = ui->menuBasic_MainMenu_Help;
	auto actionHelpPortal = ui->actionHelpPortal;
	auto actionWebsite = ui->actionWebsite;
	auto actionDiscord = ui->actionDiscord;
	auto menuLogFiles = ui->menuLogFiles;
	auto menuCrashLogs = ui->menuCrashLogs;
	auto actionCheckForUpdates = ui->actionCheckForUpdates;
	auto actionShowAbout = ui->actionShowAbout;
	actionHelpPortal->setVisible(false);
	actionWebsite->setVisible(false);
	actionDiscord->setVisible(false);
	menuLogFiles->menuAction()->setVisible(false);
	menuCrashLogs->menuAction()->setVisible(false);
	actionCheckForUpdates->setVisible(false);

	setUiDisconnected();

	//#ifndef _DEBUG
	//	ui->btnConnect->hide();
	//	ui->btnDisconnect->hide();
	//#endif
}

void OBSBasic::DoConnectToAiMusic() {
	setUiConnecting();
	aiMusicRoom.doConnect();
}

void OBSBasic::DoDisconnectFromAiMusic()
{
	setUiDisconnecting();
	aiMusicRoom.doDisconnect();
}

void OBSBasic::changeSourceName(const QString& /*name*/) {
	//OBSScene scene =
	//	GetCurrentScene(); // Unfortunately, there doesn't seem to be an
	//auto sceneItem =
	//	obs_scene_find_source(scene, name); // obs_source_set_input
	//auto source = obs_sceneitem_get_source(sceneItem);
	//if (sceneItem) {
	//	obs_source_set_name(source, name2);
	//} else {
	//	sceneItem = obs_scene_find_source(scene, name2);
	//	source = obs_sceneitem_get_source(sceneItem);
	//	obs_source_set_name(source, name1);
	//}
}

void OBSBasic::DoMusicStyle(const QString &nusicStyle) {
	aiMusicRoom.doChangeMusicStyle(nusicStyle);
}

void OBSBasic::DoGenre1() {
	DoMusicStyle(ui->btnGenre1->toolTip());
}

void OBSBasic::DoGenre2() {
	DoMusicStyle(ui->btnGenre2->toolTip());
}

void OBSBasic::DoGenre3() {
	DoMusicStyle(ui->btnGenre3->toolTip());
}

void OBSBasic::DoGenre4() {
	DoMusicStyle(ui->btnGenre4->toolTip());
}

void OBSBasic::DoGenre5() {
	DoMusicStyle(ui->btnGenre5->toolTip());
}

void OBSBasic::DoGenre6() {
	DoMusicStyle(ui->btnGenre6->toolTip());
}

void OBSBasic::OnHttpError(int step, QString url, QString curlError) {
	QString message = "HTTP request to " + url + " errored:\n\n" + curlError + "\n\nwhilst attempting step " +
			  QString::number(step) + ".";
	QMessageBox msgBox(QMessageBox::Warning, "AiMusic HTTP Error", message,
			   QMessageBox::Ok);

	if (step <= 3) {
		setUiDisconnected();
	}
	msgBox.exec();
} 

void OBSBasic::onFailed_Step1_createApplication(QString data)
{
	// Currently this will always fail - so don't bother to notify user.
	//QMessageBox msgBox(QMessageBox::Warning, "Failed to create application", data, QMessageBox::Ok);
	//msgBox.exec();
	setUiDisconnected();
}

void OBSBasic::onFailed_Step2_authenticateApplication(QString data)
{
	QMessageBox msgBox(QMessageBox::Warning,
			   "Failed to authenticate application", data,
			   QMessageBox::Ok);
	msgBox.exec();
	setUiDisconnected();
}

void OBSBasic::onFailed_Step3_createRoomStartPlayback(QString data)
{
	QMessageBox msgBox(QMessageBox::Warning, "Failed to create room", data,
			   QMessageBox::Ok);
	msgBox.exec();
	setUiDisconnected();
}

void OBSBasic::onFailed_Step4_updateRoomChangeStyle(QString data)
{
	QMessageBox msgBox(QMessageBox::Warning, "Failed to change room style",
			   data, QMessageBox::Ok);
	msgBox.exec();
}

void OBSBasic::onFailed_Step5_deleteRoom(QString data)
{
	QMessageBox msgBox(QMessageBox::Warning, "Failed to delete room.", data,
			   QMessageBox::Ok);
	msgBox.exec();
	setUiConnected();
}

void OBSBasic::OnGotRoom(QString roomId, QString musicStyle, QString streamUri) {
	// Load in a correctly configured scene for the stream.

	char szSceneFolder[512];

	if (GetConfigPath(szSceneFolder, sizeof(szSceneFolder),
			  "obs-studio/basic/scenes") < 0) {
		return;
	}

	const QString sceneFolder(szSceneFolder);
	const QString newFilePath = aiMusicProfileWrangler.makeOssiaSceneFile(
		sceneFolder, streamUri);
	if (!newFilePath.isEmpty()) {
		Load(newFilePath.toStdString().data());
	}
	setUiConnected();
	setUiForMusicStyle(musicStyle);
	//QString message("You are now connected to AiMusic playing " + musicStyle + ".");

	//QMessageBox msgBox(QMessageBox::Information, "AI Music Conneted", message, QMessageBox::Ok);
	//msgBox.exec();
}

void OBSBasic::OnDeletedRoom(QString,  QString, QString)
{
	setUiDisconnected();
	

	//QMessageBox msgBox(QMessageBox::Warning, "AiMusic HTTP Disconnected", "You are not disconnected from AiMusic.",
	//		   QMessageBox::Ok);
	//msgBox.exec();
}

void OBSBasic::OnChangedRoomStyle(QString roomId, QString musicStyle, QString streamUri) {
	setUiForMusicStyle(musicStyle);

	//QMessageBox msgBox(QMessageBox::Warning, "AiMusic Music Style Changing",
	//		   "Music style changing to " + musicStyle,
	//		   QMessageBox::Ok);
	//msgBox.exec();
}

void OBSBasic::setUiConnecting() {
	ui->btnConnect->setEnabled(false);
	ui->btnDisconnect->setEnabled(false);
	ui->btnGenre1->setEnabled(false);
	ui->btnGenre2->setEnabled(false);
	ui->btnGenre3->setEnabled(false);
	ui->btnGenre4->setEnabled(false);
	ui->btnGenre5->setEnabled(false);
	ui->btnGenre6->setEnabled(false);
}

void OBSBasic::setUiConnected() {
	ui->btnConnect->setEnabled(false);
	ui->btnDisconnect->setEnabled(true);
	ui->btnGenre1->setEnabled(true);
	ui->btnGenre2->setEnabled(true);
	ui->btnGenre3->setEnabled(true);
	ui->btnGenre4->setEnabled(true);
	ui->btnGenre5->setEnabled(true);
	ui->btnGenre6->setEnabled(true);
}

void OBSBasic::setUiDisconnecting() {
	ui->btnConnect->setEnabled(false);
	ui->btnDisconnect->setEnabled(false);
	ui->btnGenre1->setEnabled(false);
	ui->btnGenre2->setEnabled(false);
	ui->btnGenre3->setEnabled(false);
	ui->btnGenre4->setEnabled(false);
	ui->btnGenre5->setEnabled(false);
	ui->btnGenre6->setEnabled(false);
}

void OBSBasic::setUiDisconnected() {
	ui->btnConnect->setEnabled(true);
	ui->btnDisconnect->setEnabled(false);
	ui->btnGenre1->setEnabled(false);
	ui->btnGenre2->setEnabled(false);
	ui->btnGenre3->setEnabled(false);
	ui->btnGenre4->setEnabled(false);
	ui->btnGenre5->setEnabled(false);
	ui->btnGenre6->setEnabled(false);
	setUiForMusicStyle("");
}

void OBSBasic::setUiForMusicStyle(const QString &musicStyle) {
	highlightButton(ui->btnGenre1, musicStyle);
	highlightButton(ui->btnGenre2, musicStyle);
	highlightButton(ui->btnGenre3, musicStyle);
	highlightButton(ui->btnGenre4, musicStyle);
	highlightButton(ui->btnGenre5, musicStyle);
	highlightButton(ui->btnGenre6, musicStyle);
}

void OBSBasic::highlightButton(QPushButton *btn, const QString &musicStyle) {
	if( btn->toolTip() == musicStyle ){
		btn->setStyleSheet("background-color:LightGray;");
	} else {
		btn->setStyleSheet("background-color:Gray;");
	}
}

void OBSBasic::setupBtn(QPushButton *btn, const QString &musicStyle, const QString imgUrl)
{
	btn->setToolTip(musicStyle);
	btn->setMinimumWidth(minBtnWidth);
	btn->setMinimumHeight(minBtnHeight);
	btn->setStyleSheet("background-color:Gray;");

	QFont font("Arial", minBtnHeight / 5);
	font.setBold(true);
	QFontMetrics fontMetrics(font);
	const int fontWidth = fontMetrics.horizontalAdvance(musicStyle);
	const int fontHeight = fontMetrics.capHeight();

	QPixmap pixmap(imgUrl);
	QPainter painter(&pixmap);

	painter.setFont(font);
	painter.setPen(QColor(200, 100, 0));

	QString btnText(musicStyle);
	btnText.replace('_', ' ');
	painter.drawText(QPoint(10,
			10+fontHeight), btnText);

	QIcon ButtonIcon(pixmap);
	btn->setIcon(ButtonIcon);
}
