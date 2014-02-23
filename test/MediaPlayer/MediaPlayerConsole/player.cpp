extern "C"
{
#include <libavcodec/avcodec.h> 
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
}
//SDL
#include "SDL.h"
#include "SDL_thread.h"
#include "SDL_audio.h"

int quit = 0;

SwrContext *pSwrCtx;

//ffmpeg的回调函数，用来检测是否有退出消息
int decode_interrupt_cb(void) 
{   
	return quit; 
}

typedef struct PacketQueue 
{    
	AVPacketList *first_pkt, *last_pkt;   
	int nb_packets;   
	int size;    
	SDL_mutex *mutex;   
	SDL_cond *cond; 
} PacketQueue;

PacketQueue audioQueue;

void packet_queue_init(PacketQueue *q) 
{   
	memset(q, 0, sizeof(PacketQueue));   
	q->mutex = SDL_CreateMutex();   
	q->cond = SDL_CreateCond(); 
} 

int packet_queue_put(PacketQueue *q, AVPacket *pkt) 
{  
	AVPacketList *pkt1;    
	if(av_dup_packet(pkt) < 0) 
	{     
		return -1;   
	}    
	pkt1 = (AVPacketList*)av_malloc(sizeof(AVPacketList));   
	if (!pkt1)     
		return -1;    
	pkt1->pkt = *pkt;   
	pkt1->next = NULL;      
	SDL_LockMutex(q->mutex);     
	if (!q->last_pkt)      
		q->first_pkt = pkt1;   
	else      
		q->last_pkt->next = pkt1;   
	q->last_pkt = pkt1;   
	q->nb_packets++;    
	q->size += pkt1->pkt.size;   
	SDL_CondSignal(q->cond);     
	SDL_UnlockMutex(q->mutex);   
	return 0; 
}
static int packet_queue_get(PacketQueue *q, AVPacket *pkt, int block) 
{   
	AVPacketList *pkt1;   
	int ret;     
	SDL_LockMutex(q->mutex);     
	for(;;) 
	{          
		if(quit) 
		{ 
			ret = -1;       
			break;     
		}       
		pkt1 = q->first_pkt;     
		if (pkt1) 
		{        
			q->first_pkt = pkt1->next;       
			if (!q->first_pkt)     
				q->last_pkt = NULL;       
			q->nb_packets--;        
			q->size -= pkt1->pkt.size;       
			*pkt = pkt1->pkt;       
			av_free(pkt1);       
			ret = 1;       
			break;      
		} 
		else if (!block) 
		{       
			ret = 0;       
			break;     
		} 
		else 
		{        
			SDL_CondWait(q->cond, q->mutex);     
		}   
	}    
	SDL_UnlockMutex(q->mutex);   
	return ret; 
}

int audio_decode_frame(AVCodecContext *pAudioCodecCtx, uint8_t *audio_buf, int buf_size) 
{     
	AVFrame *pAudioFrame=av_frame_alloc();  
	AVPacket pkt,pkt1;  
    int frame_finished=0;  
    int pkt_pos,pkt_len;  
    int src_len=0,dst_len=0,data_size=0;  
    float pts=0;  
    //av_frame_unref(pAudioFrame);  
  
    uint8_t *out[]={audio_buf};  
      
    for(;!quit;){  
        if(packet_queue_get(&audioQueue, &pkt, 1) < 0) {  
            av_free(pAudioFrame);  
            return -1;  
        }  
        pkt1=pkt;  
        pkt_pos=0;  
        pkt_len=pkt.size;  
          
        while(pkt_pos<pkt.size && !quit)
		{  
            if((src_len = avcodec_decode_audio4(pAudioCodecCtx, pAudioFrame, &frame_finished, &pkt1))<0)
			{  
                av_free_packet(&pkt);  
                av_free(pAudioFrame);  
                return -1;  
            }  
              
            if(frame_finished)
			{  
                int len=swr_convert
					(
					pSwrCtx,
					out,
					buf_size/pAudioCodecCtx->channels/av_get_bytes_per_sample(AV_SAMPLE_FMT_S16),  
                    (const uint8_t **)pAudioFrame->data,
					pAudioFrame->nb_samples
					);  
                len=len * pAudioCodecCtx->channels * av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);  
                av_free(pAudioFrame);  
                av_free_packet(&pkt);  
                return len;  
            }
			else
			{  
                if (!pkt1.data && pAudioCodecCtx->codec->capabilities & CODEC_CAP_DELAY)
				{  
                    break;  
                }  
            }  
            pkt_pos+=src_len;//已经解码的长度  
            pkt1.data=pkt.data+pkt_pos;  
            pkt1.size=pkt.size-pkt_pos;  
        }  
        av_free_packet(&pkt);  
    }  
    av_free(pAudioFrame);  
    return dst_len;  
}

