#include "error.h"

#include "main.h"
#include "text.h"

#include "decomp.h"

#include <mgraph.h>

#include <sstream>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// GLOBAL: GTA 0x0050f5b8
int g_Error_state;

// GLOBAL: GTA 0x0050f3b8
char g_Current_file_path[256];

// GLOBAL: GTA 0x0050f4b8
char g_Error_temp_path[256];

// FUNCTION: GTA 0x004228b0
void InitError() {
    g_Error_state = 0;
    g_Current_file_path[0] = '\0';
}

// FUNCTION: GTA 0x004228e0
void QuitError() {
    if (g_Error_state == 0) {
        g_Error_state = 1;
    }
}

// FUNCTION: GTA 0x004228c0
void SetCurrentLoadFilePath(const char *path) {
    strcpy(g_Current_file_path, path);
}

// FUNCTION: GTA 0x00422900
void FatalError(tError error, int location, ...) {
    if (g_Error_state == 0 || g_Error_state == 1) {
        strcpy(g_Error_temp_path, g_Current_file_path);
        g_Error_state = 2;
        StopAll();
        strcpy(g_Current_file_path, g_Error_temp_path);
    }
    va_list ap;
    va_start(ap, location);
    switch (error) {
        case eFatalError_no_rail_found_around_DDD:
        case eFatalError_invalid_tile_D_at_DDD:
        case eFatalError_unable_to_place_car_type_D_at_DDD:
        case eFatalError_unknown_error_neg140:
        case eFatalError_invalid_map_access_at_DDD:
        case eFatalError_invalid_tl_junction_DDD:
            ShowErrorMessage(error, location, va_arg(ap, uintptr_t), va_arg(ap, int), va_arg(ap, int), va_arg(ap, int));
            break;
        default:
            ShowErrorMessage(error, location, va_arg(ap, uintptr_t));
            break;
    }
    va_end(ap);
    TRIGGER_BREAKPOINT();
    exit(-1);
}

// FUNCTION: GTA 0x004229e0
void ShowErrorMessage(tError error, int location, ...) {
    char message_buffer[80];
    b32 read_error;
    b32 network_error;
    va_list ap;
    int extra;
    uintptr_t arg;

    va_start(ap, location);
    arg = va_arg(ap, uintptr_t);
    switch (error) {
    case eFatalError_no_rail_found_around_DDD:
    case eFatalError_invalid_tile_D_at_DDD:
    case eFatalError_unable_to_place_car_type_D_at_DDD:
    case eFatalError_unknown_error_neg140:
    case eFatalError_invalid_map_access_at_DDD:
    case eFatalError_invalid_tl_junction_DDD:
        extra = va_arg(ap, int);
        extra = va_arg(ap, int);
        extra = va_arg(ap, int);
        FormatErrorMessage(error, message_buffer, &read_error, &network_error, arg, extra, extra, extra);
        break;
    default:
        FormatErrorMessage(error, message_buffer, &read_error, &network_error, arg);
        break;
    }
    va_end(ap);

    std::stringstream sstream;
    sstream << "Error " << -error << '.' << location << '\n' << message_buffer << '\0';
    std::string s = sstream.str();
    MGL_fatalError(s.c_str());
}

