#include <bits/stdc++.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sstream>
// #include <vector>

/////////////////////////////
#include <iostream>
#include <assert.h>
#include <tuple>
using namespace std;
/////////////////////////////

//Regular bold text
#define BBLK "\e[1;30m"
#define BRED "\e[1;31m"
#define BGRN "\e[1;32m"
#define BYEL "\e[1;33m"
#define BBLU "\e[1;34m"
#define BMAG "\e[1;35m"
#define BCYN "\e[1;36m"
#define ANSI_RESET "\x1b[0m"

typedef long long LL;

#define pb push_back
#define debug(x) cout << #x << " : " << x << endl
#define part cout << "-----------------------------------" << endl;

///////////////////////////////
#define MAX_CLIENTS 4
#define PORT_ARG 80010020
int dest;
int curr_node=0;
int n,m;


const int initial_msg_len = 256;


////////////////////////////////////

const LL buff_sz = 1048576;
///////////////////////////////////////////////////
typedef struct edge_details
{
    pthread_t pid;
    int server_node;
    int client_node;
    int client_socked_fd;
    int server_socket_fd;
    int server_uses_this;
    vector<pair<int,int> > server_routing;
    int port;
}edge;
edge* edge_data[1000][1000];


pair<string, int> read_string_from_socket(const int &fd, int bytes)
{
    std::string output;
    output.resize(bytes);

    int bytes_received = read(fd, &output[0], bytes - 1);
    debug(bytes_received);
    if (bytes_received <= 0)
    {
        cerr << "Failed to read data from socket. \n";
    }

    output[bytes_received] = 0;
    output.resize(bytes_received);
    // debug(output);
    return {output, bytes_received};
}

int send_string_on_socket(int fd, const string &s)
{
    // debug(s.length());
    int bytes_sent = write(fd, s.c_str(), s.length());
    if (bytes_sent < 0)
    {
        cerr << "Failed to SEND DATA via socket.\n";
    }

    return bytes_sent;
}

void print_routing_table(int client_socket_fd,vector<int>& min_dis,list<int> path[],int num)
{
    string header = "\ndest\tforw\tdelay\n";
    for(int i=1;i<num;i++)
    {
        int x = path[i].front();
        header+=to_string(i) + "\t" + to_string(x) + "\t" + to_string(min_dis[i]) + "\n";
    }
    // cout << header;
    send_string_on_socket(client_socket_fd,header);
    return;
}

///////////////////////////////

void handle_connection(int client_socket_fd,int num,vector<pair<int,int> > vec[],vector<pair<int,int> > routing[],vector<int>& min_dis,vector<int>& parent,list<int> path[])
{
    // int client_socket_fd = *((int *)client_socket_fd_ptr);
    //####################################################

    int received_num, sent_num;

    /* read message from client */
    int ret_val = 1;

    while (true)
    {
        string cmd;
        tie(cmd, received_num) = read_string_from_socket(client_socket_fd, buff_sz);
        ret_val = received_num;
        // debug(ret_val);
        // printf("Read something\n");
        if (ret_val <= 0)
        {
            // perror("Error read()");
            printf("Server could not read msg sent from client\n");
            goto close_client_socket_ceremony;
        }
        cout << "Client sent : " << cmd << endl;
        if (cmd == "exit")
        {
            cout << "Exit pressed by client" << endl;
            goto close_client_socket_ceremony;
        }
        else if(cmd == "pt")
        {
            print_routing_table(client_socket_fd,min_dis,path,num);
            continue;
        }
        else
        {
            vector<string> tokens;
            string temp = "";
            char delim = ' ';
            for(int i = 0; i < cmd.length(); i++){
                if(cmd[i] == delim){
                    tokens.push_back(temp);
                    temp = "";
                }
                else
                    temp += cmd[i];           
            }
            tokens.push_back(temp);
            if(tokens[0] == "send")
            {
                dest = stoi(tokens[1]);
                string message = cmd.substr(7);
                curr_node = 0;
                int prev_node=0;
                cout<<"*******************************************************"<<"\n";
                if(dest != 0)
                    send_string_on_socket(edge_data[curr_node][routing[curr_node][dest].second]->client_socked_fd,message);
                while(curr_node != dest)
                {
                    // send_string_on_socket(edge_data[curr_node][routing[curr_node][dest].second]->client_socked_fd,message);
                    // while(curr_node == prev_node);
                    // printf("Data received at node: %d : Source : %d : Destination : %d : Forwarded_Destination : %d : ",curr_node,prev_node,dest,routing[curr_node][dest].second);
                    prev_node = curr_node;
                    // cout << "Message: "<< message<< " :\n";

                }
                cout<<"*******************************************************"<<"\n";

            }
        }
        string msg_to_send_back = "Ack: " + cmd;

        ////////////////////////////////////////
        // "If the server write a message on the socket and then close it before the client's read. Will the client be able to read the message?"
        // Yes. The client will get the data that was sent before the FIN packet that closes the socket.

        int sent_to_client = send_string_on_socket(client_socket_fd, msg_to_send_back);
        // debug(sent_to_client);
        if (sent_to_client == -1)
        {
            perror("Error while writing to client. Seems socket has been closed");
            goto close_client_socket_ceremony;
        }
    }

close_client_socket_ceremony:
    close(client_socket_fd);
    printf(BRED "Disconnected from client" ANSI_RESET "\n");
    // return NULL;
}

