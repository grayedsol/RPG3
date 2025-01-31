/**
 * @file GRY_JSON.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "GRY_JSON.hpp"
#include "GRY_Log.hpp"
#include "rapidjson/filereadstream.h"

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