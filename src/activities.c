#include "activities.h"
#include "activities/activity.h"
#include "activities/activity_assets.h"
#include "activities/activity_party.h"
#include "activities/activity_secrets.h"
#include "activities/activity_timestamps.h"
#include "activities/party_size.h"

#include "core.h"
#include "users/user.h"
#include "util.h"

GDCALLINGCONV void *activity_manager_constructor(godot_object *p_instance, void *p_method_data)
{
    Library *lib = p_method_data;

    ActivityManager *activity_manager = lib->core_api->godot_alloc(sizeof(ActivityManager));
    memset(activity_manager, 0, sizeof(ActivityManager));

    activity_manager->object = p_instance;
    activity_manager->lib    = lib;

    return activity_manager;
}

GDCALLINGCONV void activity_manager_destructor(godot_object *p_instance, void *p_method_data, void *p_user_data)
{
    Library *lib                      = p_method_data;
    ActivityManager *activity_manager = p_user_data;

    lib->core_api->godot_free(activity_manager);
}

godot_variant activity_manager_register_command(godot_object *p_instance, void *p_method_data, void *p_user_data,
                                                int p_num_args, godot_variant **p_args)
{
    Library *lib                      = p_method_data;
    ActivityManager *activity_manager = p_user_data;

    godot_variant result_variant;

    if (p_num_args == 1) // Command
    {
        godot_string command_string           = lib->core_api->godot_variant_as_string(p_args[0]);
        godot_char_string command_char_string = lib->core_api->godot_string_utf8(&command_string);

        const char *command = lib->core_api->godot_char_string_get_data(&command_char_string);

        enum EDiscordResult result = activity_manager->internal->register_command(activity_manager->internal, command);

        lib->core_api->godot_variant_new_int(&result_variant, result);

        lib->core_api->godot_char_string_destroy(&command_char_string);
        lib->core_api->godot_string_destroy(&command_string);
    }
    else
    {
        PRINT_ERROR("Invalid number of arguments for \"register_command()\" call. Expected 1.", lib);
        lib->core_api->godot_variant_new_nil(&result_variant);
    }

    return result_variant;
}

godot_variant activity_manager_register_steam(godot_object *p_instance, void *p_method_data, void *p_user_data,
                                              int p_num_args, godot_variant **p_args)
{
    Library *lib                      = p_method_data;
    ActivityManager *activity_manager = p_user_data;

    godot_variant result_variant;

    if (p_num_args == 1) // Steam ID
    {
        uint32_t steam_id = (uint32_t)lib->core_api->godot_variant_as_uint(p_args[0]);

        enum EDiscordResult result = activity_manager->internal->register_steam(activity_manager->internal, steam_id);

        lib->core_api->godot_variant_new_int(&result_variant, result);
    }
    else
    {
        PRINT_ERROR("Invalid number of arguments for \"register_steam()\" call. Expected 1.", lib);
        lib->core_api->godot_variant_new_nil(&result_variant);
    }

    return result_variant;
}

void update_activity_callback(void *p_callback_data, enum EDiscordResult p_result)
{
    CallbackData *data                = p_callback_data;
    Library *lib                      = data->lib;
    ActivityManager *activity_manager = data->data;

    godot_variant result_variant;

    lib->core_api->godot_variant_new_int(&result_variant, p_result);

    const godot_variant *args[] = {&result_variant};

    if (data->callback_object)
    {
        if (lib->core_1_1_api->godot_is_instance_valid(data->callback_object))
            object_call(data->callback_object, &data->callback_name, 1, args, NULL, lib);
        else
            PRINT_ERROR("Callback object is no longer a valid instance.", lib);

        lib->core_api->godot_string_destroy(&data->callback_name);
    }

    godot_string signal_name = lib->core_api->godot_string_chars_to_utf8("update_activity_callback");
    object_emit_signal(activity_manager->object, &signal_name, 1, args, lib);

    lib->core_api->godot_string_destroy(&signal_name);
    lib->core_api->godot_variant_destroy(&result_variant);
    lib->core_api->godot_free(data);
}