void *edge_com(void *ed)
{
    int wel_socket_fd, client_socket_fd, port_number;
    socklen_t clilen;

    struct sockaddr_in serv_addr_obj, client_addr_obj;
    /////////////////////////////////////////////////////////////////////////
    /* create socket */
    /*
    The server program must have a special door—more precisely,
    a special socket—that welcomes some initial contact 
    from a client process running on an arbitrary host
    */
    //get welcoming socket
    //get ip,port
    /////////////////////////
    ((edge*)ed)->server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (((edge*)ed)->server_socket_fd < 0)
    {
        perror("ERROR creating server socket");
        exit(-1);
    }

    //////////////////////////////////////////////////////////////////////
    /* IP address can be anything (INADDR_ANY) */
    bzero((char *)&serv_addr_obj, sizeof(serv_addr_obj));
    port_number = ((edge*)ed)->port;
    serv_addr_obj.sin_family = AF_INET;
    // On the server side I understand that INADDR_ANY will bind the port to all available interfaces,
    serv_addr_obj.sin_addr.s_addr = INADDR_ANY;
    serv_addr_obj.sin_port = htons(port_number); //process specifies port

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /* bind socket to this port number on this machine */
    /*When a socket is created with socket(2), it exists in a name space
       (address family) but has no address assigned to it.  bind() assigns
       the address specified by addr to the socket referred to by the file
       descriptor wel_sock_fd.  addrlen specifies the size, in bytes, of the
       address structure pointed to by addr.  */

    //CHECK WHY THE CASTING IS REQUIRED
    if (bind(((edge*)ed)->server_socket_fd, (struct sockaddr *)&serv_addr_obj, sizeof(serv_addr_obj)) < 0)
    {
        printf("%d\n",port_number);
        perror("Error on bind on server socket: ");
        exit(-1);
    }
    //////////////////////////////////////////////////////////////////////////////////////

    /* listen for incoming connection requests */

    listen(((edge*)ed)->server_socket_fd, MAX_CLIENTS);
    ((edge*)ed)->client_socked_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (((edge*)ed)->client_socked_fd < 0)
    {
        perror("Error in socket creation for CLIENT");
        exit(-1);
    }
    /////////////////////////////////////////////////////////////////////////////////////
    struct sockaddr_in server_obj;
    memset(&server_obj, 0, sizeof(server_obj)); // Zero out structure
    server_obj.sin_family = AF_INET;
    server_obj.sin_port = htons(((edge*)ed)->port); //convert to big-endian order

    clilen = sizeof(client_addr_obj);
    // printf("waitinf for client\n");
    if (connect(((edge*)ed)->client_socked_fd, (struct sockaddr *)&server_obj, sizeof(server_obj)) < 0)
    {
        perror("Problem in connecting to the server");
        exit(-1);
    }
    client_socket_fd = accept(((edge*)ed)->server_socket_fd, (struct sockaddr *)&client_addr_obj, &clilen);
    if (client_socket_fd < 0)
    {
        perror("ERROR while accept() system call occurred in SERVER");
        exit(-1);
    }
    ((edge*)ed)->server_uses_this = client_socket_fd;
   
    // printf(BGRN "New client %d connected to server %d from port number %d and IP %s \n" ANSI_RESET,((edge*)ed)->client_node,((edge*)ed)->server_node, ntohs(client_addr_obj.sin_port), inet_ntoa(client_addr_obj.sin_addr));
    // printf("rhythm\n");
    int received_num;
    while(1)
    {
        string cmd;
        tie(cmd, received_num) = read_string_from_socket(client_socket_fd, buff_sz);
        curr_node = ((edge*)ed)->server_node;
        if(curr_node != dest)
            printf("Data received at node: %d : Source : %d : Destination : %d : Forwarded_Destination : %d : ",curr_node,((edge*)ed)->client_node,dest,((edge*)ed)->server_routing[dest].second);
        else
            printf("Data received at node: %d : Source : %d : Destination : %d : ",curr_node,((edge*)ed)->client_node,dest);
        cout <<"Message: " << cmd << "\n";
        if(curr_node == dest) continue;
        send_string_on_socket(edge_data[curr_node][((edge*)ed)->server_routing[dest].second]->client_socked_fd,cmd);
    }
    
    return NULL;

}

