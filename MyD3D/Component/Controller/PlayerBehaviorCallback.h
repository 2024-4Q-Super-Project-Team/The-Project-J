#pragma once
#include "physx/characterkinematic/PxControllerBehavior.h"

class PlayerBehaviorCallback : public PxControllerBehaviorCallback
{
public:
    virtual PxControllerBehaviorFlags getBehaviorFlags(const PxShape& shape, const PxActor& actor) override {

      return PxControllerBehaviorFlag::eCCT_SLIDE;

    }
    virtual PxControllerBehaviorFlags getBehaviorFlags(const PxObstacle& obstacle) {
        return PxControllerBehaviorFlag::eCCT_SLIDE;
    }
    virtual PxControllerBehaviorFlags getBehaviorFlags(const PxController& controller) {
        return PxControllerBehaviorFlag::eCCT_SLIDE;
    }

};