void audio_callback(void *userdata, Uint8 *stream, int len) 
{     

#define AVCODEC_MAX_AUDIO_FRAME_SIZE 1024

	AVCodecContext *aCodecCtx = (AVCodecContext *)userdata;   
	int len1, audio_size;     
	static uint8_t audio_buf[(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2];   
	static unsigned int audio_buf_size = 0;   
	static unsigned int audio_buf_index = 0;     
	while(len > 0) 
	{      
		if(audio_buf_index >= audio_buf_size) 
		{              
			audio_size = audio_decode_frame(aCodecCtx, audio_buf, sizeof(audio_buf));       
			if(audio_size < 0) 
			{          
				audio_buf_size = 1024;      
				memset(audio_buf, 0, audio_buf_size);       
			} 
			else 
			{      
				audio_buf_size = audio_size;       
			}        
			audio_buf_index = 0;     
		}      
		len1 = audio_buf_size - audio_buf_index;     
		if(len1 > len)       
			len1 = len;      
		memcpy(stream, (uint8_t *)audio_buf + audio_buf_index, len1);     
		len -= len1;     
		stream += len1;      
		audio_buf_index += len1;   
	} 
}

int main(int argc, char* argv[])
{
	char* filename = argv[1];

	av_register_all(); 

	//注册回调函数
	//avio_set_interrupt_cb(decode_interrupt_cb); 

	//打开文件
	AVFormatContext *pFormatCtx = avformat_alloc_context();
	if(avformat_open_input(&pFormatCtx, filename, NULL, NULL)!=0)   return -1;

	//显示文件信息
	av_dump_format(pFormatCtx, 0, filename, 0);

	//读取流信息
	if (avformat_find_stream_info(pFormatCtx, NULL) < 0)   return -1;

	//找出第一个视频流
	int idxVideoStream = -1;
	for (unsigned int i = 0; i < pFormatCtx->nb_streams; i++)
	if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
	{
		idxVideoStream = i;
		break;
	}
	if(idxVideoStream==-1)    return -1;

	//找出第一个音频流
	int idxAudioStream = -1;
	for (unsigned int i = 0; i < pFormatCtx->nb_streams; i++)
	if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
	{
		idxAudioStream = i;
		break;
	}
	if(idxAudioStream==-1)    return -1;


	//找出视频Codec
	AVCodecContext *pVideoCodecCtx = pFormatCtx->streams[idxVideoStream]->codec;
	AVCodec	*pVideoCodec = avcodec_find_decoder(pVideoCodecCtx->codec_id);
	if(pVideoCodec == NULL) return -1;

	//打开视频Codec
	if(avcodec_open2(pVideoCodecCtx, pVideoCodec, NULL)<0)   return -1;

	//找出音频Codec
	AVCodecContext *pAudioCodecCtx = pFormatCtx->streams[idxAudioStream]->codec;
	AVCodec *pAudioCodec = avcodec_find_decoder(pAudioCodecCtx->codec_id); 
	if(pAudioCodec == NULL) return -1;

	//打开音频Codec
	if(avcodec_open2(pAudioCodecCtx, pAudioCodec, NULL)<0)  return -1;

	//初始化音频重采样模块
	int64_t wanted_channel_layout = 0;  
    int wanted_nb_channels;  
    wanted_channel_layout =   
        (pAudioCodecCtx->channel_layout && pAudioCodecCtx->channels == av_get_channel_layout_nb_channels(pAudioCodecCtx->channel_layout)) ? 
		pAudioCodecCtx->channel_layout 
		: av_get_default_channel_layout(pAudioCodecCtx->channels);  

    wanted_channel_layout &= ~AV_CH_LAYOUT_STEREO_DOWNMIX;  
    wanted_nb_channels = av_get_channel_layout_nb_channels(wanted_channel_layout);
	pSwrCtx = swr_alloc_set_opts
		(
		NULL,
		wanted_channel_layout,
		AV_SAMPLE_FMT_S16,
		pAudioCodecCtx->sample_rate,  
        pAudioCodecCtx->channel_layout,
		pAudioCodecCtx->sample_fmt,  
        pAudioCodecCtx->sample_rate,
		0,
		NULL
		);  
    if(swr_init(pSwrCtx)<0) return -1;  

	//申请存放帧的空间
	AVFrame *pFrame = av_frame_alloc();
	AVFrame *pFrameYUV = av_frame_alloc();

	int numBytes = avpicture_get_size(PIX_FMT_YUV420P, pVideoCodecCtx->width,pVideoCodecCtx->height);  
	uint8_t *buffer=new uint8_t[numBytes];

	avpicture_fill((AVPicture *)pFrameYUV, buffer, PIX_FMT_YUV420P, pVideoCodecCtx->width, pVideoCodecCtx->height);

	//初始化SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) return -1;

	//打开显示窗口
	SDL_Surface *pScreen = SDL_SetVideoMode(pVideoCodecCtx->width, pVideoCodecCtx->height, 0, 0);
	if(!pScreen) return -1;

	//建立显示层
	SDL_Overlay *pOverlay = SDL_CreateYUVOverlay(pVideoCodecCtx->width, pVideoCodecCtx->height, SDL_YV12_OVERLAY, pScreen);

	//打开声音
	SDL_AudioSpec desiredAudioSpec, obtainedAudioSpec;
	desiredAudioSpec.freq = pAudioCodecCtx->sample_rate; 
	desiredAudioSpec.format = AUDIO_S16SYS;  
	desiredAudioSpec.channels = pAudioCodecCtx->channels; 
	desiredAudioSpec.silence = 0;  
	desiredAudioSpec.samples = 1024; 
	desiredAudioSpec.callback = audio_callback; 
	desiredAudioSpec.userdata = pAudioCodecCtx;

	if(SDL_OpenAudio(&desiredAudioSpec, &obtainedAudioSpec) < 0) return -1;

	packet_queue_init(&audioQueue);   
	SDL_PauseAudio(0);

	AVPacket *pPacket = (AVPacket *)malloc(sizeof(AVPacket));
	av_new_packet(pPacket, pVideoCodecCtx->width * pVideoCodecCtx->height);

	while(av_read_frame(pFormatCtx, pPacket)>=0) 
	{ 
		if(pPacket->stream_index == idxVideoStream) 
		{ 
			//解析packet
			int frameFinished;
			if(avcodec_decode_video2(pVideoCodecCtx, pFrame, &frameFinished, pPacket)<0) return -1;

			if(frameFinished)
			{
				SwsContext *pConvertCtx = sws_getContext(
					pVideoCodecCtx->width, 
					pVideoCodecCtx->height, 
					pVideoCodecCtx->pix_fmt, 
					pVideoCodecCtx->width, 
					pVideoCodecCtx->height, 
					PIX_FMT_YUV420P, 
					SWS_BICUBIC, 
					NULL, NULL, NULL);

				sws_scale(
					pConvertCtx, 
					pFrame->data, 
					pFrame->linesize, 
					0, 
					pVideoCodecCtx->height, 
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
				rect.w = pVideoCodecCtx->width;
				rect.h = pVideoCodecCtx->height;
				SDL_DisplayYUVOverlay(pOverlay, &rect);

				//SDL_Delay(40);
			}
		}
		else if(pPacket->stream_index==idxAudioStream) 
		{  
			packet_queue_put(&audioQueue, pPacket);   
		} 
		else 
		{
			av_free_packet(pPacket);
		}

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
	avcodec_close(pVideoCodecCtx);   // Close the video file  
	avformat_close_input(&pFormatCtx); 

	return 1;
}