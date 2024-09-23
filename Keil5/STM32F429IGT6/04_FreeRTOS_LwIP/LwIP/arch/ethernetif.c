#include "main.h"
#include "lwip/opt.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/timeouts.h"
#include "netif/ethernet.h"
#include "netif/etharp.h"
#include "lwip/ethip6.h"
#include "ethernetif.h"
#include <string.h>
#include "debug.h"
#include "led.h"


/* Network interface name */
#define IFNAME0 'e'
#define IFNAME1 'n'

sys_sem_t tx_sem = NULL;
sys_mbox_t eth_tx_mb = NULL;
xSemaphoreHandle s_xSemaphore = NULL;


struct ethernetif {
	struct eth_addr *ethaddr;
/* Add whatever per-interface state that is needed here. */
};


extern ETH_HandleTypeDef ETH_Handler;

static void arp_timer(void *arg);


static void low_level_init(struct netif *netif)
{ 
  HAL_StatusTypeDef hal_eth_init_status;
  
  hal_eth_init_status = HAL_OK;   //HAL_OK;

  if (hal_eth_init_status == HAL_OK)
  {
    /* Set netif link flag */  
    netif->flags |= NETIF_FLAG_LINK_UP;
  }

#if LWIP_ARP || LWIP_ETHERNET 

  /* set MAC hardware address length */
  netif->hwaddr_len = ETH_HWADDR_LEN;
  
  /* set MAC hardware address */
//  netif->hwaddr[0] =  ETH_Handler.Init.MACAddr[0];
//  netif->hwaddr[1] =  ETH_Handler.Init.MACAddr[1];
//  netif->hwaddr[2] =  ETH_Handler.Init.MACAddr[2];
//  netif->hwaddr[3] =  ETH_Handler.Init.MACAddr[3];
//  netif->hwaddr[4] =  ETH_Handler.Init.MACAddr[4];
//  netif->hwaddr[5] =  ETH_Handler.Init.MACAddr[5];
  
  
  netif->hwaddr[0] =  0x02;
  netif->hwaddr[1] =  0x00;
  netif->hwaddr[2] =  0x00;
  netif->hwaddr[3] =  0x00;
  netif->hwaddr[4] =  0x00;
  netif->hwaddr[5] =  0x00;
  
//  int i = 0;
//  for(i = 0; i < 6; i++)
//  {
//	  printf("%d ", ETH_Handler.Init.MACAddr[i]);
//  }
//  printf("\r\n");
//  for(i = 0; i < 6; i++)
//  {
//	  printf("%d ", netif->hwaddr[i]);
//  }
//  printf("\r\n");
  
  
  /* maximum transfer unit */
  netif->mtu = NETIF_MTU;
  
  #if LWIP_ARP
    netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;
  #else
    netif->flags |= NETIF_FLAG_BROADCAST;
  #endif /* LWIP_ARP */
  
  
  s_xSemaphore = xSemaphoreCreateCounting(40,0);
  
  if(sys_sem_new(&tx_sem , 0) == ERR_OK)
    PRINT_DEBUG("sys_sem_new ok\n");
  
  if(sys_mbox_new(&eth_tx_mb , 50) == ERR_OK)
    PRINT_DEBUG("sys_mbox_new ok\n");

  /* create the task that handles the ETH_MAC */
	sys_thread_new("ETHIN",
                  ethernetif_input,  /* 任务入口函数 */
                  netif,        	  /* 任务入口函数参数 */
                  NETIF_IN_TASK_STACK_SIZE,/* 任务栈大小 */
                  NETIF_IN_TASK_PRIORITY); /* 任务的优先级 */
//	sys_thread_new("ETHTX",
//                  ethernetif_output,  /* 任务入口函数 */
//                  netif,        	  /* 任务入口函数参数 */
//                  NETIF_OUT_TASK_STACK_SIZE,/* 任务栈大小 */
//                  NETIF_OUT_TASK_PRIORITY); /* 任务的优先级 */

    HAL_NVIC_SetPriority(ETH_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(ETH_IRQn);

	/* Enable ETHERNET clock  */
	__HAL_RCC_ETH_CLK_ENABLE();
	/* USER CODE END ETH_MspInit 1 */

	/* Enable MAC and DMA transmission and reception */
	HAL_ETH_Start(&ETH_Handler);
	
#endif /* LWIP_ARP || LWIP_ETHERNET */
}


static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
	static sys_sem_t ousem = NULL;
	if(ousem == NULL)
	{
		sys_sem_new(&ousem,0);
		sys_sem_signal(&ousem);
	}
	
	err_t errval;
	struct pbuf *q;

	uint8_t *buffer = (uint8_t *)(ETH_Handler.TxDesc->Buffer1Addr);
	__IO ETH_DMADescTypeDef *DmaTxDesc;
	uint32_t framelength = 0;
	uint32_t bufferoffset = 0;
	uint32_t byteslefttocopy = 0;
	uint32_t payloadoffset = 0;
	DmaTxDesc = ETH_Handler.TxDesc;
	bufferoffset = 0;

	if((DmaTxDesc->Status & ETH_DMATXDESC_OWN) != (uint32_t)RESET)
	{
		errval = ERR_USE;
		goto error;
	}
	sys_sem_wait(&ousem);
  
  /* copy frame from pbufs to driver buffers */
	for(q = p; q != NULL; q = q->next)
    {
      /* Get bytes in current lwIP buffer */
      byteslefttocopy = q->len;
      payloadoffset = 0;
    
      /* Check if the length of data to copy is bigger than Tx buffer size*/
      while( (byteslefttocopy + bufferoffset) > ETH_TX_BUF_SIZE )
      {
        /* Copy data to Tx buffer*/
        memcpy( (uint8_t*)((uint8_t*)buffer + bufferoffset),
        (uint8_t*)((uint8_t*)q->payload + payloadoffset),
        (ETH_TX_BUF_SIZE - bufferoffset) );
      
        /* Point to next descriptor */
        DmaTxDesc = (ETH_DMADescTypeDef *)(DmaTxDesc->Buffer2NextDescAddr);
      
        /* Check if the buffer is available */
        if((DmaTxDesc->Status & ETH_DMATXDESC_OWN) != (uint32_t)RESET)
        {
			errval = ERR_USE;
			goto error;
        }
      
        buffer = (uint8_t *)(DmaTxDesc->Buffer1Addr);
      
        byteslefttocopy = byteslefttocopy - (ETH_TX_BUF_SIZE - bufferoffset);
        payloadoffset = payloadoffset + (ETH_TX_BUF_SIZE - bufferoffset);
        framelength = framelength + (ETH_TX_BUF_SIZE - bufferoffset);
        bufferoffset = 0;
      }
    
      /* Copy the remaining bytes */
      memcpy( (uint8_t*)((uint8_t*)buffer + bufferoffset),
      (uint8_t*)((uint8_t*)q->payload + payloadoffset), byteslefttocopy );
      bufferoffset = bufferoffset + byteslefttocopy;
      framelength = framelength + byteslefttocopy;
    }
  
  /* Prepare transmit descriptors to give to DMA */ 
  HAL_ETH_TransmitFrame(&ETH_Handler, framelength);
  
  errval = ERR_OK;

error:
  
  if ((ETH_Handler.Instance->DMASR & ETH_DMASR_TUS) != (uint32_t)RESET)
  {
    /* Clear TUS ETHERNET DMA flag */
    ETH_Handler.Instance->DMASR = ETH_DMASR_TUS;

    /* Resume DMA transmission*/
    ETH_Handler.Instance->DMATPDR = 0;
  }
  
  sys_sem_signal(&ousem);
  
  return errval;
}


