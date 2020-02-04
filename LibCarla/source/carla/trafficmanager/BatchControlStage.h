// Copyright (c) 2019 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include <memory>

#include "carla/Logging.h"
#include "carla/rpc/ActorId.h"
#include "carla/rpc/Command.h"
#include "carla/rpc/VehicleControl.h"

#include "carla/trafficmanager/MessengerAndDataTypes.h"
#include "carla/trafficmanager/PipelineStage.h"

#include "carla/client/detail/Simulator.h"
#include "carla/client/detail/EpisodeProxy.h"

namespace carla {
namespace traffic_manager {

  /// This class receives actuation signals (throttle, brake, steer)
  /// from MotionPlannerStage class and communicates these signals to
  /// the simulator in batches to control vehicles' movement.
  class BatchControlStage : public PipelineStage {

  private:

    /// Pointer to frame received from MotionPlanner.
    std::shared_ptr<PlannerToControlFrame> data_frame;
    /// Pointer to a messenger from MotionPlanner.
    std::shared_ptr<PlannerToControlMessenger> messenger;
    /// Reference to carla client connection object.
    carla::client::detail::EpisodeProxy episodeProxyBCS;
    /// Array to hold command batch.
    std::shared_ptr<std::vector<carla::rpc::Command>> commands;
    /// Number of vehicles registered with the traffic manager.
    uint64_t number_of_vehicles;

  public:

    BatchControlStage(
        std::string stage_name,
        std::shared_ptr<PlannerToControlMessenger> messenger,
		carla::client::detail::EpisodeProxy &episodeProxy);
    ~BatchControlStage();

    void DataReceiver() override;

    void Action() override;

    void DataSender() override;

  };

} // namespace traffic_manager
} // namespace carla