godot_variant activity_manager_update_activity(godot_object *p_instance, void *p_method_data, void *p_user_data,
                                               int p_num_args, godot_variant **p_args)
{
    Library *lib                      = p_method_data;
    ActivityManager *activity_manager = p_user_data;

    godot_variant result_variant;

    if (p_num_args == 1 || p_num_args == 3) // Activity, [Callback Object, Callback Name]
    {
        godot_object *activity_object = lib->core_api->godot_variant_as_object(p_args[0]);
        Activity *activity            = lib->nativescript_api->godot_nativescript_get_userdata(activity_object);

        activity_collapse(activity_object, lib);

        CallbackData *callback_data = lib->core_api->godot_alloc(sizeof(CallbackData));
        memset(callback_data, 0, sizeof(CallbackData));
        callback_data->lib  = lib;
        callback_data->data = activity_manager;

        if (p_num_args == 3)
        {
            godot_object *callback_object  = lib->core_api->godot_variant_as_object(p_args[1]);
            godot_string callback_name     = lib->core_api->godot_variant_as_string(p_args[2]);
            callback_data->callback_object = callback_object;
            callback_data->callback_name   = callback_name;
        }

        activity_manager->internal->update_activity(activity_manager->internal, activity->internal, callback_data,
                                                    update_activity_callback);
    }
    else
    {
        PRINT_ERROR("Invalid number of arguments for \"update_activity()\" call. Expected 1 or 3.", lib);
    }

    lib->core_api->godot_variant_new_nil(&result_variant);

    return result_variant;
}

void clear_activity_callback(void *p_callback_data, enum EDiscordResult p_result)
{
    CallbackData *data                = p_callback_data;
    Library *lib                      = data->lib;
    ActivityManager *activity_manager = data->data;

    godot_variant result_variant;

    lib->core_api->godot_variant_new_int(&result_variant, p_result);

    const godot_variant *args[] = {&result_variant};

    if (data->callback_object)
    {
        if (lib->core_1_1_api->godot_is_instance_valid(data->callback_object))
            object_call(data->callback_object, &data->callback_name, 1, args, NULL, lib);
        else
            PRINT_ERROR("Callback object is no longer a valid instance.", lib);

        lib->core_api->godot_string_destroy(&data->callback_name);
    }

    godot_string signal_name = lib->core_api->godot_string_chars_to_utf8("clear_activity_callback");
    object_emit_signal(activity_manager->object, &signal_name, 1, args, lib);

    lib->core_api->godot_string_destroy(&signal_name);
    lib->core_api->godot_variant_destroy(&result_variant);
    lib->core_api->godot_free(data);
}

godot_variant activity_manager_clear_activity(godot_object *p_instance, void *p_method_data, void *p_user_data,
                                              int p_num_args, godot_variant **p_args)
{
    Library *lib                      = p_method_data;
    ActivityManager *activity_manager = p_user_data;

    godot_variant result_variant;

    if (p_num_args == 0 || p_num_args == 2) // [Callback Object, Callback Name]
    {
        CallbackData *callback_data = lib->core_api->godot_alloc(sizeof(CallbackData));
        memset(callback_data, 0, sizeof(CallbackData));
        callback_data->lib  = lib;
        callback_data->data = activity_manager;

        if (p_num_args == 2)
        {
            godot_object *callback_object  = lib->core_api->godot_variant_as_object(p_args[0]);
            godot_string callback_name     = lib->core_api->godot_variant_as_string(p_args[1]);
            callback_data->callback_object = callback_object;
            callback_data->callback_name   = callback_name;
        }

        activity_manager->internal->clear_activity(activity_manager->internal, callback_data, clear_activity_callback);
    }
    else
    {
        PRINT_ERROR("Invalid number of arguments for \"clear_activity()\" call. Expected 0 or 2.", lib);
    }

    lib->core_api->godot_variant_new_nil(&result_variant);

    return result_variant;
}

void send_request_reply_callback(void *p_callback_data, enum EDiscordResult p_result)
{
    CallbackData *data                = p_callback_data;
    Library *lib                      = data->lib;
    ActivityManager *activity_manager = data->data;

    godot_variant result_variant;

    lib->core_api->godot_variant_new_int(&result_variant, p_result);

    const godot_variant *args[] = {&result_variant};

    if (data->callback_object)
    {
        if (lib->core_1_1_api->godot_is_instance_valid(data->callback_object))
            object_call(data->callback_object, &data->callback_name, 1, args, NULL, lib);
        else
            PRINT_ERROR("Callback object is no longer a valid instance.", lib);

        lib->core_api->godot_string_destroy(&data->callback_name);
    }

    godot_string signal_name = lib->core_api->godot_string_chars_to_utf8("send_request_reply_callback");
    object_emit_signal(activity_manager->object, &signal_name, 1, args, lib);

    lib->core_api->godot_string_destroy(&signal_name);
    lib->core_api->godot_variant_destroy(&result_variant);
    lib->core_api->godot_free(data);
}

