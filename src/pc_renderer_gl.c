#ifdef PLATFORM_PC
#ifdef USE_OPENGL

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include "global.h"
#include "palette.h"

#define GBA_WIDTH  240
#define GBA_HEIGHT 160

extern u8 gVRAM[];
extern u16 gPlttBufferFaded[];
extern u16 gFramebuffer[];

// OpenGL handles
static GLuint gVAO, gVBO;
static GLuint gShaderProgram;
static GLuint gVramTex;
static GLuint gPalTex;
static GLuint gTilemapTex[4];

// Vertex shader - just a fullscreen quad
static const char *sVertShader =
    "#version 330 core\n"
    "layout(location=0) in vec2 aPos;\n"
    "layout(location=1) in vec2 aUV;\n"
    "out vec2 vUV;\n"
    "void main() {\n"
    "    gl_Position = vec4(aPos, 0.0, 1.0);\n"
    "    vUV = aUV;\n"
    "}\n";

// Fragment shader - tile renderer on the GPU
static const char *sFragShader =
    "#version 330 core\n"
    "in vec2 vUV;\n"
    "out vec4 fragColor;\n"
    "\n"
    "uniform usampler2D uVram;\n"      // raw VRAM data
    "uniform usampler2D uPalette;\n"   // palette buffer
    "uniform usampler2D uTilemap0;\n"  // BG0 tilemap
    "uniform usampler2D uTilemap1;\n"  // BG1 tilemap
    "uniform usampler2D uTilemap2;\n"  // BG2 tilemap
    "uniform usampler2D uTilemap3;\n"  // BG3 tilemap
    "uniform int uCharBase[4];\n"      // char base per BG (in tiles)
    "uniform int uBpp0;\n"             // BG0 is 8bpp
    "\n"
    "vec4 rgb555ToVec4(uint color) {\n"
    "    float r = float(color & 0x1Fu) / 31.0;\n"
    "    float g = float((color >> 5u) & 0x1Fu) / 31.0;\n"
    "    float b = float((color >> 10u) & 0x1Fu) / 31.0;\n"
    "    return vec4(r, g, b, 1.0);\n"
    "}\n"
    "\n"
    "vec4 sampleBG4bpp(int bgIdx, usampler2D tilemap, int charBase, vec2 uv) {\n"
    "    ivec2 px = ivec2(uv * vec2(240.0, 160.0));\n"
    "    ivec2 tile = px / 8;\n"
    "    ivec2 pxInTile = px % 8;\n"
    "    // Read tilemap entry\n"
    "    uint entry = texelFetch(tilemap, tile, 0).r;\n"
    "    int tileIdx = int(entry & 0x3FFu);\n"
    "    int palNum = int((entry >> 12u) & 0xFu);\n"
    "    if (tileIdx == 0) return vec4(0.0);\n"
    "    // Read pixel from tile data (4bpp = 32 bytes per tile)\n"
    "    int tileOffset = (charBase + tileIdx) * 32;\n"
    "    int byteOffset = tileOffset + pxInTile.y * 4 + pxInTile.x / 2;\n"
    "    uint byteVal = texelFetch(uVram, ivec2(byteOffset % 512, byteOffset / 512), 0).r;\n"
    "    uint colorIdx = (pxInTile.x % 2 == 0) ? (byteVal & 0xFu) : (byteVal >> 4u);\n"
    "    if (colorIdx == 0u) return vec4(0.0); // transparent\n"
    "    // Palette lookup\n"
    "    int palOffset = palNum * 16 + int(colorIdx);\n"
    "    uint color = texelFetch(uPalette, ivec2(palOffset % 256, palOffset / 256), 0).r;\n"
    "    return rgb555ToVec4(color);\n"
    "}\n"
    "\n"
    "vec4 sampleBG8bpp(int charBase, vec2 uv) {\n"
    "    ivec2 px = ivec2(uv * vec2(240.0, 160.0));\n"
    "    ivec2 tile = px / 8;\n"
    "    ivec2 pxInTile = px % 8;\n"
    "    uint entry = texelFetch(uTilemap0, tile, 0).r;\n"
    "    int tileIdx = int(entry & 0x3FFu);\n"
    "    // 8bpp = 64 bytes per tile\n"
    "    int tileOffset = charBase * 32 + tileIdx * 64;\n"
    "    int byteOffset = tileOffset + pxInTile.y * 8 + pxInTile.x;\n"
    "    uint colorIdx = texelFetch(uVram, ivec2(byteOffset % 512, byteOffset / 512), 0).r;\n"
    "    if (colorIdx == 0u) return vec4(0.0);\n"
    "    uint color = texelFetch(uPalette, ivec2(int(colorIdx) % 256, int(colorIdx) / 256), 0).r;\n"
    "    return rgb555ToVec4(color);\n"
    "}\n"
    "\n"
    "void main() {\n"
    "    // Backdrop color\n"
    "    uint backdrop = texelFetch(uPalette, ivec2(0, 0), 0).r;\n"
    "    vec4 color = rgb555ToVec4(backdrop);\n"
    "    // Render BG3 -> BG2 -> BG1 -> BG0 (back to front)\n"
    "    vec4 c3 = sampleBG4bpp(3, uTilemap3, uCharBase[3], vUV);\n"
    "    if (c3.a > 0.0) color = c3;\n"
    "    vec4 c2 = sampleBG4bpp(2, uTilemap2, uCharBase[2], vUV);\n"
    "    if (c2.a > 0.0) color = c2;\n"
    "    vec4 c1 = sampleBG4bpp(1, uTilemap1, uCharBase[1], vUV);\n"
    "    if (c1.a > 0.0) color = c1;\n"
    "    vec4 c0 = (uBpp0 == 1) ? sampleBG8bpp(uCharBase[0], vUV)\n"
    "                           : sampleBG4bpp(0, uTilemap0, uCharBase[0], vUV);\n"
    "    if (c0.a > 0.0) color = c0;\n"
    "    fragColor = color;\n"
    "}\n";

