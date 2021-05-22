#pragma once

#include "Modules/Module.h"

    class CameraControlModule : public RxEngine::Module
    {
    public:
        CameraControlModule(ecs::World * world, RxEngine::EngineMain * engine)
            : Module(world, engine) {}

        void startup() override;
        void shutdown() override;

    private :
        bool movingCamera = false;
        bool forwardDown_ = false;
        bool backwardDown_ = false;
        bool rightDown_ = false;
        bool leftDown_ = false;
    };
