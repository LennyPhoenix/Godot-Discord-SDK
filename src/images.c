#include "images.h"

GDCALLINGCONV void *image_dimensions_constructor(godot_object *p_instance, Library *p_lib)
{
    struct DiscordImageDimensions *dimensions = p_lib->api->godot_alloc(sizeof(struct DiscordImageDimensions));
    memset(dimensions, 0, sizeof(dimensions));

    return dimensions;
}

GDCALLINGCONV void image_dimensions_destructor(godot_object *p_instance, Library *p_lib,
                                               struct DiscordImageDimensions *p_dimensions)
{
    p_lib->api->godot_free(p_dimensions);
}

godot_variant image_dimensions_get_width(godot_object *p_instance, Library *p_lib,
                                         struct DiscordImageDimensions *p_dimensions)
{
    godot_variant width;

    p_lib->api->godot_variant_new_uint(&width, p_dimensions->width);

    return width;
}

GDCALLINGCONV void image_dimensions_set_width(godot_object *p_instance, Library *p_lib,
                                              struct DiscordImageDimensions *p_dimensions,
                                              godot_variant *p_width)
{
    p_dimensions->width = p_lib->api->godot_variant_as_uint(p_width);
}

godot_variant image_dimensions_get_height(godot_object *p_instance, Library *p_lib,
                                          struct DiscordImageDimensions *p_dimensions)
{
    godot_variant height;

    p_lib->api->godot_variant_new_uint(&height, p_dimensions->height);

    return height;
}

GDCALLINGCONV void image_dimensions_set_height(godot_object *p_instance, Library *p_lib,
                                               struct DiscordImageDimensions *p_dimensions,
                                               godot_variant *p_height)
{
    p_dimensions->height = p_lib->api->godot_variant_as_uint(p_height);
}

void register_image_dimensions(void *p_handle, Library *p_lib)
{
    godot_instance_create_func constructor;
    memset(&constructor, 0, sizeof(constructor));
    constructor.create_func = image_dimensions_constructor;
    constructor.method_data = p_lib;

    godot_instance_destroy_func destructor;
    memset(&destructor, 0, sizeof(destructor));
    destructor.destroy_func = image_dimensions_destructor;
    destructor.method_data = p_lib;

    p_lib->nativescript_api->godot_nativescript_register_class(p_handle,
                                                               "ImageDimensions", "Resource",
                                                               constructor, destructor);

    // Attributes
    {
        godot_property_attributes attributes;
        godot_variant default_value;
        godot_property_get_func get;
        godot_property_set_func set;

        // Width
        {
            memset(&attributes, 0, sizeof(attributes));
            attributes.type = GODOT_VARIANT_TYPE_INT;
            attributes.usage = GODOT_PROPERTY_USAGE_DEFAULT;
            attributes.rset_type = GODOT_METHOD_RPC_MODE_DISABLED;

            attributes.hint = GODOT_PROPERTY_HINT_NONE;
            attributes.hint_string = p_lib->api->godot_string_chars_to_utf8("");

            p_lib->api->godot_variant_new_uint(&default_value, 0);
            attributes.default_value = default_value;

            memset(&get, 0, sizeof(get));
            get.get_func = image_dimensions_get_width;
            get.method_data = p_lib;

            memset(&set, 0, sizeof(set));
            set.set_func = image_dimensions_set_width;
            set.method_data = p_lib;

            p_lib->nativescript_api->godot_nativescript_register_property(p_handle,
                                                                          "ImageDimensions", "width",
                                                                          &attributes,
                                                                          set, get);
        }
        // Height
        {
            memset(&attributes, 0, sizeof(attributes));
            attributes.type = GODOT_VARIANT_TYPE_INT;
            attributes.usage = GODOT_PROPERTY_USAGE_DEFAULT;
            attributes.rset_type = GODOT_METHOD_RPC_MODE_DISABLED;

            attributes.hint = GODOT_PROPERTY_HINT_NONE;
            attributes.hint_string = p_lib->api->godot_string_chars_to_utf8("");

            p_lib->api->godot_variant_new_uint(&default_value, 0);
            attributes.default_value = default_value;

            memset(&get, 0, sizeof(get));
            get.get_func = image_dimensions_get_height;
            get.method_data = p_lib;

            memset(&set, 0, sizeof(set));
            set.set_func = image_dimensions_set_height;
            set.method_data = p_lib;

            p_lib->nativescript_api->godot_nativescript_register_property(p_handle,
                                                                          "ImageDimensions", "height",
                                                                          &attributes,
                                                                          set, get);
        }
    }
}