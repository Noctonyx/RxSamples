// Main.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <filesystem>
#include "Window.hpp"
#include "RxECS.h"
#include "EngineMain.hpp"
#include "Log.h"

#include "nlohmann/json.hpp"
#include "Vfs.h"
#include "Modules/RTSCamera/RTSCamera.h"
#include "Modules/Transforms/Transforms.h"

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

        auto w = engine_->getWorld();

        auto e = w->instantiate(w->lookup("PrefabRTSCamera").id)
            .set<ecs::Name>({ "Main Camera" })
            .set<RxEngine::Transforms::WorldPosition>({ {0.0f, 0.0f, 5.0f} });

        auto r = e.getUpdate<RxEngine::RTSCamera>();
        r->dolly = 20.f;

        engine_->run();

        engine_.reset();

    } catch (const std::exception & e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
