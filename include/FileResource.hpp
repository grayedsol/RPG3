/**
 * @file FileResource.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief FileResource
 * @copyright Copyright (c) 2024
 */
#pragma once
#include <utility>
#include "GRY_Lib.hpp"

class GRY_Game;

/**
 * @brief Abstract file resource type that can be loaded after its construction.
 * 
 * @details
 * Useful for large data, such as textures.
 * 
 * Uses the copy-swap idiom.
 */
struct FileResource {
	/**
	 * @brief Path to the file to be loaded.
	 * 
	 */
	const char* path = nullptr;

	/**
	 * @brief Abstract constructor.
	 * 
	 * @details
	 * `path` will be `nullptr` by default.
	 */
	FileResource() = default;

	/**
	 * @brief Abstract constructor.
	 * 
	 * @param path Path to the file to be loaded.
	 */
	FileResource(const char* path) { setPath(path); }

	/**
	 * @brief Virtual destructor.
	 * 
	 */
	virtual ~FileResource() { delete[] path; }

	FileResource(const FileResource&) = delete;
	FileResource& operator=(const FileResource&) = delete;

	/**
	 * @brief Swap the data of two FileResources.
	 * 
	 * @param lhs A FileResource.
	 * @param rhs Another FileResource.
	 */
	friend void swap(FileResource& lhs, FileResource& rhs) {
		using std::swap;
		swap(lhs.path, rhs.path);
	}

	/**
	 * @brief Move constructor.
	 * 
	 * @param other FileResource to take data from.
	 */
	FileResource(FileResource&& other) noexcept { swap(*this, other); }

	/**
	 * @brief Set the path of the file resource.
	 * 
	 * @param path Path to the file to load.
	 */
	void setPath(const char* path) {
		if (path) { this->path = GRY_copyString(path); }
	}

	/**
	 * @brief Load a part of the resource.
	 * 
	 * @details
	 * If the resource is already loaded, this function should
	 * return true without doing anything else.
	 * 
	 * @param game Associated game class.
	 * 
	 * @returns `true` if the resource was already loaded.
	 * @returns `false` otherwise.
	 */
	virtual bool load(GRY_Game* game) = 0;

	/**
	 * @brief Load the entire resource.
	 * 
	 * @param game Associated game class.
	 */
	void loadAll(GRY_Game* game) { while (!load(game)) {}; };
};