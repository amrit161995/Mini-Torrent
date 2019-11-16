#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include<iostream>
#include<fstream>
#include<thread>
#include<arpa/inet.h>
#include<vector>
#include <bits/stdc++.h> 
#include"generateSHA.h"
#include"generateMtorrent.h"
using namespace std;

vector <string> downloads;
vector <string> downloading;
string share(string filepath,string mtfile,string ip,int port,string t1ip,int t1port,string t2ip,int t2port){
    string datapipe="";
    char por[20];
    string sha=generateSHA(filepath);
    generateMtorrent(filepath,sha,mtfile,t1ip,t1port,t2ip,t2port);
    string hashohash=generateSHAstring(sha);
    datapipe=filepath+"|"+hashohash+"|"+ip+"|"+to_string(port)+"|share";
    return datapipe;
}

string get(string ptmtorrent,string destination){
    string data;
    fstream file;
    string word;
    file.open(ptmtorrent.c_str());
    for(int i=0;i<5;i++)
        file>>word;
    //cout<<word<<endl;
   data=generateSHAstring(word)+"|get";
   //cout<<data<<endl;
   
   return data;
}
string removedata(string ptmtorrent){
    string data;
    fstream file;
    string word;
    file.open(ptmtorrent.c_str());
    for(int i=0;i<5;i++)
        file>>word;
    //cout<<word;
   data=generateSHAstring(word)+"|remove";
   remove(ptmtorrent.c_str());
   file.close();
   return data;
    //remove(ptmtorrent.c_str());
}

void filetransfer(int sock,string filename){
     FILE *fptr;
      char *buffer=(char*)malloc(sizeof(char)*1024);
      int valread;
      fptr=fopen(filename.c_str(),"w");
 while(valread = read( sock, buffer, 1024)) {


          valread=fwrite(buffer,sizeof(char),valread,fptr);
          bzero(buffer,1024);
            memset(buffer, 0, 1024);
     }

}






int createConnection(string ip1,int port1){
 char *buffer=(char*)malloc(sizeof(char)*1024);
   // fptr = fopen(filename, "w"); 
    struct sockaddr_in address; 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr;
    string command;
    int n;
    //string buffer;
   
    //string buffer="";
   char c[1]={1};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
       // return -1; 
    } 
   
    memset(&serv_addr, '0', sizeof(serv_addr)); 
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port1);
    serv_addr.sin_addr.s_addr = inet_addr(ip1.c_str());    
   
    inet_pton(AF_INET, ip1.c_str() , &serv_addr.sin_addr);
    
   
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    socklen_t addressLength = sizeof(address);
    getsockname(sock, (struct sockaddr*)&address,&addressLength);
    string ip=inet_ntoa( address.sin_addr);
    int port=(int)ntohs(address.sin_port);
    return sock;
}

void newConnection(string ip,int port,string filename,string ptmtorrent){
  int socket= createConnection(ip,port);
    string data_filename;
    fstream file;
    string word;
    file.open(ptmtorrent.c_str());
    for(int i=0;i<3;i++)
        file>>word;
   
  
  // cout<<word<<endl;
   send(socket,word.c_str(),strlen(word.c_str()), 0 );
   filetransfer(socket,filename);
     
    
}

