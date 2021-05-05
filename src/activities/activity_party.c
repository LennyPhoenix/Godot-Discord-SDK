#include "activity_party.h"

#include "util.h"

#include "party_size.h"

GDCALLINGCONV void *activity_party_constructor(godot_object *p_instance, void *p_method_data)
{
    Library *lib = p_method_data;

    INIT_OBJECT(party, ActivityParty, struct DiscordActivityParty, lib, p_instance);

    party->size = instantiate_custom_class("PartySize", "Resource", lib);
    godot_reference(party->size, lib);

    return party;
}

GDCALLINGCONV void activity_party_destructor(godot_object *p_instance, void *p_method_data, void *p_user_data)
{
    Library *lib         = p_method_data;
    ActivityParty *party = p_user_data;

    if (party->size)
        godot_unreference(party->size, lib);

    lib->core_api->godot_free(party->internal);
    lib->core_api->godot_free(party);
}

godot_variant activity_party_get_id(godot_object *p_instance, void *p_method_data, void *p_user_data)
{
    Library *lib         = p_method_data;
    ActivityParty *party = p_user_data;

    godot_variant id;

    godot_string string = lib->core_api->godot_string_chars_to_utf8(party->internal->id);
    lib->core_api->godot_variant_new_string(&id, &string);

    lib->core_api->godot_string_destroy(&string);

    return id;
}

GDCALLINGCONV void activity_party_set_id(godot_object *p_instance, void *p_method_data, void *p_user_data,
                                         godot_variant *p_id)
{
    Library *lib         = p_method_data;
    ActivityParty *party = p_user_data;

    godot_string string = lib->core_api->godot_variant_as_string(p_id);
    GODOT_STRING_TO_C_STRING(string, party->internal->id, 128, lib);

    lib->core_api->godot_string_destroy(&string);
}

godot_variant activity_party_get_size(godot_object *p_instance, void *p_method_data, void *p_user_data)
{
    Library *lib         = p_method_data;
    ActivityParty *party = p_user_data;

    godot_variant size;

    lib->core_api->godot_variant_new_object(&size, party->size);

    return size;
}

GDCALLINGCONV void activity_party_set_size(godot_object *p_instance, void *p_method_data, void *p_user_data,
                                           godot_variant *p_size)
{
    Library *lib         = p_method_data;
    ActivityParty *party = p_user_data;

    if (party->size)
        godot_unreference(party->size, lib);

    party->size = lib->core_api->godot_variant_as_object(p_size);

    if (party->size)
        godot_reference(party->size, lib);
}

struct DiscordActivityParty *activity_party_collapse(godot_object *p_instance, Library *p_lib)
{
    ActivityParty *party = p_lib->nativescript_api->godot_nativescript_get_userdata(p_instance);

    PartySize *size       = p_lib->nativescript_api->godot_nativescript_get_userdata(party->size);
    party->internal->size = *size->internal;

    return party->internal;
}

void activity_party_reconstruct(godot_object *p_instance, struct DiscordActivityParty *p_party, Library *p_lib)
{
    ActivityParty *activity_party = p_lib->nativescript_api->godot_nativescript_get_userdata(p_instance);
    *activity_party->internal     = *p_party;

    PartySize *party_size = p_lib->nativescript_api->godot_nativescript_get_userdata(activity_party->size);
    *party_size->internal = p_party->size;
}

void register_activity_party(void *p_handle, Library *p_lib)
{
    godot_instance_create_func constructor;
    memset(&constructor, 0, sizeof(godot_instance_create_func));
    constructor.create_func = activity_party_constructor;
    constructor.method_data = p_lib;

    godot_instance_destroy_func destructor;
    memset(&destructor, 0, sizeof(godot_instance_destroy_func));
    destructor.destroy_func = activity_party_destructor;
    destructor.method_data  = p_lib;

    p_lib->nativescript_api->godot_nativescript_register_class(p_handle, "ActivityParty", "Resource", constructor,
                                                               destructor);

    // Attributes
    {
        godot_property_attributes attributes;
        godot_property_get_func get;
        godot_property_set_func set;

        // ID
        {
            memset(&attributes, 0, sizeof(godot_property_attributes));
            attributes.type  = GODOT_VARIANT_TYPE_STRING;
            attributes.usage = GODOT_PROPERTY_USAGE_DEFAULT;

            godot_string string = p_lib->core_api->godot_string_chars_to_utf8("");
            p_lib->core_api->godot_variant_new_string(&attributes.default_value, &string);

            memset(&get, 0, sizeof(godot_property_get_func));
            get.get_func    = activity_party_get_id;
            get.method_data = p_lib;

            memset(&set, 0, sizeof(godot_property_set_func));
            set.set_func    = activity_party_set_id;
            set.method_data = p_lib;

            p_lib->nativescript_api->godot_nativescript_register_property(p_handle, "ActivityParty", "id", &attributes,
                                                                          set, get);

            p_lib->core_api->godot_variant_destroy(&attributes.default_value);
            p_lib->core_api->godot_string_destroy(&string);
        }
        // Size
        {
            memset(&attributes, 0, sizeof(godot_property_attributes));
            attributes.type  = GODOT_VARIANT_TYPE_OBJECT;
            attributes.usage = GODOT_PROPERTY_USAGE_DEFAULT;

            memset(&get, 0, sizeof(godot_property_get_func));
            get.get_func    = activity_party_get_size;
            get.method_data = p_lib;

            memset(&set, 0, sizeof(godot_property_set_func));
            set.set_func    = activity_party_set_size;
            set.method_data = p_lib;

            p_lib->nativescript_api->godot_nativescript_register_property(p_handle, "ActivityParty", "size",
                                                                          &attributes, set, get);
        }
    }
}
