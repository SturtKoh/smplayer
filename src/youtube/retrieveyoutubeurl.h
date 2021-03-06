/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2013 Ricardo Villalba <rvm@users.sourceforge.net>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef _RETRIEVEYOUTUBEURL_
#define _RETRIEVEYOUTUBEURL_

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMap>

#define YT_GET_VIDEOINFO
//#define YT_DASH_SUPPORT

class RetrieveYoutubeUrl : public QObject
{
	Q_OBJECT

public:
	enum Quality { FLV_240p = 5, MP4_360p = 18, MP4_720p = 22, FLV_360p = 34,
                   FLV_480p = 35, MP4_1080p = 37, WEBM_360p = 43,
                   WEBM_480p = 44, WEBM_720p = 45, WEBM_1080p = 46,
                   DASH_AUDIO_MP4_48 = 139, DASH_AUDIO_MP4_128 = 140, DASH_AUDIO_MP4_256 = 141,
                   DASH_AUDIO_WEBM_128 = 171, DASH_AUDIO_WEBM_192 = 172,
                   DASH_VIDEO_1080p = 137, DASH_VIDEO_720p = 136,
                   DASH_VIDEO_480p = 135, DASH_VIDEO_360p = 134,
                   DASH_VIDEO_240p = 133 };

	RetrieveYoutubeUrl( QObject* parent = 0 );
	~RetrieveYoutubeUrl();

	void fetchPage(const QString & url);
	void close();

	static void setUserAgent(const QString & s) { user_agent = s; };
	static QString userAgent() { return user_agent; };

	void setPreferredQuality(Quality q) { preferred_quality = q; }
	Quality preferredQuality() { return preferred_quality; }

	static QString findPreferredUrl(const QMap<int, QString>& urlMap, Quality q);
	QString findPreferredUrl();

#ifdef YT_DASH_SUPPORT
	static QString findBestAudio(const QMap<int, QString>& urlMap);
#endif

	QString urlTitle() { return url_title; }
	QString latestPreferredUrl() { return latest_preferred_url; }
	QString origUrl() { return orig_url; }

	bool isUrlSupported(const QString & url);
	QString fullUrl(const QString & url);

signals:
	void gotUrls(const QMap<int, QString>&);
	void gotPreferredUrl(const QString &);
	void gotEmptyList();
#ifdef YT_GET_VIDEOINFO
	void gotVideoInfo(const QMap<int, QString>&, QString, QString);
#endif

	void connecting(QString host);
	void errorOcurred(int error_number, QString error_str);

	void signatureNotFound(const QString & title);

protected slots:
	void gotResponse();
	void parse(QByteArray text);
#ifdef YT_GET_VIDEOINFO
	void gotVideoInfoResponse();
	void parseVideoInfo(QByteArray text);
	void fetchVideoInfoPage();
#endif

protected:
	static QString sanitizeForUnicodePoint(QString string);
	static void htmlDecode(QString& string);
	QString getVideoID(QString video_url);

	QMap<int, QString> urlMap;
	QString url_title;
	QString orig_url;
	QString latest_preferred_url;

	Quality preferred_quality;
	static QString user_agent;

#ifdef YT_GET_VIDEOINFO
	QString video_id;
#endif

private:
	QNetworkAccessManager* manager;
	QNetworkReply* reply;
};

#endif
