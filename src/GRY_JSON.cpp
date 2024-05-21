/**
 * @file GRY_JSON.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "GRY_JSON.hpp"
#include "GRY_Log.hpp"

void GRY_JSON::loadDoc(Document& doc, const char* path) {
    int BUFFER_SIZE = 65536;

    /* Open JSON file */
    FILE* fp;

    #ifdef _WIN32
    errno_t err = fopen_s(&fp, path, "rb");
    #elif __linux__
    fp = fopen(path, "rb");
    #else
    GRY_Log("GRY_JSON::loadDoc: OS not explicitly supported.\n")
    fp = fopen(path, "rb");
    #endif
    /* Create a buffer, and pass it into the stream with the file */
    char* readBuffer = new char[BUFFER_SIZE];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    /* Parse the doc */
    doc.ParseStream(is);

    /* Check that the document is valid */
    assert(doc.IsObject());
    /* Close the file */
    if (fp != 0) { fclose(fp); }
    /* Delete the buffer */
    delete[] readBuffer;
}

const GRY_JSON::Value& GRY_JSON::getProperty(const GRY_JSON::Value& value, const char* name) {
    assert(value.HasMember("properties"));
    for (auto& elem : value["properties"].GetArray()) {
        if (strcmp(name, elem["name"].GetString()) == 0) {
            return elem["value"];
        }
    }
    GRY_Log("GRY_JSON::getProperty: Could not find property \"%s\".", name);
    return value["properties"].GetArray()[0];
}