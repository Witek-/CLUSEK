#include "VehicleComponentLoader.h"
#include "../../ECS/Components/VehicleComponent.h"

void VehicleComponentLoader::Add(nlohmann::json& json, entt::registry& registry,
	const entt::registry::entity_type& entity)
{
	auto &component = registry.assign<VehicleComponent>(entity);
}