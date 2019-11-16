#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include<iostream>
#include<thread>
#include<fstream>
#include<vector>
#include<arpa/inet.h>

#include <bits/stdc++.h> 
using namespace std;
#define PORT 8080 
unordered_map <string,string> seederList;
typedef struct file_info{
	string ip;
	string port;
	string hash;
	string filename;
}FILE_INFO;

string retrieve_data(string seederfile,unordered_map <string,string> &seederList){


	fstream file;
    string word;
 	file.open(seederfile);
 	while (file >> word) {
 		vector <string> token; 
        stringstream check1(word.c_str()); 
    	string intermediate;
     	while(getline(check1, intermediate, '|')) 
     { 
         token.push_back(intermediate); 
     }

     seederList[token[1]]=word;
       
    }

    file.close();
    return "NULL";
  	
}

void send_data_to_file(string seederfile,unordered_map <string,string> &seederList){

	fstream file;
    string word;
 	file.open(seederfile,ios::out);
 	for (auto it=seederList.begin(); it!=seederList.end(); ++it){
 		word=it->second;
 		file<<word<<endl;
 	}

    file.close();
   
}


void createConnection(int server_fd,int new_socket,string seederfile){
	string line;
	 
    
    //if(!seederList.empty())
    retrieve_data(seederfile,seederList);

    //send_data_to_file("seederlist",seederList);
    // for(auto itr=seederList.begin();itr!=seederList.end();itr++)
    // 	cout<<itr->first<<endl;
   
    int  valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    int b;
    string hello;
   
    

     char *buffer=(char*)malloc(sizeof(char)*1024);
 
 //    while(valread = read( new_soc	ket , buffer, 1024)) {


 //       	valread=fwrite(buffer,sizeof(char),valread,fptr);
 
 //    }
 

   
      
   
  
//while(1){

while(1){
	
	
 memset(buffer, 0, 1024);
 
    valread = read( new_socket , buffer, 1024);
    vector <string> tokens; 
     
    stringstream check1(buffer); 
    string intermediate;
     while(getline(check1, intermediate, '|')) 
     { 
         tokens.push_back(intermediate); 
     }

 if(tokens[0]=="exit"){
     	
     	new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen);
     	memset(buffer, 0, 1024);
     	//valread = read( new_socket , buffer, 1024);
     	//printf("%s",buffer); 

     }
        else if(tokens[1]=="get"){
     	//cout<<"2";
     	string returndata=seederList[tokens[0]];
     	//cout<<tokens[0]<<endl;
     	if(returndata=="")
     		returndata="null";
     	//cout<<returndata<<endl;
     	send(new_socket , returndata.c_str(), strlen(returndata.c_str()) , 0 ); 
     	new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen);
     	memset(buffer, 0, 1024);
     	//valread = read( new_socket , buffer, 1024);
     	//printf("%s",buffer); 

     }

    else if(tokens[1]=="remove"){
    	string returndata=seederList[tokens[0]];
     	seederList.erase(tokens[0]);
     	 if(!seederList.empty()){
     	 	send_data_to_file(seederfile,seederList);

   		}
   		else
   		{
   			ofstream myfile;
      	 myfile.open(seederfile,ios::out);
     	myfile<<"";
     	
     	myfile.close();	
   		}
     	//   myfile.open("seederlist");
     	// myfile<<hello<<endl;
     	// //seederList[tokens[1]]=buffer;
     	// myfile.close();	
     	//send(new_socket , "", strlen(returndata.c_str()) , 0 ); 
     	//memset(buffer, 0, 1024);
     	//valread = read( new_socket , buffer, 1024);
     	//printf("%s",buffer); 
     	//new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen);

     }

      	else if(tokens[4]=="share"){
      	seederList[tokens[1]]=buffer;
      	send_data_to_file(seederfile,seederList);
      	//new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen);
     	//printf("3");
     }

    // cout<<allfiles[0].filename;
       
    //getline(cin,hello);
	
    //send(new_socket , hello.c_str(), strlen(hello.c_str()) , 0 ); 

}
}

int main(int args,char *argv[]){
	//cout<<"!"<<endl;
	vector <string> ip_port;

     
    stringstream check1(argv[1]); 
    string intermediate;
     while(getline(check1, intermediate, ':')) 
     {
         ip_port.push_back(intermediate); 
     }
    
    string ip=ip_port[0];
    int port=atoi(ip_port[1].c_str());
   	string other_tracker=argv[2];
    string seederfile_name=argv[3];
    //if(!seederList.empty())
    
    //send_data_to_file("seederlist",seederList);
    // for(auto itr=seederList.begin();itr!=seederList.end();itr++)
    // 	cout<<itr->first<<endl;



   vector<thread> vothreads;
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    int b;
   
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    address.sin_family = AF_INET; 
   	//address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( port ); 
    address.sin_addr.s_addr = inet_addr(ip.c_str());
   	bind(server_fd, (struct sockaddr *)&address,sizeof(address));
    listen(server_fd, 3); 
    int i=0;
    while(new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen)){
    	
    	i++;
    	 cout<<i<<endl;
    	 //new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen);
    	 thread new_connection(createConnection,server_fd,new_socket,seederfile_name);
    	  vothreads.push_back(move(new_connection));
    	 
   }

   for(int i=0;i<vothreads.size();i++)
   		vothreads[i].join();
 	
  // pthread_t thread_id;
	
   
    //new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen);
 //    FILE *fptr;
 //   fptr = fopen("a.png", "w");
    

//}
   // printf("Hello message sent\n"); 

   
	return 0;
}