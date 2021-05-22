// Main.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <filesystem>
#include <random>

#include "RxECS.h"
#include "EngineMain.hpp"
#include "Log.h"

#include "nlohmann/json.hpp"
#include "Vfs.h"
#include "Modules/CameraControl/CameraControl.h"
#include "Modules/RTSCamera/RTSCamera.h"
#include "Modules/SceneCamera/SceneCamera.h"
#include "Modules/Transforms/Transforms.h"
#include "Modules/WorldObject/WorldObject.h"


struct YRotateSpeed
{
    float s;
};

void loadAssetList()
{
    std::ifstream f("config.json");
    nlohmann::json js;
    f >> js;
    f.close();

    std::vector<std::filesystem::path> assetPaths;

    auto v = RxAssets::vfs();

    if (js["mounts"].is_array()) {
        nlohmann::json j2 = js["mounts"];
        for (auto & j3: j2) {
            if (j3.is_object()) {
                auto path = j3["path"].get<std::string>();
                auto mount = j3["mount"].get<std::string>();

                v->addMount(path, mount);
            }
        }
    }

    v->scan();
}

int main()
{
    try {
        std::unique_ptr<RxEngine::EngineMain> engine_;

        spdlog::info("Starting Triangle WIP");
        spdlog::set_level(spdlog::level::info);

        loadAssetList();

        engine_ = std::make_unique<RxEngine::EngineMain>();
        engine_->addInitConfigFile("/lua/startup.lua");
        engine_->startup();

        auto world = engine_->getWorld();

        engine_->addUserModule(std::make_shared<CameraControlModule>(world, engine_.get()));

        engine_->loadModules();

        auto e = world->lookup("PrefabRTSCamera")
                      .instantiate("Main Camera")
                      .set<RxEngine::Transforms::WorldPosition>({{0.0f, 1.1f, 5.0f}});

        auto r = e.getUpdate<RxEngine::RTSCamera>();
        r->dolly = 10.f;

        auto sc = world->getSingletonUpdate<RxEngine::SceneCamera>();
        sc->camera = e.id;

        auto prefab = world->lookup("prototype/testcube");

        int size = 50;

        std::random_device rd; //Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd());
        std::uniform_real_distribution distrib(-8.f, 8.0f);

        for (int32_t i = -size; i < size; i++) {
            for (int32_t j = -size; j < size; j++) {
                auto e1 = prefab.instantiate().add<RxEngine::WorldObject>();
                e1.set<RxEngine::Transforms::WorldPosition>({{i * 3.0f, 0.0f, j * 3.0f}});
                e1.set<YRotateSpeed>({distrib(gen)});
            }
        }

        world->createSystem("Main:Rotate")
             .inGroup("Pipeline:FixedUpdate")
             .withQuery<YRotateSpeed, RxEngine::Transforms::LocalRotation>()
             .each<YRotateSpeed, RxEngine::Transforms::LocalRotation>(
                 [](ecs::EntityHandle e,
                    const YRotateSpeed * rs,
                    RxEngine::Transforms::LocalRotation * lr)
                 {
                     auto delta = e.world->deltaTime();
                     lr->rotation.y = lr->rotation.y + rs->s * delta;
                 }
             );

        engine_->run();

        engine_.reset();

    } catch (const std::exception & e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
