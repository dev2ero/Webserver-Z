#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

#include <iostream>
#include <map>
#include <unordered_map>

using namespace std;

class HttpHandler {
public:
    enum ERROR_TYPE {
        ERR_SUCCESS = 0,
        ERR_READ_REQUEST_FAIL,
        ERR_NOT_IMPLEMENTED,
        ERR_HTTP_VERSION_NOT_SUPPORTED,
        ERR_INTERNAL_SERVER_ERR,
        ERR_CONNECTION_CLOSED,
        ERR_BAD_REQUEST,
        ERR_SEND_RESPONSE_FAIL
    };
    explicit HttpHandler(int fd = -1);
    ~HttpHandler();
    void run_event_loop();
    int get_client_fd() { return client_fd_; }
private:
    const size_t MAXBUF = 1024;
    int client_fd_;
    string request_;
    unordered_map<string, string> headers_;
    string method_;
    string path_;
    string http_version_;
    bool is_keeep_alive_;
    size_t pos_;
    void print_connection_status();
    ERROR_TYPE read_request();
    ERROR_TYPE parse_uri();
    ERROR_TYPE parse_http_header();
    ERROR_TYPE send_response(const string& response_code, const string& response_msg,
            const string& response_body_type, const string& response_body);
    ERROR_TYPE handle_error(const string& err_code, const string& err_meg);
    string escape_str(const string& str);
};

class MimeType {
private:
    map<string, string> mime_map_;
    string get_mine_type_(string suffix) {
        if(mime_map_.find(suffix) != mime_map_.end())
            return mime_map_[suffix];
        else
            return mime_map_["default"];
    }
public:
    MimeType() {
        mime_map_["doc"] = "application/msword";
        mime_map_["gz"] = "application/x-gzip";
        mime_map_["ico"] = "application/x-ico";

        mime_map_["gif"] = "image/gif";
        mime_map_["jpg"] = "image/jpeg";
        mime_map_["png"] = "image/png";
        mime_map_["bmp"] = "image/bmp";

        mime_map_["mp3"] = "audio/mp3";
        mime_map_["avi"] = "video/x-msvideo";

        mime_map_["html"] = "text/html";
        mime_map_["htm"] = "text/html";
        mime_map_["css"] = "text/html";
        mime_map_["js"] = "text/html";

        mime_map_["c"] = "text/plain";
        mime_map_["txt"] = "text/plain";
        mime_map_["default"] = "text/plain";
    }

    static string get_mine_type(string suffix) {
        static MimeType* _mime_ty = new MimeType();
        return _mime_ty->get_mine_type_(suffix);
    }
};

#endif