godot_variant activity_manager_send_request_reply(godot_object *p_instance, void *p_method_data, void *p_user_data,
                                                  int p_num_args, godot_variant **p_args)
{
    Library *lib                      = p_method_data;
    ActivityManager *activity_manager = p_user_data;

    godot_variant result_variant;

    if (p_num_args == 2 || p_num_args == 4) // User ID, Reply, [Callback Object, Callback Name]
    {
        int64_t user_id                             = lib->core_api->godot_variant_as_int(p_args[0]);
        enum EDiscordActivityJoinRequestReply reply = lib->core_api->godot_variant_as_int(p_args[1]);

        CallbackData *callback_data = lib->core_api->godot_alloc(sizeof(CallbackData));
        memset(callback_data, 0, sizeof(CallbackData));
        callback_data->lib  = lib;
        callback_data->data = activity_manager;

        if (p_num_args == 4)
        {
            godot_object *callback_object  = lib->core_api->godot_variant_as_object(p_args[2]);
            godot_string callback_name     = lib->core_api->godot_variant_as_string(p_args[3]);
            callback_data->callback_object = callback_object;
            callback_data->callback_name   = callback_name;
        }

        activity_manager->internal->send_request_reply(activity_manager->internal, user_id, reply, callback_data,
                                                       send_request_reply_callback);
    }
    else
    {
        PRINT_ERROR("Invalid number of arguments for \"send_request_reply()\" call. Expected 2 or 4.", lib);
    }

    lib->core_api->godot_variant_new_nil(&result_variant);

    return result_variant;
}

void send_invite_callback(void *p_callback_data, enum EDiscordResult p_result)
{
    CallbackData *data                = p_callback_data;
    Library *lib                      = data->lib;
    ActivityManager *activity_manager = data->data;

    godot_variant result_variant;

    lib->core_api->godot_variant_new_int(&result_variant, p_result);

    const godot_variant *args[] = {&result_variant};

    if (data->callback_object)
    {
        if (lib->core_1_1_api->godot_is_instance_valid(data->callback_object))
            object_call(data->callback_object, &data->callback_name, 1, args, NULL, lib);
        else
            PRINT_ERROR("Callback object is no longer a valid instance.", lib);

        lib->core_api->godot_string_destroy(&data->callback_name);
    }

    godot_string signal_name = lib->core_api->godot_string_chars_to_utf8("send_invite_callback");
    object_emit_signal(activity_manager->object, &signal_name, 1, args, lib);

    lib->core_api->godot_string_destroy(&signal_name);
    lib->core_api->godot_variant_destroy(&result_variant);
    lib->core_api->godot_free(data);
}

godot_variant activity_manager_send_invite(godot_object *p_instance, void *p_method_data, void *p_user_data,
                                           int p_num_args, godot_variant **p_args)
{
    Library *lib                      = p_method_data;
    ActivityManager *activity_manager = p_user_data;

    godot_variant result_variant;

    if (p_num_args == 3 || p_num_args == 5) // User ID, Type, Content, [Callback Object, Callback Name]
    {
        int64_t user_id                      = lib->core_api->godot_variant_as_int(p_args[0]);
        enum EDiscordActivityActionType type = lib->core_api->godot_variant_as_int(p_args[1]);
        godot_string content_string          = lib->core_api->godot_variant_as_string(p_args[2]);

        godot_char_string content_char_string = lib->core_api->godot_string_utf8(&content_string);
        const char *content                   = lib->core_api->godot_char_string_get_data(&content_char_string);

        CallbackData *callback_data = lib->core_api->godot_alloc(sizeof(CallbackData));
        memset(callback_data, 0, sizeof(CallbackData));
        callback_data->lib  = lib;
        callback_data->data = activity_manager;

        if (p_num_args == 5)
        {
            godot_object *callback_object  = lib->core_api->godot_variant_as_object(p_args[3]);
            godot_string callback_name     = lib->core_api->godot_variant_as_string(p_args[4]);
            callback_data->callback_object = callback_object;
            callback_data->callback_name   = callback_name;
        }

        activity_manager->internal->send_invite(activity_manager->internal, user_id, type, content, callback_data,
                                                send_invite_callback);

        lib->core_api->godot_char_string_destroy(&content_char_string);
        lib->core_api->godot_string_destroy(&content_string);
    }
    else
    {
        PRINT_ERROR("Invalid number of arguments for \"send_invite()\" call. Expected 3 or 5.", lib);
    }

    lib->core_api->godot_variant_new_nil(&result_variant);

    return result_variant;
}

