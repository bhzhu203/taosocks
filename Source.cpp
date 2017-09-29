
#include <cassert>

#include <iostream>
#include <string>
#include <cstring>
#include <functional>

#include "winsock_helper.h"
#include "socks_server.h"
#include "iocp_model.h"
#include "thread_dispatcher.h"
#include "server_socket.h"
#include "client_socket.h"
#include "packet_manager.h"

using namespace taosocks;




int main()
{
    WSA::Startup();
    IOCP iocp;
    Dispatcher disp;

    ServerSocket server(disp);
    PacketManager pktmgr(disp);

    iocp.Attach(&server);
    iocp.Attach(&pktmgr.GetClient());

    server.OnAccepted([&](ClientSocket* client) {
        iocp.Attach(client);
        auto ss = new SocksServer(pktmgr, client);
    });

    pktmgr.Start();
    server.Start(INADDR_ANY, 8080);

    return disp.Run();
}