static GLuint CompileShader(GLenum type, const char *src)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    GLint ok;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[512];
        glGetShaderInfoLog(shader, 512, NULL, log);
        SDL_Log("Shader error: %s", log);
    }
    return shader;
}

void PC_GL_Init(SDL_Window *window)
{
    // Create shader program
    GLuint vert = CompileShader(GL_VERTEX_SHADER, sVertShader);
    GLuint frag = CompileShader(GL_FRAGMENT_SHADER, sFragShader);
    gShaderProgram = glCreateProgram();
    glAttachShader(gShaderProgram, vert);
    glAttachShader(gShaderProgram, frag);
    glLinkProgram(gShaderProgram);
    glDeleteShader(vert);
    glDeleteShader(frag);

    // Fullscreen quad
    float verts[] = {
        -1.f, -1.f,  0.f, 1.f,
         1.f, -1.f,  1.f, 1.f,
         1.f,  1.f,  1.f, 0.f,
        -1.f, -1.f,  0.f, 1.f,
         1.f,  1.f,  1.f, 0.f,
        -1.f,  1.f,  0.f, 0.f,
    };
    glGenVertexArrays(1, &gVAO);
    glGenBuffers(1, &gVBO);
    glBindVertexArray(gVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);

    // VRAM texture - store as R8UI, 512 wide
    glGenTextures(1, &gVramTex);
    glBindTexture(GL_TEXTURE_2D, gVramTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, 512, 0x18000/512, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Palette texture - 256 R16UI entries
    glGenTextures(1, &gPalTex);
    glBindTexture(GL_TEXTURE_2D, gPalTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R16UI, 256, 2, 0, GL_RED_INTEGER, GL_UNSIGNED_SHORT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Tilemap textures - 32x32 R16UI each
    glGenTextures(4, gTilemapTex);
    for (int i = 0; i < 4; i++) {
        glBindTexture(GL_TEXTURE_2D, gTilemapTex[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R16UI, 32, 32, 0, GL_RED_INTEGER, GL_UNSIGNED_SHORT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    SDL_Log("OpenGL renderer initialized");
}

// Screen base offsets matching sBgTemplates
static const int sScreenBases[] = {31, 30, 29, 28};
static const int sCharBases[]   = {0,  1,  2,  3};

void PC_RenderFrame(void)
{
    // Upload VRAM
    glBindTexture(GL_TEXTURE_2D, gVramTex);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 512, 0x18000/512, GL_RED_INTEGER, GL_UNSIGNED_BYTE, gVRAM);

    // Upload palette (BG + OBJ = 512 entries)
    glBindTexture(GL_TEXTURE_2D, gPalTex);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 256, 2, GL_RED_INTEGER, GL_UNSIGNED_SHORT, gPlttBufferFaded);

    // Upload tilemaps
    for (int i = 0; i < 4; i++) {
        glBindTexture(GL_TEXTURE_2D, gTilemapTex[i]);
        u16 *tilemap = (u16 *)(gVRAM + sScreenBases[i] * 0x800);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 32, 32, GL_RED_INTEGER, GL_UNSIGNED_SHORT, tilemap);
    }

    // Draw
    glUseProgram(gShaderProgram);

    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, gVramTex);
    glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, gPalTex);
    glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, gTilemapTex[0]);
    glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, gTilemapTex[1]);
    glActiveTexture(GL_TEXTURE4); glBindTexture(GL_TEXTURE_2D, gTilemapTex[2]);
    glActiveTexture(GL_TEXTURE5); glBindTexture(GL_TEXTURE_2D, gTilemapTex[3]);

    glUniform1i(glGetUniformLocation(gShaderProgram, "uVram"),     0);
    glUniform1i(glGetUniformLocation(gShaderProgram, "uPalette"),  1);
    glUniform1i(glGetUniformLocation(gShaderProgram, "uTilemap0"), 2);
    glUniform1i(glGetUniformLocation(gShaderProgram, "uTilemap1"), 3);
    glUniform1i(glGetUniformLocation(gShaderProgram, "uTilemap2"), 4);
    glUniform1i(glGetUniformLocation(gShaderProgram, "uTilemap3"), 5);
    glUniform1iv(glGetUniformLocation(gShaderProgram, "uCharBase"), 4, sCharBases);
    glUniform1i(glGetUniformLocation(gShaderProgram, "uBpp0"), 1); // title screen BG0 is 8bpp

    glBindVertexArray(gVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

#endif // USE_OPENGL
#endif // PLATFORM_PC
