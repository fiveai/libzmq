/*
    Copyright (c) 2007-2015 Contributors as noted in the AUTHORS file

    This file is part of 0MQ.

    0MQ is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    0MQ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __ZMQ_SERVER_HPP_INCLUDED__
#define __ZMQ_SERVER_HPP_INCLUDED__

#include <map>

#include "socket_base.hpp"
#include "session_base.hpp"
#include "stdint.hpp"
#include "blob.hpp"
#include "msg.hpp"
#include "fq.hpp"

namespace zmq
{

    class ctx_t;
    class pipe_t;

    //  TODO: This class uses O(n) scheduling. Rewrite it to use O(1) algorithm.
    class server_t :
        public socket_base_t
    {
    public:

        server_t (zmq::ctx_t *parent_, uint32_t tid_, int sid);
        ~server_t ();

        //  Overrides of functions from socket_base_t.
        void xattach_pipe (zmq::pipe_t *pipe_, bool subscribe_to_all_);
        int xsetsockopt (int option_, const void *optval_, size_t optvallen_);
        int xsend (zmq::msg_t *msg_);
        int xrecv (zmq::msg_t *msg_);
        bool xhas_in ();
        bool xhas_out ();
        void xread_activated (zmq::pipe_t *pipe_);
        void xwrite_activated (zmq::pipe_t *pipe_);
        void xpipe_terminated (zmq::pipe_t *pipe_);

    protected:

        blob_t get_credential () const;

    private:

        //  Fair queueing object for inbound pipes.
        fq_t fq;
        
        struct outpipe_t
        {
            zmq::pipe_t *pipe;
            bool active;
        };

        //  Outbound pipes indexed by the peer IDs.
        typedef std::map <uint32_t, outpipe_t> outpipes_t;
        outpipes_t outpipes;
        
        //  Routing IDs are generated. It's a simple increment and wrap-over
        //  algorithm. This value is the next ID to use (if not used already).
        uint32_t next_rid;
    
        server_t (const server_t&);
        const server_t &operator = (const server_t&);
    };

}

#endif
