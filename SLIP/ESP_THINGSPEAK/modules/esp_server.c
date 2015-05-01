#include "esp_server.h"


#include "rest.h"

static struct espconn *pTcpServer; /* So far only one single server thread is allowed */
static int local_port = 80; /* Set the TCP server listening port as port 80 */
static uint16_t server_timeover = 180;
static linkConType pLink;
static configInfo* config;
static BOOL WIFI_STA = FALSE;

/*--------------------------------------------------------------------------------
static ETSTimer timer;
static uint16_t len;
static char* hst;
static char* str;

void ICACHE_FLASH_ATTR
timer_cb(void *arg)
{
	uint32_t rst = (uint32_t) arg;
	REST_Request(rst, "GET", str);
}

void ICACHE_FLASH_ATTR
user_continue(void)
{
	uint32_t rst;
	uint16_t time = 60000;

	str = "/update?api_key=GPVP0E6QQVWU47LZ&field1=50";
	hst = "api.thingspeak.com";
	len = os_strlen(hst);

	WIFI_Connect("BCRLovs", "23456", NULL);
	
	rst = REST_Setup(hst, len, 80,0x00000000);

	os_timer_disarm(&timer);
	os_timer_setfn(&timer, (os_timer_func_t *)timer_cb, rst);
	os_timer_arm(&timer, 60000, 1);

}
--------------------------------------------------------------------------------*/



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

	/* Take the 7th to 10th characters of pdata to temp */
	os_memset(temp, 0x00, sizeof(temp));
	os_memcpy(temp, pdata+6, 4);

	if(!os_strcmp(temp, "HTTP")) /* return a html if the client attach no info on URL */
	{
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
		
		if(!ESP_STR_Delimit('&', 3, tmp, ptr))
			msg = " <h1>WRONG INFORMATION, PLEASE ENTER AGAIN <h1>";
		else{
			// TODO: MODULIZE the following part
			config = (configInfo*)os_zalloc(sizeof(configInfo));

			// TODO: The URL replace white space by +. Revert the change
			os_memcpy(config->SSID, tmp+5, ptr[0]-5);
			os_memcpy(config->PWD, tmp+ptr[0]+6, ptr[1]-ptr[0]-6);
			os_memcpy(config->API, tmp+ptr[1]+6, ptr[2]-ptr[1]-6);
			os_memcpy(config->TIME, tmp+ptr[2]+6, strlen(tmp)-ptr[1]-6);

			msg=""; /* Initialize the char array */
			os_sprintf(msg, "<html><head><title> DONE </title></head><body>\
			<h1> SUCCESSFULLY UPDATE </h1> <h2>%s <br> %s <br> %s <br> %s <br>\
			<h2></body></html>",config->SSID, config->PWD, config->API,config->TIME);
			
			WIFI_STA = TRUE;
		}
	}

	length = os_strlen(msg);
	espconn_sent(pespconn, msg, length);
	espconn_disconnect(pespconn);

	//system_deep_sleep_set_option(0);
	//system_deep_sleep(30000000);
}

/**
  * @brief  Callback function for disconnect the TCP
  * @param  arg: ESPCONN STRUCT
  * @retval None
  */
LOCAL void ICACHE_FLASH_ATTR
ESP_TcpClient_Discon_cb(void *arg)
{
	if(WIFI_STA)
	{
		uint32_t input =0x10101010;
		/* Branch to the TCP update function in main.c */
		WIFI_STA = FALSE;
		/* Delete the TCP connection */
		os_free(pTcpServer);
		/* Store information into permanent memory, set the flag */
		spi_flash_erase_sector(0x3f);
		spi_flash_write((0x3f)*4096, (uint32 *) &input, 4);
		spi_flash_write((0x3f)*4096+4, (uint32 *) config, sizeof(configInfo));
		os_free(config);
		/* Restart the system */
		system_restart();
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