void accept_invite_callback(void *p_callback_data, enum EDiscordResult p_result)
{
    CallbackData *data                = p_callback_data;
    Library *lib                      = data->lib;
    ActivityManager *activity_manager = data->data;

    godot_variant result_variant;

    lib->core_api->godot_variant_new_int(&result_variant, p_result);

    const godot_variant *args[] = {&result_variant};

    if (data->callback_object)
    {
        if (lib->core_1_1_api->godot_is_instance_valid(data->callback_object))
            object_call(data->callback_object, &data->callback_name, 1, args, NULL, lib);
        else
            PRINT_ERROR("Callback object is no longer a valid instance.", lib);

        lib->core_api->godot_string_destroy(&data->callback_name);
    }

    godot_string signal_name = lib->core_api->godot_string_chars_to_utf8("accept_invite_callback");
    object_emit_signal(activity_manager->object, &signal_name, 1, args, lib);

    lib->core_api->godot_string_destroy(&signal_name);
    lib->core_api->godot_variant_destroy(&result_variant);
    lib->core_api->godot_free(data);
}

godot_variant activity_manager_accept_invite(godot_object *p_instance, void *p_method_data, void *p_user_data,
                                             int p_num_args, godot_variant **p_args)
{
    Library *lib                      = p_method_data;
    ActivityManager *activity_manager = p_user_data;

    godot_variant result_variant;

    if (p_num_args == 1 || p_num_args == 3) // User ID, [Callback Object, Callback Name]
    {
        int64_t user_id = lib->core_api->godot_variant_as_int(p_args[0]);

        CallbackData *callback_data = lib->core_api->godot_alloc(sizeof(CallbackData));
        memset(callback_data, 0, sizeof(CallbackData));
        callback_data->lib = lib;

        if (p_num_args == 3)
        {
            godot_object *callback_object  = lib->core_api->godot_variant_as_object(p_args[1]);
            godot_string callback_name     = lib->core_api->godot_variant_as_string(p_args[2]);
            callback_data->callback_object = callback_object;
            callback_data->callback_name   = callback_name;
        }

        activity_manager->internal->accept_invite(activity_manager->internal, user_id, callback_data,
                                                  accept_invite_callback);
    }
    else
    {
        PRINT_ERROR("Invalid number of arguments for \"accept_invite()\" call. Expected 1 or 3.", lib);
    }

    lib->core_api->godot_variant_new_nil(&result_variant);

    return result_variant;
}

