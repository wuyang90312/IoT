#include "esp_server.h"
#include "espconn.h"
#include "debug.h"
#include "esp_string.h"

static struct espconn *pTcpServer; /* So far only one single server thread is allowed */
static int local_port = 80; /* Set the TCP server listening port as port 80 */
static uint16_t server_timeover = 180;
static linkConType pLink;
static BOOL WIFI_STA = FALSE;

/**
  * @brief  Client received callback function.
  * @param  arg: contain the ip link information
  * @param  pdata: received data
  * @param  len: the lenght of received data
  * @retval None
  */
LOCAL void ICACHE_FLASH_ATTR
ESP_TcpClient_Recv(void *arg, char *pdata, unsigned short len)
{
	char* msg;
	int length = 0;
	struct espconn *pespconn = (struct espconn *)arg;
	linkConType *linkTemp = (linkConType *)pespconn->reverse;
	char temp[32];

	INFO("receive data..\r\n");
	os_sprintf(temp, "+IPD,%d,%d:", linkTemp->linkId, len);
	INFO(temp);
	INFO("!---");
	INFO(pdata);
	INFO("---!");
	/* Take the 7th to 10th characters of pdata to temp */
	os_memset(temp, 0x00, sizeof(temp));
	os_memcpy(temp, pdata+6, 4);

	if(!os_strcmp(temp, "HTTP")) /* return a html if the client attach no info on URL */
	{
		INFO("-------------------------\r\n");
		/* Send a message back to the TCP client */
		msg = "<html><head><title> connection configuration </title></head><body>\
		<h1> CONFIGURATION </h1><form  method=\"get\">\
		<p><pre>   SSID      <input type=\"text\" name=\"SSID\"></pre> \
		<pre>   PASSWORD  <input type=\"text\" name=\"PAWD\"></pre>\
		<pre>   API KEY   <input type=\"text\" name=\"APIK\"></pre> \
		<pre>   Update Duration(Sec) <input type=\"text\" name=\"TIME\"></pre></p>\
		<pre>   <a type=\"submit\" value=\"submit\"><button>SUBMIT</button></a></pre></form></body></html>";
	}
	else{
		/* Parse the message got from url */
		uint16_t position, ptr[3];
		position = ESP_STR_ExtractData(5, "HTTP/1.1", pdata,8);
		char tmp[128];
		os_memset(tmp, 0x00, sizeof(tmp));
		os_memcpy(tmp, pdata+6, (position - 7));
		
		INFO("%d>>>>>>>>>>>>>>>>>>>>>>>%s\r\n",position, tmp);
		if(!ESP_STR_Delimit('&', 3, tmp, ptr))
			msg = " WRONG INFORMATION, PLEASE ENTER AGAIN";
		else{
			// TODO: MODULIZE the following part
			os_memset(temp, 0x00, sizeof(temp));
			os_memcpy(temp, tmp, ptr[0]);
			INFO("%d  %s\n",ptr[0], temp);
			os_memset(temp, 0x00, sizeof(temp));
			os_memcpy(temp, tmp+ptr[0]+1, ptr[1]-ptr[0]);
			INFO("%d  %s\n",ptr[1], temp);
			os_memset(temp, 0x00, sizeof(temp));
			os_memcpy(temp, tmp+ptr[1]+1, ptr[2]-ptr[1]);
			INFO("%d  %s\n",ptr[2], temp);
			os_memset(temp, 0x00, sizeof(temp));
			os_memcpy(temp, tmp+ptr[2]+1, strlen(tmp)-ptr[1]-1);
			INFO("%s\n", temp);
			msg = "<html><head><title> DONE </title></head><body>\
			<h1> SUCCESSFULLY UPDATE </h1> </body></html>";

			WIFI_STA = TRUE;
		}
	}

	length = os_strlen(msg);
	espconn_sent(pespconn, msg, length);

	INFO("SEND ALREADY \r\n");
	espconn_disconnect(pespconn);
	INFO("TCP disconnect ALREADY\r\n");


}

/**
  * @brief  Client received callback function.
  * @param  arg: contain the ip link information
  * @param  pdata: received data
  * @param  len: the lenght of received data
  * @retval None
  */
LOCAL void ICACHE_FLASH_ATTR
SP_TcpClient_Discon_cb(void *arg)
{
	if(WIFI_STA)
	{
		WIFI_Connect((uint8_t*)GLOBAL_SSID, (uint8_t*)GLOBAL_PWD, NULL);
		WIFI_STA = FALSE;
	}
}

/**
  * @brief  Tcp server connect repeat callback function.
  * @param  arg: contain the ip link information
  * @retval None
  */
LOCAL void ICACHE_FLASH_ATTR
ESP_TcpServer_Recon_cb(void *arg, sint8 errType)
{
  INFO("Re-connected\r\n");
}

/**
  * @brief  Tcp server listend callback function.
  * @param  arg: contain the ip link information
  * @retval None
  */
LOCAL void ICACHE_FLASH_ATTR
ESP_TcpServerListen(void *arg)
{
	struct espconn *pespconn = (struct espconn *)arg;

	INFO("get tcpClient:\r\n");

	pLink.teToff = FALSE;
	pLink.teType = teServer;
	pLink.repeaTime = 0;
	pLink.pCon = pespconn;
	pLink.linkId = 0;

	pespconn->reverse = &pLink;
	espconn_regist_recvcb(pespconn, ESP_TcpClient_Recv);
	espconn_regist_reconcb(pespconn, ESP_TcpServer_Recon_cb);
	espconn_regist_disconcb(pespconn, ESP_TcpClient_Discon_cb);
	espconn_regist_sentcb(pespconn, NULL);

}

/**
  * @brief  Setup commad of module as server.
  * @param  void
  * @retval  None
  */
uint32_t ICACHE_FLASH_ATTR
ESP_SetupIpServer()
{
	INFO("Start a TCP server\r\n");
	pTcpServer = (struct espconn *)os_zalloc(sizeof(struct espconn));
	if (pTcpServer == NULL)
	{
		INFO("TcpServer Failure\r\n");
		return 0xFFFFFFFF;
	}
	pTcpServer->type = ESPCONN_TCP;
	pTcpServer->state = ESPCONN_NONE;
	pTcpServer->proto.tcp = (esp_tcp *)os_zalloc(sizeof(esp_tcp));
	pTcpServer->proto.tcp->local_port = local_port;
	espconn_regist_connectcb(pTcpServer, ESP_TcpServerListen);
	espconn_accept(pTcpServer);
	espconn_regist_time(pTcpServer, server_timeover, 0);

	return 0;
}
