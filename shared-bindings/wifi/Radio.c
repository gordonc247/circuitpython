/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 Scott Shawcroft for Adafruit Industries
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <string.h>

#include "py/objproperty.h"
#include "py/runtime.h"
#include "shared-bindings/wifi/__init__.h"

//| class Radio:
//|     """Native wifi radio.
//|
//|        This class manages the station and access point functionality of the native
//|        Wifi radio.
//|
//|     """
//|

//|     def __init__(self) -> None:
//|         """You cannot create an instance of `wifi.Radio`.
//|         Use `wifi.radio` to access the sole instance available."""
//|         ...
//|

//|     enabled: bool
//|     """True when the wifi radio is enabled."""
//|
STATIC mp_obj_t wifi_radio_get_enabled(mp_obj_t self) {
    return mp_obj_new_bool(common_hal_wifi_radio_get_enabled(self));

}
MP_DEFINE_CONST_FUN_OBJ_1(wifi_radio_get_enabled_obj, wifi_radio_get_enabled);

const mp_obj_property_t wifi_radio_enabled_obj = {
    .base.type = &mp_type_property,
    .proxy = { (mp_obj_t)&wifi_radio_get_enabled_obj,
               (mp_obj_t)&mp_const_none_obj,
               (mp_obj_t)&mp_const_none_obj },
};

//|     mac_address: Address
//|     """MAC address of the wifi radio. (read-only)"""
//|
STATIC mp_obj_t wifi_radio_get_mac_address(mp_obj_t self) {
    return MP_OBJ_FROM_PTR(common_hal_wifi_radio_get_mac_address(self));

}
MP_DEFINE_CONST_FUN_OBJ_1(wifi_radio_get_mac_address_obj, wifi_radio_get_mac_address);

const mp_obj_property_t wifi_radio_mac_address_obj = {
    .base.type = &mp_type_property,
    .proxy = { (mp_obj_t)&wifi_radio_get_mac_address_obj,
               (mp_obj_t)&mp_const_none_obj,
               (mp_obj_t)&mp_const_none_obj },
};