static struct pbuf * low_level_input(struct netif *netif)
{
  struct pbuf *p = NULL;
  struct pbuf *q = NULL;
  uint16_t len = 0;
  uint8_t *buffer;
  __IO ETH_DMADescTypeDef *dmarxdesc;
  uint32_t bufferoffset = 0;
  uint32_t payloadoffset = 0;
  uint32_t byteslefttocopy = 0;
  uint32_t i=0;
  
  
  /* get received frame */
  if (HAL_ETH_GetReceivedFrame(&ETH_Handler) != HAL_OK)
  {
    PRINT_ERR("receive frame faild\n");
    return NULL;
  }
  /* Obtain the size of the packet and put it into the "len" variable. */
  len = ETH_Handler.RxFrameInfos.length;
  buffer = (uint8_t *)ETH_Handler.RxFrameInfos.buffer;
  
  PRINT_INFO("receive frame %d\n", len);
  if (len > 0)
  {
    /* We allocate a pbuf chain of pbufs from the Lwip buffer pool */
    p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);
  }
  
  if (p != NULL)
  {
    dmarxdesc = ETH_Handler.RxFrameInfos.FSRxDesc;
    bufferoffset = 0;
    for(q = p; q != NULL; q = q->next)
    {
      byteslefttocopy = q->len;
      payloadoffset = 0;
      

      while( (byteslefttocopy + bufferoffset) > ETH_RX_BUF_SIZE )
      {
        /* Copy data to pbuf */
        memcpy( (uint8_t*)((uint8_t*)q->payload + payloadoffset), 
        (uint8_t*)((uint8_t*)buffer + bufferoffset), 
        (ETH_RX_BUF_SIZE - bufferoffset));
        
        /* Point to next descriptor */
        dmarxdesc = (ETH_DMADescTypeDef *)(dmarxdesc->Buffer2NextDescAddr);
        buffer = (uint8_t *)(dmarxdesc->Buffer1Addr);
        
        byteslefttocopy = byteslefttocopy - (ETH_RX_BUF_SIZE - bufferoffset);
        payloadoffset = payloadoffset + (ETH_RX_BUF_SIZE - bufferoffset);
        bufferoffset = 0;
      }
      /* Copy remaining data in pbuf */
      memcpy( (uint8_t*)((uint8_t*)q->payload + payloadoffset),
      (uint8_t*)((uint8_t*)buffer + bufferoffset), byteslefttocopy);
      bufferoffset = bufferoffset + byteslefttocopy;
    }
  }  
  
    /* Release descriptors to DMA */
    /* Point to first descriptor */
    dmarxdesc = ETH_Handler.RxFrameInfos.FSRxDesc;
    /* Set Own bit in Rx descriptors: gives the buffers back to DMA */
    for (i=0; i< ETH_Handler.RxFrameInfos.SegCount; i++)
    {  
      dmarxdesc->Status |= ETH_DMARXDESC_OWN;
      dmarxdesc = (ETH_DMADescTypeDef *)(dmarxdesc->Buffer2NextDescAddr);
    }
    
    /* Clear Segment_Count */
    ETH_Handler.RxFrameInfos.SegCount =0;  
  
  /* When Rx Buffer unavailable flag is set: clear it and resume reception */
  if ((ETH_Handler.Instance->DMASR & ETH_DMASR_RBUS) != (uint32_t)RESET)  
  {
    /* Clear RBUS ETHERNET DMA flag */
    ETH_Handler.Instance->DMASR = ETH_DMASR_RBUS;
    /* Resume DMA reception */
    ETH_Handler.Instance->DMARPDR = 0;
  }
  return p;
}