void servermode(string sip,int sport){
    string line;
    

    //send_data_to_file("seederlist",seederList);
    // for(auto itr=seederList.begin();itr!=seederList.end();itr++)
    //  cout<<itr->first<<endl;
    char *buffer=(char*)malloc(sizeof(char)*1024);
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    int b;
    string hello;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = inet_addr(sip.c_str());
    address.sin_port = htons( sport ); 
    bind(server_fd, (struct sockaddr *)&address,sizeof(address));
    listen(server_fd, 3);
     
    while(1){
    new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen);
    
    FILE *fptr;
     valread = read( new_socket , buffer, 1024);
    fptr = fopen(buffer, "r");
    int n;
    char *buffer=(char*)malloc(sizeof(char)*1024);
 
    while (valread=fread(buffer,sizeof(char),1024,fptr))
     {
      //myfile.read(buffer,1024);
       n=write(new_socket , buffer,valread ); 
       bzero(buffer,1024);
       //memset(buffer,'\0', 1024);
    }
}
 

}
void clientmode(string tip,int tport,string sip,int sport,string t2ip,int t2port){

 // char filename[100]; 
   
   
     char *buffer=(char*)malloc(sizeof(char)*1024);
   // fptr = fopen(filename, "w"); 
    struct sockaddr_in address; 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr;
    string command;
    
    int n;
    //string buffer;
   
    //string buffer="";
   char c[1]={1};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        //return -1; 
    } 
   
    memset(&serv_addr, '0', sizeof(serv_addr)); 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(tport); 
    serv_addr.sin_addr.s_addr = inet_addr(tip.c_str());
   
    inet_pton(AF_INET, tip.c_str(), &serv_addr.sin_addr);
    
   
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    socklen_t addressLength = sizeof(address);
    getsockname(sock, (struct sockaddr*)&address, &addressLength);
   


 COMMAND:
   while(1){

     memset(buffer, 0, 1024);
     getline(cin,command);  
     vector <string> tokens; 
     stringstream check1(command.c_str()); 
     string intermediate;
     while(getline(check1, intermediate, ' ')) 
     { 
         tokens.push_back(intermediate); 
     }

     if(tokens[0]=="share"){
        string temp=share(tokens[1],tokens[2],sip,sport,tip,tport,t2ip,t2port);
        send(sock,temp.c_str(),strlen(temp.c_str()), 0 );
        //close(sock);
         //createConnection("127.0.0.1",8080);
    }

    else if(tokens[0]=="get"){
        string temp=get(tokens[1],tokens[2]);
       
        send(sock,temp.c_str(),strlen(temp.c_str()), 0 );
        valread = read( sock , buffer, 1024);
        cout<<buffer<<endl;
        

        vector <string> seperatedData; 
        stringstream check1(buffer); 
        string intermediate;
        while(getline(check1, intermediate, '|')) 
        { 
         seperatedData.push_back(intermediate); 
        }

        
        close(sock);
        //cout<<seperatedData[0]<<endl;

        vector <string> file_path; 
        stringstream check2(seperatedData[0]); 
        string intermediate1;
        while(getline(check2, intermediate1, '/')) 
        { 
         file_path.push_back(intermediate1); 
        }
        //cout<<file_path[file_path.size()-1]<<endl;

         string destination=tokens[2]+"/"+file_path[file_path.size()-1];
        // cout<<destination<<endl;
        downloading.push_back(file_path[file_path.size()-1]);
        thread newconnection(newConnection,seperatedData[2],atoi(seperatedData[3].c_str()),destination,tokens[1]);
        thread server(servermode,sip,sport);
         thread client(clientmode,tip,tport,sip,sport,t2ip,t2port);
       cout<<"File Downloaded"<<endl;
        downloading.pop_back();
       downloads.push_back(file_path[file_path.size()-1]);
        server.join();
        client.join();
      // filetransfer(socket,seperatedData[0]);
        newconnection.join();
        

    }

    else if(tokens[0]=="remove"){
        string temp=removedata(tokens[1]);
        //cout<<"1";
      send(sock,temp.c_str(),strlen(temp.c_str()), 0 );
      //close(sock);
       //createConnection("127.0.0.1",8080);
     //removedata(tokens[1]);
    }

    else if(tokens[0]=="exit"){
        string temp="exit";
        //cout<<"1";
      send(sock,temp.c_str(),strlen(temp.c_str()), 0 );
      close(sock);
      exit(0);
      //close(sock);
       //createConnection("127.0.0.1",8080);
     //removedata(tokens[1]);
    }

    else if(tokens[0]=="show" &&tokens[1]=="downloads"){
    	cout<<"Files Downloaded:"<<endl;
        for(int i=0;i<downloads.size();i++){
        	
        	cout<<"[S]"<<downloads[i]<<endl;
        }

        cout<<"Files Downloading:"<<endl;
        for(int i=0;i<downloading.size();i++){
        	
        	cout<<"[D]"<<downloading[i]<<endl;
        }

      //close(sock);
       //createConnection("127.0.0.1",8080);
     //removedata(tokens[1]);
    }
     //valread = read( sock , buffer, 1024); 
     //printf("%s\n",buffer ); 
}

}






int main(int args,char *argv[]){
	//cout<<"!"<<endl;
	//first tracker ip port ------------------------------------------------------------------------------------------------
    vector <string> t1ip_port;
    stringstream check1(argv[2]); 
    string intermediate;
     while(getline(check1, intermediate, ':')) 
     { 
         t1ip_port.push_back(intermediate); 
     }
    
    string t1ip=t1ip_port[0];
    int t1port=atoi(t1ip_port[1].c_str());
    //second tracker ip port -----------------------------------------------------------------------------------------------
      vector <string> t2ip_port;
    stringstream check3(argv[3]); 
    string intermediate2;
     while(getline(check3, intermediate2, ':')) 
     { 
         t2ip_port.push_back(intermediate2); 
     }
    
    string t2ip=t2ip_port[0];
    int t2port=atoi(t2ip_port[1].c_str());




     vector <string> sip_port;
    stringstream check2(argv[1]); 
    string intermediate1;
     while(getline(check2, intermediate1, ':')) 
     { 
         sip_port.push_back(intermediate1); 
     }
    
    string sip=sip_port[0];
    int sport=atoi(sip_port[1].c_str());
    
    //cout<<sip;
   // string<<sport;
    thread server(servermode,sip,sport);
    thread client(clientmode,t1ip,t1port,sip,sport,t2ip,t2port);
      
    server.join();
    client.join();
     
  //      FILE *fptr;
//       //ifstream myfile;
//      fptr=fopen("tu.png","r");
// //myfile.open ("k.pdf", ios::in);

// // if (myfile.is_open())
// //    {
//    //memset(buffer,'\0', 1024);
//     while (valread=fread(buffer,sizeof(char),1024,fptr))
//     {
//       //myfile.read(buffer,1024);
//        n=write(sock , buffer,valread ); 
//        bzero(buffer,1024);
//        //memset(buffer,'\0', 1024);
//     }
    

   
	return 0;
}