void register_activity_manager(void *p_handle, Library *p_lib)
{
    godot_instance_create_func constructor;
    memset(&constructor, 0, sizeof(godot_instance_create_func));
    constructor.create_func = activity_manager_constructor;
    constructor.method_data = p_lib;

    godot_instance_destroy_func destructor;
    memset(&destructor, 0, sizeof(godot_instance_destroy_func));
    destructor.destroy_func = activity_manager_destructor;
    destructor.method_data  = p_lib;

    p_lib->nativescript_api->godot_nativescript_register_class(p_handle, "ActivityManager", "Object", constructor,
                                                               destructor);

    // Methods
    {
        godot_instance_method method;
        godot_method_attributes attributes = {GODOT_METHOD_RPC_MODE_DISABLED};

        // Register Command
        {
            memset(&method, 0, sizeof(godot_instance_method));
            method.method      = activity_manager_register_command;
            method.method_data = p_lib;

            p_lib->nativescript_api->godot_nativescript_register_method(p_handle, "ActivityManager", "register_command",
                                                                        attributes, method);
        }
        // Register Steam
        {
            memset(&method, 0, sizeof(godot_instance_method));
            method.method      = activity_manager_register_steam;
            method.method_data = p_lib;

            p_lib->nativescript_api->godot_nativescript_register_method(p_handle, "ActivityManager", "register_steam",
                                                                        attributes, method);
        }
        // Update Activity
        {
            memset(&method, 0, sizeof(godot_instance_method));
            method.method      = activity_manager_update_activity;
            method.method_data = p_lib;

            p_lib->nativescript_api->godot_nativescript_register_method(p_handle, "ActivityManager", "update_activity",
                                                                        attributes, method);
        }
        // Clear Activity
        {
            memset(&method, 0, sizeof(godot_instance_method));
            method.method      = activity_manager_clear_activity;
            method.method_data = p_lib;

            p_lib->nativescript_api->godot_nativescript_register_method(p_handle, "ActivityManager", "clear_activity",
                                                                        attributes, method);
        }
        // Send Request Reply
        {
            memset(&method, 0, sizeof(godot_instance_method));
            method.method      = activity_manager_send_request_reply;
            method.method_data = p_lib;

            p_lib->nativescript_api->godot_nativescript_register_method(p_handle, "ActivityManager",
                                                                        "send_request_reply", attributes, method);
        }
        // Send Invite
        {
            memset(&method, 0, sizeof(godot_instance_method));
            method.method      = activity_manager_send_invite;
            method.method_data = p_lib;

            p_lib->nativescript_api->godot_nativescript_register_method(p_handle, "ActivityManager", "send_invite",
                                                                        attributes, method);
        }
        // Accept Invite
        {
            memset(&method, 0, sizeof(godot_instance_method));
            method.method      = activity_manager_accept_invite;
            method.method_data = p_lib;

            p_lib->nativescript_api->godot_nativescript_register_method(p_handle, "ActivityManager", "accept_invite",
                                                                        attributes, method);
        }
    }

    // Signals
    {
        godot_signal signal;

        // Activity Join
        {
            memset(&signal, 0, sizeof(godot_signal));
            signal.name = p_lib->core_api->godot_string_chars_to_utf8("activity_join");

            godot_signal_argument join_secret;
            {
                memset(&join_secret, 0, sizeof(godot_signal_argument));
                join_secret.name = p_lib->core_api->godot_string_chars_to_utf8("join_secret");

                join_secret.type = GODOT_VARIANT_TYPE_STRING;
            }

            godot_signal_argument args[] = {join_secret};
            signal.args                  = args;
            signal.num_args              = 1;

            p_lib->nativescript_api->godot_nativescript_register_signal(p_handle, "ActivityManager", &signal);

            p_lib->core_api->godot_string_destroy(&join_secret.name);
            p_lib->core_api->godot_string_destroy(&signal.name);
        }
        // Spectate Join
        {
            memset(&signal, 0, sizeof(godot_signal));
            signal.name = p_lib->core_api->godot_string_chars_to_utf8("activity_spectate");

            godot_signal_argument spectate_secret;
            {
                memset(&spectate_secret, 0, sizeof(godot_signal_argument));
                spectate_secret.name = p_lib->core_api->godot_string_chars_to_utf8("spectate_secret");

                spectate_secret.type = GODOT_VARIANT_TYPE_STRING;
            }

            godot_signal_argument args[] = {spectate_secret};
            signal.args                  = args;
            signal.num_args              = 1;

            p_lib->nativescript_api->godot_nativescript_register_signal(p_handle, "ActivityManager", &signal);

            p_lib->core_api->godot_string_destroy(&spectate_secret.name);
            p_lib->core_api->godot_string_destroy(&signal.name);
        }
        // Activity Join Request
        {
            memset(&signal, 0, sizeof(godot_signal));
            signal.name = p_lib->core_api->godot_string_chars_to_utf8("activity_join_request");

            godot_signal_argument user;
            {
                memset(&user, 0, sizeof(godot_signal_argument));
                user.name = p_lib->core_api->godot_string_chars_to_utf8("user");

                user.type = GODOT_VARIANT_TYPE_OBJECT;
            }

            godot_signal_argument args[] = {user};
            signal.args                  = args;
            signal.num_args              = 1;

            p_lib->nativescript_api->godot_nativescript_register_signal(p_handle, "ActivityManager", &signal);

            p_lib->core_api->godot_string_destroy(&user.name);
            p_lib->core_api->godot_string_destroy(&signal.name);
        }
        // Activity Invite
        {
            memset(&signal, 0, sizeof(godot_signal));
            signal.name = p_lib->core_api->godot_string_chars_to_utf8("activity_invite");

            godot_signal_argument type;
            {
                memset(&type, 0, sizeof(godot_signal_argument));
                type.name = p_lib->core_api->godot_string_chars_to_utf8("type");

                type.type = GODOT_VARIANT_TYPE_INT;
            }
            godot_signal_argument user;
            {
                memset(&user, 0, sizeof(godot_signal_argument));
                user.name = p_lib->core_api->godot_string_chars_to_utf8("user");

                user.type = GODOT_VARIANT_TYPE_OBJECT;
            }
            godot_signal_argument activity;
            {
                memset(&activity, 0, sizeof(godot_signal_argument));
                activity.name = p_lib->core_api->godot_string_chars_to_utf8("activity");

                activity.type = GODOT_VARIANT_TYPE_OBJECT;
            }

            godot_signal_argument args[] = {type, user, activity};
            signal.args                  = args;
            signal.num_args              = 3;

            p_lib->nativescript_api->godot_nativescript_register_signal(p_handle, "ActivityManager", &signal);

            p_lib->core_api->godot_string_destroy(&activity.name);
            p_lib->core_api->godot_string_destroy(&user.name);
            p_lib->core_api->godot_string_destroy(&type.name);
            p_lib->core_api->godot_string_destroy(&signal.name);
        }
        // Update Activity Callback
        {
            memset(&signal, 0, sizeof(godot_signal));
            signal.name = p_lib->core_api->godot_string_chars_to_utf8("update_activity_callback");

            godot_signal_argument result;
            {
                memset(&result, 0, sizeof(godot_signal_argument));
                result.name = p_lib->core_api->godot_string_chars_to_utf8("result");

                result.type = GODOT_VARIANT_TYPE_INT;
            }

            godot_signal_argument args[] = {result};
            signal.args                  = args;
            signal.num_args              = 1;

            p_lib->nativescript_api->godot_nativescript_register_signal(p_handle, "ActivityManager", &signal);

            p_lib->core_api->godot_string_destroy(&result.name);
            p_lib->core_api->godot_string_destroy(&signal.name);
        }
        // Clear Activity Callback
        {
            memset(&signal, 0, sizeof(godot_signal));
            signal.name = p_lib->core_api->godot_string_chars_to_utf8("clear_activity_callback");

            godot_signal_argument result;
            {
                memset(&result, 0, sizeof(godot_signal_argument));
                result.name = p_lib->core_api->godot_string_chars_to_utf8("result");

                result.type = GODOT_VARIANT_TYPE_INT;
            }

            godot_signal_argument args[] = {result};
            signal.args                  = args;
            signal.num_args              = 1;

            p_lib->nativescript_api->godot_nativescript_register_signal(p_handle, "ActivityManager", &signal);

            p_lib->core_api->godot_string_destroy(&result.name);
            p_lib->core_api->godot_string_destroy(&signal.name);
        }
        // Send Request Reply Callback
        {
            memset(&signal, 0, sizeof(godot_signal));
            signal.name = p_lib->core_api->godot_string_chars_to_utf8("send_request_reply_callback");

            godot_signal_argument result;
            {
                memset(&result, 0, sizeof(godot_signal_argument));
                result.name = p_lib->core_api->godot_string_chars_to_utf8("result");

                result.type = GODOT_VARIANT_TYPE_INT;
            }

            godot_signal_argument args[] = {result};
            signal.args                  = args;
            signal.num_args              = 1;

            p_lib->nativescript_api->godot_nativescript_register_signal(p_handle, "ActivityManager", &signal);

            p_lib->core_api->godot_string_destroy(&result.name);
            p_lib->core_api->godot_string_destroy(&signal.name);
        }
        // Send Invite Callback
        {
            memset(&signal, 0, sizeof(godot_signal));
            signal.name = p_lib->core_api->godot_string_chars_to_utf8("send_invite_callback");

            godot_signal_argument result;
            {
                memset(&result, 0, sizeof(godot_signal_argument));
                result.name = p_lib->core_api->godot_string_chars_to_utf8("result");

                result.type = GODOT_VARIANT_TYPE_INT;
            }

            godot_signal_argument args[] = {result};
            signal.args                  = args;
            signal.num_args              = 1;

            p_lib->nativescript_api->godot_nativescript_register_signal(p_handle, "ActivityManager", &signal);

            p_lib->core_api->godot_string_destroy(&result.name);
            p_lib->core_api->godot_string_destroy(&signal.name);
        }
        // Accept Invite Callback
        {
            memset(&signal, 0, sizeof(godot_signal));
            signal.name = p_lib->core_api->godot_string_chars_to_utf8("accept_invite_callback");

            godot_signal_argument result;
            {
                memset(&result, 0, sizeof(godot_signal_argument));
                result.name = p_lib->core_api->godot_string_chars_to_utf8("result");

                result.type = GODOT_VARIANT_TYPE_INT;
            }

            godot_signal_argument args[] = {result};
            signal.args                  = args;
            signal.num_args              = 1;

            p_lib->nativescript_api->godot_nativescript_register_signal(p_handle, "ActivityManager", &signal);

            p_lib->core_api->godot_string_destroy(&result.name);
            p_lib->core_api->godot_string_destroy(&signal.name);
        }
    }
}

