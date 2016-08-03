/*
 * Copyright (c) 2012 - 2016 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <Eldbus.h>

#define CBHM_DBUS_OBJPATH "/org/tizen/cbhm/dbus"
#ifndef CBHM_DBUS_INTERFACE
#define CBHM_DBUS_INTERFACE "org.tizen.cbhm.dbus"
#endif /* CBHM_DBUS_INTERFACE */

static Eldbus_Proxy      *eldbus_proxy = NULL;
static Eldbus_Connection *cbhm_conn    = NULL;

Eldbus_Proxy* cbhm_proxy_get()
{
    return eldbus_proxy;
}

static void _cbhm_on_name_owner_changed(void *data EINA_UNUSED,
      const char *bus EINA_UNUSED, const char *old_id EINA_UNUSED,
      const char *new_id EINA_UNUSED)
{
    /* If client should know the time clipboard service is started or stoped,
     * use this function. */
}

void cbhm_eldbus_init()
{
   Eldbus_Object *eldbus_obj;

   cbhm_conn = eldbus_connection_get(ELDBUS_CONNECTION_TYPE_SESSION);
   eldbus_obj = eldbus_object_get(cbhm_conn, CBHM_DBUS_INTERFACE, CBHM_DBUS_OBJPATH);
   eldbus_proxy = eldbus_proxy_get(eldbus_obj, CBHM_DBUS_INTERFACE);
   eldbus_name_owner_changed_callback_add(cbhm_conn, CBHM_DBUS_INTERFACE,
         _cbhm_on_name_owner_changed, cbhm_conn, EINA_TRUE);
}

void cbhm_eldbus_deinit()
{
   if (cbhm_conn)
      eldbus_connection_unref(cbhm_conn);
}

