/**
 * @file GRY_ECS.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief GRY_ECS
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "ECS.hpp"
#include <tuple>

/**
 * @brief Database-like structure that manages entities and components.
 * 
 * @details
 * Stores all component data, and keeps track of which entities are being used.
 * The number of active entities cannot exceed `ECS::MAX_ENTITIES`.
 * 
 * @tparam Ts List of component data types.
 */
template<class... Ts>
struct GRY_ECS {
	using TupleType = std::tuple<ComponentSet<Ts>...>;
	using entity = ECS::entity;

	/**
	 * @brief Collection of all ComponentSets, one for each component type.
	 * 
	 */
	TupleType components;

	/**
	 * @brief Constructor.
	 * 
	 */
	GRY_ECS() = default;

	GRY_ECS(const GRY_ECS&) = delete;
	GRY_ECS& operator=(const GRY_ECS&) = delete;

	/**
	 * @brief Retrieve a reference to the ComponentSet with type `T`.
	 * 
	 * @tparam T Type of the desired ComponentSet.
	 * @return Reference to the ComponentSet with type `T`.
	 * 
	 * @sa getComponentReadOnly
	 */
	template<typename T>
	ComponentSet<T>& getComponent() { return std::get<ComponentSet<T>>(components); }

	/**
	 * @copybrief getComponent
	 * 
	 * @tparam T 
	 * @return `const` reference to the ComponentSet with type `T`.
	 * 
	 * @sa getComponent
	 */
	template<typename T>
	const ComponentSet<T>& getComponentReadOnly() const { return std::get<ComponentSet<T>>(components); }

	/**
	 * @brief Create an entity that is not in use.
	 * 
	 * @details
	 * Logs an error and returns `ECS::NONE` if the number of existing
	 * entities has reached its limit.
	 * 
	 * @return An entity that can be used to lookup component data.
	 */
    const entity createEntity() {
		entity e;
		if (back < ECS::MAX_ENTITIES) { e = back++; }
		else if (deadEntities.size() > 0) {
			e = deadEntities.back();
			deadEntities.pop_back();
		}
		else {
			e = ECS::NONE;
			GRY_Log("[EntityManager] Tried to get a new entity, but there are none left to give.\n");
		}
		return e;
	}

    /**
     * @brief Remove the entity's associated component data and free it of use.
	 * 
	 * @details
	 * This is the base case for freeEntity().
	 * The recursive case deletes the entity's component data.
     * 
     * @param e Entity to free.
     * @par Returns
	 * 	Nothing.
     */
	template<std::size_t I = 0>
	typename std::enable_if<(I == std::tuple_size<decltype(components)>::value)>::type
	freeEntity(entity e) { deadEntities.push_back(e); }

    /**
     * @brief @copybrief freeEntity
	 * 
	 * @details
	 * This is the recursive case for freeEntity().
	 * The base case adds the entity to the container of dead entities.
     * 
     * @param e Entity to free.
     * @par Returns
	 * 	Nothing.
     */
	template<std::size_t I = 0>
	typename std::enable_if<(I < std::tuple_size<decltype(components)>::value)>::type
	freeEntity(entity e) {
		if (std::get<I>(components).contains(e)) {
			std::get<I>(components).remove(e);
		}
		freeEntity<I + 1>(e);
	}

private:
	/**
	 * @brief Container to keep track of entities that are no longer in use.
	 * 
	 */
    std::vector<entity> deadEntities;
	
	/**
	 * @brief Next entity to be created.
	 * 
	 * @details
	 * If the number of created entities has reached `ECS::MAX_ENTITIES`,
	 * then the next entity will come from `deadEntities` instead.
	 */
	entity back = 0;
};
