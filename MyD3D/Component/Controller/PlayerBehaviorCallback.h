#pragma once
#include "physx/characterkinematic/PxControllerBehavior.h"

class DynamicBehaviorCallback : public PxControllerBehaviorCallback {
public:
    PxControllerBehaviorFlags flag;

    DynamicBehaviorCallback()
    {
        flag = PxControllerBehaviorFlag::eCCT_CAN_RIDE_ON_OBJECT;
    }
    void SetFlags(PxControllerBehaviorFlags _flag) {
        flag = _flag;
    }

    virtual PxControllerBehaviorFlags getBehaviorFlags(const PxShape& shape, const PxActor& actor) override {

        return flag;

    }
    virtual PxControllerBehaviorFlags getBehaviorFlags(const PxObstacle& obstacle) {
        return flag;
    }
    virtual PxControllerBehaviorFlags getBehaviorFlags(const PxController& controller) {
        return flag;
    }
};


