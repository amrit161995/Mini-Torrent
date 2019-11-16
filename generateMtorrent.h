#ifndef GENERATEMTORRENT_H_  
#define GENERATEMTORRENT_H_ 
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<iostream>
#include <openssl/sha.h>
#include <fstream>
#include<vector>
using namespace std;

void generateMtorrent (string filename , string sha,string torrentfile,string t1ip,int t1port,string t2ip,int t2port); 

#endif // GENERATEMTORRENT_H_ 