//|     def start_scanning_networks(self, *, start_channel=1, stop_channel=11) -> Iterable[Network]:
//|         """Scans for available wifi networks over the given channel range. Make sure the channels are allowed in your country."""
//|         ...
//|
STATIC mp_obj_t wifi_radio_start_scanning_networks(mp_obj_t self_in) {
    wifi_radio_obj_t *self = MP_OBJ_TO_PTR(self_in);

    return common_hal_wifi_radio_start_scanning_networks(self);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(wifi_radio_start_scanning_networks_obj, wifi_radio_start_scanning_networks);

//|     def stop_scanning_networks(self) -> None:
//|         """Stop scanning for Wifi networks and free any resources used to do it."""
//|         ...
//|
STATIC mp_obj_t wifi_radio_stop_scanning_networks(mp_obj_t self_in) {
    wifi_radio_obj_t *self = MP_OBJ_TO_PTR(self_in);

    common_hal_wifi_radio_stop_scanning_networks(self);

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(wifi_radio_stop_scanning_networks_obj, wifi_radio_stop_scanning_networks);

//|     def connect(self, ssid: ReadableBuffer, password: ReadableBuffer = b"", *, channel: Optional[int] = 0, timeout: Optional[float] = None) -> bool:
//|         """Connects to the given ssid and waits for an ip address. Reconnections are handled
//|            automatically once one connection succeeds."""
//|         ...
//|
STATIC mp_obj_t wifi_radio_connect(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_ssid, ARG_password, ARG_channel, ARG_timeout };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_ssid, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
        { MP_QSTR_password,  MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
        { MP_QSTR_channel, MP_ARG_KW_ONLY |  MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_timeout, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_obj = mp_const_none} },
    };

    wifi_radio_obj_t *self = MP_OBJ_TO_PTR(pos_args[0]);
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    mp_float_t timeout = 0;
    if (args[ARG_timeout].u_obj != mp_const_none) {
        timeout = mp_obj_get_float(args[ARG_timeout].u_obj);
    }


    mp_buffer_info_t ssid;
    ssid.len = 0;
    if (args[ARG_ssid].u_obj != MP_OBJ_NULL) {
        mp_get_buffer_raise(args[ARG_ssid].u_obj, &ssid, MP_BUFFER_READ);
    }

    mp_buffer_info_t password;
    password.len = 0;
    if (args[ARG_password].u_obj != MP_OBJ_NULL) {
        mp_get_buffer_raise(args[ARG_password].u_obj, &password, MP_BUFFER_READ);
    }

    return mp_obj_new_bool(common_hal_wifi_radio_connect(self, ssid.buf, ssid.len, password.buf, password.len, args[ARG_channel].u_int, timeout));
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(wifi_radio_connect_obj, 1, wifi_radio_connect);

//|     ipv4_address: Optional[ipaddress.IPv4Address]
//|     """IP v4 Address of the radio when connected to an access point. None otherwise."""
//|
STATIC mp_obj_t wifi_radio_get_ipv4_address(mp_obj_t self) {
    return common_hal_wifi_radio_get_ipv4_address(self);

}
MP_DEFINE_CONST_FUN_OBJ_1(wifi_radio_get_ipv4_address_obj, wifi_radio_get_ipv4_address);

const mp_obj_property_t wifi_radio_ipv4_address_obj = {
    .base.type = &mp_type_property,
    .proxy = { (mp_obj_t)&wifi_radio_get_ipv4_address_obj,
               (mp_obj_t)&mp_const_none_obj,
               (mp_obj_t)&mp_const_none_obj },
};

//|     def ping(self, ip, *, timeout: float = 0.5) -> float:
//|         """Ping an IP to test connectivity. Returns echo time in seconds."""
//|         ...
//|
STATIC mp_obj_t wifi_radio_ping(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_ip, ARG_timeout };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_ip, MP_ARG_REQUIRED | MP_ARG_OBJ, },
        { MP_QSTR_timeout, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_obj = mp_const_none} },
    };

    wifi_radio_obj_t *self = MP_OBJ_TO_PTR(pos_args[0]);
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    mp_float_t timeout = 0.5;
    if (args[ARG_timeout].u_obj != mp_const_none) {
        timeout = mp_obj_get_float(args[ARG_timeout].u_obj);
    }

    mp_int_t time_ms = common_hal_wifi_radio_ping(self, args[ARG_ip].u_obj, timeout);

    return mp_obj_new_float(time_ms / 1000);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(wifi_radio_ping_obj, 1, wifi_radio_ping);

STATIC const mp_rom_map_elem_t wifi_radio_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_enabled), MP_ROM_PTR(&wifi_radio_enabled_obj) },
    { MP_ROM_QSTR(MP_QSTR_mac_address), MP_ROM_PTR(&wifi_radio_mac_address_obj) },

    { MP_ROM_QSTR(MP_QSTR_start_scanning_networks),    MP_ROM_PTR(&wifi_radio_start_scanning_networks_obj) },
    { MP_ROM_QSTR(MP_QSTR_stop_scanning_networks),    MP_ROM_PTR(&wifi_radio_stop_scanning_networks_obj) },

    { MP_ROM_QSTR(MP_QSTR_connect),    MP_ROM_PTR(&wifi_radio_connect_obj) },
    // { MP_ROM_QSTR(MP_QSTR_connect_to_enterprise),    MP_ROM_PTR(&wifi_radio_connect_to_enterprise_obj) },

    { MP_ROM_QSTR(MP_QSTR_ipv4_address),    MP_ROM_PTR(&wifi_radio_ipv4_address_obj) },

    // { MP_ROM_QSTR(MP_QSTR_access_point_active),   MP_ROM_PTR(&wifi_radio_access_point_active_obj) },
    // { MP_ROM_QSTR(MP_QSTR_start_access_point), MP_ROM_PTR(&wifi_radio_start_access_point_obj) },

    { MP_ROM_QSTR(MP_QSTR_ping), MP_ROM_PTR(&wifi_radio_ping_obj) },
};

STATIC MP_DEFINE_CONST_DICT(wifi_radio_locals_dict, wifi_radio_locals_dict_table);

const mp_obj_type_t wifi_radio_type = {
    .base = { &mp_type_type },
    .name = MP_QSTR_Radio,
    .locals_dict = (mp_obj_t)&wifi_radio_locals_dict,
};