#if 0
void ethernetif_input(struct netif *netif)
{
  err_t err;
  struct pbuf *p;

  /* move received packet into a new pbuf */
  p = low_level_input(netif);
  
	while(p)
	{
		err = netif->input(p, netif);
		if(err != HAL_OK)
		{
			LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
			pbuf_free(p);
			p = NULL;
		}			
		p = low_level_input(netif);
	}

  /* no packet could be read, silently ignore this */
//  if (p == NULL) return;
//    
//  /* entry point to the LwIP stack */
//  err = netif->input(p, netif);
//    
//  if (err != ERR_OK)
//  {
//    LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
//    pbuf_free(p);
//    p = NULL;    
//  }
}
#endif


void ethernetif_input(void *pParams) {
	struct netif *netif;
	struct pbuf *p = NULL;
	netif = (struct netif*) pParams;
    LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));

	while(1) 
    {
        if(xSemaphoreTake( s_xSemaphore, portMAX_DELAY ) == pdTRUE)
		{
			/* move received packet into a new pbuf */
			// taskENTER_CRITICAL();
			p = low_level_input(netif);
			// taskEXIT_CRITICAL();
			
			/* points to packet payload, which starts with an Ethernet header */
			if(p != NULL)
			{
				taskENTER_CRITICAL();
				/* full packet send to tcpip_thread to process */
				if (netif->input(p, netif) != ERR_OK)
				{
					LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
					pbuf_free(p);
					p = NULL;
				}
				taskEXIT_CRITICAL();
			}
		}
	}
}

#if !LWIP_ARP

static err_t low_level_output_arp_off(struct netif *netif, struct pbuf *q, const ip4_addr_t *ipaddr)
{  
  err_t errval;
  errval = ERR_OK;

  return errval;
  
}
#endif /* LWIP_ARP */ 

