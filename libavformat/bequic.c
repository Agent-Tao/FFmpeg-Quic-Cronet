#include "avformat.h"
#include "url.h"
#include "libavutil/opt.h"

#include <be_quic.h>

typedef struct BeQuicContext {
    const AVClass *class;
    int fd;
    int verify_certificate;
    int timeout;
} BeQuicContext;

#define OFFSET(x) offsetof(BeQuicContext, x)
#define D AV_OPT_FLAG_DECODING_PARAM
#define E AV_OPT_FLAG_ENCODING_PARAM

static const AVOption bequic_options[] = {
    { "verify_certificate",     "Whether verify certificate.",              OFFSET(verify_certificate),     AV_OPT_TYPE_BOOL,   { .i64 = 1 },   0,   1,          .flags = D|E },
    { "timeout",                "Quic session establish timeout in ms.",    OFFSET(timeout),                AV_OPT_TYPE_INT,    { .i64 = -1 },  -1,  INT_MAX,    .flags = D|E },
    { NULL }
};

static const AVClass bequic_class = {
    .class_name = "bequic",
    .item_name  = av_default_item_name,
    .option     = bequic_options,
    .version    = LIBAVUTIL_VERSION_INT,
};

static int quic_open(URLContext *h, const char *uri, int flags) {
    BeQuicContext *s = h->priv_data;
    int ret = 0;
    int rv = 0;
    char *url = NULL;
    int url_len = 0;
    const char *http_scheme = "http://";
    const char *https_scheme = "https://";
    const char *scheme = NULL;
    const char *deli = "://";
    char *p = NULL;
    char protocol[1024] = {0};
    do {
        if (uri == NULL) {            
            ret = AVERROR_UNKNOWN;
            break;
        }
        
        av_url_split(protocol, sizeof(protocol), NULL, 0, NULL, 0, NULL, NULL, 0, uri);
        if (strncmp(protocol, "quic", strlen(protocol)) == 0) {
            scheme = http_scheme;
        } else if (strncmp(protocol, "quics", strlen(protocol)) == 0) {
            scheme = https_scheme;
        } else {
            ret = AVERROR_PROTOCOL_NOT_FOUND;
            break;
        }

        //Replace protocol with certain scheme.
        p       = strstr(uri, deli);
        p       += strlen(deli);
        url_len = strlen(scheme) + strlen(p) + 1;
        url     = (char*)malloc(url_len);
        memset(url, 0, url_len);
        strcpy(url, scheme);
        strcat(url, p);

        printf("be_quic_open %s verify:%s timeout:%d.\n", url, s->verify_certificate?"true":"false", s->timeout);

        rv = be_quic_open(
            url,
            NULL, //Default to "GET"
            NULL,
            0,
            NULL,
            0,
            s->verify_certificate,
            s->timeout);
        if (rv <= 0) {
            ret = AVERROR_UNKNOWN;
            break;
        }
        
        s->fd = rv;
    } while (0);

    if (url != NULL) {
        free(url);
    }
    return ret;
}

static int quic_read(URLContext *h, uint8_t *buf, int size) {
    BeQuicContext *s = h->priv_data;
    int ret = be_quic_read(s->fd, buf, size, s->timeout);
    return ret;
}

static int quic_write(URLContext *h, const uint8_t *buf, int size) {
    BeQuicContext *s = h->priv_data;
    int ret = be_quic_write(s->fd, buf, size);
    return ret;
}

static int64_t quic_seek(URLContext *h, int64_t off, int whence) {  
    BeQuicContext *s = h->priv_data;
    int64_t ret = be_quic_seek(s->fd, off, whence);
    return ret;
}

static int quic_close(URLContext *h) {
    BeQuicContext *s = h->priv_data;
    be_quic_close(s->fd);
    return 0;
}

static int quic_get_file_handle(URLContext *h) {
    BeQuicContext *s = h->priv_data;
    return s->fd;
}

const URLProtocol ff_bequic_protocol = {
    .name                = "quic",
    .url_open            = quic_open,
    .url_read            = quic_read,
    .url_write           = quic_write,
    .url_seek            = quic_seek,
    .url_close           = quic_close,
    .url_get_file_handle = quic_get_file_handle,
    .priv_data_size      = sizeof(BeQuicContext),
    .priv_data_class     = &bequic_class,
    .flags               = URL_PROTOCOL_FLAG_NETWORK,
    .default_whitelist   = "quic,quics"
};

const URLProtocol ff_bequics_protocol = {
    .name                = "quics",
    .url_open            = quic_open,
    .url_read            = quic_read,
    .url_write           = quic_write,
    .url_seek            = quic_seek,
    .url_close           = quic_close,
    .url_get_file_handle = quic_get_file_handle,
    .priv_data_size      = sizeof(BeQuicContext),
    .priv_data_class     = &bequic_class,
    .flags               = URL_PROTOCOL_FLAG_NETWORK,
    .default_whitelist   = "quic,quics"
};
