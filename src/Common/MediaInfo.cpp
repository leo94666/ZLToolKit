//
// Created by Administrator on 24-4-12.
//

#include "MediaInfo.h"
#include "Common/Parser.h"

using namespace std;
using namespace toolkit;

/////////////////////////////////////MediaInfo//////////////////////////////////////

namespace mediakit {

    void MediaInfo::parse(const std::string &url_in) {
        full_url = url_in;
        auto url = url_in;
        auto pos = url.find("?");
        if (pos != string::npos) {
            params = url.substr(pos + 1);
            url.erase(pos);
        }

        auto schema_pos = url.find("://");
        if (schema_pos != string::npos) {
            schema = url.substr(0, schema_pos);
        } else {
            schema_pos = -3;
        }
        auto split_vec = split(url.substr(schema_pos + 3), "/");
        if (split_vec.size() > 0) {
            splitUrl(split_vec[0], host, port);
            vhost = host;
            if (vhost == "localhost" || isIP(vhost.data())) {
                //如果访问的是localhost或ip，那么则为默认虚拟主机
                vhost = DEFAULT_VHOST;
            }
        }
        if (split_vec.size() > 1) {
            app = split_vec[1];
        }
        if (split_vec.size() > 2) {
            string stream_id;
            for (size_t i = 2; i < split_vec.size(); ++i) {
                stream_id.append(split_vec[i] + "/");
            }
            if (stream_id.back() == '/') {
                stream_id.pop_back();
            }
            stream = stream_id;
        }

        auto kv = Parser::parseArgs(params);
        auto it = kv.find(VHOST_KEY);
        if (it != kv.end()) {
            vhost = it->second;
        }

        GET_CONFIG(bool, enableVhost, General::kEnableVhost);
        if (!enableVhost || vhost.empty()) {
            //如果关闭虚拟主机或者虚拟主机为空，则设置虚拟主机为默认
            vhost = DEFAULT_VHOST;
        }
    }

    MediaSource::Ptr
    MediaSource::createFromMP4(const string &schema, const string &vhost, const string &app, const string &stream,
                               const string &file_path, bool check_app) {
        GET_CONFIG(string, appName, Record::kAppName);
        if (check_app && app != appName) {
            return nullptr;
        }
#ifdef ENABLE_MP4
        try {
            auto reader = std::make_shared<MP4Reader>(vhost, app, stream, file_path);
            reader->startReadMP4();
            return MediaSource::find(schema, vhost, app, stream);
        } catch (std::exception &ex) {
            WarnL << ex.what();
            return nullptr;
        }
#else
        WarnL << "创建MP4点播失败，请编译时打开\"ENABLE_MP4\"选项";
        return nullptr;
#endif //ENABLE_MP4
    }
}