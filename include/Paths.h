#pragma once

namespace Paths
{
    // Directories (CMake defines should be string literals, e.g., "-DSHADER_DIR=\"shaders/\"")
    constexpr char ShaderDir[] =
#ifdef SHADER_DIR
        SHADER_DIR;
#else
        "shaders/";
#endif

    constexpr char AssetDir[] =
#ifdef ASSET_DIR
        ASSET_DIR;
#else
        "assets/";
#endif

    // Precomposed file paths (as static char arrays)
    constexpr char shaderVertPath[] = "shaders/shader.vert";
    constexpr char shaderFragPath[] = "shaders/shader.frag";
    constexpr char textShaderVertPath[] = "shaders/shaderText.vert";
    constexpr char textShaderFragPath[] = "shaders/shaderText.frag";
    constexpr char fontAsset[] = "assets/minogram_6x10.png";
}
