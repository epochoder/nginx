
/*
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_HTTP_AUTOINDEX_READABILITY_H_INCLUDED_
#define _NGX_HTTP_AUTOINDEX_READABILITY_H_INCLUDED_


static u_char  ngx_http_autoindex_html_title[] =
    "<html>" CRLF
    "<head><title>Index of "
;


static u_char  ngx_http_autoindex_html_style[] =
    "</title>" CRLF
    "<style>" CRLF
    "body{font:14px/1.35 monospace;margin:1em;}" CRLF
    "pre{white-space:pre-wrap;word-break:break-word;}" CRLF
    "a{overflow-wrap:anywhere;}" CRLF
    "</style>" CRLF
;


static u_char  ngx_http_autoindex_html_header[] =
    "</head>" CRLF
    "<body>" CRLF
    "<h1>Index of "
;


static u_char  ngx_http_autoindex_html_h1_end[] =
    "</h1>"
;


static u_char  ngx_http_autoindex_html_listing_start[] =
    "<hr><pre><a href=\"../\">../</a>" CRLF
;


static u_char  ngx_http_autoindex_html_listing_end[] =
    "</pre><hr>"
;


static u_char  ngx_http_autoindex_html_tail[] =
    "</body>" CRLF
    "</html>" CRLF
;


#endif /* _NGX_HTTP_AUTOINDEX_READABILITY_H_INCLUDED_ */
