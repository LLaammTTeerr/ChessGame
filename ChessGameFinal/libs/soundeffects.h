#pragma once

#include "chess.hpp"
#include <QtMultimedia/QSoundEffect>
#include <QCoreApplication>

class SoundEffect {
public:
    SoundEffect();
    ~SoundEffect(void);
    void soundMoveSelf(void);
    void soundPromote(void);
    void soundCapture(void);
    void soundMoveCheck(void);
    void soundCastling(void);
	void setVolume(float volume) { this->volume = volume; }

private:
	float volume = 1.0f;
    QSoundEffect moveSelfSound;
    QSoundEffect promoteSound;
    QSoundEffect captureSound;
    QSoundEffect moveCheckSound;
	QSoundEffect castlingSound;
};
