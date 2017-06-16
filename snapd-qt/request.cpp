/*
 * Copyright (C) 2016 Canonical Ltd.
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2 or version 3 of the License.
 * See http://www.gnu.org/copyleft/lgpl.html the full text of the license.
 */

#include <snapd-glib/snapd-glib.h>

#include "Snapd/request.h"

struct QSnapdRequestPrivate
{
    QSnapdRequestPrivate (void *snapd_client)
    {
        client = SNAPD_CLIENT (g_object_ref (snapd_client));
        cancellable = g_cancellable_new ();
    }

    ~QSnapdRequestPrivate ()
    {
        g_cancellable_cancel (cancellable);
        g_object_unref (cancellable);
        g_object_unref (client);
        if (change != NULL)
            g_object_unref (change);
    }

    SnapdClient *client;
    GCancellable *cancellable;
    bool finished;
    QSnapdRequest::QSnapdError error;
    QString errorString;
    SnapdChange *change = NULL;
};

QSnapdRequest::QSnapdRequest (void *snapd_client, QObject *parent) :
    QObject (parent),
    d_ptr (new QSnapdRequestPrivate (snapd_client)) {}

QSnapdRequest::~QSnapdRequest ()
{
    delete d_ptr;
}

void* QSnapdRequest::getClient () const
{
    Q_D(const QSnapdRequest);
    return d->client;
}

void* QSnapdRequest::getCancellable () const
{
    Q_D(const QSnapdRequest);
    return d->cancellable;
}

void QSnapdRequest::finish (void *error)
{
    Q_D(QSnapdRequest);

    d->finished = true;
    if (error == NULL) {
        d->error = NoError;
        d->errorString = "";
    }
    else {
        GError *e = (GError *) error;
        if (e->domain == SNAPD_ERROR) {
            switch ((SnapdError) e->code)
            {
            case SNAPD_ERROR_CONNECTION_FAILED:
                d->error = ConnectionFailed;
                break;
            case SNAPD_ERROR_WRITE_FAILED:
                d->error = WriteFailed;
                break;
            case SNAPD_ERROR_READ_FAILED:
                d->error = ReadFailed;
                break;
            case SNAPD_ERROR_BAD_REQUEST:
                d->error = BadRequest;
                break;
            case SNAPD_ERROR_BAD_RESPONSE:
                d->error = BadResponse;
                break;
            case SNAPD_ERROR_AUTH_DATA_REQUIRED:
                d->error = AuthDataRequired;
                break;
            case SNAPD_ERROR_AUTH_DATA_INVALID:
                d->error = AuthDataInvalid;
                break;
            case SNAPD_ERROR_TWO_FACTOR_REQUIRED:
                d->error = TwoFactorRequired;
                break;
            case SNAPD_ERROR_TWO_FACTOR_INVALID:
                d->error = TwoFactorInvalid;
                break;
            case SNAPD_ERROR_PERMISSION_DENIED:
                d->error = PermissionDenied;
                break;
            case SNAPD_ERROR_FAILED:
                d->error = Failed;
                break;
            case SNAPD_ERROR_TERMS_NOT_ACCEPTED:
                d->error = TermsNotAccepted;
                break;
            case SNAPD_ERROR_PAYMENT_NOT_SETUP:
                d->error = PaymentNotSetup;
                break;
            case SNAPD_ERROR_PAYMENT_DECLINED:
                d->error = PaymentDeclined;
                break;
            case SNAPD_ERROR_ALREADY_INSTALLED:
                d->error = AlreadyInstalled;
                break;
            case SNAPD_ERROR_NOT_INSTALLED:
                d->error = NotInstalled;
                break;
            case SNAPD_ERROR_NO_UPDATE_AVAILABLE:
                d->error = NoUpdateAvailable;
                break;
            case SNAPD_ERROR_PASSWORD_POLICY_ERROR:
                d->error = PasswordPolicyError;
                break;
            case SNAPD_ERROR_NEEDS_DEVMODE:
                d->error = NeedsDevmode;
                break;
            case SNAPD_ERROR_NEEDS_CLASSIC:
                d->error = NeedsClassic;
                break;
            case SNAPD_ERROR_NEEDS_CLASSIC_SYSTEM:
                d->error = NeedsClassicSystem;
                break;
            default:
                /* This indicates we should add a new entry here... */
                d->error = UnknownError;
                break;
            }
        }
        else
            d->error = UnknownError;
        d->errorString = e->message;
    }
    emit complete ();
}

bool QSnapdRequest::isFinished () const
{
    Q_D(const QSnapdRequest);
    return d->finished;
}

QSnapdRequest::QSnapdError QSnapdRequest::error () const
{
    Q_D(const QSnapdRequest);
    return d->error;
}

QString QSnapdRequest::errorString () const
{
    Q_D(const QSnapdRequest);
    return d->errorString;
}

void QSnapdRequest::cancel ()
{
    Q_D(QSnapdRequest);
    g_cancellable_cancel (d->cancellable);
}

void QSnapdRequest::handleProgress (void *change)
{
    Q_D(QSnapdRequest);
    d->change = SNAPD_CHANGE (g_object_ref (change));
    emit progress ();
}

QSnapdChange *QSnapdRequest::change () const
{
    Q_D(const QSnapdRequest);
    return new QSnapdChange (d->change);
}
