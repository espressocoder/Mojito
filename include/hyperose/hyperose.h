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
/// @file hyperose.h
/// @synopsis 
/// @author Lan Jian, air.petrichor@gmail.com
/// @version v0.0.1
/// @date 2017-05-29

#ifndef HYPEROSE_H
#define HYPEROSE_H

#include "mongoose/mongoose.h"
#include <map>
#include <string>
#include <functional>

class hyperose
{
public:
    using handler = std::function<void(std::string, std::string)>;

public:
    virtual ~hyperose();

    void init(uint32_t port);
    bool start();
    bool close();

    bool register_handler(std::string uri, handler f);
    void unregister_handler(std::string uri);
    void loop(int m);

    void send_reply(std::string uri, std::string reply);
    void send_error(std::string uri, int errcode, std::string reply);

protected:
    using handler_map = std::map<std::string, handler>;
    using connection_map = std::multimap<std::string, mg_connection*>;

private:
    static void hyp_ev_handler(struct mg_connection* sconn, int ev, void* ev_data);
    static void hyp_handle_request(struct mg_connection* sconn, int ev, void* ev_data);

public:
    static handler_map handlers_;
    static connection_map connections_;

    char port_[11];
    struct mg_mgr mgr_;
};


#endif //HYPEROSE_H
