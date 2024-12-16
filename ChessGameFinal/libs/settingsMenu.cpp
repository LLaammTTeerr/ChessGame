#include "settingsMenu.h"
#include <iostream>

SettingsMenu::SettingsMenu(QObject* parent):
    QGraphicsScene(parent),
    txtSettings(nullptr),
    txtSoundEffect(nullptr),
    txtMusic(nullptr),
    soundEffectSlider(nullptr),
    backgroundMusic(nullptr),
    backgroundMusicSlider(nullptr),
    btnBack(nullptr)
{
	setSceneRect(0, 0, 1000, 600);
	setBackgroundBrush(QBrush(QColor("#2B3467")));
	txtSettings = new QGraphicsTextItem("SETTINGS");
	txtSettings->setFont(QFont("Arial", 40, QFont::Bold));
	txtSettings->setDefaultTextColor(Qt::white);
	txtSettings->setPos(500 - txtSettings->boundingRect().width() / 2, 30);
	addItem(txtSettings);

	txtSoundEffect = new QGraphicsTextItem("Sound Effect");
	txtSoundEffect->setFont(QFont("Arial", 14, QFont::Bold));
	txtSoundEffect->setDefaultTextColor(Qt::white);
	txtSoundEffect->setPos(70, 150);
	addItem(txtSoundEffect);

	txtMusic = new QGraphicsTextItem("Music");
	txtMusic->setFont(QFont("Arial", 14, QFont::Bold));
	txtMusic->setDefaultTextColor(Qt::white);
	txtMusic->setPos(70, 200);
	addItem(txtMusic);	

	soundEffectSlider = new QSlider(Qt::Horizontal);
	soundEffectSlider->setRange(0, 100);
	soundEffectSlider->setValue(100);
	soundEffectSlider->setGeometry(250, 150, 300, txtSoundEffect->boundingRect().height());
	addWidget(soundEffectSlider);
	connect(soundEffectSlider, &QSlider::valueChanged, [this](int value) {
		soundEffectVolume = value / 100.0;
		emit soundEffectVolumeChanged();
	});

	backgroundMusic = new QSoundEffect(this);
	backgroundMusic->setSource(QUrl::fromLocalFile("./resources/backgroundmusic/Canon_in_D_Pachelbel.wav"));
	backgroundMusic->setVolume(0.5);
	backgroundMusic->setLoopCount(QSoundEffect::Infinite);
	backgroundMusic->play();

	backgroundMusicSlider = new QSlider(Qt::Horizontal);
	backgroundMusicSlider->setRange(0, 100);
	backgroundMusicSlider->setValue(50);
	backgroundMusicSlider->setGeometry(250, 200, 300, txtMusic->boundingRect().height());
	addWidget(backgroundMusicSlider);
	connect(backgroundMusicSlider, &QSlider::valueChanged, [this](int value) {
		musicVolume = value / 100.0;
		backgroundMusic->setVolume(musicVolume);
	});	

    soundEffectSlider->setStyleSheet(R"(
        QSlider {
            background: transparent;
        }
        QSlider::groove:horizontal {
            border: 1px solid #bbb;
            background: #eee;
            height: 10px;
            border-radius: 4px;
        }
        QSlider::handle:horizontal {
            background: #3498db;
            border: 1px solid #5c5c5c;
            width: 20px;
            height: 20px;
            margin: -5px 0;
            border-radius: 10px;
        }
        QSlider::handle:horizontal:hover {
            background: #1abc9c;
        }
        QSlider::sub-page:horizontal {
            background: #66ccff;
            border: 1px solid #4a4a4a;
            height: 10px;
            border-radius: 4px;
        }
        QSlider::add-page:horizontal {
            background: #ccc;
            border: 1px solid #4a4a4a;
            height: 10px;
            border-radius: 4px;
        }
    )");

	backgroundMusicSlider->setStyleSheet(R"(
        QSlider {
            background: transparent;
        }
        QSlider::groove:horizontal {
            border: 1px solid #bbb;
            background: #eee;
            height: 10px;
            border-radius: 4px;
        }
        QSlider::handle:horizontal {
            background: #3498db;
            border: 1px solid #5c5c5c;
            width: 20px;
            height: 20px;
            margin: -5px 0;
            border-radius: 10px;
        }
        QSlider::handle:horizontal:hover {
            background: #1abc9c;
        }
        QSlider::sub-page:horizontal {
            background: #66ccff;
            border: 1px solid #4a4a4a;
            height: 10px;
            border-radius: 4px;
        }
        QSlider::add-page:horizontal {
            background: #ccc;
            border: 1px solid #4a4a4a;
            height: 10px;
            border-radius: 4px;
        }
    )");

	btnBack = new SvgButton("./resources/buttons/back.svg");
	btnBack->fixSize(50);
	btnBack->setPos(10, 10);
	addItem(btnBack);
	connect(btnBack, &SvgButton::clicked, [this]() {
		emit backActivated();
	});

	listMusic.setGeometry(70, 250, 480, 300);
    QFont font = QFont("Courier New", 16);
    //font.setBold(true);
    listMusic.setFont(font);
	addWidget(&listMusic);
	initListMusic(".\\resources\\backgroundmusic");

	connect(&listMusic, &QListWidget::itemClicked, [this](QListWidgetItem* item) {
		backgroundMusic->setSource(QUrl::fromLocalFile("./resources/backgroundmusic/" + item->text()));
		backgroundMusic->play();
	});
}

void SettingsMenu::initListMusic(const QString& path) {
    //QDir dir(path);
    //QStringList files = dir.entryList(QDir::Files);
    //for (const QString& file : files) {
    //    QListWidgetItem* item = new QListWidgetItem(file);
    //    listMusic.addItem(item);
    //}
}

