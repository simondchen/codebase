#include "packet.h"
#include <stdio.h>
int get_start_packet(PACKET *pk_start,char *addr){
	memcpy(pk_start->dstmac,mac_8021x_standard,6);
	memcpy(pk_start->srcmac,addr,6);
	pk_start->auth_type[0]=0x88;
	pk_start->auth_type[1]=0x8e;
	pk_start->version=0x01;
	pk_start->pkt_type=PACKET_TYPE_START;
	memset(pk_start->length,0,2);
	return 0;
}

int get_identity_packet(PACKET *pk_id,int id,char *hwaddr){
	memcpy(pk_id->dstmac,mac_8021x_standard,6);
	memcpy(pk_id->srcmac,hwaddr,6);
	pk_id->auth_type[0]=0x88;
	pk_id->auth_type[1]=0x8e;
	pk_id->version=0x01;
	pk_id->pkt_type=PACKET_TYPE_EAP;
	//pay attention to big,little ending
	//pk_id->length[0]=0x0d;
	//pk_id->length[1]=0x00;
	pk_id->length[0]=0x00;
	pk_id->length[1]=0x0d;
	htons(pk_id);
	pk_id->body.response_identity.code=CODE_RESPONSE;
	pk_id->body.response_identity.id=id;
	pk_id->body.response_identity.bodylength[0]=0x00;
	pk_id->body.response_identity.bodylength[1]=0x0d;
	pk_id->body.response_identity.type=TYPE_IDENTITY;
	char *username="00101320";
	memcpy(pk_id->body.response_identity.identity,username,strlen(username));
	return 0;
}

int packet_type(PACKET *pkt){
	switch(pkt->pkt_type){
		case PACKET_TYPE_EAP:
			printf("eap packet\n");
			break;
		default:
			printf("unknown type:%d\n",pkt->pkt_type);
			break;
	}
	return pkt->body.test.type;
}
