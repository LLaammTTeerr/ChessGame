#include "../header/SoundEffects.h"

SoundEffect::SoundEffect() {
    moveSelfSound.setSource(QUrl::fromLocalFile(":/assets/soundeffect/move-self.wav"));
    moveSelfSound.setVolume(volume);

    promoteSound.setSource(QUrl::fromLocalFile(":/assets/soundeffect/promote.wav"));
    promoteSound.setVolume(volume);

    captureSound.setSource(QUrl::fromLocalFile(":/assets/soundeffect/capture.wav"));
    captureSound.setVolume(volume);

    moveCheckSound.setSource(QUrl::fromLocalFile(":/assets/soundeffect/move-check.wav"));
	moveCheckSound.setVolume(volume);

    castlingSound.setSource(QUrl::fromLocalFile(":/assets/soundeffect/castle.wav"));
	castlingSound.setVolume(volume);
}

void SoundEffect::resetVolume(float volume) {
    this->volume = volume;
    moveSelfSound.setVolume(volume);
    promoteSound.setVolume(volume);
    captureSound.setVolume(volume);
    moveCheckSound.setVolume(volume);
    castlingSound.setVolume(volume);
}

void SoundEffect::soundMoveSelf(void) {
    moveSelfSound.play();
}

void SoundEffect::soundPromote(void) {
    promoteSound.play();
}

void SoundEffect::soundCapture(void) {
    captureSound.play();
}

void SoundEffect::soundMoveCheck(void) {
	moveCheckSound.play();
}

void SoundEffect::soundCastling(void) {
	castlingSound.play();
}

SoundEffect::~SoundEffect(void) {
    // Destructor implementation (if needed)
}
