//
// Created by Administrator on 24-4-12.
//

#ifndef ZLTOOLKIT_MEDIAINFO_H
#define ZLTOOLKIT_MEDIAINFO_H


#include <string>
#include <memory>
namespace mediakit {


    struct MediaTuple {
        std::string vhost;
        std::string app;
        std::string stream;
        std::string params;

        std::string shortUrl() const {
            return vhost + '/' + app + '/' + stream;
        }
    };

/**
 * 解析url获取媒体相关信息
 */
    class MediaInfo : public MediaTuple {
    public:
        MediaInfo() = default;

        MediaInfo(const std::string &url) { parse(url); }

        void parse(const std::string &url);

        std::string getUrl() const { return schema + "://" + shortUrl(); }

    public:
        uint16_t port = 0;
        std::string full_url;
        std::string schema;
        std::string host;
    };

}

#endif //ZLTOOLKIT_MEDIAINFO_H
