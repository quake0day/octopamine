#include "main.h"


int fcfs(int client_socket_id)
{
    client_schedule[iput_req] = client_socket_id;
    iput_req++;
    if (iput_req == MAXCLIENT)
        iput_req = 0;
    if (iput_req == iget_req)
    {
        exit(1);
    }
    return 1;
}


int sjf(int client_socket_id)
{
    return 1;
}