void on_activity_join(void *p_event_data, const char *p_join_secret)
{
    Core *core   = p_event_data;
    Library *lib = core->lib;

    godot_string signal = lib->core_api->godot_string_chars_to_utf8("activity_join");

    godot_variant join_secret_variant;

    godot_string join_secret_string = lib->core_api->godot_string_chars_to_utf8(p_join_secret);
    lib->core_api->godot_variant_new_string(&join_secret_variant, &join_secret_string);

    const godot_variant *args[] = {&join_secret_variant};

    object_emit_signal(core->activities->object, &signal, 1, args, lib);

    lib->core_api->godot_string_destroy(&join_secret_string);
    lib->core_api->godot_variant_destroy(&join_secret_variant);
    lib->core_api->godot_string_destroy(&signal);
}

void on_activity_spectate(void *p_event_data, const char *p_spectate_secret)
{
    Core *core   = p_event_data;
    Library *lib = core->lib;

    godot_string signal = lib->core_api->godot_string_chars_to_utf8("activity_spectate");

    godot_variant spectate_secret_variant;

    godot_string spectate_secret_string = lib->core_api->godot_string_chars_to_utf8(p_spectate_secret);
    lib->core_api->godot_variant_new_string(&spectate_secret_variant, &spectate_secret_string);

    const godot_variant *args[] = {&spectate_secret_variant};

    object_emit_signal(core->activities->object, &signal, 1, args, lib);

    lib->core_api->godot_string_destroy(&signal);
    lib->core_api->godot_variant_destroy(&spectate_secret_variant);
    lib->core_api->godot_string_destroy(&spectate_secret_string);
}

