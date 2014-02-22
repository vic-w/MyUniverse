// Convert.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include "decoder.h"

int main(int argc, char* argv[])
{
	//char filename[]="nxn.m4a";
	//char filename[]="nxn.wma";
	char filename[]="WavinFlag.aac";
	//char filename[]="nwn.flv";
	if(decode_audio(filename) == 0)
		printf("Decode audio successfully.\n");

	return 0;
}
