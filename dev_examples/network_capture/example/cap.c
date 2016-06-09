
// gcc -g -Wall cap.c -o cap -lpcap

#include <pcap.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define MAXBYTES2CAPTURE 2048

void processPacket(u_char * arg,
    const struct pcap_pkthdr* pkthdr,
    const u_char * packet)
{
    int i=0, *counter =(int*)arg;
    
    printf("------------------------- \n");
    printf("Packet count: %d \n", ++(*counter));
    printf("Recved packet size: %d \n", pkthdr->len);
    printf("Payload: \n");
    
    for(i=0;i<pkthdr->len;i++)
    {
//        printf("%2x ", packet[i]);
        if( packet[i]>='a' && packet[i]<='z' )
            printf(" %c ",packet[i]);
        else
            printf("%2x ", packet[i]);
        
        if( (i%16==0&&i!=0) || i==pkthdr->len-1 )
            printf("\n");
    }
    return ;
}

int main(int argc,char * argv[])
{
    int count =0;
    char dev[] = "eth0";		/* Device to sniff on */
    pcap_t * descr =NULL;
    char errbuf[PCAP_ERRBUF_SIZE], *device=NULL;
    
    struct bpf_program fp;		/* The compiled filter expression */
	char filter_exp[] = "port 18600";	/* The filter expression */
	 bpf_u_int32 mask;		/* The netmask of our sniffing device */
	 bpf_u_int32 net;		/* The IP of our sniffing device */
    
    memset(errbuf,0,PCAP_ERRBUF_SIZE);
    
    if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1) {
		 fprintf(stderr, "Can't get netmask for device %s\n", dev);
		 net = 0;
		 mask = 0;
	 }
     
    descr =pcap_open_live(device, MAXBYTES2CAPTURE, 1, 512, errbuf);
    
    if (descr == NULL) {
        fprintf(stderr, "Couldn't open device %s: %s\n", device, errbuf);
        return(2);
    }
    
    if (pcap_compile(descr, &fp, filter_exp, 0, net) == -1) {
        fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(descr));
        return(2);
    }
    if (pcap_setfilter(descr, &fp) == -1) {
        fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(descr));
        return(2);
    }
     
     
    pcap_loop(descr, -1, processPacket, (u_char*)&count);
    
    return 0;
}