err_t ethernetif_init(struct netif *netif)
{
 	struct ethernetif *ethernetif;

	// LWIP_ASSERT("netif != NULL", (netif != NULL));

	ethernetif = mem_malloc(sizeof(struct ethernetif));

	if (ethernetif == NULL) {
		PRINT_ERR("ethernetif_init: out of memory\n");
		return ERR_MEM;
	}
  
  LWIP_ASSERT("netif != NULL", (netif != NULL));
//  
#if LWIP_NETIF_HOSTNAME
  /* Initialize interface hostname */
  netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */
  netif->state = ethernetif;
  netif->name[0] = IFNAME0;
  netif->name[1] = IFNAME1;

#if LWIP_IPV4
#if LWIP_ARP || LWIP_ETHERNET
#if LWIP_ARP
  netif->output = etharp_output;
#else
  netif->output = low_level_output_arp_off;
#endif /* LWIP_ARP */
#endif /* LWIP_ARP || LWIP_ETHERNET */
#endif /* LWIP_IPV4 */
 
#if LWIP_IPV6
  netif->output_ip6 = ethip6_output;
#endif /* LWIP_IPV6 */

  netif->linkoutput = low_level_output;
  
  /* initialize the hardware */
  low_level_init(netif);
  ethernetif->ethaddr = (struct eth_addr *) &(netif->hwaddr[0]);

  return ERR_OK;
}

static void arp_timer(void *arg)
{
  etharp_tmr();
  sys_timeout(ARP_TMR_INTERVAL, arp_timer, NULL);
}

//u32_t sys_now()
//{
//	return HAL_GetTick();
//}

/* USER CODE BEGIN 6 */

#if LWIP_NETIF_LINK_CALLBACK

void ethernetif_update_config(struct netif *netif)
{
  __IO uint32_t tickstart = 0;
  uint32_t regvalue = 0;
  
  if(netif_is_link_up(netif))
  { 
    /* Restart the auto-negotiation */
    if(heth.Init.AutoNegotiation != ETH_AUTONEGOTIATION_DISABLE)
    {
      /* Enable Auto-Negotiation */
      HAL_ETH_WritePHYRegister(&heth, PHY_BCR, PHY_AUTONEGOTIATION);
      
      /* Get tick */
      tickstart = HAL_GetTick();
      
      /* Wait until the auto-negotiation will be completed */
      do
      {
        HAL_ETH_ReadPHYRegister(&heth, PHY_BSR, &regvalue);
        
        /* Check for the Timeout ( 1s ) */
        if((HAL_GetTick() - tickstart ) > 1000)
        {     
          /* In case of timeout */ 
          goto error;
        }   
      } while (((regvalue & PHY_AUTONEGO_COMPLETE) != PHY_AUTONEGO_COMPLETE));
      
      /* Read the result of the auto-negotiation */
      HAL_ETH_ReadPHYRegister(&heth, PHY_SR, &regvalue);
      

      if((regvalue & PHY_DUPLEX_STATUS) != (uint32_t)RESET)
      {

        heth.Init.DuplexMode = ETH_MODE_FULLDUPLEX;  
      }
      else
      {

        heth.Init.DuplexMode = ETH_MODE_HALFDUPLEX;           
      }

      if(regvalue & PHY_SPEED_STATUS)
      {  
        /* Set Ethernet speed to 10M following the auto-negotiation */
        heth.Init.Speed = ETH_SPEED_10M; 
      }
      else
      {   
        /* Set Ethernet speed to 100M following the auto-negotiation */ 
        heth.Init.Speed = ETH_SPEED_100M;
      }
    }
    else /* AutoNegotiation Disable */
    {
    error :
      /* Check parameters */
      assert_param(IS_ETH_SPEED(heth.Init.Speed));
      assert_param(IS_ETH_DUPLEX_MODE(heth.Init.DuplexMode));
      
      /* Set MAC Speed and Duplex Mode to PHY */
      HAL_ETH_WritePHYRegister(&heth, PHY_BCR, 
      ((uint16_t)(heth.Init.DuplexMode >> 3) |
       (uint16_t)(heth.Init.Speed >> 1))); 
    }

    /* ETHERNET MAC Re-Configuration */
    HAL_ETH_ConfigMAC(&heth, (ETH_MACInitTypeDef *) NULL);

    /* Restart MAC interface */
    HAL_ETH_Start(&heth);   
  }
  else
  {
    /* Stop MAC interface */
    HAL_ETH_Stop(&heth);
  }

  ethernetif_notify_conn_changed(netif);
}
#endif


