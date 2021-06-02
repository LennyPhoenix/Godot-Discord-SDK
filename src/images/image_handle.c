#include "image_handle.h"

#include "util.h"

GDCALLINGCONV void *image_handle_constructor(godot_object *p_instance, void *p_method_data)
{
    Library *lib = p_method_data;

    INIT_OBJECT(handle, ImageHandle, struct DiscordImageHandle, lib, p_instance);

    return handle;
}

GDCALLINGCONV void image_handle_destructor(godot_object *p_instance, void *p_method_data, void *p_user_data)
{
    Library *lib        = p_method_data;
    ImageHandle *handle = p_user_data;

    lib->core_api->godot_free(handle->internal);
    lib->core_api->godot_free(handle);
}

godot_variant image_handle_get_type(godot_object *p_instance, void *p_method_data, void *p_user_data)
{
    Library *lib        = p_method_data;
    ImageHandle *handle = p_user_data;

    godot_variant type;

    lib->core_api->godot_variant_new_int(&type, handle->internal->type);

    return type;
}

GDCALLINGCONV void image_handle_set_type(godot_object *p_instance, void *p_method_data, void *p_user_data,
                                         godot_variant *p_type)
{
    Library *lib        = p_method_data;
    ImageHandle *handle = p_user_data;

    handle->internal->type = lib->core_api->godot_variant_as_int(p_type);
}

godot_variant image_handle_get_id(godot_object *p_instance, void *p_method_data, void *p_user_data)
{
    Library *lib        = p_method_data;
    ImageHandle *handle = p_user_data;

    godot_variant id;

    lib->core_api->godot_variant_new_int(&id, handle->internal->id);

    return id;
}

GDCALLINGCONV void image_handle_set_id(godot_object *p_instance, void *p_method_data, void *p_user_data,
                                       godot_variant *p_id)
{
    Library *lib        = p_method_data;
    ImageHandle *handle = p_user_data;

    handle->internal->id = lib->core_api->godot_variant_as_int(p_id);
}

godot_variant image_handle_get_size(godot_object *p_instance, void *p_method_data, void *p_user_data)
{
    Library *lib        = p_method_data;
    ImageHandle *handle = p_user_data;

    godot_variant size;

    lib->core_api->godot_variant_new_uint(&size, handle->internal->size);

    return size;
}

GDCALLINGCONV void image_handle_set_size(godot_object *p_instance, void *p_method_data, void *p_user_data,
                                         godot_variant *p_size)
{
    Library *lib        = p_method_data;
    ImageHandle *handle = p_user_data;

    handle->internal->size = (uint32_t)lib->core_api->godot_variant_as_int(p_size);
}

void register_image_handle(void *p_handle, Library *p_lib)
{
    godot_instance_create_func constructor;
    memset(&constructor, 0, sizeof(godot_instance_create_func));
    constructor.create_func = image_handle_constructor;
    constructor.method_data = p_lib;

    godot_instance_destroy_func destructor;
    memset(&destructor, 0, sizeof(godot_instance_destroy_func));
    destructor.destroy_func = image_handle_destructor;
    destructor.method_data  = p_lib;

    p_lib->nativescript_api->godot_nativescript_register_class(p_handle, "ImageHandle", "Resource", constructor,
                                                               destructor);

    // Attributes
    {
        godot_property_attributes attributes;
        godot_property_get_func get;
        godot_property_set_func set;

        // Type
        {
            memset(&attributes, 0, sizeof(godot_property_attributes));
            attributes.type  = GODOT_VARIANT_TYPE_INT;
            attributes.usage = GODOT_PROPERTY_USAGE_DEFAULT;

            attributes.hint        = GODOT_PROPERTY_HINT_ENUM;
            attributes.hint_string = p_lib->core_api->godot_string_chars_to_utf8("User");

            p_lib->core_api->godot_variant_new_int(&attributes.default_value, DiscordImageType_User);

            memset(&get, 0, sizeof(godot_property_get_func));
            get.get_func    = image_handle_get_type;
            get.method_data = p_lib;

            memset(&set, 0, sizeof(godot_property_set_func));
            set.set_func    = image_handle_set_type;
            set.method_data = p_lib;

            p_lib->nativescript_api->godot_nativescript_register_property(p_handle, "ImageHandle", "type", &attributes,
                                                                          set, get);

            p_lib->core_api->godot_variant_destroy(&attributes.default_value);
            p_lib->core_api->godot_string_destroy(&attributes.hint_string);
        }
        // ID
        {
            memset(&attributes, 0, sizeof(godot_property_attributes));
            attributes.type  = GODOT_VARIANT_TYPE_INT;
            attributes.usage = GODOT_PROPERTY_USAGE_DEFAULT;

            memset(&get, 0, sizeof(godot_property_get_func));
            get.get_func    = image_handle_get_id;
            get.method_data = p_lib;

            memset(&set, 0, sizeof(godot_property_set_func));
            set.set_func    = image_handle_set_id;
            set.method_data = p_lib;

            p_lib->nativescript_api->godot_nativescript_register_property(p_handle, "ImageHandle", "id", &attributes,
                                                                          set, get);
        }
        // Size
        {
            memset(&attributes, 0, sizeof(godot_property_attributes));
            attributes.type  = GODOT_VARIANT_TYPE_INT;
            attributes.usage = GODOT_PROPERTY_USAGE_DEFAULT;

            memset(&get, 0, sizeof(godot_property_get_func));
            get.get_func    = image_handle_get_size;
            get.method_data = p_lib;

            memset(&set, 0, sizeof(godot_property_set_func));
            set.set_func    = image_handle_set_size;
            set.method_data = p_lib;

            p_lib->nativescript_api->godot_nativescript_register_property(p_handle, "ImageHandle", "size", &attributes,
                                                                          set, get);
        }
    }
}