void make_routing_table(vector<pair<int,int> > vec[],vector<pair<int,int> > routing[],int src,int num)
{
    priority_queue<pair<int,int>, vector<pair<int,int> >, greater<pair<int,int> > > pq;
    vector<int> min_dis;
    vector<int> parent;
    for(int i=0;i<num;i++){
        min_dis.push_back(INT32_MAX);
        parent.push_back(-1);
    }
    pq.push(make_pair(0,src));
    min_dis[src] = 0;
    while(pq.size() != 0)
    {
        int temp_src = pq.top().second;
        pq.pop();
        for(int i=0;i<vec[temp_src].size();i++)
        {
            int neighbour = vec[temp_src][i].first;
            // printf("%d\n",neighbour);
            if(min_dis[neighbour] > min_dis[temp_src] + vec[temp_src][i].second)
            {
                min_dis[neighbour] = min_dis[temp_src] + vec[temp_src][i].second;
                parent[neighbour] = temp_src;
                pq.push(make_pair(min_dis[neighbour],neighbour));
            }
        }
    }
    list<int> path[num];
    // list<int>::iterator it;
    // routing[src].push_back(make_pair(src,src));
    for(int i=0;i<num;i++)
    {
        if(i == src) {
            routing[src].push_back(make_pair(src,src));
            continue;
        }
        int vertex = i;
        path[i].push_front(vertex);
        while(vertex != src)
        {
            path[i].push_front(parent[vertex]);
            vertex = parent[vertex];
        }
        path[i].pop_front();
        routing[src].push_back(make_pair(i,path[i].front()));
    }
}

