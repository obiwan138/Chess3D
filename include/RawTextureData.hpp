/**
 * @author obiwan138
 * @enum RawTextureData
 * @brief Structure to store raw texture data
 */

#pragma once

#include <vector>

/**
 * @struct RawTextureData
 * @brief Structure to store raw texture data
 */
struct RawTextureData 
{
    // Raw texture data
    std::vector<unsigned char> data;

    // Texture dimensions
    uint32_t width = 0;
    uint32_t height = 0;
};
