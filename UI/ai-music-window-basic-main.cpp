#include <QMessageBox>
#include <QPainter>
#include <QDir>
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

	setupBtn(ui->btnGenre1, AiMusicRoom::getMusicStyle(0), ":res/images/1.png");
	setupBtn(ui->btnGenre2, AiMusicRoom::getMusicStyle(1),
		 ":res/images/2.png");
	setupBtn(ui->btnGenre3, AiMusicRoom::getMusicStyle(2),
		 ":res/images/3.png");

	setupBtn(ui->btnGenre4, AiMusicRoom::getMusicStyle(3),
		 ":res/images/4.png");
	setupBtn(ui->btnGenre5, AiMusicRoom::getMusicStyle(4),
		 ":res/images/5.png");
	setupBtn(ui->btnGenre6, AiMusicRoom::getMusicStyle(5),
		 ":res/images/6.png");

	// File Menu
	//auto menuFile = ui->menu_File;
	auto actionShowRecordings = ui->actionShow_Recordings;
	auto actionRemux = ui->actionRemux;
	auto actionSettings = ui->action_Settings;
	auto actionShowSettingsFolder = ui->actionShowSettingsFolder;
	auto actionShowProfileFolder = ui->actionShowProfileFolder;
	auto actionAlwaysOnTop = ui->actionAlwaysOnTop;
	//auto actionExit = ui->actionE_xit;
	actionShowRecordings->setVisible(false);
	actionRemux->setVisible(false);
	actionSettings->setVisible(false);
	actionShowSettingsFolder->setVisible(false);
	actionShowProfileFolder->setVisible(false);
	actionAlwaysOnTop->setVisible(false);
	//auto actionConnect = menuFile->addAction("Connect");
	//auto actionDisconnect = menuFile->addAction("Disconnect");

	// Edit Menu
	auto menuEdit = ui->menuBasic_MainMenu_Edit;
	//auto actionCopySource = ui->actionCopySource;
	//auto actionPasteRef = ui->actionPasteRef;
	//auto actionPasteDup = ui->actionPasteDup;
	//auto actionCopyFilters = ui->actionCopyFilters;
	//auto actionPasteFilters = ui->actionPasteFilters;

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
	//auto menuHelp = ui->menuBasic_MainMenu_Help;
	auto actionHelpPortal = ui->actionHelpPortal;
	auto actionWebsite = ui->actionWebsite;
	auto actionDiscord = ui->actionDiscord;
	auto menuLogFiles = ui->menuLogFiles;
	auto menuCrashLogs = ui->menuCrashLogs;
	auto actionCheckForUpdates = ui->actionCheckForUpdates;
	//auto actionShowAbout = ui->actionShowAbout;
	actionHelpPortal->setVisible(false);
	actionWebsite->setVisible(false);
	actionDiscord->setVisible(false);
	menuLogFiles->menuAction()->setVisible(false);
	menuCrashLogs->menuAction()->setVisible(false);
	actionCheckForUpdates->setVisible(false);

	connect(ui->btnConnect, SIGNAL(clicked()), this,
		SLOT(DoConnectToAiMusic()));
	connect(ui->btnDisconnect, SIGNAL(clicked()), this,
		SLOT(DoDisconnectFromAiMusic()));

	//connect(actionConnect, SIGNAL(triggered()), this,
	//	SLOT(DoConnectToAiMusic()));
	//connect(actionDisconnect, SIGNAL(triggered()), this,
	//	SLOT(DoDisconnectFromAiMusic()));

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

bool OBSBasic::changeSourceUri(const QString &streamUri)
{
	// See documentation at
	// https://obsproject.com/docs/reference-sources.html#c.obs_source_t
	// https://obsproject.com/docs/reference-settings.html#c.obs_data_t
	OBSScene scene =
		GetCurrentScene(); // Unfortunately, there doesn't seem to be an
	obs_sceneitem_t* sceneItem =
		obs_scene_find_source(scene, "Ossia Music" ); // obs_source_set_input
	obs_source_t *source = obs_sceneitem_get_source(sceneItem);
	obs_data_t *data = obs_source_get_settings(source);
	const char *json = obs_data_get_json(data);
	if (json == nullptr) {
		return false;
	}

	const QString strJson(json);

	const auto aiMusicIndex = strJson.indexOf("aimusic.services");
	if (aiMusicIndex <= 0) {
		return false;
	}

	const auto quoteBefore = strJson.lastIndexOf('"', aiMusicIndex);
	const auto quoteAfter = strJson.indexOf('"', aiMusicIndex);
	const QString newJson =
		strJson.left(quoteBefore + 1) + streamUri +
			  strJson.right(strJson.length() - quoteAfter);

	obs_data_t *newData =
		obs_data_create_from_json( newJson.toStdString().data() );
	obs_source_update(source, newData);
	return true;
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
	// setUiConnected(); - can't do this - we may be deleting the room on the way out.
}

void OBSBasic::OnGotRoom(QString roomId, QString musicStyle, QString streamUri)
{
	auto changedSourceUri = changeSourceUri(streamUri);
	if (!changedSourceUri) {
		// If we weren't able to modify the Ai Music stream in the 
		// scene, then we prepare and load a default scene.
		char szSceneFolder[512];
		GetConfigPath(szSceneFolder, sizeof(szSceneFolder),
			      "obs-studio/basic/scenes");

		const QString sceneFolder(szSceneFolder);
		if (sceneFolder.isEmpty()) {
			return;
		}

		const QString sceneFilePath =
				aiMusicProfileWrangler.makeOssiaSceneFile(
					sceneFolder, streamUri);

		if (sceneFilePath.isEmpty()) {
			return;
		}

		Load(sceneFilePath.toStdString().data());
		RefreshSceneCollections();
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
	auto toolTip = btn->toolTip().toUpper();
	auto upperStyle = musicStyle.toUpper();

	if (toolTip == upperStyle) {
		btn->setStyleSheet("background-color:LightGray;");
		return;
	}

	if (AiMusicRoom::getStreamNameFromMusicStyle(toolTip) == upperStyle) {
		btn->setStyleSheet("background-color:LightGray;");
		return;
	}

	btn->setStyleSheet("background-color:Gray;");
}

void OBSBasic::setupBtn(QPushButton *btn, const QString &musicStyle, const QString imgUrl)
{
	btn->setToolTip(musicStyle);

	QPixmap pixmap(imgUrl);
	auto buttonWidth = pixmap.width();
	auto buttonHeight = pixmap.height();
	btn->resize(buttonWidth, buttonHeight);

	btn->setMinimumWidth(minBtnWidth);
	btn->setMinimumHeight(minBtnHeight);
	btn->setStyleSheet("background-color:Gray;");

	QFont font("Arial", buttonWidth / 8);
	font.setBold(true);
	QFontMetrics fontMetrics(font);
	const int fontWidth = fontMetrics.horizontalAdvance(musicStyle);
	const int fontHeight = fontMetrics.capHeight();

	QPainter painter(&pixmap);

	painter.setFont(font);
	painter.setPen(QColor(200, 100, 0));

	const int margin = (buttonWidth / 20);
	QString btnText(musicStyle);
	btnText.replace('_', ' ');
	QStringList btnTexts = btnText.split(' ');
	auto xPos = margin;
	auto yPos = margin + fontHeight;
	for (const auto &text : btnTexts) {
		painter.drawText(QPoint(xPos, yPos), text);
		yPos += fontHeight + margin;
	}

	

	QIcon ButtonIcon(pixmap);
	btn->setIcon(ButtonIcon);
}
