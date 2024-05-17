/**
 * @file ECS.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief Defines essential constants for the ECS.
 * @copyright Copyright (c) 2024
 */
#pragma once
#include <stdint.h>

namespace ECS {
    /**
     * @brief Identifier to retrieve component data.
     * 
     * @details
     * Must be an unsigned integral type.
     */
    typedef uint8_t entity;

    /**
     * @brief Maximum number of entities the ECS will manage.
     * 
     * @details
     * This number does not include `ECS::NONE`.
     */
    const uint8_t MAX_ENTITIES = 255;
    
    /**
     * @brief Represents the absence of an entity.
     * 
     */
    const entity NONE = MAX_ENTITIES;
}