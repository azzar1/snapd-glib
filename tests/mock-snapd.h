/*
 * Copyright (C) 2017 Canonical Ltd.
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2 or version 3 of the License.
 * See http://www.gnu.org/copyleft/lgpl.html the full text of the license.
 */

#ifndef __MOCK_SNAPD_H__
#define __MOCK_SNAPD_H__

#include <glib-object.h>
#include <gio/gio.h>

G_BEGIN_DECLS

#define MOCK_TYPE_SNAPD  (mock_snapd_get_type ())

G_DECLARE_FINAL_TYPE (MockSnapd, mock_snapd, MOCK, SNAPD, GObject)

struct _MockSnapdClass
{
    /*< private >*/
    GObjectClass parent_class;
};

typedef struct
{
    int id;
    gchar *email;
    gchar *username;
    gchar *password;
    gchar *otp;
    gchar **ssh_keys;
    gchar *macaroon;
    gchar **discharges;
    gboolean sudoer;
    gboolean known;
    gboolean terms_accepted;
    gboolean has_payment_methods;
    GList *private_snaps;
} MockAccount;

typedef struct
{
    gchar *name;
    gboolean automatic;
    gboolean enabled;
} MockAlias;

typedef struct
{
    GList *apps;
    gchar *broken;
    gchar *channel;
    gchar *confinement;
    gchar *contact;
    gchar *description;
    gchar *developer;
    gboolean devmode;
    int download_size;
    gchar *icon;
    gchar *icon_mime_type;
    GBytes *icon_data;
    gchar *id;
    gchar *install_date;
    int installed_size;
    gboolean jailmode;
    gchar *license;
    gchar *name;
    GList *prices;
    gboolean is_private;
    gchar *revision;
    GList *screenshots;
    gchar *status;
    gchar *summary;
    gchar *title;
    gchar *tracking_channel;
    GList *tracks;
    gboolean trymode;
    gchar *type;
    gchar *version;
    GList *store_sections;
    GList *plugs;
    GList *slots_;
    gboolean disabled;
    gboolean dangerous;
    gchar *snap_data;
    gchar *snap_path;
    gchar *error;
    gboolean restart_required;
    gboolean preferred;
} MockSnap;

typedef struct
{
    gchar *name;
    gchar *daemon;
    gchar *desktop_file;
    gboolean enabled;
    gboolean active;
    GList *aliases;
} MockApp;

typedef struct
{
    gchar *name;
    GList *channels;
} MockTrack;

typedef struct
{
    gchar *risk;
    gchar *branch;
    gchar *confinement;
    gchar *epoch;
    gchar *revision;
    int size;
    gchar *version;
} MockChannel;

typedef struct
{
    gdouble amount;
    gchar *currency;
} MockPrice;

typedef struct
{
    gchar *url;
    int width;
    int height;
} MockScreenshot;

typedef struct _MockSlot MockSlot;

typedef struct
{
    MockSnap *snap;
    gchar *name;
    gchar *interface;
    // FIXME: Attributes
    gchar *label;
    MockSlot *connection;
} MockPlug;

struct _MockSlot
{
    MockSnap *snap;
    gchar *name;
    gchar *interface;
    // FIXME: Attributes
    gchar *label;
};

MockSnapd      *mock_snapd_new                    (void);

const gchar    *mock_snapd_get_socket_path        (MockSnapd     *snapd);

void            mock_snapd_set_close_on_request   (MockSnapd     *snapd,
                                                   gboolean       close_on_request);

gboolean        mock_snapd_start                  (MockSnapd     *snapd,
                                                   GError       **error);

void            mock_snapd_stop                   (MockSnapd     *snapd);

void            mock_snapd_set_confinement        (MockSnapd     *snapd,
                                                   const gchar   *confinement);

void            mock_snapd_set_store              (MockSnapd     *snapd,
                                                   const gchar   *name);

void            mock_snapd_set_managed            (MockSnapd     *snapd,
                                                   gboolean       managed);

void            mock_snapd_set_on_classic         (MockSnapd     *snapd,
                                                   gboolean       on_classic);

void            mock_snapd_set_suggested_currency (MockSnapd     *snapd,
                                                   const gchar   *currency);

void            mock_snapd_set_progress_total     (MockSnapd     *snapd,
                                                   int            total);

void            mock_snapd_set_spawn_time         (MockSnapd     *snapd,
                                                   const gchar   *spawn_time);

void            mock_snapd_set_ready_time         (MockSnapd     *snapd,
                                                   const gchar   *ready_time);

MockAccount    *mock_snapd_add_account            (MockSnapd     *snapd,
                                                   const gchar   *email,
                                                   const gchar   *username,
                                                   const gchar   *password);

void            mock_account_set_otp              (MockAccount   *account,
                                                   const gchar   *otp);

void            mock_account_set_ssh_keys         (MockAccount   *account,
                                                   gchar        **ssh_keys);

MockAccount    *mock_snapd_find_account_by_username (MockSnapd     *snapd,
                                                     const gchar   *username);

