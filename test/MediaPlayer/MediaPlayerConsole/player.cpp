extern "C"
{
#include <libavcodec/avcodec.h> 
#include <libavformat/avformat.h>
#include "libswscale/swscale.h"
}
//SDL
#include "SDL.h"
#include "SDL_thread.h"

int main(int argc, char* argv[])
{
	char* filename = argv[1];

	av_register_all(); 

	//打开文件
	AVFormatContext *pFormatCtx = avformat_alloc_context();
	if(avformat_open_input(&pFormatCtx, filename, NULL, NULL)!=0)   return -1;

	//av_dump_format(pFormatCtx, 0, filename, 0);

	//读取流信息
	if (avformat_find_stream_info(pFormatCtx, NULL) < 0)   return -1;

	//找出第一个流
	int idxVideoStream = -1;
	for (unsigned int i = 0; i < pFormatCtx->nb_streams; i++)
	if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
	{
		idxVideoStream = i;
		break;
	}
	if(idxVideoStream==-1)    return -1;

	//找出Codec
	AVCodecContext *pCodecCtx = pFormatCtx->streams[idxVideoStream]->codec;
	AVCodec	*pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
	if(pCodec == NULL) return -1;

	//打开Codec
	if(avcodec_open2(pCodecCtx, pCodec, NULL)<0)   return -1;

	//申请存放帧的空间
	AVFrame *pFrame = av_frame_alloc();
	AVFrame *pFrameYUV = av_frame_alloc();

	int numBytes = avpicture_get_size(PIX_FMT_YUV420P, pCodecCtx->width,pCodecCtx->height);  
	uint8_t *buffer=new uint8_t[numBytes];

	avpicture_fill((AVPicture *)pFrameYUV, buffer, PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);

	//初始化SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) return -1;

	//打开显示窗口
	SDL_Surface *pScreen = SDL_SetVideoMode(pCodecCtx->width, pCodecCtx->height, 0, 0);
	if(!pScreen) return -1;

	//建立显示层
	SDL_Overlay *pOverlay = SDL_CreateYUVOverlay(pCodecCtx->width, pCodecCtx->height, SDL_YV12_OVERLAY, pScreen);

	AVPacket *pPacket = (AVPacket *)malloc(sizeof(AVPacket));
	av_new_packet(pPacket, pCodecCtx->width * pCodecCtx->height);

	while(av_read_frame(pFormatCtx, pPacket)>=0) 
	{ 
		if(pPacket->stream_index == idxVideoStream) 
		{ 
			//解析packet
			int frameFinished;
			if(avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, pPacket)<0) return -1;

			if(frameFinished)
			{
				SwsContext *pConvertCtx = sws_getContext(
					pCodecCtx->width, 
					pCodecCtx->height, 
					pCodecCtx->pix_fmt, 
					pCodecCtx->width, 
					pCodecCtx->height, 
					PIX_FMT_YUV420P, 
					SWS_BICUBIC, 
					NULL, NULL, NULL);

				sws_scale(
					pConvertCtx, 
					pFrame->data, 
					pFrame->linesize, 
					0, 
					pCodecCtx->height, 
					pFrameYUV->data, 
					pFrameYUV->linesize);
				sws_freeContext(pConvertCtx);


				SDL_LockYUVOverlay(pOverlay);
				pOverlay->pixels[0] = pFrameYUV->data[0];
				pOverlay->pixels[2] = pFrameYUV->data[1];
				pOverlay->pixels[1] = pFrameYUV->data[2];
				pOverlay->pitches[0] = pFrameYUV->linesize[0];
				pOverlay->pitches[2] = pFrameYUV->linesize[1];
				pOverlay->pitches[1] = pFrameYUV->linesize[2];
				SDL_UnlockYUVOverlay(pOverlay);
				SDL_Rect rect;
				rect.x = 0;
				rect.y = 0;
				rect.w = pCodecCtx->width;
				rect.h = pCodecCtx->height;
				SDL_DisplayYUVOverlay(pOverlay, &rect);

				SDL_Delay(40);
			}
		}
		av_free_packet(pPacket);

		//处理SDL消息
		SDL_Event event;       
		SDL_PollEvent(&event);    
		switch(event.type) 
		{    
		case  SDL_QUIT:      
			SDL_Quit();      
			exit(0);      
			break;    
		default:      
			break;    
		}
	}

	av_free(buffer); 
	av_free(pFrameYUV);   // Free the YUV frame 
	av_free(pFrame);   // Close the codec  
	avcodec_close(pCodecCtx);   // Close the video file  
	avformat_close_input(&pFormatCtx); 
}