int main(int argc, char *argv[])
{

    // int i, j, k, t, n;
    cin>>n>>m;
    vector<pair<int,int> > vec[n];
    vector<pair<int,int> > routing[n];
    for(int i=0;i<m;i++)
    {
        int node1,node2,edge;
        cin>>node1>>node2>>edge;
        vec[node1].push_back(make_pair(node2,edge));
        vec[node2].push_back(make_pair(node1,edge));
    }
    // for(int i=0;i<n;i++)
    // {
    //     cout<<i<<":";
    //     for(int j=0;j<vec[i].size();j++)
    //     {
    //         cout<<vec[i][j].first<<" ";
    //     }
    //     cout<<"\n";
    // }
    priority_queue<pair<int,int>, vector<pair<int,int> >, greater<pair<int,int> > > pq;
    vector<int> min_dis;
    vector<int> parent;
    for(int i=0;i<n;i++){
        min_dis.push_back(INT32_MAX);
        parent.push_back(-1);
    }
    pq.push(make_pair(0,0));
    min_dis[0] = 0;
    while(pq.size() != 0)
    {
        int temp_src = pq.top().second;
        pq.pop();
        for(int i=0;i<vec[temp_src].size();i++)
        {
            int neighbour = vec[temp_src][i].first;
            // printf("%d\n",neighbour);
            if(min_dis[neighbour] > min_dis[temp_src] + vec[temp_src][i].second)
            {
                min_dis[neighbour] = min_dis[temp_src] + vec[temp_src][i].second;
                parent[neighbour] = temp_src;
                pq.push(make_pair(min_dis[neighbour],neighbour));
            }
        }
    }
    list<int> path[n];
    list<int>::iterator it;
    routing[0].push_back(make_pair(0,0));
    for(int i=1;i<n;i++)
    {
        int vertex = i;
        path[i].push_front(vertex);
        while(vertex != 0)
        {
            path[i].push_front(parent[vertex]);
            vertex = parent[vertex];
        }
        path[i].pop_front();
        routing[0].push_back(make_pair(i,(int)path[i].front()));
        make_routing_table(vec,routing,i,n);
    }
    pthread_t tid[n][n];

    // edge* edge_data[n][n];
    int counter=1;
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<vec[i].size();j++)
        {
            edge *tmp;
            tmp = (edge*)malloc(sizeof(edge));
            tmp->client_node = i;
            tmp->server_node = vec[i][j].first;
            tmp->server_routing = routing[vec[i][j].first];
            tmp->port = PORT_ARG + counter;
            counter++;
            // cout << j << "\n";
            edge_data[i][vec[i][j].first] = tmp;
            pthread_create(&(tmp->pid),NULL,edge_com,(void*)(tmp));
            tid[i][j] = tmp->pid;
            // pthread_join(tmp->pid,NULL);
            usleep(0.2);
        }
    }
    

    // for(int i=0;i<n;i++)
    // {
    //     cout<<i<<";\n";
    //     for(int j=0;j<n;j++)
    //     {
    //         cout<<routing[i][j].first << " "<<routing[i][j].second<<'\n';
    //     }
    //     cout<<"**************\n";
    // }
    // for(int i=0;i<n;i++)
    // {
    //     for(int j=0;j<path[i].size();j++)
    //     {
    //         cout<<path[i].front()<<" ";
    //     }
    //     cout<<"\n";
    //     // cout<<min_dis[i]<<" "<<parent[i]<<"\n";
    // }
    
    int wel_socket_fd, client_socket_fd, port_number;
    socklen_t clilen;

    struct sockaddr_in serv_addr_obj, client_addr_obj;
    /////////////////////////////////////////////////////////////////////////
    /* create socket */
    /*
    The server program must have a special door—more precisely,
    a special socket—that welcomes some initial contact 
    from a client process running on an arbitrary host
    */
    //get welcoming socket
    //get ip,port
    /////////////////////////
    wel_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (wel_socket_fd < 0)
    {
        perror("ERROR creating welcoming socket");
        exit(-1);
    }

    //////////////////////////////////////////////////////////////////////
    /* IP address can be anything (INADDR_ANY) */
    bzero((char *)&serv_addr_obj, sizeof(serv_addr_obj));
    port_number = PORT_ARG;
    serv_addr_obj.sin_family = AF_INET;
    // On the server side I understand that INADDR_ANY will bind the port to all available interfaces,
    serv_addr_obj.sin_addr.s_addr = INADDR_ANY;
    serv_addr_obj.sin_port = htons(port_number); //process specifies port

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /* bind socket to this port number on this machine */
    /*When a socket is created with socket(2), it exists in a name space
       (address family) but has no address assigned to it.  bind() assigns
       the address specified by addr to the socket referred to by the file
       descriptor wel_sock_fd.  addrlen specifies the size, in bytes, of the
       address structure pointed to by addr.  */

    //CHECK WHY THE CASTING IS REQUIRED
    if (bind(wel_socket_fd, (struct sockaddr *)&serv_addr_obj, sizeof(serv_addr_obj)) < 0)
    {
        perror("Error on bind on welcome socket: ");
        exit(-1);
    }
    //////////////////////////////////////////////////////////////////////////////////////

    /* listen for incoming connection requests */

    listen(wel_socket_fd, MAX_CLIENTS);
    cout << "Server has started listening on the LISTEN PORT" << endl;
    clilen = sizeof(client_addr_obj);

    while (1)
    {
        /* accept a new request, create a client_socket_fd */
        /*
        During the three-way handshake, the client process knocks on the welcoming door
of the server process. When the server “hears” the knocking, it creates a new door—
more precisely, a new socket that is dedicated to that particular client. 
        */
        //accept is a blocking call
        printf("Waiting for a new client to request for a connection\n");
        client_socket_fd = accept(wel_socket_fd, (struct sockaddr *)&client_addr_obj, &clilen);
        if (client_socket_fd < 0)
        {
            perror("ERROR while accept() system call occurred in SERVER");
            exit(-1);
        }

        printf(BGRN "New client connected from port number %d and IP %s \n" ANSI_RESET, ntohs(client_addr_obj.sin_port), inet_ntoa(client_addr_obj.sin_addr));
        
        handle_connection(client_socket_fd,n,vec,routing,min_dis,parent,path);
    }
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<vec[i].size();j++)
        {
            pthread_join(edge_data[i][vec[i][j].first]->pid,NULL);
        }
    }

    close(wel_socket_fd);
    return 0;
}