MockAccount    *mock_snapd_find_account_by_email  (MockSnapd     *snapd,
                                                   const gchar   *email);

MockSnap       *mock_account_add_private_snap     (MockAccount   *account,
                                                   const gchar   *name);

MockSnap       *mock_snapd_add_snap               (MockSnapd     *snapd,
                                                   const gchar   *name);

MockSnap       *mock_snapd_find_snap              (MockSnapd     *snapd,
                                                   const gchar   *name);

void            mock_snapd_add_store_section      (MockSnapd     *snapd,
                                                   const gchar   *name);

MockSnap       *mock_snapd_add_store_snap         (MockSnapd     *snapd,
                                                   const gchar   *name);

MockApp        *mock_snap_add_app                 (MockSnap      *snap,
                                                   const gchar   *name);

void            mock_app_set_daemon               (MockApp       *app,
                                                   const gchar   *daemon);

void            mock_app_set_desktop_file         (MockApp       *app,
                                                   const gchar   *desktop_file);

void            mock_app_add_auto_alias           (MockApp       *app,
                                                   const gchar   *alias);

void            mock_app_add_manual_alias         (MockApp       *app,
                                                   const gchar   *alias,
                                                   gboolean       enabled);

MockAlias      *mock_app_find_alias               (MockApp       *app,
                                                   const gchar   *name);

void            mock_snap_set_broken              (MockSnap      *snap,
                                                   const gchar   *broken);

void            mock_snap_set_channel             (MockSnap      *snap,
                                                   const gchar   *channel);

MockTrack      *mock_snap_add_track               (MockSnap      *snap,
                                                   const gchar   *name);

MockChannel    *mock_track_add_channel            (MockTrack     *track,
                                                   const gchar   *risk,
                                                   const gchar   *branch);

void            mock_channel_set_confinement      (MockChannel  *channel,
                                                   const gchar   *confinement);

void            mock_channel_set_epoch            (MockChannel  *channel,
                                                   const gchar   *epoch);

void            mock_channel_set_revision         (MockChannel  *channel,
                                                   const gchar   *revision);

void            mock_channel_set_version          (MockChannel  *channel,
                                                   const gchar   *version);

void            mock_snap_set_confinement         (MockSnap      *snap,
                                                   const gchar   *confinement);

void            mock_snap_set_contact             (MockSnap      *snap,
                                                   const gchar   *contact);

void            mock_snap_set_description         (MockSnap      *snap,
                                                   const gchar   *description);

void            mock_snap_set_developer           (MockSnap      *snap,
                                                   const gchar   *developer);

void            mock_snap_set_error               (MockSnap      *snap,
                                                   const gchar   *error);

void            mock_snap_set_icon                (MockSnap      *snap,
                                                   const gchar   *icon);

void            mock_snap_set_icon_data           (MockSnap      *snap,
                                                   const gchar   *mime_type,
                                                   GBytes        *data);

void            mock_snap_set_id                  (MockSnap      *snap,
                                                   const gchar   *id);

void            mock_snap_set_install_date        (MockSnap      *snap,
                                                   const gchar   *install_date);

void            mock_snap_set_license             (MockSnap      *snap,
                                                   const gchar   *license);

MockPrice      *mock_snap_add_price               (MockSnap      *snap,
                                                   gdouble        amount,
                                                   const gchar   *currency);

void            mock_snap_set_revision            (MockSnap      *snap,
                                                   const gchar   *revision);

MockScreenshot *mock_snap_add_screenshot          (MockSnap      *snap,
                                                   const gchar   *url,
                                                   int            width,
                                                   int            height);

void            mock_snap_set_status              (MockSnap      *snap,
                                                   const gchar   *status);

void            mock_snap_set_summary             (MockSnap      *snap,
                                                   const gchar   *summary);

void            mock_snap_set_title               (MockSnap      *snap,
                                                   const gchar   *title);

void            mock_snap_set_tracking_channel    (MockSnap      *snap,
                                                   const gchar   *channel);

void            mock_snap_set_type                (MockSnap      *snap,
                                                   const gchar   *type);

void            mock_snap_set_version             (MockSnap      *snap,
                                                   const gchar   *version);

void            mock_snap_add_store_section       (MockSnap      *snap,
                                                   const gchar   *section);

MockPlug       *mock_snap_add_plug                (MockSnap      *snap,
                                                   const gchar   *name);

MockSlot       *mock_snap_add_slot                (MockSnap      *snap,
                                                   const gchar   *name);

void            mock_snapd_add_assertion          (MockSnapd     *snapd,
                                                   const gchar   *assertion);

GList          *mock_snapd_get_assertions         (MockSnapd     *snapd);

const gchar    *mock_snapd_get_last_user_agent    (MockSnapd     *snapd);

const gchar    *mock_snapd_get_last_accept_language (MockSnapd     *snapd);

const gchar    *mock_snapd_get_last_allow_interaction (MockSnapd *snapd);

G_END_DECLS

#endif /* __MOCK_SNAPD_H__ */
