/**
 * WarfaceBot, a blind XMPP client for Warface (FPS)
 * Copyright (C) 2015 Levak Borok <levak92@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <wb_tools.h>
#include <wb_stream.h>
#include <wb_session.h>
#include <wb_xmpp_wf.h>

#include <stdio.h>

enum e_notif_type
{
    NOTIF_ACHIEVEMENT = 4,
    NOTIF_FRIEND_REQUEST = 64,
    NOTIF_CONS_LOGIN = 256,
    NOTIF_ANNOUNCEMENT = 512,
};

void xmpp_iq_confirm_notification(const char *notif)
{
    char *notif_id = get_info(notif, "id='", "'", NULL);
    char *notif_type = get_info(notif, "type='", "'", NULL);

    if (notif_type == NULL)
    {
        fprintf(stderr, "Cannot determine notification type: \n%s\n", notif);
        return;
    }

    enum e_notif_type i_notif_type = strtol(notif_type, NULL, 10);

    switch (i_notif_type)
    {
        /* Accept any friend requests */
        case NOTIF_FRIEND_REQUEST:
            send_stream_format(session.wfs,
                               "<iq to='masterserver@warface/%s' type='get'>"
                               " <query xmlns='urn:cryonline:k01'>"
                               "  <confirm_notification>"
                               "   <notif id='%s' type='%s'>"
                               "    <confirmation result='0' status='%d'"
                               "                  location=''/>"
                               "   </notif>"
                               "  </confirm_notification>"
                               " </query>"
                               "</iq>",
                               session.channel, notif_id,
                               notif_type, session.status);
            break;
        default:
            break;
    }

    free(notif_type);
    free(notif_id);
}
