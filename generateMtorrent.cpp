#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include<string.h>
#include"generateMtorrent.h"
using namespace std;

void generateMtorrent (string filename , string sha,string torrentfile,string t1ip,int t1port,string t2ip,int t2port){
  struct stat st;
    stat(filename.c_str(), &st);
    long long size = st.st_size;   
  ofstream myfile;
  string tracker_ip1=t1ip;
  string tracker_ip2=t2ip;
  int port1=t1port;
  int port2=t2port;
    string name="";
  for(int i=0;i<20;i++)
    name+=sha[i];
  myfile.open (torrentfile);
  myfile<<tracker_ip1<<":"<<port1<<endl;
  myfile<<tracker_ip2<<":"<<port2<<endl;
    myfile<<filename<<endl;
      myfile<<size<<endl;
      myfile<<sha<<endl;
  myfile.close();
}