// FUNCTION: GTA 0x00422b90
void FormatErrorMessage(tError error, char *buffer, b32 *read_error, b32 *network_error, ...) {
    // GLOBAL: GTA 0x0050f5c0
    static char static_buffer_0050f5c0[32];
    // GLOBAL: GTA 0x0060f370
    static char static_buffer_0050f370[64];
    va_list ap;
    const char *cstr;
    int valint;
    int i;

    va_start(ap, network_error);

    *read_error = FALSE;
    *network_error = FALSE;
    switch (error) {
    case eFatalError_networkoff:
        sprintf(buffer, "%s -\n%s", GetTranslatedString("ipxnotinstalled"), GetTranslatedString("networkoff"));
        break;
    case eFatalError_uartnotresponding:
        sprintf(static_buffer_0050f5c0, GetTranslatedString("uartnotresponsing"), va_arg(ap, char *)); // FIXME: determine argument type from 'uartnotresponsing'
        sprintf(buffer, "%s -\n%s", static_buffer_0050f5c0, GetTranslatedString("networkoff"));
        break;
    case eFatalError_vesa_mode_failure_1:
    case eFatalError_vesa_mode_failure_2:
    case eFatalError_vesa_mode_failure_3:
    case eFatalError_vesa_mode_failure_4:
    case eFatalError_vesa_mode_failure_5:
    case eFatalError_vesa_mode_failure_6:
        strcpy(buffer, "VESA mode failure");
        break;
    case eFatalError_vesa_mode_failure_7:
        strcpy(buffer, va_arg(ap, const char *));
        break;
    case eFatalError_firetruck_attending_a_non_fire_object:
        sprintf(buffer, "Firetruck attending a non-fire object");
        break;
    case eFatalError_new_powerup_type_D_already_exists:
        sprintf(buffer,"New powerup type %d already exists", va_arg(ap, int));
        break;
    case eFatalError_3dfx_lock_failed:
        strcpy(buffer, "3DFX lock failed");
        break;
    case eFatalError_car_model_D_has_been_initialised_with_invalid_max_speed:
        sprintf(buffer, "Car (model %d) has been initialised with invalid max speed. Check CAR.TXT", va_arg(ap, int));
        break;
    case eFatalError_invalid_crime_type_D:
        sprintf(buffer, "invalid crime type : %d", va_arg(ap, int));
        break;
    case eFatalError_error_generating_univbe_drv:
        strcpy(buffer, "error generating univbe.drv");
        break;
    case eFatalError_univbe_check_install_function_failed:
        strcpy(buffer, "UniVBE check install function failed");
        break;
    case eFatalError_invalid_ped_type_in_a_create_ped_D:
        sprintf(buffer,"Invalid ped type in a CREATE_PED: %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_token_D:
        sprintf(buffer,"Invalid token: %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_train_type_D:
        sprintf(buffer,"Invalid train type: %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_face_for_an_explosion_D:
        sprintf(buffer,"Invalid face for an explosion: %d", va_arg(ap, int));
        break;
    case eFatalError_accessing_a_mission_command_line_that_doesnt_exist_D:
        sprintf(buffer,"Accessing a mission command line that doesn't exist: %d", va_arg(ap, int));
        break;
    case eFatalError_nested_start_mission_commands_current_miss_number_D:
        sprintf(buffer,"Nested 'start_mission' commands. Current miss number: %d", va_arg(ap, int));
        break;
    case eFatalError_nested_end_mission_commands_current_miss_number_D:
        sprintf(buffer,"Nested 'end_mission' commands. Current miss number: %d", va_arg(ap, int));
        break;
    case eFatalError_no_end_mission_for_mission_num_D:
        sprintf(buffer,"No 'end_mission' for mission num: %d", va_arg(ap, int));
        break;
    case eFatalError_too_many_goals_defined_in_mission_ini_count_D:
        sprintf(buffer,"Too many goals defined in mission.ini. Count: %d", va_arg(ap, int));
        break;
    case eFatalError_too_many_commands_defined_in_mission_ini_count_D:
        sprintf(buffer,"Too many commands defined in mission.ini. Count: %d", va_arg(ap, int));
        break;
    case eFatalError_string_is_too_long_for_text_wrapper:
        sprintf(buffer,"String is too long for text wrapper");
        break;
    case eFatalError_invalid_rotation_in_sprite_D:
        sprintf(buffer,"Invalid rotation in sprite %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_powerup_id_D:
        sprintf(buffer,"Invalid powerup id: %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_mission_multiplier_D:
        sprintf(buffer,"Invalid mission multiplier: %d", va_arg(ap, int));
        break;
    case eFatalError_car_has_been_warped_unknown_rotation:
        sprintf(buffer,"Car has been warped...  unknown rotation");
        break;
    case eFatalError_cannot_find_the_default_display_mode:
        strcpy(buffer, "Cannot find the default display mode");
        break;
    case eFatalError_insufficient_real_memory_for_univb:
        strcpy(buffer, "insufficient real memory for UniVBE");
        break;
    case eFatalError_invalid_process_number_in_set_trigger_started_D:
        sprintf(buffer, "Invalid process number in 'set_trigger_started': %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_trigger_number_in_set_trigger_started_D:
        sprintf(buffer, "Invalid trigger number in 'set_trigger_started': %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_car_number_D:
        sprintf(buffer, "Invalid car number: %d", va_arg(ap, int));
        break;
    case eFatalError_collidable_flats_overflow:
        strcpy(buffer, "Collidable flats overflow");
        break;
    case eFatalError_invalid_usage_of_collidable_flat_D:
        sprintf(buffer, "Invalid usage of collidable flat : %d", va_arg(ap, int));
        break;
    case eFatalError_collidable_flat_not_disabled_D:
        sprintf(buffer, "collidable flat not disabled : %d", va_arg(ap, int));
        break;
    case eFatalError_no_routine_in_do_police_anim_D:
        sprintf(buffer, "no routine in do_police_anim : %d", va_arg(ap, int));
        break;
    case eFatalError_too_many_police_cars_assigned_to_chase_suggest_inc_max_cars_per_chase_perha_ps_D:
        sprintf(buffer, "too many police cars assigned to chase. suggest: inc MAX_CARS_PER_CHASE perhaps?  : %d", va_arg(ap, int));
        break;
    case eFatalError_illegal_value_for_a_briefing_message_D:
        sprintf(buffer,"Illegal value for a briefing message : %d", va_arg(ap, int));
        break;
    case eFatalError_unable_to_create_blades_for_helicopter:
        sprintf(buffer,"Unable to create blades for helicopter");
        break;
    case eFatalError_chopper_already_created:
        sprintf(buffer,"Chopper already created!");
        break;
    case eFatalError_hamiltons_no_longer_exist_line_at_fault_D:
        sprintf(buffer,"Hamiltons no longer exist! Line at fault: %d", va_arg(ap, int));
        break;
    case eFatalError_bad_area_D:
        sprintf(buffer,"Bad area: %d", va_arg(ap, int));
        break;
    case eFatalError_trying_to_give_a_weapon_to_a_non_ped_line_num_D:
        sprintf(buffer,"Trying to give a weapon to a non-ped - line num: %d", va_arg(ap, int));
        break;
    case eFatalError_trying_to_free_up_a_non_mission_car:
        sprintf(buffer,"Trying to free up a non-mission car");
        break;
    case eFatalError_invalid_chase_access_D:
        sprintf(buffer,"Invalid chase access: %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_sample_access_D:
        sprintf(buffer,"Invalid sample access: %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_door_access_D:
        sprintf(buffer,"Invalid door access: %d", va_arg(ap, int));
        break;
    case eFatalError_please_run_settings_first_error_open_registry_key:
        sprintf(buffer,"Please run 'Settings' first (error: open registry key)");
        break;
    case eFatalError_please_run_settings_first_error_query_registry_value:
        sprintf(buffer,"Please run 'Settings' first (error: query registry value)");
        break;
    case eFatalError_cannot_close_registry_key:
        sprintf(buffer,"Cannot close registry key");
        break;
    case eFatalError_invalid_font_char_width_D:
        sprintf(buffer,"Invalid font char width : %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_location_in_a_store_data_loc_D:
        sprintf(buffer,"Invalid location in a 'store_data' loc:%d", va_arg(ap, int));
        break;
    case eFatalError_invalid_location_in_a_retrive_data_loc_D:
        sprintf(buffer,"Invalid location in a 'retrieve_data' loc:%d", va_arg(ap, int));
        break;
    case eFatalError_trying_to_create_a_car_thats_already_been_created_D:
        sprintf(buffer,"Trying to create a car that's already been created: %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_goal_in_a_retrieve_data_goal_D_likely_to_be_a_mission_ini_problem:
        sprintf(buffer,"Invalid goal in a 'retrieve_data' - goal: %d\nLikely to be a mission.ini problem", va_arg(ap, int));
        break;
    case eFatalError_invalid_goal_in_a_store_data_goal__D_likely_to_be_a_mission_ini_problem:
        sprintf(buffer,"Invalid goal in a 'store_data' - goal: %d\nLikely to be a mission.ini problem", va_arg(ap, int));
        break;
    case eFatalError_trying_to_retrieve_data_from_a_goal_with_wrong_type_D:
        sprintf(buffer, "Trying to retrieve data from a goal with wrong type: %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_ped_for_switching_to_policeped_linenum_D:
        sprintf(buffer, "Invalid Ped for switching to policeped: linenum %d", va_arg(ap, int));
        break;
    case eFatalError_cannot_find_a_suitable_display_mode:
        strcpy(buffer, "Cannot find a suitable display mode");
        break;
    case eFatalError_invalid_model_num_for_a_car_model_D:
        sprintf(buffer, "Invalid model num for a car: model %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_car_num_when_trying_to_get_car_coords_D:
        sprintf(buffer, "Invalid car num when trying to get car coords: %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_car_num_D:
        sprintf(buffer, "Invalid car num: %d", va_arg(ap, int));
        break;
    case eFatalError_possible_kill_car_on_a_previously_killed_car_D:
        sprintf(buffer, "Possible 'kill car' on a previously killed car: %d", va_arg(ap, int));
        break;
    case eFatalError_univbe_drv_not_found:
        strcpy(buffer, "univbe.drv not found");
        break;
    case eFatalError_univbe_drv_not_valid:
        strcpy(buffer, "univbe.drv not valid");
        break;
    case eFatalError_univbe_driver_old_version:
        strcpy(buffer, "UniVBE driver old version");
        break;
    case eFatalError_insufficient_memory_for_univbe:
        strcpy(buffer, "insufficient memory for UniVBE");
        break;
    case eFatalError_invalid_request_for_a_trigger_started_D:
        sprintf(buffer, "Invalid request for a 'trigger started': %d", va_arg(ap, int));
        break;
    case eFatalError_cannot_malloc_D_bytes:
        sprintf(buffer, "cannot malloc %d bytes", va_arg(ap, int));
        break;
    case eFatalError_cannot_open_S:
        sprintf(buffer, "cannot open '%s'", va_arg(ap, const char *));
        break;
    case eFatalError_cannot_read_data_from_S:
        sprintf(buffer, "cannot read data from '%s'", va_arg(ap, const char *));
        break;
    case eFatalError_cannot_write_data_to_S:
        sprintf(buffer, "cannot write data to '%s'", va_arg(ap, const char *));
        break;
    case eFatalError_map_file_S_is_wrong_version:
        sprintf(buffer, "map file '%s' is wrong version", va_arg(ap, const char *));
        break;
    case eFatalError_cannot_create_a_640x480_video_mode:
        sprintf(buffer, "cannot create a 640x480 video mode");
        break;
    case eFatalError_invalid_number_of_characters_in_font_file_S:
        sprintf(buffer, "invalid number of characters in font file '%s'", va_arg(ap, const char *));
        break;
    case eFatalError_volume_out_of_range:
        sprintf(buffer, "volume out of range");
        break;
    case eFatalError_invalid_tl_junction_DDD:
        sprintf(buffer, "Invalid TL junction (%d,%d,%d)", va_arg(ap, int), va_arg(ap, int), va_arg(ap, int));
        break;
    case eFatalError_accessing_collision_list_that_is_already_in_use_D:
        sprintf(buffer, "Accessing collision list that is already in use : %d", va_arg(ap, int));
        break;
    case eFatalError_possible_loop_in_the_list_list_elements_found_exceed_lots_of_items_in_list:
        sprintf(buffer, "Possible Loop in the list (List elements found exceed LOTS_OF_ITEMS_IN_LIST");
        break;
    case eFatalError_invalid_escort_vehicle_D:
        sprintf(buffer, "Invalid escort vehicle : %d", va_arg(ap, int));
        break;
    case eFatalError_illegal_motorbike_frame_no_D:
        sprintf(buffer, "Illegal motorbike frame no %d", va_arg(ap, int));
        break;
    case eFatalError_no_available_arrest_release_location_D:
        sprintf(buffer, "No available arrest release location : %d", va_arg(ap, int));
        break;
    case eFatalError_no_available_hospital_release_location_D:
        sprintf(buffer, "No available hospital release location : %d", va_arg(ap, int));
        break;
    case eFatalError_create_failed_in_mission_cpp__ini_line_D:
        sprintf(buffer, "Create failed in mission.cpp  ini line: %d", va_arg(ap, int));
        break;
    case eFatalError_ped_with_illegal_type_D:
        sprintf(buffer, "ped with illegal type: %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_parameter_value_D:
        sprintf(buffer, "invalid parameter value : %d", va_arg(ap, int));
        break;
    case eFatalError_no_direction_arrow_above_policestation_dir_D:
        sprintf(buffer, "No direction arrow above policestation: dir=%d", va_arg(ap, int));
        break;
    case eFatalError_no_direction_arrow_above_hospital_dir_D:
        sprintf(buffer, "No direction arrow above hospital: dir=%d", va_arg(ap, int));
        break;
    case eFatalError_no_slot_to_store_barrier_no_in_see_ianj:
        sprintf(buffer, "No slot to store barrier no in. See IanJ", va_arg(ap, int));
        break;
    case eFatalError_improper_number_in_a_gta_demand_see_bri_num_D:
        sprintf(buffer, "Improper number in a GTA Demand, see Bri: num=%d", va_arg(ap, int));
        break;
    case eFatalError_too_many_trains:
        strcpy(buffer, "too many trains");
        break;
    case eFatalError_too_many_traffic_lights:
        strcpy(buffer, "too many traffic lights");
        break;
    case eFatalError_too_many_cars_associated_with_roadblock_D:
        sprintf(buffer, "too many cars associated with roadblock : %d", va_arg(ap, int));
        break;
    case eFatalError_vehicles_route_is_too_long_to_fit_in_path_D_array:
        sprintf(buffer, "Vehicle's route is too long to fit in path[%d] array", va_arg(ap, int));
        break;
    case eFatalError_improper_object_type_in_mission_ini_line_D:
        sprintf(buffer, "Improper object type in mission.ini   line:%d", va_arg(ap, int));
        break;
    case eFatalError_improper_mission_command_in_mission_ini_line_D:
        sprintf(buffer, "Improper mission command in mission.ini   line:%d", va_arg(ap, int));
        break;
    case eFatalError_no_path_to_load_route_into_D:
        sprintf(buffer, "no path to load route into : %d", va_arg(ap, int));
        break;
    case eFatalError_setup_screen_failed:
        strcpy(buffer, "setup screen failed");
        break;
    case eFatalError_vesa_mode_error_D:
        sprintf(buffer, "VESA mode error : %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_object_string_in_mission_ini_S:
        sprintf(buffer, "Invalid object string in mission.ini :%s", va_arg(ap, const char *));
        break;
    case eFatalError_invalid_mission_command_string_in_mission_ini_S:
        sprintf(buffer, "Invalid 'mission command' string in mission.ini :%s", va_arg(ap, const char *));
        break;
    case eFatalError_missing_paramater_for_line_in_mission_ini_could_be_obj_or_command_line_num_D:
        sprintf(buffer, "Missing paramater for line in mission.ini, could be obj or command\n line num:%d", va_arg(ap, const char *));
        break;
    case eFatalError_invalid_request_for_object_line_number_request_for_line_D:
        sprintf(buffer, "Invalid request for object line number, request for line %d", va_arg(ap, int));
        break;
    case eFatalError_request_for_an_object_line_number_has_produced_a_line_num_of_neg1_request_was_D:
        sprintf(buffer, "Request for an object line number has produced a line_num of -1\nrequest was %d", va_arg(ap, int));
        break;
    case eFatalError_request_for_a_mission_line_succ_gives_a_line_of_neg1_request_was_D:
        sprintf(buffer, "Request for a mission line(succ) gives a line of -1.Request was %d", va_arg(ap, int));
        break;
    case eFatalError_request_for_a_mission_line_fail_gives_a_line_of_neg1_request_was_D:
        sprintf(buffer, "Request for a mission line(fail) gives a line of -1.Request was %d", va_arg(ap, int));
        break;
    case eFatalError_mission_code_is_trying_to_process_a_command_with_invalid_type_physical_line_D:
        sprintf(buffer, "Mission code is trying to process a command with invalid type. Physical line %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_taxi_hunt_access_D:
        sprintf(buffer, "Invalid Taxi Hunt access : %d", va_arg(ap, int));
        break;
    case eFatalError_modem_error_result_code_D:
        sprintf(buffer, "modem error - result code %d", va_arg(ap, int));
        break;
    case eFatalError_modem_configuration_item_too_long_S:
        cstr = va_arg(ap, const char *);
        for (i = 0; i < sizeof(static_buffer_0050f370) && cstr[i] != '\r'; i++) {
            static_buffer_0050f370[i] = cstr[i];
        }
        static_buffer_0050f370[i] = '\0';
        sprintf(buffer, "modem configuration item too long : %s", static_buffer_0050f370);
        break;
    case eFatalError_invalid_config_number_S:
        sprintf(buffer,"invalid config number : %s", va_arg(ap, const char *));
        break;
    case eFatalError_trying_to_create_a_special_ped_when_chase_ped_has_not_been_created_objline_D:
        sprintf(buffer,"Trying to create a special ped when chase ped has not been created: objline: %d", va_arg(ap, int));
        break;
    case eFatalError_trying_to_create_a_driver_ped_for_a_futurecar_objline_D:
        sprintf(buffer, "Trying to create a driver ped for a FUTURECAR:  objline: %d", va_arg(ap, int));
        break;
    case eFatalError_linear_velocity_exceeds_cars_top_speed_front_speed_lin_vel_mismatc:
        sprintf(buffer,"Linear velocity exceeds car's top speed. front_speed/lin_vel mismatch");
        break;
    case eFatalError_no_suitable_vesa_modes_available:
        strcpy(buffer, "no suitable VESA modes available");
        break;
    case eFatalError_vesa_mode_D_not_working:
        sprintf(buffer, "VESA mode %d not working", va_arg(ap, int));
        break;
    case eFatalError_invalid_serial_port_speed_D:
        sprintf(buffer, "invalid serial port speed : %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_bank_alarm_access_D:
        sprintf(buffer, "Invalid bank alarm access : %d", va_arg(ap, int));
        break;
    case eFatalError_object_line_num_D_is_defined_twice_in_mission_ini:
        sprintf(buffer, "Object line num %d is defined twice in mission.ini", va_arg(ap, int));
        break;
    case eFatalError_line_num_too_big_in_mission_ini_number_D:
        sprintf(buffer, "Line num too big in mission.ini   number:%d", va_arg(ap, int));
        break;
    case eFatalError_illegal_line_num_for_a_mission_command_access_by_a_trigger_line_num_accessed_D:
        sprintf(buffer, "Illegal line num for a mission command access by a trigger - line num accessed:%d", va_arg(ap, int));
        break;
    case eFatalError_attempt_to_kill_a_kickstart_that_hasnt_been_executed_kill_line_num_D:
        sprintf(buffer, "Attempt to 'kill' a kickstart that hasn't been executed - 'kill' line num: %d", va_arg(ap, int));
        break;
    case eFatalError_too_many_next_kicks_off_the_one_kickstart_line_next_kick_is_D:
        sprintf(buffer, "Too many next_kicks off the one kickstart line. Next_kick is %d", va_arg(ap, int));
        break;
    case eFatalError_accessing_a_kickstart_that_isnt_correct_type_line_num_is_D:
        sprintf(buffer, "Accessing a KICKSTART that isn't correct type.line num is %d", va_arg(ap, int));
        break;
    case eFatalError_accessing_a_kickstart_that_hasnt_executed__nextkick_linenum_D:
        sprintf(buffer, "Accessing a KICKSTART that hasn't executed: nextkick linenum: %d", va_arg(ap, int));
        break;
    case eFatalError_attempting_to_send_a_ped_to_car_with_no_back_door:
        strcpy(buffer, "Attempting to send a ped to car with no back door.");
        break;
    case eFatalError_ped_creation_enemy_ped_is_invalid_linenum_of_create_D:
        sprintf(buffer, "Ped Creation: 'enemy ped' is invalid. linenum of create: %d", va_arg(ap, int));
        break;
    case eFatalError_ped_on_trying_to_create_a_ped_that_isnt_defined_as_a_futureped_line_D:
        sprintf(buffer, "PED_ON: trying to create a ped that isn't defined as a futureped. line:%d", va_arg(ap, int));
        break;
    case eFatalError_car_on_trying_to_create_a_car_that_isnt_defined_as_a_car_line_D:
        sprintf(buffer, "CAR_ON: trying to create a car that isn't defined as a car. line:%d", va_arg(ap, int));
        break;
    case eFatalError_trying_to_do_an_arrowped_on_something_other_than_a_ped_line_D:
        sprintf(buffer, "Trying to do an 'ARROWPED' on something other than a ped. line:%d", va_arg(ap, int));
        break;
    case eFatalError_trying_to_do_an_arrowcar_on_something_other_than_a_car_line_D:
        sprintf(buffer, "Trying to do an 'ARROWCAR' on something other than a car. line:%d", va_arg(ap, int));
        break;
    case eFatalError_trying_to_remap_a_ped_that_isnt_defined_as_a_ped_line_D:
        sprintf(buffer, "Trying to remap a ped that isn't defined as a ped. line: %d", va_arg(ap, int));
        break;
    case eFatalError_trying_to_remap_a_car_that_isnt_defined_as_a_car_line_D:
        sprintf(buffer, "Trying to remap a car that isn't defined as a car. line: %d", va_arg(ap, int));
        break;
    case eFatalError_trying_to_create_a_mission_target_too_close_to_another_line_D:
        sprintf(buffer, "Trying to create a mission target too close to another. line: %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_player_number_D:
        sprintf(buffer, "Invalid player number: %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_sprite_number_in_car_D:
        sprintf(buffer, "Invalid sprite number in car: %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_car_remap_number_D:
        sprintf(buffer, "Invalid car remap number: %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_trigger_request_D:
        sprintf(buffer, "Invalid trigger request: %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_width_in_sprite_D:
        sprintf(buffer, "Invalid width in sprite %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_height_in_sprite_D:
        sprintf(buffer, "Invalid height in sprite %d", va_arg(ap, int));
        break;
    case eFatalError_traffic_lights_error_code_D:
        sprintf(buffer, "Traffic lights error code %d", va_arg(ap, int));
        break;
    case eFatalError_public_transport_error_code_D:
        sprintf(buffer, "Public transport error code %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_z_in_explosion_D:
        sprintf(buffer, "Invalid z in explosion : %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_hunt_target_D:
        sprintf(buffer, "Invalid hunt target : %d", va_arg(ap, int));
        break;
    case eFatalError_not_an_error:
        strcpy(buffer, "Not an error");
        break;
    case eFatalError_unable_to_open_file:
        sprintf(buffer, "Unable to open file : %s", g_Current_file_path);
        *read_error = 1;
        break;
    case eFatalError_read_failure_in_file:
        sprintf(buffer, "Read failure in file : %s", g_Current_file_path);
        *read_error = 1;
        break;
    case eFatalError_unable_to_close_file:
        sprintf(buffer, "Unable to close file: %s", g_Current_file_path);
        *read_error = 1;
        break;
    case eFatalError_mouse_driver_not_installed:
        strcpy(buffer, "Mouse driver not installed");
        break;
    case eFatalError_out_of_dos_memory_allocating_D_bytes:
        sprintf(buffer, "Out of DOS memory allocating %d bytes", va_arg(ap, int));
        break;
    case eFatalError_attempt_to_allocate_zero_bytes_dos_memory:
        strcpy(buffer, "Attempt to allocate zero bytes DOS memory");
        break;
    case eFatalError_attempt_to_allocate_non_16_D_bytes_dos_momory:
        sprintf(buffer, "Attempt to allocate non-16 %d bytes DOS memory", va_arg(ap, int));
        break;
    case eFatalError_dos_memory_freeing_failur:
        strcpy(buffer, "DOS memory freeing failure");
        break;
    case eFatalError_out_of_memory_allocating_D_bytes:
        sprintf(buffer, "Out of memory allocating %d bytes", va_arg(ap, int));
        break;
    case eFatalError_vesa_function_X_not_supported:
        sprintf(buffer, "VESA function %xh not supported", va_arg(ap, int));
        break;
    case eFatalError_vesa_function_X_failed:
        sprintf(buffer, "VESA function %xh failed", va_arg(ap, int));
        break;
    case eFatalError_real_mode_interrupt_X_call_failed:
        sprintf(buffer, "Real mode Interrupt %xh call failed", va_arg(ap, int));
        break;
    case eFatalError_vesa_svga_mode_X_not_supported:
        sprintf(buffer, "VESA SVGA mode %xh not supported", va_arg(ap, int));
        break;
    case eFatalError_attempting_to_allocate_zero_bytes:
        strcpy(buffer, "Attempting to allocate zero bytes of memory");
        break;
    case eFatalError_ftell_error_in_file:
        sprintf(buffer, "Ftell error in file: %s", g_Current_file_path);
        *read_error = 1;
        break;
    case eFatalError_fseek_error_in_file:
        sprintf(buffer, "Fseek error in file: %s", g_Current_file_path);
        *read_error = 1;
        break;
    case eFatalError_int_31_fn_X_call_failed:
        sprintf(buffer, "Int 31 fn %xh call failed", va_arg(ap, int));
        break;
    case eFatalError_mouse_not_installed:
        strcpy(buffer, "Mouse not installed");
        break;
    case eFatalError_invalid_switch_S:
        sprintf(buffer, "Invalid switch : %s", va_arg(ap, const char *));
        break;
    case eFatalError_mscdex_not_installed:
        strcpy(buffer, "MSCDEX not installed");
        break;
    case eFatalError_drive_C_is_not_a_cd:
        sprintf(buffer, "Drive %c: is not a CD", va_arg(ap, int));
        break;
    case eFatalError_cd_device_io_error_X:
        sprintf(buffer, "CD device I/O error %04.4X", va_arg(ap, int));
        break;
    case eFatalError_track_D_not_present_on_cd:
        sprintf(buffer, "Track %d not present on CD", va_arg(ap, int));
        break;
    case eFatalError_incorrect_file_version_D_in_path_expecting_D:
        sprintf(buffer, "Incorrect file version %d in %s - expecting %d", va_arg(ap, int), g_Current_file_path, va_arg(ap, int));
        break;
    case eFatalError_map_change_overflow_by_D_bytes:
        sprintf(buffer, "Map change overflow by %d bytes", va_arg(ap, int));
        break;
    case eFatalError_attempt_to_read_from_not_opened_file:
        strcpy(buffer, "attempt to read from not opened file");
        break;
    case eFatalError_file_too_large_S_exceeds_max_size_by_D_bytes:
        sprintf(buffer, "File too large: %s exceeds max size by %d bytes", g_Current_file_path, va_arg(ap, int));
        break;
    case eFatalError_memory_buffer_exceeds_max_size_by_D_byte:
        sprintf(buffer, "Memory buffer exceeds max size by %d bytes", va_arg(ap, int));
        break;
    case eFatalError_sprite_table_overflow:
        strcpy(buffer, "sprite table overflow");
        break;
    case eFatalError_sprite_spare_buffer_overflow:
        strcpy(buffer, "sprite spare buffer overflow");
        break;
    case eFatalError_spare_graphic_request_exceeds_max_size_by_D_bytes:
        sprintf(buffer, "spare graphic request exceeds max size by %d bytes", va_arg(ap, int));
        break;
    case eFatalError_illegal_access_of_not_spare_sprite:
        strcpy(buffer, "illegal access of not-spare sprite");
        break;
    case eFatalError_sprite_not_fully_enclosed:
        strcpy(buffer, "sprite not fully enclosed");
        break;
    case eFatalError_loop_pointer_in_tree_key_D:
        sprintf(buffer, "loop pointer in tree : key = %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_z_coord_in_sprite_D:
        sprintf(buffer, "invalid z co-ord in sprite : %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_block_number_D:
        sprintf(buffer, "invalid block number : %d", va_arg(ap, int));
        break;
    case eFatalError_ground_block_data_invalid_size_D:
        sprintf(buffer, "ground block data invalid size : %d", va_arg(ap, int));
        break;
    case eFatalError_empty_map:
        strcpy(buffer, "empty map");
        break;
    case eFatalError_car_D_unknown_vehicle_type_D:
        valint = va_arg(ap, int);
        sprintf(buffer, "car %d unknown vehicle type : %d", valint % 256, valint / 256);
        break;
    case eFatalError_car_D_invalid_size_D:
        valint = va_arg(ap, int);
        sprintf(buffer, "car %d invalid size : %d", valint % 256, valint / 256);
        break;
    case eFatalError_invalid_into_size_in_object_info_D:
        sprintf(buffer, "invalid into size in object info %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_size_in_object_info_D:
        sprintf(buffer, "invalid size in object info %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_coordinate_in_object_pos_D:
        sprintf(buffer, "invalid co-ordinate in object pos %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_rotation_in_object_pos_D:
        sprintf(buffer, "invalid rotation in object pos %d", va_arg(ap, int));
        break;
    case eFatalError_sprite_D_delta_D_has_zero_length:
        sprintf(buffer, "sprite %d delta %d has zero length", va_arg(ap, int), va_arg(ap, int));
        break;
    case eFatalError_replay_file_S_has_invalid_size_D_bytes:
        sprintf(buffer, "replay file %s has invalid size : %d bytes", g_Current_file_path, va_arg(ap, int));
        break;
    case eFatalError_invalid_string_in_parameter_list:
        strcpy(buffer, "invalid string in parameter list");
        break;
    case eFatalError_repeated_file_in_parameter_list:
        sprintf(buffer, "repeated file in parameter list : %s", g_Current_file_path);
        break;
    case eFatalError_write_failure_on_file:
        sprintf(buffer, "Write failure on file : %s", g_Current_file_path);
        *read_error = 1;
        break;
    case eFatalError_writing_zero_bytes_to_file:
        sprintf(buffer, "Writing zero bytes to file : %s", g_Current_file_path);
        break;
    case eFatalError_ground_blocks_in_path_exceed_maximum_size_by_D_byte:
        sprintf(buffer, "Ground blocks in %s exceed maximum size by %d bytes", g_Current_file_path, va_arg(ap, int));
        break;
    case eFatalError_sprite_data_in_path_exceed_maximum_size_by_D_byte:
        sprintf(buffer, "Sprite data in %s exceed maximum size by %d bytes", g_Current_file_path, va_arg(ap, int));
        break;
    case eFatalError_maximum_roadblock_nodes_exceeded:
        strcpy(buffer, "Maximum roadblock nodes exceeded");
        break;
    case eFatalError_invalid_number_of_players:
        strcpy(buffer, "Invalid number of players");
        break;
    case eFatalError_network_error_D:
        sprintf(buffer, "Network error : %d", va_arg(ap, int));
        *network_error = 1;
        break;
    case eFatalError_network_startup_abandoned:
        strcpy(buffer, "Network startup abandoned");
        *network_error = 1;
        break;
    case eFatalError_network_active_nodes_failure:
        strcpy(buffer, "Network active nodes failure");
        *network_error = 1;
        break;
    case eFatalError_network_version_mismatc:
        strcpy(buffer, "Network version mismatch");
        *network_error = 1;
        break;
    case eFatalError_invalid_xfer_type_received_D:
        sprintf(buffer, "invalid xfer type received : %d", va_arg(ap, int));
        *network_error = 1;
        break;
    case eFatalError_invalid_cycle_received_D:
        sprintf(buffer, "invalid cycle received : %d", va_arg(ap, int));
        *network_error = 1;
        break;
    case eFatalError_timed_out_transmitting_to_node_D:
        sprintf(buffer, "timed out transmitting to node %d", va_arg(ap, int));
        *network_error = 1;
        break;
    case eFatalError_repeat_data_received_from_node_D:
        sprintf(buffer, "repeat data received from node %d", va_arg(ap, int));
        *network_error = 1;
        break;
    case eFatalError_network_timed_out_receiving_data:
        strcpy(buffer, "network timed out receiving data");
        *network_error = 1;
        break;
    case eFatalError_network_message_received_from_same_console:
        strcpy(buffer, "network message received from same console");
        *network_error = 1;
        break;
    case eFatalError_invalid_data_in_startup_ini_file:
        strcpy(buffer, "invalid data in startup.ini file");
        break;
    case eFatalError_joystick_D_calibration_data_missing:
        sprintf(buffer, "joystick %d calibration data missing", va_arg(ap, int));
        break;
    case eFatalError_joystick_D_disconnected:
        sprintf(buffer, "joystick %d disconnected", va_arg(ap, int));
        break;
    case eFatalError_mission_template_D_not_available:
        sprintf(buffer, "mission template %d not available", va_arg(ap, int));
        break;
    case eFatalError_async_faces_overflow:
        strcpy(buffer, "Async faces overflow");
        break;
    case eFatalError_out_of_lids:
        strcpy(buffer, "out of lids");
        break;
    case eFatalError_out_of_sides:
        strcpy(buffer, "out of sides");
        break;
    case eFatalError_invalid_block_for_face_access:
        strcpy(buffer, "invalid block for face access");
        break;
    case eFatalError_invalid_case_D:
        sprintf(buffer, "invalid case : %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_async_code_D:
        sprintf(buffer, "invalid async code : %d", va_arg(ap, int));
        break;
    case eFatalError_too_many_frames_D:
        sprintf(buffer, "too many frames : %d", va_arg(ap, int));
        break;
    case eFatalError_alarm_list_overflow:
        strcpy(buffer, "alarm list overflow");
        break;
    case eFatalError_create_car_failed_on_type_D:
        sprintf(buffer, "create car failed on type %d", va_arg(ap, int));
        break;
    case eFatalError_door_overflow:
        strcpy(buffer, "door overflow");
        break;
    case eFatalError_alarm_type_D_not_found:
        sprintf(buffer, "alarm type %d not found", va_arg(ap, int));
        break;
    case eFatalError_create_ped_failed:
        strcpy(buffer, "create ped failed");
        break;
    case eFatalError_incorrect_vesa_bios_version_DD_v2_0_required:
        sprintf(buffer, "incorrect Vesa Bios version ( %d.%d ) - v2.0 required", va_arg(ap, int), va_arg(ap, int));
        break;
    case eFatalError_no_cars_left_to_allocate_after_D:
        sprintf(buffer, "no cars left to allocate after %d", va_arg(ap, int));
        break;
    case eFatalError_run_the_miles_design_setsound_utility:
        sprintf(buffer, "run the Miles Design SETSOUND utility");
        break;
    case eFatalError_text_identifier_S_not_found_in_language_file:
        sprintf(buffer, "text identifier [%s] not found in language file", va_arg(ap, const char *));
        break;
    case eFatalError_invalid_format_in_text_file:
        sprintf(buffer, "invalid format in text file : %s", g_Current_file_path);
        break;
    case eFatalError_cannot_find_S_in_file:
        sprintf(buffer, "cannot find %s in file %s", va_arg(ap, const char *), g_Current_file_path);
        break;
    case eFatalError_actual_mission_D_not_available:
        sprintf(buffer, "actual mission %d not available", va_arg(ap, int));
        break;
    case eFatalError_invalid_integer_encountered_in_file:
        sprintf(buffer, "invalid integer encountered in file %s", g_Current_file_path);
        break;
    case eFatalError_text_string_too_long_in_file:
        sprintf(buffer, "text string too long in file %s", g_Current_file_path);
        break;
    case eFatalError_end_of_file_in_text_string_in_file:
        sprintf(buffer, "end of file in text string in file %s", g_Current_file_path);
        break;
    case eFatalError_map_name_not_specified:
        strcpy(buffer, "map name not specified");
        break;
    case eFatalError_style_number_not_specified:
        strcpy(buffer, "style number not specified");
        break;
    case eFatalError_ungetc_failure_in_file:
        sprintf(buffer, "ungetc failure in file %s", g_Current_file_path);
        break;
    case eFatalError_missing_C_in_path:
        sprintf(buffer, "missing '%c' in %s", va_arg(ap, int), g_Current_file_path);
        break;
    case eFatalError_navigational_data_in_path_is_invalid_size_D_bytes:
        sprintf(buffer, "navigational data in %s is invalid size (%d bytes)", g_Current_file_path, va_arg(ap, int));
        break;
    case eFatalError_ipx_not_installed:
        strcpy(buffer, "IPX not installed");
        break;
    case eFatalError_ipx_mac_packet_size_invalid_D:
        sprintf(buffer, "IPX mac packet size invalid : %d", va_arg(ap, int));
        break;
    case eFatalError_ipx_socket_table_full:
        strcpy(buffer, "IPX socket table full");
        break;
    case eFatalError_ipx_socket_D_already_open:
        sprintf(buffer, "IPX socket %d already open", va_arg(ap, int));
        break;
    case eFatalError_ipx_wrong_socket_opened_D:
        sprintf(buffer, "IPX wrong socket opened : %d", va_arg(ap, int));
        break;
    case eFatalError_ipx_listen_failure_code_D:
        sprintf(buffer, "IPX listen failure, code : %d", va_arg(ap, int));
        break;
    case eFatalError_ipx_max_send_time_exceeded:
        strcpy(buffer, "IPX max send time exceeded");
        break;
    case eFatalError_ipx_out_packet_max_size_exceeded_trying_D_bytes:
        sprintf(buffer, "IPX out packet max size exceeded, trying %d bytes", va_arg(ap, int));
        break;
    case eFatalError_ipx_received_data_max_size_exceeded_trying_D_bytes:
        sprintf(buffer, "IPX received data max size exceeded, trying %d bytes", va_arg(ap, int));
        break;
    case eFatalError_no_lives_nodes_found:
        strcpy(buffer, "no live nodes found");
        break;
    case eFatalError_invalid_serial_port_number_D:
        sprintf(buffer, "invalid serial port number : %d", va_arg(ap, int));
        break;
    case eFatalError_uart_not_responding_on_comD:
        sprintf(buffer, "UART not responding on COM%d", va_arg(ap, int));
        break;
    case eFatalError_message_too_long_at_D_chars:
        sprintf(buffer, "Message too long at %d chars", va_arg(ap, int));
        break;
    case eFatalError_map_contains_unconnected_road_with_direction:
        strcpy(buffer, "Map contains unconnected road with direction");
        break;
    case eFatalError_no_traffic_light_error:
        strcpy(buffer, "No Traffic Light Error");
        break;
    case eFatalError_general_traffic_light_error:
        strcpy(buffer, "General Traffic Light Error");
        break;
    case eFatalError_non_existant_token_used:
        strcpy(buffer, "Non Existant Token used");
        break;
    case eFatalError_non_existant_traffic_light_used:
        strcpy(buffer, "Non Existant Traffic Light used");
        break;
    case eFatalError_coord_range_error:
        strcpy(buffer, "Coord Range Error");
        break;
    case eFatalError_traffic_lights_not_initializd:
        strcpy(buffer, "Traffic Lights not initializd");
        break;
    case eFatalError_data_range_error:
        strcpy(buffer, "Data Range Error");
        break;
    case eFatalError_too_many_traffic_lights_used:
        strcpy(buffer, "Too Many Traffic Lights used");
        break;
    case eFatalError_unrecognized_junction:
        strcpy(buffer, "Unrecognized Junction");
        break;
    case eFatalError_too_many_lights_in_junction:
        strcpy(buffer, "Too Many Lights in Junction");
        break;
    case eFatalError_no_rail_found_around_DDD:
        sprintf(buffer, "No Rail Found around (%d,%d,%d)", va_arg(ap, int), va_arg(ap, int), va_arg(ap, int));
        break;
    case eFatalError_no_rail_junction_rail_light_found:
        strcpy(buffer, "No Rail Junction Rail light found");
        break;
    case eFatalError_switch_and_crossing_too_close:
        strcpy(buffer, "Switch and Crossing too close");
        break;
    case eFatalError_unrecognized_switch_or_station:
        strcpy(buffer, "Unrecognized Switch or Station");
        break;
    case eFatalError_no_station_found_on_track:
        strcpy(buffer, "No Station found on Track");
        break;
    case eFatalError_incorrect_number_of_players_for_network_game:
        sprintf(buffer, "Incorrect number of players for network game.");
        break;
    case eFatalError_list_error_see_ian_D:
        sprintf(buffer, "List Error see Ian %d", va_arg(ap, int));
        break;
    case eFatalError_adding_twice_inform_ian_D:
        sprintf(buffer, "ADDING TWICE inform Ian %d", va_arg(ap, int));
        break;
    case eFatalError_getting_car_lt_zero_D:
        sprintf(buffer, "Getting car < zero..: %d", va_arg(ap, int));
        break;
    case eFatalError_getting_ped_lt_zero_D:
        sprintf(buffer, "Getting ped < zero..: %d", va_arg(ap, int));
        break;
    case eFatalError_getting_invalid_object_D:
        sprintf(buffer, "Getting invalid object.. : %d", va_arg(ap, int));
        break;
    case eFatalError_routefinder_given_invalid_start_coordinates:
        sprintf(buffer, "Routefinder given invalid start coordinates.");
        break;
    case eFatalError_routefinder_given_invalid_target_coordinates:
        sprintf(buffer, "Routefinder given invalid target coordinates.");
        break;
    case eFatalError_railway_track_invalid:
        strcpy(buffer, "Railway track invalid");
        break;
    case eFatalError_text_is_too_long_S:
        strncpy(static_buffer_0050f5c0, va_arg(ap, const char *), sizeof(static_buffer_0050f5c0 - 1));
        sprintf(buffer, "Text is too long : %s", static_buffer_0050f5c0);
        break;
    case eFatalError_invalid_crime_access_D:
        sprintf(buffer, "invalid crime access : %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_config_name_S:
        sprintf(buffer, "invalid config name : %s", va_arg(ap, const char *));
        break;
    case eFatalError_invalid_tile_D_at_DDD:
        sprintf(buffer, "Invalid tile %d at (%d,%d,%d)", va_arg(ap, int), va_arg(ap, int), va_arg(ap, int), va_arg(ap, int));
        break;
    case eFatalError_unable_to_place_car_type_D_at_DDD:
        sprintf(buffer, "Unable to place car type %d at (%d,%d,%d)", va_arg(ap, int), va_arg(ap, int), va_arg(ap, int), va_arg(ap, int));
        break;
    case eFatalError_invalid_map_access_at_DDD:
        sprintf(buffer, "Invalid map access at (%d,%d,%d)", va_arg(ap, int), va_arg(ap, int), va_arg(ap, int));
        break;
    case eFatalError_D_map_errors:
        sprintf(buffer, "%d map errors\n", va_arg(ap, int));
        break;
    case eFatalError_invalid_grid_width_D:
        sprintf(buffer, "invalid grid width : %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_grid_height_D:
        sprintf(buffer, "invalid grid height : %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_alarm_wait_D:
        sprintf(buffer, "Invalid alarm : wait = %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_hunt_access_D:
        sprintf(buffer, "Invalid hunt access : %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_fire_access_D:
        sprintf(buffer, "Invalid fire access : %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_tv_van_access_D:
        sprintf(buffer, "Invalid tv van access : %d", va_arg(ap, int));
        break;
    case eFatalError_invalid_stage_access_D:
        sprintf(buffer, "Invalid stage access : %d", va_arg(ap, int));
        break;
    case eFatalError_unable_to_allocate_timer_handle:
        strcpy(buffer, "unable to allocate timer handle");
        break;
    case eFatalError_invalid_hells_angel_access_D:
        sprintf(buffer, "Invalid Hells Angel access : %d", va_arg(ap, int));
        break;
    case eFatalError_illegal_request_for_a_sentinel:
        sprintf(buffer, "Illegal request for a sentinel");
        break;
    case eFatalError_not_valid_console_D:
        sprintf(buffer, "Not valid console : %d", va_arg(ap, int));
        break;
    case eFatalError_modem_configuration_data_missing:
        strcpy(buffer, "modem configuration data missing");
        break;
    case eFatalError_too_many_triggers_defined_D:
        sprintf(buffer, "Too many triggers defined: %d", va_arg(ap, int));
        break;
    default:
        strcpy(buffer, "Unknown error");
        break;
    }
}
