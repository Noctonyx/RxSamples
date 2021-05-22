#include "CameraControl.h"

#include "EngineMain.hpp"
#include "Window.hpp"
#include "Modules/RTSCamera/RTSCamera.h"
#include "Modules/SceneCamera/SceneCamera.h"
#include "Modules/Transforms/Transforms.h"

using namespace DirectX;

void CameraControlModule::startup()
{
    world_->createSystem("Camera:Scroll")
          .inGroup("Pipeline:Update")
          .withStream<RxEngine::MouseScroll>()
          .withWrite<RxEngine::RTSCamera>()
          .execute<RxEngine::MouseScroll>([](ecs::World * world, const RxEngine::MouseScroll * ms)
          {
              const auto scene_camera = world->getSingleton<RxEngine::SceneCamera>();
              const auto rts_camera = world->getUpdate<RxEngine::RTSCamera>(scene_camera->camera);

              const auto arm_delta = ms->y_offset * world->deltaTime() * 25.0f;
              rts_camera->dolly = std::clamp(rts_camera->dolly + arm_delta, 5.0f, 200.0f);

              return true;
          });

    world_->createSystem("Camera:RightButton")
          .inGroup("Pipeline:Update")
          .withStream<RxEngine::MouseButton>()
          .execute<RxEngine::MouseButton>(
              [this](ecs::World * world, const RxEngine::MouseButton * mb)
              {
                  auto window = engine_->getWindow();

                  if (mb->button == 1) {
                      window->setMouseVisible(!mb->pressed);
                      movingCamera = mb->pressed;
                  }
                  return true;
              });

    world_->createSystem("Camera:MoveMouse")
          .inGroup("Pipeline:Update")
          .withStream<RxEngine::MousePosition>()
          .execute<RxEngine::MousePosition>(
              [this](ecs::World * world, const RxEngine::MousePosition * mp)
              {
                  if (!movingCamera) {
                      return false;
                  }
                  auto sc = world->getSingleton<RxEngine::SceneCamera>();
                  if (!sc) {
                      return false;
                  }
                  auto delta = world->deltaTime();
                  const auto delta_rot = XMVectorSet(-mp->deltaY * delta,
                                                     -mp->deltaX * delta, 0, 0);

                  //spdlog::info("Mouse delta {0} {1}, {2}", delta, mp->deltaX, mp->deltaY);
                  auto lr = world->getUpdate<RxEngine::Transforms::LocalRotation>(sc->camera);
                  if (!lr) {
                      return false;
                  }
                  XMVECTOR rot = XMLoadFloat3(&lr->rotation);
                  rot = XMVectorAdd(rot, delta_rot);
                  rot = XMVectorClamp(
                      rot,
                      XMVectorSet(-XM_PIDIV2, -XM_2PI, -XM_2PI, 0.f),
                      XMVectorSet(-0.05f, XM_2PI, XM_2PI, 0.f)
                  );

                  XMStoreFloat3(&lr->rotation, XMVectorModAngles(rot));
                  return true;
              });

    world_->createSystem("Camera:Keys")
          .inGroup("Pipeline:Early")
          .withStream<RxEngine::KeyboardKey>()
          .execute<RxEngine::KeyboardKey>(
              [this](ecs::World * world, const RxEngine::KeyboardKey * kb)
              {
                  auto sc = world->getSingleton<RxEngine::SceneCamera>();
                  if (!sc) {
                      return false;
                  }

                  if (kb->key == RxEngine::EKey::W && kb->mods == RxEngine::EInputMod::None) {
                      forwardDown_ = kb->action != RxEngine::EInputAction::Release;
                      return true;
                  } else if (kb->key == RxEngine::EKey::S && kb->mods ==
                      RxEngine::EInputMod::None) {
                      backwardDown_ = kb->action != RxEngine::EInputAction::Release;
                      return true;
                  }

                  if (kb->key == RxEngine::EKey::D && kb->mods == RxEngine::EInputMod::None) {
                      rightDown_ = kb->action != RxEngine::EInputAction::Release;
                      return true;
                  } else if (kb->key == RxEngine::EKey::A && kb->mods ==
                      RxEngine::EInputMod::None) {
                      leftDown_ = kb->action != RxEngine::EInputAction::Release;
                      return true;
                  }

                  return false;
              });

    world_->createSystem("Camera:Move")
          .inGroup("Pipeline:Update")
          .execute([this](ecs::World * world)
          {
              auto sc = world->getSingleton<RxEngine::SceneCamera>();
              if (!sc) {
                  return;
              }
              auto delta = world->deltaTime();

              float rawForward = forwardDown_ ? 1.0f : backwardDown_ ? -1.0f : 0.f;
              float rawRight = rightDown_ ? 1.0f : leftDown_ ? -1.0f : 0.f;

              auto rts = world->get<RxEngine::RTSCamera>(sc->camera);
              auto right = rts->getRight();
              auto forward = rts->getForward();

              DirectX::XMVECTOR move = DirectX::XMVectorZero();

              move = DirectX::XMVectorAdd(
                  move,
                  DirectX::XMVectorScale(
                      DirectX::XMVector3Normalize(
                          DirectX::XMVectorSetY(forward, 0.f)
                      ),
                      (10.0f * delta * rawForward)
                  )
              );

              move = DirectX::XMVectorAdd(
                  move,
                  DirectX::XMVectorScale(
                      DirectX::XMVector3Normalize(
                          DirectX::XMVectorSetY(right, 0.f)
                      ),
                      (10.0f * delta * rawRight)
                  )
              );

              if (DirectX::XMVectorGetX(DirectX::XMVector3Length(move)) > 0.001f) {
                  //position += move;
                  auto wp = world->getUpdate<RxEngine::Transforms::WorldPosition>(sc->camera);
                  XMStoreFloat3(&wp->position, XMVectorAdd(XMLoadFloat3(&wp->position), move));
              }

              return;
          });
#if 0
        world_->createSystem("Camera:Rotate")
              .inGroup("Pipeline:Update")
              .withRead<MouseStatus>()
              .execute([&movingCamera](ecs::World * world)
              {
                  if(!movingCamera) {
                      return;
                  }

                  auto delta = world->deltaTime();

                  auto ms = world->getSingleton<MouseStatus>();
                  auto sc = world->getSingleton<SceneCamera>();
                  if (!sc) {
                      return;
                  }

                  if (!ms || !ms->button2) {
                      return;
                  }

                  const auto delta_rot = XMVectorSet(-ms->deltaMouseY * delta,
                                                    -ms->deltaMouseX * delta, 0, 0);

                  auto lr = world->getUpdate<Transforms::LocalRotation>(sc->camera);
                  if (!lr) {
                      return;
                  }
                  XMVECTOR rot = XMLoadFloat3(&lr->rotation);
                  rot = XMVectorAdd(rot, delta_rot);
                  rot = XMVectorClamp(
                      rot,
                      XMVectorSet(-XM_PIDIV2, -XM_2PI, -XM_2PI,0.f),
                      XMVectorSet(-0.05f, XM_2PI, XM_2PI, 0.f)
                  );

                  XMStoreFloat3(&lr->rotation, XMVectorModAngles(rot));
              });
#endif
}

void CameraControlModule::shutdown() { }
