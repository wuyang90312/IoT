#include "esp_server.h"
#include "espconn.h"
#include "debug.h"

static struct espconn *pTcpServer; /* So far only one single server thread is allowed */
static int local_port = 80; /* Set the TCP server listening port as port 80 */
static uint16_t server_timeover = 180;
static linkConType pLink;

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
	struct espconn *pespconn = (struct espconn *)arg;
	linkConType *linkTemp = (linkConType *)pespconn->reverse;
	char temp[32];

	INFO("receive data..\r\n");
	os_sprintf(temp, "\r\n+IPD,%d,%d:", linkTemp->linkId, len);
	INFO(temp);
	INFO(pdata);
	//uart0_tx_buffer(pdata, len);

}

/**
  * @brief  Tcp server connect repeat callback function.
  * @param  arg: contain the ip link information
  * @retval None
  */
LOCAL void ICACHE_FLASH_ATTR
ESP_TcpServer_Recon_cb(void *arg, sint8 errType)
{
  /*struct espconn *pespconn = (struct espconn *)arg;
  at_linkConType *linkTemp = (at_linkConType *)pespconn->reverse;
  char temp[16];

  os_printf("S conect C: %p\r\n", arg);

  if(pespconn == NULL)
  {
    return;
  }

  linkTemp->linkEn = false;
  linkTemp->pCon = NULL;
  os_printf("con EN? %d\r\n", linkTemp->linkId);
  at_linkNum--;
  if (at_linkNum == 0)
  {
    mdState = m_unlink;
  }

  if(at_ipMux)
  {
    os_sprintf(temp, "%d,CONNECT\r\n", linkTemp->linkId);
    uart0_sendStr(temp);
  }
  else
  {
    INFO("CONNECT\r\n");
  }

  disAllFlag = false;

  if(linkTemp->teToff == TRUE)
  {
    linkTemp->teToff = FALSE;
    at_backOk;
  }
  ETS_UART_INTR_ENABLE();
  specialAtState = true;
  at_state = at_statIdle;*/
  
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
	espconn_regist_disconcb(pespconn, NULL);
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
	INFO("Start a TCP server");
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

	return 1;
}
