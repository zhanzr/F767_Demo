#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "enc28j60.h"
#include "ip_arp_udp_tcp.h"
#include "net.h"
#include "simple_server.h"

extern void LD2_ON(void);
extern void LD2_OFF(void);
extern volatile uint16_t g_ADCBuf[2];

// please modify the following two lines. mac and ip have to be unique
// in your local area network. You can not have the same numbers in
// two devices:
static unsigned char myip[4] = {192,168,32,33};
extern unsigned char mymac[6];

// base url (you can put a DNS name instead of an IP addr. if you have
// a DNS server (baseurl must end in "/"):
static char baseurl[]="http://192.168.1.1/";
static unsigned int mywwwport = 80; // listen port for tcp/www (max range 1-254)
// or on a different port:
//static char baseurl[]="http://10.0.0.24:88/";
//static unsigned int mywwwport =88; // listen port for tcp/www (max range 1-254)
//
static unsigned int myudpport =1200; // listen port for udp
// how did I get the mac addr? Translate the first 3 numbers into ascii is: TUX

#define BUFFER_SIZE 1500//400
static unsigned char buf[BUFFER_SIZE+1];

// the password string (only the first 5 char checked), (only a-z,0-9,_ characters):
static char password[]="SZIII"; // must not be longer than 9 char

// 
unsigned char verify_password(char *str)
{
	// the first characters of the received string are
	// a simple password/cookie:
	if (strncmp(password,str,5)==0)
	{
	    return(1);
	}
	return(0);
}

// takes a string of the form password/commandNumber and analyse it
// return values: -1 invalid password, otherwise command number
//                -2 no command given but password valid
signed char analyse_get_url(char *str)
{
	unsigned char i=0;
	if (verify_password(str)==0)
	{
		return(-1);
	}
	// find first "/"
	// passw not longer than 9 char:
	while(*str && i<10 && *str >',' && *str<'{')
	{
        if (*str=='/')
		{
            str++;
            break;
        }
        i++;
        str++;
	}
	if (*str < 0x3a && *str > 0x2f)
	{
        // is a ASCII number, return it
        return(*str-0x30);
	}
	return(-2);
}

// prepare the webpage by writing the data to the tcp send buffer
unsigned int print_webpage(unsigned char *buf,unsigned char on_off)
	{
    unsigned int plen;
    plen=fill_tcp_data_p(buf,0,("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nPragma: no-cache\r\n\r\n"));
    plen=fill_tcp_data_p(buf,plen,("<center><p>DS1 LED�����: "));
    if (on_off)
	{
        plen=fill_tcp_data_p(buf,plen,("<font color=\"#00FF00\"> ��</font>"));
    }
	else
	{
        plen=fill_tcp_data_p(buf,plen,("��"));
    }

    plen=fill_tcp_data_p(buf,plen,(" <small><a href=\""));
    plen=fill_tcp_data(buf,plen,baseurl);
    plen=fill_tcp_data(buf,plen,password);
    plen=fill_tcp_data_p(buf,plen,("\">[ˢ��]</a></small></p>\n<p><a href=\""));

    plen=fill_tcp_data(buf,plen,baseurl);
    plen=fill_tcp_data(buf,plen,password);
    if (on_off)
	{
        plen=fill_tcp_data_p(buf,plen,("/0\">�ر�DS1 LED</a><p>"));
    }
	else
	{
        plen=fill_tcp_data_p(buf,plen,("/1\">����DS1 LED</a><p>"));
	}

    plen=fill_tcp_data_p(buf,plen,("<center><hr><br>STM32F767 Nucleo BoardSTM32������ WEB ��ҳ����\n</center>"));
    
    return(plen);
}

