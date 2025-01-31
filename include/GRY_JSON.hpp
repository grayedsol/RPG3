/**
 * @file GRY_JSON.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief GRY_JSON
 * @copyright Copyright (c) 2024
 */
#pragma once
#pragma warning(push, 0)
#include "rapidjson/document.h"
#pragma warning(pop)

/**
 * @brief Wrapper for simple rapidjson functionality.
 * 
 */
namespace GRY_JSON {
    /**
     * @brief @copybrief rapidjson::Document
     * 
     */
    using Document = rapidjson::Document;
    /**
     * @brief @copybrief rapidjson::Value
     * 
     */
    using Value = rapidjson::Value;

    /**
     * @brief Load the JSON data from a file into a document.
     * 
     * @param doc Document to load data into.
     * @param path Path of the JSON file.
     */
    void loadDoc(Document& doc, const char* path);
}