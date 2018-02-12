#ifndef PLAYER_H_
#define PLAYER_H_

#include <config.h>
#include <playerobject.h>
#include "camera.h"
#include "window.h"

class Player: public PlayerObject {
public:
	Player();

	void update(const Window& win);

	// In relative space, camera position is always the origin
	Camera getRelativeCamera(float width, float height, float zFar) const {
		Camera res;
		res.setPerspective(mFOV, width / height, 0.1f, zFar);
		res.setRotation(mRotation);
		return res;
	}

private:
	float mFOV, mMouseSensitivity;
	Uint8 mForwardKey, mBackwardKey, mLeftKey, mRightKey, mUpKey, mDownKey;
};

#endif