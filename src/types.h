#ifndef TYPES_H_
#define TYPES_H_

#include "discord_game_sdk.h"

#include <gdnative_api_struct.gen.h>

/**
 * @brief The library object, contains helpful structs and a reference to the gdnlib.
 */
typedef struct Library
{
    const godot_gdnative_core_api_struct *core_api;
    const godot_gdnative_core_1_1_api_struct *core_1_1_api;
    const godot_gdnative_ext_nativescript_api_struct *nativescript_api;

    const godot_object *gdnlib;
} Library;

// Forward declarations
typedef struct Core Core;
typedef struct UserManager UserManager;
typedef struct ImageManager ImageManager;
typedef struct ActivityManager ActivityManager;
typedef struct RelationshipManager RelationshipManager;
typedef struct NetworkManager NetworkManager;

/**
 * @brief Struct for storing callback data, e.g. objects and function names.
 */
typedef struct CallbackData
{
    godot_object *callback_object;
    godot_string callback_name;

    void *data;

    Library *lib;
} CallbackData;

#endif // TYPES_H_
