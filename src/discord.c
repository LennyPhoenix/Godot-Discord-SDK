#include "discord.h"

Library lib;

void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *p_options)
{
    lib.api = p_options->api_struct;

    for (unsigned int i = 0; i < lib.api->num_extensions; i++)
    {
        switch (lib.api->extensions[i]->type)
        {
        case GDNATIVE_EXT_NATIVESCRIPT:
            lib.nativescript_api = (godot_gdnative_ext_nativescript_api_struct *)lib.api->extensions[i];

            if (lib.nativescript_api->next)
            {
                const godot_gdnative_api_struct *extension = lib.nativescript_api->next;

                while (extension)
                {
                    if (extension->version.major == 1 && extension->version.minor == 1)
                    {
                        lib.nativescript_1_1_api = (const godot_gdnative_ext_nativescript_1_1_api_struct *)extension;
                    }

                    extension = extension->next;
                }
            }

            break;

        default:
            break;
        };
    };
}

void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *p_options)
{
    lib.api = NULL;
    lib.nativescript_api = NULL;
    lib.nativescript_1_1_api = NULL;
}

void GDN_EXPORT godot_nativescript_init(void *p_handle)
{
    if (lib.nativescript_1_1_api)
    {
        godot_string string;
        lib.api->godot_string_new(&string);
        lib.api->godot_string_parse_utf8(&string, "Loaded nativescript 1.1...");
        lib.api->godot_print(&string);
        lib.api->godot_string_destroy(&string);
    }

    register_core(p_handle, &lib);
}