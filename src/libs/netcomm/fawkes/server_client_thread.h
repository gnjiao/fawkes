
/***************************************************************************
 *  client_thread.h - Thread to handle Fawkes network client
 *
 *  Created: Fri Nov 17 15:30:15 2006
 *  Copyright  2006  Tim Niemueller [www.niemueller.de]
 *
 *  $Id$
 *
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software Foundation,
 *  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02111-1307, USA.
 */

#ifndef __NETCOMM_FAWKES_CLIENT_THREAD_H_
#define __NETCOMM_FAWKES_CLIENT_THREAD_H_

#include <core/threading/thread.h>
#include <list>

class StreamSocket;
class FawkesNetworkServerThread;
class FawkesNetworkMessage;
class FawkesNetworkMessageQueue;
class WaitCondition;
class Mutex;
class FawkesNetworkServerClientSendThread;

class FawkesNetworkServerClientThread : public Thread
{
 public:
  FawkesNetworkServerClientThread(StreamSocket *s, FawkesNetworkServerThread *parent);
  ~FawkesNetworkServerClientThread();

  virtual void once();
  virtual void loop();

  unsigned int clid() const;
  void         set_clid(unsigned int client_id);

  bool alive() const;
  void enqueue(FawkesNetworkMessage *msg);

  void force_send();
  void connection_died();

 private:

  void recv();

  unsigned int                _clid;
  bool                        _alive;
  StreamSocket               *_s;
  FawkesNetworkServerThread  *_parent;
  FawkesNetworkMessageQueue  *_inbound_queue;

  FawkesNetworkServerClientSendThread  *_send_slave;
};

#endif