void on_activity_join_request(void *p_event_data, struct DiscordUser *p_user)
{
    Core *core   = p_event_data;
    Library *lib = core->lib;

    godot_string signal = lib->core_api->godot_string_chars_to_utf8("activity_join_request");

    godot_variant user_variant;

    godot_object *user_object = instantiate_custom_class("User", "Resource", lib);
    User *user                = lib->nativescript_api->godot_nativescript_get_userdata(user_object);
    memcpy(user->internal, p_user, sizeof(struct DiscordUser));

    lib->core_api->godot_variant_new_object(&user_variant, user_object);

    const godot_variant *args[] = {&user_variant};

    object_emit_signal(core->activities->object, &signal, 1, args, lib);

    lib->core_api->godot_variant_destroy(&user_variant);
    lib->core_api->godot_string_destroy(&signal);
}

void on_activity_invite(void *p_event_data, enum EDiscordActivityActionType p_type, struct DiscordUser *p_user,
                        struct DiscordActivity *p_activity)
{
    Core *core   = p_event_data;
    Library *lib = core->lib;

    godot_string signal = lib->core_api->godot_string_chars_to_utf8("activity_invite");

    godot_variant type_variant;
    godot_variant user_variant;
    godot_variant activity_variant;

    lib->core_api->godot_variant_new_int(&type_variant, p_type);

    godot_object *user_object = instantiate_custom_class("User", "Resource", lib);
    User *user                = lib->nativescript_api->godot_nativescript_get_userdata(user_object);
    memcpy(user->internal, p_user, sizeof(struct DiscordUser));

    lib->core_api->godot_variant_new_object(&user_variant, user_object);

    godot_object *activity_object = instantiate_custom_class("Activity", "Resource", lib);
    activity_reconstruct(activity_object, p_activity, lib);

    lib->core_api->godot_variant_new_object(&activity_variant, activity_object);

    const godot_variant *args[] = {&type_variant, &user_variant, &activity_variant};

    object_emit_signal(core->activities->object, &signal, 3, args, lib);

    lib->core_api->godot_variant_destroy(&activity_variant);
    lib->core_api->godot_variant_destroy(&user_variant);
    lib->core_api->godot_variant_destroy(&type_variant);
    lib->core_api->godot_string_destroy(&signal);
}
