// Copyright (C) 
// 
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU GeneratorExiteral Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., Free Road, Shanghai 000000, China.
// 
/// @file hyperose.cpp
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-05-29

#include <hyperose/hyperose.h>

// static val initialize out of class
hyperose::handler_map hyperose::handlers_;
hyperose::connection_map hyperose::connections_;

void hyperose::init(uint32_t port)
{
    memset(port_, 0, sizeof(port_));
    snprintf(port_, sizeof(port_), "%u", port);
}

bool hyperose::start()
{
    mg_mgr_init(&mgr_, NULL);
    auto nc = mg_bind(&mgr_, port_, hyp_ev_handler);

    if (nullptr == nc) {
        return false;
    }

    mg_set_protocol_http_websocket(nc);
    return true;
}

bool hyperose::close()
{
    mg_mgr_free(&mgr_);
    return true;
}

bool hyperose::register_handler(std::string uri, handler f)
{
    auto it = handlers_.find(uri);
    if (handlers_.end() != it) {
        // already registered, return false
        return false;
    }
    // handlers_emplace(uri, f) return a obj of pair such as pair<iterator, bool>
    return handlers_.emplace(uri, f).second;
}

void hyperose::unregister_handler(std::string uri)
{
    auto it = handlers_.find(uri);
    if (it != handlers_.end()) {
        handlers_.erase(it);
    }
}

void hyperose::loop(int m)
{
    mg_mgr_poll(&mgr_, m);
}

void hyperose::hyp_ev_handler(struct mg_connection* sconn, int ev, void* ev_data)
{
    switch (ev) {
        case MG_EV_HTTP_REQUEST:
            hyp_handle_request(sconn, ev, ev_data);
            break;
        default:
            break;
    }
}

void hyperose::hyp_handle_request(struct mg_connection* sconn, int ev, void* ev_data)
{
    http_message* hmsg = (http_message*)ev_data;
    std::string uri(hmsg->uri.p, hmsg->uri.len);

    auto it = handlers_.find(uri);
    if (it == handlers_.end()) {
        return;
    }

    // registerd handler previously found
    // save this connection to connection_container
    connections_.emplace(uri, sconn);

    // handle this request by registered handler found in handlers_
    // below is a function calling
    it->second(std::string(hmsg->query_string.p, hmsg->query_string.len),
                std::string(hmsg->body.p, hmsg->body.len));
}

void hyperose::send_reply(std::string uri, std::string reply)
{
    // range is a obj of pair looks like pair<interator, iterator>
    auto range = connections_.equal_range(uri);
    if (range.first == range.second) {
         // range.first == range.second == connections_.end()
         // it means to exclude uri
        return;
    }

    // it points to pair<std::string, mg_connection*>
    auto it = range.first;
    mg_printf(it->second, 
            "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type:text/html\r\nContent-Length:%u\r\n\r\n%s\r\n",
            (uint32_t)reply.length(),
            reply.c_str());

    it->second->flags |= MG_F_SEND_AND_CLOSE;
    connections_.erase(it);
}

void hyperose::send_error(std::string uri, int errcode, std::string reply)
{
    auto range = connections_.equal_range(uri);
    if (range.first == range.second) {
        return;
    }
    auto it = range.first;
    mg_printf(it->second, 
                "HTTP/1.1 %d %s\r\n", errcode,
                reply.c_str());
    it->second->flags |= MG_F_SEND_AND_CLOSE;
    connections_.erase(uri);
}
