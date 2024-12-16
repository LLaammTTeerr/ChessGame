#pragma once

#include "settingsMenu.h"
#include "svgbutton.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>
#include <QSoundEffect>
#include <QUrl>
#include <QSlider>
#include <QListWidget>
#include <QDir>
#include <QFileInfo>


class SettingsMenu : public QGraphicsScene {
	Q_OBJECT

public:
	SettingsMenu(QObject* parent = nullptr);
	float musicVolume = 0.5, soundEffectVolume = 1.0;
signals:
	void backActivated();
	void soundEffectVolumeChanged();

private:
	QGraphicsTextItem* txtSettings, * txtMusic, * txtSoundEffect;
	QSoundEffect* backgroundMusic;
	QSlider* backgroundMusicSlider, * soundEffectSlider;
	SvgButton* btnBack;

	QListWidget listMusic;
	void initListMusic(const QString &path);
};
