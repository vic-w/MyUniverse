// Convert.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include "decoder.h"

int main(int argc, char* argv[])
{
	//char filename[]="nxn.m4a";
	//char filename[]="C:\\Users\\blueds\\Downloads\\4 - 6 - 3.6 Complex Aggregates - Lists (6-53).mp4";
	char filename[]="WavinFlag.aac";
	//char filename[]="Track 1-1.mp3";
	if(decode_audio(filename) == 0)
		printf("Decode audio successfully.\n");

	return 0;
}