int simple_server(void)
{  
    unsigned int plen,dat_p,i1=0,payloadlen=0;
    unsigned char i=0;
	char *buf1 = 0;
    signed char cmd;
    
	init_ip_arp_udp_tcp(mymac,myip,mywwwport);
    printf("MAC:0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n",mymac[0],mymac[1],mymac[2],mymac[3],mymac[4],mymac[5]);
    printf("IP:%d.%d.%d.%d\n",myip[0],myip[1],myip[2],myip[3]);
    printf("Port:%d\n",mywwwport);

	//init the ethernet/ip layer:
    while(1)
    {
        //�ж��Ƿ��н��յ���Ч�İ�
        plen = enc28j60PacketReceive(BUFFER_SIZE, buf);
        //����յ���Ч�İ���plen��Ϊ��0ֵ��
        if(plen==0)
        {
            continue; //û���յ���Ч�İ����˳����¼��
        }
		//���յ�Ŀ�ĵ�ַΪ����IP��ARP��ʱ������ARP��Ӧ��
        if(eth_type_is_arp_and_my_ip(buf,plen))
        {
			make_arp_answer_from_request(buf);
            continue;
        }
        
        //�ж��Ƿ���յ�Ŀ�ĵ�ַΪ����IP�ĺϷ���IP��
        if(eth_type_is_ip_and_my_ip(buf,plen)==0) 
        {
            continue;
        }
        //����յ�ICMP��
        if(buf[IP_PROTO_P]==IP_PROTO_ICMP_V && buf[ICMP_TYPE_P]==ICMP_TYPE_ECHOREQUEST_V)
        {

            printf("\n\r�յ�����[%d.%d.%d.%d]���͵�ICMP��",buf[ETH_ARP_SRC_IP_P],buf[ETH_ARP_SRC_IP_P+1],
                                                           buf[ETH_ARP_SRC_IP_P+2],buf[ETH_ARP_SRC_IP_P+3]);
			make_echo_reply_from_request(buf, plen);
			continue;
        }
        
        //����յ�TCP�����Ҷ˿�Ϊ80
		if (buf[IP_PROTO_P]==IP_PROTO_TCP_V&&buf[TCP_DST_PORT_H_P]==0&&buf[TCP_DST_PORT_L_P]==mywwwport)
		{
		    printf("\n\rSTM32F767 Nucleo Board���յ�TCP�����˿�Ϊ80��");
            if (buf[TCP_FLAGS_P] & TCP_FLAGS_SYN_V)
			{
			    printf("������ΪSYN");
                make_tcp_synack_from_syn(buf);
                continue;
            }
	        if (buf[TCP_FLAGS_P] & TCP_FLAGS_ACK_V)
			{
			    printf("������ΪACK");
	            init_len_info(buf); // init some data structures
	            dat_p=get_tcp_data_pointer();
	            if (dat_p==0)
				{
	                if (buf[TCP_FLAGS_P] & TCP_FLAGS_FIN_V)
					{
	                    make_tcp_ack_from_any(buf);/*������Ӧ*/
	                }
	                // ����һ��û�����ݵ�ACK��Ӧ���ȴ���һ����
	                continue;
	            }
				if (strncmp("GET ",(char *)&(buf[dat_p]),4)!=0)
				{
			        // �����Telnet��ʽ��¼������������ʾ��Ϣ
			        plen=fill_tcp_data_p(buf,0,("STM32F767 Nucleo Board\r\n\n\rHTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>200 OK</h1>"));
			        goto SENDTCP;
				}
				if (strncmp("/ ",(char *)&(buf[dat_p+4]),2)==0)
				{
				    //�����ͨ����ҳ��ʽ��¼�����������ʾ��Ϣ
			        plen=fill_tcp_data_p(buf,plen,("<p>Webserver: "));
							uint8_t tmp_adc_read_buf[64];
							sprintf((char*)tmp_adc_read_buf, "ADC:%u %f\n", g_ADCBuf[0], (g_ADCBuf[1]*3.3)/4095);
			        plen=fill_tcp_data(buf,plen,(const char*)tmp_adc_read_buf);
			        plen=fill_tcp_data_p(buf,plen,("password</p>"));
			        goto SENDTCP;
				}
                //������ҳ���Ƶ���������
				cmd=analyse_get_url((char *)&(buf[dat_p+5]));
				if (cmd==-1)
				{
			        plen=fill_tcp_data_p(buf,0,("HTTP/1.0 401 Unauthorized\r\nContent-Type: text/html\r\n\r\n<h1>401 Unauthorized</h1>"));
			        goto SENDTCP;
				}
                //��ҳ���Ƶ���LED��DS1
				if (cmd==1)
				{
                    LD2_ON();
					i=1;
				}
                //��ҳ����Ϩ��LED��DS1
				if (cmd==0)
				{
                    LD2_OFF();
					i=0;
				}
				//������ҳ��Ϣ
				plen=print_webpage(buf,(i));
				
			SENDTCP:
				make_tcp_ack_from_any(buf); // send ack for http get
				make_tcp_ack_with_data(buf,plen); // send data
				continue;
			}
		}

	    //UDP��������1200�˿ڵ�UDP��
		if (buf[IP_PROTO_P]==IP_PROTO_UDP_V&&buf[UDP_DST_PORT_H_P]==4&&buf[UDP_DST_PORT_L_P]==0xb0)
		{
			payloadlen=	  buf[UDP_LEN_H_P];
			payloadlen=payloadlen<<8;
			payloadlen=(payloadlen+buf[UDP_LEN_L_P])-UDP_HEADER_LEN;
	
            //ANSWER
            for(i1=0; i1<payloadlen; i1++)
            {         
                buf1[i1]=buf[UDP_DATA_P+i1];
            }
			
			//make_udp_reply_from_request(buf,str,strlen(str),myudpport);
			make_udp_reply_from_request(buf,buf1,payloadlen,myudpport);
		}
	}
}
