/*********************************************************
 AEROHIVE CONFIDENTIAL

 Copyright [2006] - [2018] Aerohive Networks, Inc.
 All Rights Reserved.

 NOTICE: All information herein is and remains the property
 of Aerohive Networks, Inc. and its suppliers, if any.

 The intellectual and technical concepts contained herein
 are proprietary to Aerohive Networks, Inc. and its
 suppliers and may be covered by U.S. and foreign patents
 and/or pending patent applications, and are protected by
 trade secret and copyright law.

 Disclosure, dissemination or reproduction of this
 information or the intellectual or technical concepts
 expressed by this information is prohibited unless prior
 written permission is obtained from Aerohive Networks, Inc.
 **********************************************************/
/*
 * This file contains all functions related to handling FE datapath
 *
 * Copyright (c) 2006-2008 AeroHive Wireless, Inc.
 *
 * PROPRIETARY RIGHTS of AERO HIVE are involved in the
 * subject matter of this material.  All manufacturing, reproduction,
 * use, and sales rights pertaining to this subject matter are governed
 * by the license agreement.  The recipient of this software implicitly
 * accepts the terms of the license.
 *
 * Creation Date: 03/15/2007
 * Author: Sreekanth Reddy
 *
 */
#include "ah_abstraction_kernel.h"
#include "ah_abstraction_sys.h"
#include "ah_abstraction_net.h"
#include "ah_abstraction_device.h"
#include "ah_abstraction_proto.h"
#include "ah_abstraction_ip.h"
#include "ah_board.h"
#include "ah_ksyscall.h"
#include "ah_ksys_dev.h"
#include "ah_klist.h"
#include "ah_kdbg.h"
#include "ah_types.h"
#include "ah_ssysapi.h"
#include "ah_kafe.h"
#include "ah_zone.h"
#include "ah_policy.h"
#include "ah_tunnel.h"
#include "ah_mac_sess.h"
#include "ah_vector.h"
#include "ah_screen.h"
#include "ah_cwp.h"
#include "ah_arp.h"
#include "ah_arp_priv.h"
#include "ah_qos_inc.h"
#include "ah_qos_policy.h"
#include "ah_qos_main.h"
#include "ah_port.h"
#include "ah_cwp_msg.h"
#include "ah_mac_learn.h"
#include "ah_mac_tbl.h"
#include "ah_smpi.h"
#include "ah_kmpi.h"
#include "ah_route_msg.h"
#include "ah_ff.h"
#include "ah_kdhcp.h"
#include "ah_station.h"
#include "ah_demux.h"
#include "ah_counter.h"
#include "ah_ip_sess.h"
#include "ah_firewall.h"
#include "ah_mac_firewall.h"
#include "ah_sess_sync.h"
#include "ah_route_lookup.h"
#include "ah_parse.h"
#include "ah_flow_ipc.h"
#include "ah_self.h"
#include "ah_ip_gate.h"
#include "ah_frag.h"
#include "ah_tcp.h"
#include "ah_icmp.h"
#include "ah_tftp.h"
#include "ah_cac.h"
#include "ah_tb.h"
#include "ah_pkt_queue.h"
#include "ah_klog.h"
#include "ah_nat.h"
#include "ah_dev_mac.h"
#include "ah_kevent.h"
#include "ah_self.h"
#include "ah_http.h"
#include "ah_app_flow.h"
#include "awe_realtime.h"
#include "ah_l7.h"
#include "ah_sw_dev.h"
#include "ah_ndisc_priv.h"
#include "qos/ah_qos_cli.h"
#include "ah_red_if.h"
#include "ah_sfl.h"
#include "ah_shim_mutex.h"
#include "ah_shim_skb.h"
#include "ah_shim_tasklet.h"
#include "ah_shim_assert.h"
#include "ah_shim_locks.h"
#include "ah_ksys_dev.h"

#define IS_AH_FLOW_IN_ROUTE_LIST(f) (is_ah_flow_in_proxy_list(f))
#define AH_FLOW_NEED_LINK_ROUTE(f)  (!is_ah_flow_dst_broadcast(f))
#define ah_route_unlink_mac_sess    ah_fdb_unlink_mac_sess

static int ah_mac_learn_enable = 0;
uint32_t fe_seconds_tic = 0;
int ah_agg_hash_mode = 0;
static int amrp_has_wifi_neighbor = 0;

AH_DEFINE_PER_CPU(ah_mac_sess_t, gsess);
AH_DEFINE_PER_CPU(ah_mac_flow_t *, gflow);
AH_DEFINE_PER_CPU(ah_mac_flow_t , tmp_flow);

/* linked list to store all MAC sessions */
ah_spinlock_t                 ah_mac_sess_list_lock;
ah_spinlock_t                 ah_mac_sess_invalid_list_lock;
static ah_spinlock_t                 ah_mac_sess_create_lock;
ah_rwlock_t                   ah_fe_global_lock;

struct ah_list_head           ah_mac_sess_list;
static struct ah_hlist_head          ah_mac_sess_invalid_list;

struct ah_timer_list          flow_timer;
ah_mac_sess_timer_table_t *ah_mac_sess_timer_table = NULL;
ah_flow_table_t           *flow_table = NULL;

ah_abs_atomic_t                   g_ah_mac_sess_cnt = ATOMIC_INIT (0);
static ah_abs_atomic_t                   ah_flow_log_msg_cnt = ATOMIC_INIT (0);
ah_abs_atomic_t                   g_max_mac_sess_per_station = ATOMIC_INIT (0);
ah_abs_atomic_t                   g_max_ip_sess_per_station  = ATOMIC_INIT (0);

boolean                    ah_flow_system_ready = TRUE /* FALSE */;
boolean                    ah_ip_stateful_fw_enable = FALSE;
boolean                    ah_alg_enabled = FALSE;
boolean                    ah_proxy_arp_enabled = TRUE;
boolean                    ah_proxy_arp_bypass_req_from_sta_to_external = FALSE;
boolean                    ah_proxy_nd_enabled = TRUE;   //unlike proxy_arp, we make proxy_nd FALSE by default.
boolean                    ah_proxy_nd_bypass_req_from_sta_to_external = FALSE;
boolean                    ah_enhanced_proxy_enabled = FALSE; //default for enhanced proxy-arp-ndp is disabled
boolean                    ah_inter_ssid_flood_enabled = TRUE;
boolean                    ah_flow_initialized = FALSE;
boolean                    ah_drop_non_mgt_traffic_enable = FALSE;

boolean                    ah_fe_disabled = FALSE;
boolean                    ah_fe_sess_sync_vlan = FALSE;
int                        ah_unicast_flow_refresh_time = AH_UNICAST_FLOW_DEF_REFRESH_TIME * HZ;
int                        ah_broadcast_flow_refresh_time = AH_BROADCAST_FLOW_DEF_REFRESH_TIME * HZ;
int                        ah_mcast_prune_threshold = AH_MCAST_PRUNE_THRESHOLD;
#if defined(AH_SUPPORT_FE_VCON)
int                        ah_vwire_ifp1 = 0, ah_vwire_ifp2 = 0;
#endif
static int                        kflow_mh = -1;

static unsigned long     ah_mac_sess_id_bitmap[AH_MAC_SESS_ID_BITMAP_SIZE] __attribute__ ((aligned (64)));
unsigned long     ah_ip_sess_create_status = 0;

#if defined(AH_SUPPORT_FE_VCON)
static uchar                      ah_vwire_nhop[ETH_ALEN];
#endif

boolean                     ah_dhcp_spoof_shield_enabled = TRUE;
boolean                     ah_arp_spoof_shield_enabled = FALSE;
boolean                           ah_bonjour_gateway_enabled = FALSE; // bonjour gateway service enabled

#if AH_SUPPORT_DHCP_OPT_82
boolean						ah_dhcp_opt82_enabled = FALSE;
#endif

static int ah_dev_flow_device_event (struct ah_notifier_block *unused, unsigned long event, void *ptr);
struct ah_notifier_block ah_dev_flow_device_notifier = {
	.notifier_call = ah_dev_flow_device_event
};
static int ah_flow_device_event (struct ah_notifier_block *unused, unsigned long event, void *ptr);

struct ah_notifier_block ah_flow_device_notifier = {
	.notifier_call = ah_flow_device_event
};

static ah_flow_vector_t flow_fast_vector_list[] = {
	ah_sanity_vector,
	ah_find_flow_vector,
	NULL
};

static ah_flow_vector_t first_path_vector_list[] = {
	ah_first_path_vector,
	NULL
};

static ah_flow_vector_t no_flow_fast_path_vector_list[] = {
	ah_ipv6_misc_vector,
	ah_arp_vector,
	ah_qos_cp_vector,
	ah_main_vector,
	NULL,
};


static void ah_invalidate_sess_by_dev (struct ah_abs_net_device *dev);

static inline void ah_get_mac_sess_id_bit_params (int mac_sess_id, int *word, int *bitpos)
{
	*word = mac_sess_id / AH_BITS_IN_INTEGER;
	*bitpos = mac_sess_id & AH_BITS_IN_INTEGER_MASK;
}

static inline void ah_clr_mac_sess_id_bit (int mac_sess_id)
{
	int word;
	int bitpos;

	ah_kassert(mac_sess_id <= AH_MAX_MAC_SESS_CNT);
	ah_get_mac_sess_id_bit_params (mac_sess_id, &word, &bitpos);
	ah_clear_bit (bitpos, (void *)&ah_mac_sess_id_bitmap[word]);
}

static inline boolean ah_pkt_is_ip_flow_needed(struct ah_sk_buff *skb)
{
	ah_mac_flow_t *mf = (skb ? skb2macflow(skb) : NULL);
	ah_mac_flow_t *mr = (mf ? macflow2peer(mf) : NULL);


#if AH_SUPPORT_USEG

	if (ah_useg_enable) {

		return TRUE;	/* always need ip-sess */
		}

#endif	/* #if AH_SUPPORT_USEG */


	return ((mf && mf->out_dev && (is_access_dev(mf->out_dev) ||
								   is_backhaul_app_enabled(mf->out_dev) || mf->cavc_port_index != 0)) ||
			(mr && mr->out_dev && (is_access_dev(mr->out_dev) ||
								   is_backhaul_app_enabled(mr->out_dev) || mr->cavc_port_index != 0)) ||
			(skb && is_skb_decap_pak(skb) && /* treat out of inxp backhaul tunnel as from access */
			 (ah_get_zone_by_tunnel_id(get_skb_tunnel_id(skb)) == AH_ZONE_BACKHAUL) &&
			 (mr && mr->policy && mr->policy->is_inxp))); // only unicast packet will have mr->policy
			 /*NOTE: for CAVC to work, please add is_cavc flag here */

}

void ah_dump_skb (struct ah_sk_buff *skb)
{
	int pkt_dump_sz = 0;

	/* dump skb fields and packet data */
	*_ah_kernel_trace_dump = 1;

	/*
	 * dump skb fields: len, data_len, head, data, tail, end,
	 * truesize, fraglist, nr_frags
	 */
	ah_abs_printk(KERN_ERR "skb:%p len:%d data_len:%d head:%p data:%p "
		   "tail:%p end:%p flags:0x%x dev:%s "
		   "truesize:%d fraglist:%p nr_frags:%d\n",
		   skb, skb->len, skb->data_len,
		   skb->head, skb->data, skb->tail, skb->end, skb2ahb(skb)->ab_flag,
		   skb->dev ? skb->dev->name : "<NULL>", skb->truesize,
		   skb_shinfo(skb)->frag_list, skb_shinfo(skb)->nr_frags);

	/* dump struct ah_sk_buff */
	ah_abs_printk(KERN_ERR "dumping skb struct(%p):\n", skb);
	ah_hexdumpk((unsigned char *) skb, sizeof(struct ah_sk_buff));

	/* dump skb_shinfo */
	ah_abs_printk(KERN_ERR "dumping skb_shinfo(%p):\n", skb_shinfo(skb));
	ah_hexdumpk((unsigned char *) (skb_shinfo(skb)), sizeof(struct ah_skb_shared_info));

	/* dump skb packet data */
	if (skb->tail > skb->data) {
		pkt_dump_sz =  skb_tail_pointer(skb) - skb->data;
	}

	if (pkt_dump_sz > 512) {
		pkt_dump_sz = 512;
	}

	ah_abs_printk(KERN_ERR "dumping %d bytes of skb packet data (%p):\n",
		   pkt_dump_sz, skb->data);
	ah_hexdumpk((unsigned char *) skb->data, pkt_dump_sz);

	if (skb_is_nonlinear(skb) && ah_skb_linearize(skb) == 0) {
		pkt_dump_sz = skb->len;
		if (pkt_dump_sz > 512) {
			pkt_dump_sz = 512;
		}
		ah_abs_printk(KERN_ERR "dumping %d bytes of linearized skb packet data (%p):\n",
			   pkt_dump_sz, skb->data);
		ah_hexdumpk((unsigned char *) skb->data, pkt_dump_sz);
	}

	*_ah_kernel_trace_dump = 0;

}

#if defined(AH_SUPPORT_FE_VCON)
void ah_set_vwire_interface (int ifp1_index, int ifp2_index, uchar *nhop)
{
	ah_vwire_ifp1 = ifp1_index;
	ah_vwire_ifp2 = ifp2_index;
	memcpy (ah_vwire_nhop, nhop, ETH_ALEN);
}

static inline int ah_get_vwire_interface_peer (int ifp_index)
{
	if (likely (ah_vwire_ifp1 && ah_vwire_ifp2)) {

		if (ifp_index == ah_vwire_ifp1) {
			return ah_vwire_ifp2;
		}

		if (ifp_index == ah_vwire_ifp2) {
			return ah_vwire_ifp1;
		}
	}

	return -1;
}
#endif

static int ah_get_mac_sess_avail_id (void)
{
	int word, i;
	volatile int bit;

	for (word = 0; word < AH_MAC_SESS_ID_BITMAP_SIZE; word++) {

		if (ah_mac_sess_id_bitmap[word] == 0xffffffff) {
			continue;
		}

		for (i = 0; i < 32; i++) {
			bit = ffz(ah_mac_sess_id_bitmap[word]);

			/*
			 * sess_id_bitmap[word] is 0xffffffff, race condition: multi cores both enter here
			 * For GM arm platfrom, return value < zero
			 */
			if (bit > 31 || bit < 0) {

//              ah_logk (AH_SYS_KMOD_FE, AH_LOG_ERR, "invalid bitpos %d\n", bit);
//              ah_kassert (bit <= 31);
				break;
			}

			if (test_and_set_bit (bit, &ah_mac_sess_id_bitmap[word]) == 0) {
				return word * AH_BITS_IN_INTEGER + bit;
			}
		}

	}

	return -1;
}

int ah_flow_device_counters (struct ah_abs_net_device *dev)
{
	ah_port_t *p;
	struct ah_abs_net_device *mgt_dev;
	ah_mgt_if_t *mif;
	int is_ethernet_ifp = 0;

	if (dev == NULL || !dev2ahdev(dev)) {
		return 0;
	}

	if (!(mgt_dev = dev2mgt_dev (dev)) || !(mif = dev2ahdev(mgt_dev)->mif))  {
		return 0;
	}

	ah_abs_atomic_set(&mif->ami_if_cnt[AH_DEV_TYPE_ACCESS_IDX], 0);
	/*
	 * only for Ethernet backhaul counter
	 */
	if (!is_wireless_dev(dev)) {
		is_ethernet_ifp = 1;
		ah_abs_atomic_set(&mif->ami_if_cnt[AH_DEV_TYPE_PORTAL_IDX], 0);
	}

	ah_read_lock_bh (&mif->ami_listlock);

	list_for_each_entry (p, &mif->ami_portlist, ahp_list) {

		if (!dev2ahdev(p->ahp_dev)) {
			continue;
		}

		if (!(ah_wrapper_dev_get_flags (p->ahp_dev) & IFF_RUNNING) || !netif_carrier_ok(p->ahp_dev)) {
			continue;
		}

		if (AH_SYS_DEV_IS_MEMBER_INTERFACE (dev2ahdev(p->ahp_dev))) {
			continue;
		}

		if (is_access_dev (p->ahp_dev)) {
			ah_abs_atomic_inc(&mif->ami_if_cnt[AH_DEV_TYPE_ACCESS_IDX]);
			afe_detail_dbgk (NULL,
							 "interface %s is UP, increase access ifp count to %d\n",
							 dev2name (p->ahp_dev), ah_abs_atomic_read(&mif->ami_if_cnt[AH_DEV_TYPE_ACCESS_IDX]));

		} else if (is_ethernet_ifp &&
				   !is_wireless_dev (p->ahp_dev) && is_backhl_dev(p->ahp_dev)) {
			ah_abs_atomic_inc(&mif->ami_if_cnt[AH_DEV_TYPE_PORTAL_IDX]);
			afe_detail_dbgk (NULL,
							 "interface %s is UP, increase portal ifp count to %d\n",
							 dev2name (p->ahp_dev), ah_abs_atomic_read(&mif->ami_if_cnt[AH_DEV_TYPE_PORTAL_IDX]));
		}
	}

	ah_logk(AH_SYS_KMOD_FE, AH_LOG_INFO,
			"access interface counter %d, portal interface counter %d\n",
			ah_abs_atomic_read(&mif->ami_if_cnt[AH_DEV_TYPE_ACCESS_IDX]),
			ah_abs_atomic_read(&mif->ami_if_cnt[AH_DEV_TYPE_PORTAL_IDX]));

	ah_read_unlock_bh (&mif->ami_listlock);

	return 0;
}

/*
 * This function will be called when network device events occur
 * We need to record management IPs and keep them in our hash table
 *
 * event          action
 * -------        ------------------
 * down to up     add the entry
 * up to down     mark the device status to down
 * unregister     delete the entry
 *
 */
static int ah_flow_device_event (struct ah_notifier_block *unused,
								 unsigned long event, void *ptr)
{
    struct ah_abs_net_device *dev = netdev_notifier_info_to_dev(ptr);

	/*
	 * only handle certain dev with ah_dev attached
	 */
	if (!dev || !dev2ahdev(dev) ||
		!(is_access_dev(dev) ||
		  AH_SYS_DEV_IS_PORTAL(dev2ahdev(dev)) ||
		  AH_SYS_DEV_IS_BACKHAUL(dev2ahdev(dev)) ||
		  AH_SYS_DEV_IS_VLAN_INTERFACE(dev2ahdev(dev)))) {
		afe_detail_dbgk (NULL, "ignore event 0x%x from %s\n", event, dev2name(dev));
		return NOTIFY_DONE;
	}

	switch (event) {

	case NETDEV_DOWN:
		/*
		 * vlan interface for switch
		 */
		if (AH_SYS_DEV_IS_VLAN_INTERFACE(dev2ahdev(dev)) && dev2mgt_dev(dev)) {

			ah_write_lock_bh(&ah_fe_global_lock);
			ah_vlan_dev_clear_mac(dev2ahdev(dev), dev2mgt_dev(dev)->dev_addr);
			ah_write_unlock_bh(&ah_fe_global_lock);
			break;
		}
	/* no break */

	case NETDEV_UP:
	case NETDEV_CHANGE:
		afe_detail_dbgk (NULL, "dev %s state is %s\n", dev2name(dev),
						 (event == NETDEV_UP) ? "UP" : (event == NETDEV_DOWN) ? "DOWN" : "CHANGE");
		ah_flow_device_counters(dev);
		break;

	default:
		afe_detail_dbgk (NULL,  "ignore event 0x%x, interface %s\n",
						 event, dev2name(dev));
		break;
	}

	return NOTIFY_DONE;
}

static int ah_dev_flow_device_event (struct ah_notifier_block *unused, unsigned long event, void *ptr)
{
	ah_sys_dev_t *ah_dev = (ah_sys_dev_t *)ptr;
	ah_kevent_ah_dev_change_t evt;

	if (ah_dev == NULL) {
		return NOTIFY_DONE;
	}

	switch (event) {

	case AH_DEV_EVENT_INIT:

		/*
		 * create dev mac list
		 */
		ah_dev_mac_init (ah_dev);

#if AH_SUPPORT_USEG
		ah_useg_init_sys_dev(ah_dev);
#endif

		break;

	case AH_DEV_EVENT_DELETE:

		/*
		 * delete device mac list
		 */
		ah_dev_mac_free (ah_dev);

#if AH_SUPPORT_USEG
		ah_useg_deinit_sys_dev(ah_dev);
#endif

		/*
		* delete walled garden entries
		*/
		ah_clear_all_walled_garden_entries (ah_dev);

		/*
		 * interface is getting deleted, invalidate all
		 * the sessions on this interface
		 */
		afe_detail_dbgk (NULL, "dev %s is deleted\n", dev2name (ah_dev->dev));
		ah_write_lock_bh (&ah_fe_global_lock);
		ah_invalidate_sess_by_dev (ah_dev->dev);
		ah_write_unlock_bh (&ah_fe_global_lock);
		break;

	case AH_DEV_EVENT_CHG_FLAG:

		/*
		 * Ethernet interface mode chg by CLI
		 */
		ah_flow_device_counters(ahdev2dev(ah_dev));

		/*
		 * post a kevent notifying any interested apps about interface mode change
		 */
		evt.if_index = ahdev2dev(ah_dev)->ifindex;
		evt.dev_type = (AH_SYS_DEV_IS_ETH_ACCESS (ah_dev) ||
						AH_SYS_DEV_IS_ETH_BRIDGE (ah_dev))
					   ? 0 : AH_DEV_TYPE_BACKHAUL ;
		ah_kevent_send (AH_KEVENT_AH_DEV_TYPE_CHANAGE, &evt, sizeof(evt));

	default:
		break;

	}
	return NOTIFY_DONE;
}


static inline void ah_add_flow_to_hash (ah_mac_flow_t *f)
{
	/* add to hash list */
	hlist_add_head_rcu (&f->hash_list, &flow_table->flow_hash[f->hash]);

	/* increment the number of flows in this hash */
	ah_abs_atomic_inc (&flow_table->flow_hash_cnt[f->hash]);
}

static inline void ah_del_flow_from_hash (ah_mac_flow_t *f)
{

	hlist_del_rcu (&f->hash_list);

	/* decrement the number of flows in this hash */
	ah_abs_atomic_dec (&flow_table->flow_hash_cnt[f->hash]);
}

#ifdef AH_DEBUG_MAC_SESS_LIST
void dump_mac_sess(ah_mac_sess_t *s);
#endif

/*
 * This function can be called when the following locks are held
 *
 * 1. ah_mac_session_create_lock
 */
static inline void ah_add_mac_sess_to_list (ah_mac_sess_t *s)
{
	ah_abs_spin_lock_bh (&ah_mac_sess_list_lock);
#ifdef AH_DEBUG_MAC_SESS_LIST
	if ((((int) &s->mac_sess_list) & 0x3) ||
		(((int) ah_mac_sess_list.prev) & 0x3) ||
		(((int) &s->mac_sess_list) < 0x80000000)) {
		dump_mac_sess(s);
		mac_sess_panic("pre add mac sess to list check failed\n");
	}
#endif
	list_add_tail_rcu (&s->mac_sess_list, &ah_mac_sess_list);
#ifdef AH_DEBUG_MAC_SESS_LIST
	if ((((int) s->mac_sess_list.prev) & 0x3) ||
		(((int) s->mac_sess_list.prev->next) & 0x3) ||
		(((int) s->mac_sess_list.prev->next) < 0x80000000) ||
		(((int) s->mac_sess_list.next) & 0x3) ||
		s->mac_sess_list.next != &ah_mac_sess_list) {
		dump_mac_sess(s);
		dump_mac_sess(list_entry(s->mac_sess_list.prev,  typeof(*s), mac_sess_list));
		mac_sess_panic("post add mac sess to list check failed\n");
	}
#endif
	ah_abs_spin_unlock_bh (&ah_mac_sess_list_lock);
}

/*
 * this function can be called when the following locks are held
 *
 * 1. ah_mac_sess_list_lock
 * 2. ah_mac_sess_timer_table->mac_sess_tick_lock [tic]
 */
static inline void ah_del_mac_sess_from_list (ah_mac_sess_t *s, boolean have_lock)
{
	/* we already have a lock at this point, no need to grab ah_mac_sess_list_lock again! */
#ifdef AH_DEBUG_MAC_SESS_LIST
	if (((int) s->mac_sess_list.prev) & 0x3) {
		dump_mac_sess(s);
		mac_sess_panic("fail del mac_sess from list check for prev pointer\n");
	}
#endif
	if (!have_lock) {
		ah_abs_spin_lock_bh (&ah_mac_sess_list_lock);
	}

	list_del_rcu (&s->mac_sess_list);

	if (!have_lock) {
		ah_abs_spin_unlock_bh (&ah_mac_sess_list_lock);
	}
}

/*
 * this funcion can be called when the following locks are held
 *
 *  1. ah_mac_sess_timer_table->mac_sess_tick_lock [prev-tic] - note that prev-tic != tic:
 *  2. ah_mac_session_create_lock
 *  3. ah_mac_sess_invalid_list_lock
 */
static inline void ah_add_mac_sess_to_ager_ring (ah_mac_sess_t *s, int tic)
{

	if (tic >= AH_MAC_SESS_TIMER_ENTRIES) {
		ah_logk (AH_SYS_KMOD_FE, AH_LOG_ERR,
				 "invalid MAC sess ageout tic [%u], out of range\n", tic);
		ah_kassert(0);
		return;
	}

	ah_abs_spin_lock_bh(&ah_mac_sess_timer_table->mac_sess_tick_lock[tic]);

	hlist_add_head (&s->ageout_list, &ah_mac_sess_timer_table->mac_sess_tick[tic]);
	s->ageout_tic = tic;

	ah_abs_spin_unlock_bh(&ah_mac_sess_timer_table->mac_sess_tick_lock[tic]);
}

/*
 * this funcion can be called when the following locks are held
 *
 * 1. ah_mac_sess_invalid_list_lock
 * 2. h_mac_sess_timer_table->mac_sess_tick_lock [prev_tic]
 */
static inline void ah_del_mac_sess_from_ager_ring (ah_mac_sess_t *s, boolean have_lock)
{
	if (!have_lock) {
		ah_abs_spin_lock_bh(&ah_mac_sess_timer_table->mac_sess_tick_lock[s->ageout_tic]);
	}

	hlist_del (&s->ageout_list);

	if (!have_lock) {
		ah_abs_spin_unlock_bh(&ah_mac_sess_timer_table->mac_sess_tick_lock[s->ageout_tic]);
	}
}

/*
 * this funcion can be called when the following locks are held
 *
 * 1. ah_mac_sess_create_lock
 */
static inline void ah_add_mac_sess_to_ager (ah_mac_sess_t *s)
{
	if (ah_refresh_mac_sess (s) == 0) {
		ah_add_mac_sess_to_ager_ring (s, ah_abs_atomic_read(&s->next_ageout_tic));
	}
}

/*
 * this funcion can be called when the following locks are held
 *
 * 1. ah_mac_sess_invalid_list_lock
 * 2. ah_mac_sess_timer_table->mac_sess_tick_lock []
 */
void ah_add_mac_sess_to_ager_with_timeout (ah_mac_sess_t *s, int timediff)
{
	uint32_t diff_in_tic;

	ah_mac_sess_update_expire_time (s, timediff);

	diff_in_tic = timediff / (AH_MAC_SESS_AGER_TIC_MULTIPLIER * HZ);
	diff_in_tic = diff_in_tic == 0 ? 1 : diff_in_tic;
	ah_abs_atomic_set(&s->next_ageout_tic, (diff_in_tic + ah_mac_sess_timer_table->mac_sess_timer_cur_tick) % AH_MAC_SESS_TIMER_ENTRIES);

	ah_add_mac_sess_to_ager_ring (s, ah_abs_atomic_read(&s->next_ageout_tic));
}

boolean should_deny_ipv4_or_v6(ah_mac_sess_t *s, struct ah_sk_buff *skb)
{
	if (unlikely(ah_is_mac_sess_deny_ipv6(s) &&
				 skb->protocol == __constant_htons(ETH_P_IPV6))) {
		ah_inc_drop_pak_counter (ah_get_skb_input_dev(skb), AH_DROP_DENY_IPV6);
        afe_detail_dbgk(skb, "drop ipv6 packet for mac-session %d", s->id);
		return TRUE;
	} else if (unlikely(ah_is_mac_sess_deny_ipv4(s) &&
						skb->protocol == __constant_htons(ETH_P_IP))) {
		ah_inc_drop_pak_counter (ah_get_skb_input_dev(skb), AH_DROP_DENY_IPV4);
        afe_detail_dbgk(skb, "drop ipv4 packet for mac-session %d", s->id);
		return TRUE;
	}

	return FALSE;
}

static ah_mac_flow_t *ah_create_mac_sess (ah_mac_flow_t *f,
		struct ah_sk_buff *skb, boolean from_sync, int *should_drop)
{
	ah_mac_sess_t *ns, *s = macflow2sess(f);
	ah_mac_flow_t *nf, *r = macflow2peer(f), *nr, *tf;
	int avail_id;
	uint hash;
	static ah_abs_atomic_t ah_mac_sess_exceed_msg_cnt = ATOMIC_INIT(0);
	static ah_abs_atomic_t ah_mac_sess_per_sta_exceed_msg_cnt = ATOMIC_INIT(0);

	if (ah_get_mac_sess_cnt() > AH_MAX_MAC_SESS_CNT) {

		if (ah_abs_atomic_read(&ah_flow_log_msg_cnt) < AH_MAX_LOG_MSG_CNT) {
			ah_logk(AH_SYS_KMOD_FE, AH_LOG_ERR,
					"exceeds maximum number of flows allowed [%u]\n",
					AH_MAX_MAC_SESS_CNT);
			ah_abs_atomic_inc (&ah_flow_log_msg_cnt);
		}

		ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_EXCEED_MAX_MAC_SESSIONS);
		return NULL;

	} else {
		ah_abs_atomic_set(&ah_flow_log_msg_cnt, 0);
	}

	if (ah_create_vector_list_from_index (f->vector_index) < 0) {
		return NULL;
	}

	if (!is_ah_flow_not_installed(r) && ah_create_vector_list_from_index (r->vector_index) < 0) {
		return NULL;
	}

	if (!(ns = ah_kcalloc_atomic (sizeof(ah_mac_sess_t))) ) {
		ah_logk(AH_SYS_KMOD_FE, AH_LOG_ERR, "alloc mem in kernel failed\n");
		ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_EXCEED_MAX_MAC_SESSIONS);
		return NULL;
	}

	if ((avail_id = ah_get_mac_sess_avail_id ()) < 0) {
		ah_logk(AH_SYS_KMOD_FE, AH_LOG_DEBUG, "no more MAC sessions available\n");
		ah_kfree (ns);
		ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_EXCEED_MAX_MAC_SESSIONS);
		return NULL;
	}

	rwlock_init (&ns->mac_sess_rw_lock);

#ifdef AH_DEBUG_MAC_SESS_LIST
	ns->mac_sess_sig1 = AH_MAC_SESS_SIG;
	ns->mac_sess_sig2 = AH_MAC_SESS_SIG;
#endif

	/*
	 * setup the ageout time to be 1 sec to start with
	 * it will be refreshed later.
	 */
	ah_mac_sess_update_expire_time (ns, HZ);

	nf = &ns->f_flow;

	/* copy the session stuff */

	ns->upid                          = s->upid;
	ns->flag                          = s->flag;
	ns->ingress_mac_fw_policy         = s->ingress_mac_fw_policy;
	ns->ingress_mac_fw_policy_rule    = s->ingress_mac_fw_policy_rule;
	ns->egress_mac_fw_policy          = s->egress_mac_fw_policy;
	ns->egress_mac_fw_policy_rule     = s->egress_mac_fw_policy_rule;


	/* fill the forward wing */
	memcpy(nf->src_mac, f->src_mac, ETH_ALEN);
	memcpy(nf->dst_mac, f->dst_mac, ETH_ALEN);
	memcpy(nf->nhop, f->nhop, ETH_ALEN);

	nf->iif              = f->iif;
	nf->zone_id          = f->zone_id;
	nf->upid             = f->upid;
	nf->tunnel_id        = f->tunnel_id;
	nf->policy           = f->policy;
	nf->vector_index     = f->vector_index;
	nf->oif              = f->oif;
	nf->out_dev          = f->out_dev;
	nf->vlan             = f->vlan;
	nf->flag             = f->flag;
	nf->nhop_metric      = f->nhop_metric;
	nf->monitor_duration = f->monitor_duration;
	nf->monitor_pkt_cnt  = f->monitor_pkt_cnt;
	nf->hash             = ah_flow_hash((ah_mac_t *)nf->src_mac, (ah_mac_t *)nf->dst_mac, nf->zone_id);
	if (ah_agg_hash_mode) {
		nf->agg_mem_hash = ah_get_agg_hash_from_mac (f->dst_mac);
	} else {
		nf->agg_mem_hash = ah_get_agg_hash_from_mac (f->src_mac);
	}
	nf->in_acl_grp       = f->in_acl_grp;
	nf->out_acl_grp      = f->out_acl_grp;
	nf->cavc_port_index  = f->cavc_port_index;
	memcpy (nf->mp, f->mp, ETH_ALEN);
	ah_mac_flow_clr_pak (nf);
	ah_mac_flow_clr_byte (nf);
	ah_abs_spin_lock_init (&nf->ip_flow_list_lock);
	INIT_HLIST_HEAD (&nf->ip_flow_head);
	ah_abs_atomic_set(&nf->ip_flow_cnt, 0);

	if (f->dst_mac[0] & 1) {
		set_ah_flow_dst_broadcast (nf);
	}

	/* set forward flow bit for initiating wing */
	set_ah_mac_flow_forward (nf);

	if (IS_AH_FLOW_IN_ROUTE_LIST(f) && AH_FLOW_NEED_LINK_ROUTE(f)) {
		/* replace the old flow with new flow in the route list */
		ah_fib_replace_route_list (nf, f);
		ah_mac_flow_clear_route_fields (f);
	}

	nr = &ns->r_flow;

	/*
	 * copy mac address even though this flow may not be
	 * installed, it may be used for route lookup later.
	 */
	memcpy(nr->src_mac, r->src_mac, ETH_ALEN);
	memcpy(nr->dst_mac, r->dst_mac, ETH_ALEN);

	/*
	 * even if the reverse wing is not valid, we keep upid/iif/zone
	 * up to date in it since the reverse wing holds the route for the src mac.
	 */
	nr->upid             = r->upid;
	nr->iif              = r->iif;
	nr->zone_id          = r->zone_id;
	nr->oif              = r->oif;
	nr->out_dev          = r->out_dev;
	nr->vlan             = r->vlan;
	nr->flag             = r->flag;
	nr->nhop_metric      = r->nhop_metric;
	nr->monitor_duration = r->monitor_duration;
	nr->monitor_pkt_cnt  = r->monitor_pkt_cnt;
	memcpy (nr->mp, r->mp, ETH_ALEN);
	ah_mac_flow_clr_pak (nr);
	ah_mac_flow_clr_byte (nr);
	ah_abs_spin_lock_init (&nr->ip_flow_list_lock);
	INIT_HLIST_HEAD (&nr->ip_flow_head);
	ah_abs_atomic_set(&nr->ip_flow_cnt, 0);
	nr->in_acl_grp       = r->in_acl_grp;
	nr->out_acl_grp      = r->out_acl_grp;
	nr->cavc_port_index  = r->cavc_port_index;

	memcpy(nr->nhop, r->nhop, ETH_ALEN);

	/* fill the reverse wing if it's not broadcast session */
	if (!is_ah_flow_not_installed (r)) {

		nr->tunnel_id    = r->tunnel_id;
		nr->policy       = r->policy;
		nr->vector_index = r->vector_index;
		nr->hash         = ah_flow_hash((ah_mac_t *)nr->src_mac, (ah_mac_t *)nr->dst_mac, nr->zone_id);
		if (ah_agg_hash_mode) {
			nr->agg_mem_hash = ah_get_agg_hash_from_mac (nr->dst_mac);
		} else {
			nr->agg_mem_hash = ah_get_agg_hash_from_mac (nr->src_mac);
		}
	}

	/*
	 * we need to store the route even if the reverse wing is not valid,
	 * because it holds the route for the src mac.
	 */
	if (IS_AH_FLOW_IN_ROUTE_LIST (r)) {
		/* replace the old flow with new flow in the route list */
		ah_fib_replace_route_list (nr, r);
		ah_mac_flow_clear_route_fields (r);
	}

	if (nr->out_dev && is_access_dev (nr->out_dev)) {

		/*
		 * check for packet that results in
		 * nf->station same as nr->station
		 */
		if ((ah_compare_ether_addr(f->src_mac, f->dst_mac) == 0) &&
			!is_ah_flow_to_self(nr) &&
			AH_FLOW_NEED_LINK_ROUTE(f) && nf->out_dev && is_access_dev(nf->out_dev) &&
			!is_ah_flow_to_self (nf)) {

			/* log message */
			ah_logk(AH_SYS_KMOD_FE, AH_LOG_INFO,
					"drop bad skb with same src/dst mac (%m) dev(%s) protocol(%04x) len(%d) ab_flag(%x)\n",
					f->src_mac, skb ? dev2name(skb->dev) : "", skb ? skb->protocol : 0,
					skb ? skb->len : 0, skb ? skb2ahb(skb)->ab_flag : 0);
			afe_detail_dbgk (skb,
							 "drop bad skb with same src/dst mac (%m->%m) dev(%s) protocol(%04x)\n",
							 f->src_mac, f->dst_mac, skb ? dev2name(skb->dev) : "", skb ? skb->protocol : 0);

			/* drop packet */
			ah_route_unlink_mac_sess (nf, FALSE);
			ah_route_unlink_mac_sess (nr, FALSE);
			ah_clr_mac_sess_id_bit (avail_id);
			ah_kfree (ns);
			return NULL;
		}

		hash = ah_sta_mac_hash(f->src_mac);
		ah_write_lock_bh(&sta_list->sta_hash_lock[hash]);

		if (!is_ah_flow_to_self (nr)) {
			nf->station = ah_create_and_get_station (f->src_mac);
		}

		if (nf->station) {

			/*
			 * always allow from sync sessions, most likely they never exceed this limit because
			 * the original AP would have done this check anyways. just in case..
			 */
			if (!from_sync && ah_abs_atomic_read (&g_max_mac_sess_per_station)) {

				if (ah_abs_atomic_read (&nf->station->cur_mac_sess_cnt) >= ah_abs_atomic_read (&g_max_mac_sess_per_station)) {

					if (ah_abs_atomic_read (&ah_mac_sess_per_sta_exceed_msg_cnt ) < AH_MAX_MAC_SESS_EXCEED_MSG_CNT) {
						ah_logk(AH_SYS_KMOD_FE, AH_LOG_INFO, "no more MAC sessions per station (%m) available\n", f->src_mac);
						ah_abs_atomic_inc (&ah_mac_sess_exceed_msg_cnt);
					} else {
						afe_detail_or_drop_dbgk(skb, "no more MAC sessions per station (%m) available\n", f->src_mac);
					}

					ah_route_unlink_mac_sess (nf, FALSE);
					ah_route_unlink_mac_sess (nr, FALSE);
					ah_clr_mac_sess_id_bit (avail_id);
					ah_kfree (ns);
					ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_EXCEED_MAX_MAC_SESSIONS_PER_STATION);
					ah_write_unlock_bh(&sta_list->sta_hash_lock[hash]);
					return NULL;

				} else {
					ah_abs_atomic_set (&ah_mac_sess_per_sta_exceed_msg_cnt , 0);
				}

			}

			ah_start_monitor_station (nf->station, nr->monitor_duration, nr->monitor_pkt_cnt, FALSE);

		}

		ah_write_unlock_bh(&sta_list->sta_hash_lock[hash]);
	}

	if (AH_FLOW_NEED_LINK_ROUTE(f) && nf->out_dev && is_access_dev (nf->out_dev)) {

		hash = ah_sta_mac_hash(f->dst_mac);
		ah_write_lock_bh(&sta_list->sta_hash_lock[hash]);

		if (!is_ah_flow_to_self (nf)) {
			nr->station = ah_create_and_get_station (f->dst_mac);
		}

		if (nr->station) {

			/*
			 * always allow from sync sessions, most likely they never exceed this limit because
			 * the original AP would have done this check anyways. just in case..
			 */
			if (!from_sync && ah_abs_atomic_read (&g_max_mac_sess_per_station)) {

				if (ah_abs_atomic_read (&nr->station->cur_mac_sess_cnt) >= ah_abs_atomic_read (&g_max_mac_sess_per_station)) {

					if (ah_abs_atomic_read (&ah_mac_sess_per_sta_exceed_msg_cnt ) < AH_MAX_MAC_SESS_EXCEED_MSG_CNT) {
						ah_logk(AH_SYS_KMOD_FE, AH_LOG_INFO, "no more MAC sessions per station (%m) available\n", f->dst_mac);
						ah_abs_atomic_inc (&ah_mac_sess_exceed_msg_cnt);
					} else {
						afe_detail_or_drop_dbgk(skb, "no more MAC sessions per station (%m) available\n", f->dst_mac);
					}

					ah_route_unlink_mac_sess (nf, FALSE);
					ah_route_unlink_mac_sess (nr, FALSE);
					ah_clr_mac_sess_id_bit (avail_id);
					ah_kfree (ns);
					ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_EXCEED_MAX_MAC_SESSIONS_PER_STATION);
					ah_write_unlock_bh(&sta_list->sta_hash_lock[hash]);
					return NULL;

				} else {
					ah_abs_atomic_set (&ah_mac_sess_per_sta_exceed_msg_cnt , 0);
				}

			}

			ah_start_monitor_station (nr->station, nf->monitor_duration, nf->monitor_pkt_cnt, FALSE);

		}

		ah_write_unlock_bh(&sta_list->sta_hash_lock[hash]);
	}

	/*
	 * clear up old session flags, note that here old session is gsess and we don't want any
	 * pending flags to be stuck in that session. Right now clear only to-self flag, otherwise
	 * in case the pkt processing is aborted, because this flag is still set, we will try to
	 * validate to-self pkt which may fail.
	 *
	 * TODO: clean this up, clear all flags after cleaning up fib/proxy flags in flow from 1->0
	 */
	clr_ah_flow_to_self (f);
	clr_ah_flow_to_self (r);

	ah_set_mac_sess_alloced (ns);

	ns->id = avail_id;

	if (from_sync) {
		ah_set_mac_sess_from_sync (ns);
	}


	/*
	 * at the time session creation, it's possible that the other
	 * core is creating the same session from the opposite side.
	 * so let's check if the session exists again.
	 */
	ah_abs_spin_lock_bh (&ah_mac_sess_create_lock);

	if ((tf = ah_find_flow_by_tuple (nf, (ah_mac_t *)nf->src_mac, (ah_mac_t *)nf->dst_mac, nf->zone_id))) {

		ah_route_unlink_mac_sess (nf, FALSE);
		ah_route_unlink_mac_sess (nr, FALSE);

		ah_clr_mac_sess_id_bit (avail_id);

		ah_kfree (ns);
		ah_abs_spin_unlock_bh (&ah_mac_sess_create_lock);

		if (skb) {
			ah_mac_flow_inc_pak (tf);
			ah_mac_flow_add_byte (tf, skb->len);
		}

		afe_detail_dbgk (skb, "mac session [id %u] already exists, created by other core\n", macflow2sess(tf)->id);
		return tf;

	}

	ah_add_mac_sess_to_ager (ns);


	/* add sess to the linked list */
	ah_add_mac_sess_to_list (ns);

	if (!is_ah_flow_not_installed (nr)) {
		ah_abs_spin_lock_bh(&flow_table->flow_hash_lock[nr->hash]);
		ah_add_flow_to_hash (nr);
		ah_abs_spin_unlock_bh(&flow_table->flow_hash_lock[nr->hash]);
	}

	ah_abs_spin_lock_bh(&flow_table->flow_hash_lock[nf->hash]);
	ah_add_flow_to_hash (nf);
	ah_abs_spin_unlock_bh(&flow_table->flow_hash_lock[nf->hash]);

	ah_abs_spin_unlock_bh (&ah_mac_sess_create_lock);

	ns->creation_time = ah_jiffies;

	/* if logging on the FW policy is allowed, use it */
	if (unlikely (ah_is_mac_sess_create_log_on (ns)))
		afe_logk (AH_LOG_INFO,
				  "MAC session (id %u) %m -> %m is created, user-name %s, hostname %s -> user-name %s, hostname %s\n",
				  ns->id, nf->src_mac, nf->dst_mac, (nf->station && nf->station->username[0]) ? nf->station->username : "N/A",
				  (nf->station && nf->station->hostname[0]) ? nf->station->hostname : "N/A",
				  (nr->station && nr->station->username[0]) ? nr->station->username : "N/A",
				  (nr->station && nr->station->hostname[0]) ? nr->station->hostname : "N/A");

	afe_detail_dbgk (skb, "mac sess id [%u] created, ageout time %u sec\n",
					 ns->id, ((int)(ah_get_mac_sess_age_out_time (ns))) / HZ);

	afe_detail_dbgk (skb, " mac session(id %u) %m->%m, flags 0x%x/0x%x, nf->acl 0x%x/0x%x nr->acl 0x%x/0x%x "
					 "nf->upid=%d nf->vid=%d nr->upid=%d nr->vid=%d\n",
					 ns->id, nf->src_mac, nf->dst_mac, nf->flag, nr->flag, nf->in_acl_grp, nf->out_acl_grp,
					 nr->in_acl_grp, nr->out_acl_grp, nf->upid, nf->vlan, nr->upid, nr->vlan);
	if (unlikely ((is_ah_flow_to_self (nf) || is_ah_flow_to_self (nr)) && ah_to_self_log_enabled)) {
        afe_logk (AH_LOG_INFO, "%m->%m, %s\n", nf->src_mac, nf->dst_mac, AH_MAC_SESS_TO_SELF_LOG_MSG);
	}

	ah_inc_mac_sess_cnt ();

	if (skb && should_deny_ipv4_or_v6(ns, skb)) {
		*should_drop = 1;
	} else {
		*should_drop = 0;

		ah_mac_flow_inc_pak(nf);

		if (skb) {
			ah_mac_flow_add_byte (nf, skb->len);
		}
	}
	if (nf->station) {
		ah_sta_inc_mac_sess_cnt (nf->station);
	}

	if (nr->station) {
		ah_sta_inc_mac_sess_cnt (nr->station);
	}

	ah_ff_match_for_mac_sess (ns);

	return nf;
}

/*
 * this funcion can be called when the following locks are held
 *
 * 1. mac_sess->mac_sess_rw_lock
 * 2. ah_mac_sess_timer_table->mac_sess_tick_lock [tic]
      (not relevant, because in this case session won't be added to session invalid list)
 * 3. ah_fe_global_lock
 * 4. ah_mac_acl_grp_list->list_lock
 * 5. ah_mac_acl_grp_t->cfg_list_lock
 * 6. policy_list->hlist_lock[i]
 *
 * Parameters
 * s      - MAC sesson that's to be invalidated
 * code   - invalidation code
 * ageout - invalidation period in millisecnds
 *
 */
void ah_invalidate_mac_sess (ah_mac_sess_t *s, int code, int ageout)
{
	ah_mac_flow_t *f = &s->f_flow;

	if (ah_abs_atomic_test_and_set_mac_sess_invalid (s)) {
		return;
	}

	afe_mac_sess_dbgk (s, "invalidate MAC sess (%u) for (%u) seconds, reason (%s)\n",
					   s->id, ageout / HZ + 5, ah_get_sess_invalidate_code_str(code));

	s->ageout_code = code;
	s->invalidate_period = ageout;

	ah_set_mac_sess_no_refresh (s);

	ah_invalidate_all_ip_sessions_of_mac_sess (s, AH_MAC_SESS_AGEOUT, ageout);

	/*
	 * natural ageout occurs from timer module, so there's no need
	 * to add the session to invalid list, timer will automically
	 * add to the next tic.
	 */
	if (code != AH_NATURAL_AGEOUT) {

		/*
		 * instead of deleting/adding from/to the session the ager list
		 * directly, add it invalid session list which will be picked by
		 * the timer module. this is done in order to avoid timer lock
		 * deadlocks.
		 */

		ah_abs_spin_lock_bh (&ah_mac_sess_invalid_list_lock);
		hlist_add_head (&s->invalid_sess_list, &ah_mac_sess_invalid_list);
		ah_abs_spin_unlock_bh (&ah_mac_sess_invalid_list_lock);

	}

	if (ah_is_mac_sess_close_log_on (s)) {
		int lifetime_sec = get_mac_sess_life_time (s) / HZ;
		ah_mac_flow_t *r = macflow2peer(f);

		afe_logk (AH_LOG_INFO,
				  "MAC session (id %u) %m -> %m is invalidated, reason %s, lifetime %u min %u sec, "
				  "user-name %s, hostname %s -> user-name %s, hostname %s\n",
				  s->id, f->src_mac, f->dst_mac,
				  ah_get_sess_invalidate_code_str (code),
				  lifetime_sec / 60, lifetime_sec % 60,
				  (f->station && f->station->username[0]) ? f->station->username : "N/A",
				  (f->station && f->station->hostname[0]) ? f->station->hostname : "N/A",
				  (r->station && r->station->username[0]) ? r->station->username : "N/A",
				  (r->station && r->station->hostname[0]) ? r->station->hostname : "N/A");
	}

}

static void free_mac_sess ( struct ah_rcu_head *rcu)
{
	ah_kfree(container_of(rcu, ah_mac_sess_t, rcu));
}

/*
 * this function can be called when the following locks are held
 *
 * 1. ah_mac_sess_list_lock
 * 2. ah_mac_sess_timer_table->mac_sess_tick_lock [tic]
 */
static void ah_delete_mac_sess (ah_mac_sess_t *s, boolean have_lock)
{
	ah_mac_flow_t *f, *r;

	f = &s->f_flow;
	r = macflow2peer(f);

	afe_mac_sess_dbgk (s, "mac sess id [%u] is deleted\n", s->id);

	/* delete both flows from hash tables */

	ah_abs_spin_lock_bh(&flow_table->flow_hash_lock[f->hash]);
	ah_del_flow_from_hash (f);
	ah_abs_spin_unlock_bh(&flow_table->flow_hash_lock[f->hash]);

	if (!is_ah_flow_not_installed (r)) {
		ah_abs_spin_lock_bh(&flow_table->flow_hash_lock[r->hash]);
		ah_del_flow_from_hash (r);
		ah_abs_spin_unlock_bh(&flow_table->flow_hash_lock[r->hash]);
	}

	/* remove flows from route linked lists.. */
	if (IS_AH_FLOW_IN_ROUTE_LIST (f)) {
		ah_route_unlink_mac_sess (f, FALSE);
	}

	ah_route_unlink_mac_sess (r, FALSE);

	ah_del_mac_sess_from_list (s, have_lock);

	/* release the id into the free pool */
	ah_clr_mac_sess_id_bit (s->id);

	if (f->station) {
		ah_sta_dec_mac_sess_cnt (f->station);
	}

	if (r->station) {
		ah_sta_dec_mac_sess_cnt (r->station);
	}

	ah_call_rcu (&s->rcu, free_mac_sess);

	ah_dec_mac_sess_cnt ();
}

static void ah_delete_all_mac_sessions (void)
{
	ah_mac_sess_t *s, *n;

	ah_abs_spin_lock_bh (&ah_mac_sess_list_lock);

	list_for_each_entry_safe (s, n, &ah_mac_sess_list, mac_sess_list) {

		ah_delete_mac_sess (s, TRUE);
	}

	ah_abs_spin_unlock_bh (&ah_mac_sess_list_lock);
}

void ah_clr_mac_sess_by_id  (uint32_t id)
{
	ah_mac_sess_t *s;

	ah_rcu_read_lock ();

	list_for_each_entry_rcu (s, &ah_mac_sess_list, mac_sess_list) {

		if (ah_is_mac_sess_invalid (s)) {
			continue;
		}

		if (s->id == id) {
			ah_invalidate_mac_sess (s, AH_CLI_DELETE, AH_CLI_DELETE_AGEOUT_TIME);
		}

	}

	ah_rcu_read_unlock ();
}

void ah_clr_mac_sess_by_params (char *src_mac, char *dst_mac)
{
	ah_mac_sess_t *s;
	ah_mac_flow_t *f, *r;
	boolean zero_src_mac = 0, zero_dst_mac = 0;

	zero_src_mac = (ah_compare_ether_addr(src_mac, zero_mac) == 0);
	zero_dst_mac = (ah_compare_ether_addr(dst_mac, zero_mac) == 0);

	ah_wrapper_ah_rcu_read_lock_bh ();

	list_for_each_entry_rcu (s, &ah_mac_sess_list, mac_sess_list) {

		if (ah_is_mac_sess_invalid (s)) {
			continue;
		}

		f = &s->f_flow;
		r = &s->r_flow;

		if (((zero_src_mac || (ah_compare_ether_addr(f->src_mac, src_mac) == 0)) &&
			 (zero_dst_mac || (ah_compare_ether_addr(f->dst_mac, dst_mac) == 0))) ||
			((zero_src_mac || (ah_compare_ether_addr(r->src_mac, src_mac) == 0)) &&
			 (zero_dst_mac || (ah_compare_ether_addr(r->dst_mac, dst_mac) == 0))))  {

			ah_invalidate_mac_sess (s, AH_CLI_DELETE, AH_CLI_DELETE_AGEOUT_TIME);

		}

	}

	ah_wrapper_ah_rcu_read_unlock_bh ();
}

void ah_clr_mac_sess_by_dev(struct ah_abs_net_device *dev)
{
	ah_invalidate_sess_by_dev(dev);
}

static ah_mac_sess_t *ah_find_mac_sess_by_id (uint32_t id)
{
	ah_mac_sess_t *s;

	ah_rcu_read_lock ();

	list_for_each_entry_rcu (s, &ah_mac_sess_list, mac_sess_list) {

		if (s->id == id) {
			ah_rcu_read_unlock ();
			return s;
		}
	}

	ah_rcu_read_unlock ();

	return NULL;
}

static inline ah_mac_flow_t
*ah_find_flow_by_tuple_inline (ah_mac_flow_t *f, ah_mac_t *smac, ah_mac_t *dmac, int32_t zone_id)
{
	uint8_t hash;
	ah_mac_flow_t *tf;

	hash = ah_flow_hash(smac, dmac, zone_id);

	if (likely (f)) {
		f->hash = hash;
	}

	ah_rcu_read_lock ();

	/*
	 * try to get existing entry
	 */
    ah_hlist_for_each_entry_rcu (tf, h, &flow_table->flow_hash[hash], hash_list) {

		if (unlikely (ah_is_mac_sess_invalid (macflow2sess(tf)))) {
			continue;
		}

		if (!ah_compare_ether_addr(tf->src_mac, (char *)smac) &&
			!ah_compare_ether_addr(tf->dst_mac, (char *)dmac) &&
			tf->zone_id == zone_id) {

			ah_rcu_read_unlock ();

			return tf;
		}
	}

	ah_rcu_read_unlock ();

	return NULL;
}

/*
 * this funcion can be called when the following locks are held
 * 1. ah_mac_sess_create_lock
 */
ah_mac_flow_t *ah_find_flow_by_tuple (ah_mac_flow_t *f, ah_mac_t *smac, ah_mac_t *dmac, int32_t zone_id)
{
	return ah_find_flow_by_tuple_inline (f, smac, dmac, zone_id);

}

static inline ah_mac_flow_t *ah_find_flow_by_skb (struct ah_sk_buff *skb)
{
	ah_mac_t *smac, *dmac;
	ah_mac_flow_t *f;

	/* get the global flow */
	f = (ah_mac_flow_t *)skb2macflow(skb);

	if (is_tcp_fastpath(skb)) {
		ah_fill_flow_lookup_params_fastpath(skb, smac, dmac, f->iif, f->zone_id);
	} else {
		ah_fill_flow_lookup_params(skb, smac, dmac, f->iif, f->zone_id);
	}

	f = ah_find_flow_by_tuple_inline(f, smac, dmac, f->zone_id);

	return f;
}

/*
 * this function can be called when the following locks are held
 *
 * 1. mac_sess->mac_sess_rw_lock
 */
static inline int convert_flow_from_tunnel_to_clear (ah_mac_flow_t *f)
{
	clr_ah_flow_encap(f);

	f->tunnel_id = 0;

	clr_ah_flow_encap_vec(f);

	if (ah_create_vector_list_from_index (f->vector_index) < 0) {
		afe_detail_or_drop_dbgk (NULL, "error in convert flow [%d] from tunnel to clear, drop pak\n", macflow2sess(f)->id);
		ah_invalidate_mac_sess (macflow2sess(f), AH_VECTOR_CREATE_FAIL, AH_MALLOC_FAIL_AGEOUT_TIME);
		return -1;
	}

	afe_mac_sess_dbgk (macflow2sess(f), "convert MAC session [%m -> %m] id [%d] from tunnel to clear\n",
					   f->src_mac, f->dst_mac, macflow2sess(f)->id);

	return 0;
}

/*
 * this function can be called when the following locks are held
 *
 * 1. mac_sess->mac_sess_rw_lock
 */
static inline int convert_flow_from_clear_to_tunnel (ah_mac_flow_t *f)
{
	ah_tunnel_t *t;
	ah_policy_t *p = f->policy;
	ah_mac_sess_t *s = macflow2sess(f);

	ah_kassert(p);

	t = ah_get_tunnel_by_id (p->tun_id);
	ah_kassert(t);

	f->tunnel_id = p->tun_id;
	set_ah_flow_encap(f);

	set_ah_flow_encap_vec(f);
	if (ah_create_vector_list_from_index (f->vector_index) < 0) {
		afe_mac_sess_dbgk (macflow2sess(f), "error in convert MAC session [%d] from clear to tunnel, drop pak\n",
						   macflow2sess(f)->id);
		ah_invalidate_mac_sess (macflow2sess(f), AH_VECTOR_CREATE_FAIL, AH_MALLOC_FAIL_AGEOUT_TIME);
		return -1;
	}

	afe_mac_sess_dbgk (macflow2sess(f), "convert MAC session [%m -> %m], session id [%d] from clear to tunnel\n",
					   f->src_mac, f->dst_mac, s->id);

	return 0;
}

int ah_flow_handle_tunnel_transition (ah_mac_flow_t *f)
{
	int rc;
	ah_mac_sess_t *s;

	if (likely (!is_ah_mac_flow_tunnel_transition (f))) {
		return 0;
	}

	rc = 0;
	s = macflow2sess(f);
	ah_write_lock_bh (&s->mac_sess_rw_lock);

	/* take care of (tunnel <-> clear) flow transition */
	if (is_ah_flow_dst_tun_loc_sta(f) || is_ah_flow_to_self (f) || !f->policy) {

		if (is_ah_flow_encap(f)) {
			rc = convert_flow_from_tunnel_to_clear (f);
		}

	} else {

		if (f->policy && !is_ah_flow_encap(f)) {
			rc = convert_flow_from_clear_to_tunnel (f);
		}
	}

	ah_write_unlock_bh (&s->mac_sess_rw_lock);

	clr_ah_mac_flow_tunnel_transition (f);

	return rc;
}

#if ZERO
static inline int ah_flow_handle_cwp_transiton (struct ah_sk_buff *skb, ah_mac_flow_t *f)
{
	boolean need_cwp = 0;
	boolean have_cwp_vec = is_ah_flow_cwp_vec(f) ? 1 : 0;
	ah_mac_sess_t *s = macflow2sess(f);
	ah_mac_flow_t *r = macflow2peer (f);

	if (!is_skb_reinject(skb) &&
		((dev2ahdev(skb->dev) && AH_SYS_DEV_IS_CWP_ANY_ENABLED(dev2ahdev(skb->dev))) ||
		 (f->out_dev && dev2ahdev(f->out_dev) && AH_SYS_DEV_IS_CWP_ANY_ENABLED(dev2ahdev(f->out_dev))))) {
		need_cwp = 1;
	}

	if (likely (need_cwp == have_cwp_vec)) {
		return 0;
	}

	ah_write_lock_bh (&s->mac_sess_rw_lock);

	if (need_cwp) {

		set_ah_flow_cwp_vec(f);
		set_ah_flow_cwp_vec(r);

		afe_mac_sess_dbgk (s, "MAC session (id %u), add CWP processing\n",
						   f->src_mac, f->dst_mac, s->id);

	} else {

		clr_ah_flow_cwp_vec(f);
		clr_ah_flow_cwp_vec(r);

		afe_mac_sess_dbgk (s, "MAC session (id %u), remove CWP processing\n",
						   f->src_mac, f->dst_mac, s->id);

	}

	if (unlikely (ah_create_vector_list_from_index (f->vector_index) < 0)) {
		ah_invalidate_mac_sess (macflow2sess(f), AH_VECTOR_CREATE_FAIL, AH_MALLOC_FAIL_AGEOUT_TIME);
		ah_write_unlock_bh (&s->mac_sess_rw_lock);
		return -1;
	}

	if (ah_create_vector_list_from_index (r->vector_index) < 0) {
		ah_invalidate_mac_sess (macflow2sess(r), AH_VECTOR_CREATE_FAIL, AH_MALLOC_FAIL_AGEOUT_TIME);
		ah_write_unlock_bh (&s->mac_sess_rw_lock);
		return -1;
	}

	ah_write_unlock_bh (&s->mac_sess_rw_lock);

	return 0;
}
#endif

static inline int
ah_common_pkt_proc_fastpath (struct ah_sk_buff *skb, ah_mac_flow_t *f,
							 ah_mac_flow_t *r, struct ah_ethhdr *e_hdr)
{
	ah_mac_sess_t *s = NULL;

	if (unlikely (f->vlan != AH_GET_VLAN_VID(AH_VLAN_TAG_P(skb->cb)))) {

		if (is_ah_flow_skip_self (f) && f->vlan) {

			afe_detail_or_drop_dbgk (skb, "pkt with vlan is [%d], mac_flow vlan is [%d]\n",
									 AH_GET_VLAN_VID(AH_VLAN_TAG_P(skb->cb)), f->vlan);

			/* drop the pak, and hope sequence paks can pass */
			ah_route_unlink_mac_sess (f, FALSE);
			ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_NO_MATCHING_VLAN);
			return -1;
		}

		f->vlan = AH_GET_VLAN_VID(AH_VLAN_TAG_P(skb->cb));
		set_ah_mac_flow_vlan_present (f);
	}


	/*
	 * all self pkts are from 'mgt0' where as the routes are setup
	 * on different interfaces like eth0, wifix.y, so avoid this
	 * check from self pkts.
	 *
	 * tunnel routes are installed on mgt0 where as pkts come on eth0.
	 * pkts from switch phisical ports are treated as from 'mux0'
	 */

	if (unlikely (skb->dev != r->out_dev)) {
		s = macflow2sess (f);

		ah_read_lock_bh (&s->mac_sess_rw_lock);

		/*
		 * note that there's no route yet when we are processing the
		 * hive_broadcast_pkt, so let them go even when
		 * theres' no reverse route. Also, don't let the flow to be created
		 * so that we don't need to end up tearing down this session and
		 * creating a new one because of interface change
		 */
		if (unlikely ((is_ah_flow_to_self(f) && !is_ah_hive_broadcast_pkt(ah_eth_hdr(skb))))) {

			afe_detail_dbgk (skb, "to-self pkt with route on incorrect/no interface, let it go to self\n");
			set_ah_flow_no_flow (f);

		} else if (unlikely (!is_skb_from_self(skb) && !is_skb_decap_pak(skb) && !is_skb_reinject(skb))) {

			/*
			 * reason on why we need to have skb_rinject to bypass this check. in some cases,
			 * it's possible that we have an ARP-entry learnt on mgt0 (if the ARP req or reply)
			 * is through the tunnel where as the route is on physical interface. For example,
			 *
			 *                           tunnel
			 *      station --- AP1 <--------------->  AP2
			 *
			 * In this case, AP2's ARP entry  may learnt on mgt0 in AP1 where the route for AP2
			 * may be on physical interface. So we will have to skip this check for such cases.
			 */

			afe_detail_or_drop_dbgk (skb, "pkt arrived on incorrect interface [%s], route for %m is on [%s]\n",
									 dev2name(skb->dev), f->src_mac, dev2name(r->out_dev));
			ah_read_unlock_bh (&s->mac_sess_rw_lock);
			ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_WRONG_IN_DEV);
			return -1;

		}

		ah_read_unlock_bh (&s->mac_sess_rw_lock);
	}

	if (unlikely (is_skb_from_self (skb))) {

		if (is_ah_flow_to_self (f)) {
			afe_detail_or_drop_dbgk (skb, "self pkt to be delivered to self? drop pkt\n");
			ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_SELF_TO_SELF);
			return -1;
		}

	} else {
		if ( unlikely (dev2ahdev(skb->dev)->screen_method_list) &&
			 /* unless 'screen' feature turned on, don't try to even call the screening function */
			 unlikely (ah_screen_packet_proc (skb) == AH_CWP_DROP_PAK) ) {
			ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_SCREEN_CHECK_FAIL);
			return -1;
		}
	}

	return 0;
}

static inline int ah_common_pkt_proc (struct ah_sk_buff *skb, ah_mac_flow_t *f, struct ah_ethhdr *e_hdr)
{
	ah_mac_flow_t *r = macflow2peer (f);

	if (ah_common_pkt_proc_fastpath(skb, f, r, e_hdr) < 0) {
		return -1;
	}

	/*
	 * strip the VLAN tag for pre-auth pkts because when the
	 * pre-auth reply comes from AP2, the pkt will have AP2's
	 * vlan tag and driver will drop the pkt on it's way out
	 * to the client. so strip the vlan tag before sending it
	 * out to the client.
	 * Refer to bug 2444.
	 *
	 *
	 *     AP1   ---  AP2
	 *     |
	 *     |
	 *     |
	 *   Client
	 *
	 */

	if (unlikely (!is_ah_flow_to_self (f) && is_ah_preauth_pkt(e_hdr))) {

		afe_detail_dbgk (skb, "pre-auth pkt, remove the vlan tag\n");
		AH_CLR_VLAN_CB (skb->cb);

	}

	return 0;
}

static ah_policy_t *ah_update_tunnel_policy (struct ah_sk_buff *skb, struct ah_abs_net_device *in_dev, ah_mac_flow_t *f)
{
	ah_policy_t *fp;

	fp =  ah_find_policy (in_dev, f->src_mac, f->dst_mac);

	if (fp) {

		f->policy = fp;
		f->tunnel_id = fp->tun_id;
		set_ah_mac_flow_tunnel_transition (f);

		// setup switch port for CAVC
		if (fp->is_cavc) {
			f->cavc_port_index = fp->if_index;
		}

		afe_detail_dbgk (skb, "found policy id %u mac %m dir %s tun-id %u for mac-sess %u\n",
						 fp->id, fp->mac, ah_get_dir_str(fp->dir), fp->tun_id, macflow2sess(f)->id);

		/*
		 * if it's not a locally attached station and if a policy found, overwrite
		 * routing and change outgoing interface to mgt0. for locally attached stations
		 * if we install the session on mgt0, we try to deliver to the pkt to self and
		 * pkt will eventually get dropped. as long as the pkt is getting tunneled,
		 * outgoing interface doesn't matter.
		 */
		/*
		 * now the session lookup is based on zone and not on interface, so there's
		 * no need to change the interface to mgt0. earlier, because of interface
		 * based session lookup, we had to anchor the reverse flow mgt0 because pkts
		 * out of tunnel would come on mgt0. now, since mgt0 and eth0/wifi1.1 are all
		 * in the same zone (backhaul), there's no need to change the interface.
		 */
		/*
		if (!is_ah_flow_dst_tun_loc_sta (f))
		    f->out_dev = g_mgt_if;
		*/

	}

	return fp;
}

static void ah_process_deny_filter (struct ah_sk_buff *skb, ah_mac_sess_t *s, uint16_t attr1, uint16_t attr2)
{
	ah_user_profile_entry_t *up1, *up2;
	uint32_t deny_flags;

	if (!(up1 = ah_get_user_profile_from_attr(attr1)) ||
		!(up2 = ah_get_user_profile_from_attr(attr2))) {
		afe_detail_dbgk(skb, "attributes [%d] or [%d] does't exist\n", attr1, attr2);
		return;
	}

	deny_flags = up1->deny_type | up2->deny_type;

	ah_clr_mac_sess_deny_ipv4(s);
	ah_clr_mac_sess_deny_ipv6(s);

	if (deny_flags & AH_UP_DENY_IPV4) {
		ah_set_mac_sess_deny_ipv4(s);
	}

	if (deny_flags & AH_UP_DENY_IPV6) {
		ah_set_mac_sess_deny_ipv6(s);
	}
}

/*
 * ah_first_path_vector_common
 *
 * first path vector to create session both from the pkt
 * and from the session sync message.
 *
 * @skb       - packet pointers, NULL if it's from ssession sync
 * @f         - template flow where all the parameters are filled in
 * @from_sync - TRUE if it's from session sync
 *
 */
int ah_force_ipsess = 0;
ahcpd(firstpathc);


void *ah_first_path_common (struct ah_sk_buff *skb, ah_mac_flow_t *f, boolean from_sync)
{
	ah_mac_flow_t     *r = macflow2peer (f);
	ah_mac_sess_t     *s = macflow2sess(f);
	int                rc1, rc2, rc4, rc = 0;
	struct ah_abs_net_device *in_dev;
	ah_policy_t       *fp = NULL, *rp = NULL;
	ah_tunnel_t       *t;
//  int                route_zone_id;
	boolean            drop_pak = FALSE;
	boolean            reinject_pak = FALSE;

	int         should_drop = 0;

	ahcp_start();

	if (from_sync == FALSE && skb == NULL) {
		ah_logk(AH_SYS_KMOD_FE, AH_LOG_ERR,
				"skb is NULL for non-synced function call of ah_first_path_common!\n");
		return NULL;
	}
	in_dev = skb ? skb->dev : NULL;

	/* do the route looukup first */
	rc1 = ah_kafe_forward_lookup (skb, in_dev, f);

	/* now do reverse route lookup */
	rc2 = ah_kafe_reverse_lookup (skb, in_dev, r);

	/*
	 * we need to make sure that we do both forward and reverse route
	 * lookups before calling probe function since probe now appending
	 * next-hop metric into the payload.
	 *
	 * also, the reason why we call probe even when route indicates to
	 * drop the pkt is to inform the probe req originator that there's no
	 * route.
	 */
	if (skb && is_ah_probe_pkt (ah_eth_hdr(skb))) {

		rc4 = ah_proc_probe_pkt (skb, rc1 & AH_ROUTE_DROP_PAK, is_ah_flow_to_self (f));

		if (rc4 & AH_ROUTE_REINJECT_PAK) {
			reinject_pak = TRUE;
			ah_probe_reinject_pak (skb, ah_get_cpu_var(gflow));
			goto end_first_path;
		}

		if (rc4) {
			rc1 = rc4;
		}

	}

	/* if forward routing tells us to drop the pkt.. */
	if (rc1 & AH_ROUTE_DROP_PAK) {
		ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_NO_ROUTE);
		drop_pak = TRUE;
		goto end_first_path;
	}

	/*
	 * if reverse routing tells us to drop the pkt..
	 *
	 * allow to-self traffic to come in even if there's no reverse route.
	 * In case where eth0 interface is down ,we may not have a default route
	 * so unicast control pkts like AMRP, EAP pkts will be dropped if we
	 * enforce the reverse route check for to-self traffic. This will not
	 * bring up AMRP adjacency and default-route wlll not be learnt. so skip
	 * the reverse route check for to-self traffic.
	 */
	/* don't drop ah-llc pkts if indicated by reverse route lookup,
	 *    otherwise amrp register won't go through to portal from > 3 wifi-mesh hop away;
	 */
	if (!is_ah_flow_to_self(f) &&
		(!skb || (ah_eth_hdr(skb) && !is_ah_llc_pkt(ah_eth_hdr(skb)))) &&
		(rc2 & AH_ROUTE_DROP_PAK)) {
		ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_NO_ROUTE);
		drop_pak = TRUE;
		goto end_first_path;
	}

	/*
	 * if the session creation is happening from a sync msg,
	 * first get the interface for the src-mac based on route
	 * lookup and do a flow lookup first. if the flow already
	 * exists, get out.
	 */
	if (from_sync == TRUE) {

		ah_mac_flow_t *ef;

		/*
		 * fill in the interfaces based on route lookup in case
		 * this is a session sync message.
		 */

		in_dev = r->out_dev;

		if (!r->out_dev) {
			afe_detail_or_drop_dbgk (NULL, "no route for %m, don't create sess from sync msg\n", r->dst_mac);
			ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_SYNC_NULL_DEV);
			drop_pak = TRUE;
			goto end_first_path;
		}

		/*
		if ((f->zone_id != (route_zone_id = get_ahdev_zone (dev2ahdev(r->out_dev))))) {

		    afe_detail_or_drop_dbgk (NULL, "zone mismatch for the synced sess, sess zone [%s] != route zone [%s]\n",
		                    ah_zone_str(f->zone_id), ah_zone_str(route_zone_id));
		    ah_inc_drop_pak_counter (skb? ah_get_skb_input_dev(skb) : NULL, AH_DROP_ZONE_MISMATCH);
		    drop_pak = TRUE;
		    goto end_first_path;

		} else {

		    afe_detail_dbgk (NULL, "same zone %s after route lookup to %m\n", ah_zone_str (f->zone_id), f->src_mac);
		}
		*/

		f->iif = r->out_dev->ifindex;
		f->zone_id = get_ahdev_zone(dev2ahdev(r->out_dev));

		if (!is_ah_flow_dst_broadcast(f)) {

			if (!f->out_dev) {
				afe_detail_or_drop_dbgk (NULL, "no route for %m, don't create sess from sync msg\n", f->dst_mac);
				ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_SYNC_NULL_DEV);
				drop_pak = TRUE;
				goto end_first_path;
			}


			/*
			if (r->zone_id != (route_zone_id = get_ahdev_zone (dev2ahdev(f->out_dev)))) {

			    afe_detail_or_drop_dbgk (NULL, "zone mismatch for synced sess, sess zone [%s] != route zone [%s]\n",
			                    ah_zone_str(r->zone_id), ah_zone_str(route_zone_id));

			    ah_inc_drop_pak_counter (skb? ah_get_skb_input_dev(skb) : NULL, AH_DROP_ZONE_MISMATCH);
			    drop_pak = TRUE;
			    goto end_first_path;

			 } else {

			    afe_detail_dbgk (NULL, "same zone %s after route lookup to %m\n", ah_zone_str(r->zone_id), f->dst_mac);

			 }
			 */

			r->iif = f->out_dev->ifindex;
			r->zone_id = get_ahdev_zone(dev2ahdev(f->out_dev));

		}

		/* if the session already exists.. */
		if ((ef = ah_find_flow_by_tuple (f,
										 (ah_mac_t *)f->src_mac,
										 (ah_mac_t *)f->dst_mac,
										 f->zone_id))) {


			f = ef;
			afe_sync_dbgk ("MAC session [%u] already exists\n", macflow2sess(ef)->id);
			goto end_first_path;
		}

	}

	if (skb) {

		if (ah_common_pkt_proc (skb, f, ah_eth_hdr(skb)) < 0) {
			drop_pak = TRUE;
			goto end_first_path;
		}

	}


	/* check if inter station traffic is allowed */
	if (!is_ah_flow_dst_broadcast (f)) {

		if ((is_ah_mac_flow_sta_real (f)) && is_ah_mac_flow_sta_real (r)) {

			if (in_dev && AH_SYS_DEV_IS_INTER_STATION_TRAFFIC_DISABLE (dev2ahdev(in_dev))) {
                afe_detail_or_drop_dbgk (skb, "inter station traffic not allowed on %s, drop pkt\n", dev2name (in_dev));
				drop_pak = TRUE;
				goto end_first_path;
			}

			if (f->out_dev && AH_SYS_DEV_IS_INTER_STATION_TRAFFIC_DISABLE (dev2ahdev(f->out_dev))) {
                afe_detail_or_drop_dbgk (skb, "inter station traffic not allowed on %s, drop pkt\n", dev2name (f->out_dev));
				drop_pak = TRUE;
				goto end_first_path;
			}

		}
	}

	/* Deny IPv4/IPv6 check */
	ah_process_deny_filter(skb, s, f->upid, r->upid);

	/* FW check on ingress side */
	if (in_dev && (ah_ingress_mac_filter (skb, r->upid, in_dev, f) < 0)) {
		drop_pak = TRUE;
		goto end_first_path;
	}

	/* FW check on egress side */
	if (f->out_dev && ah_egress_mac_filter (skb, f->upid, f->out_dev, f) < 0) {
		drop_pak = TRUE;
		goto end_first_path;
	}

	/* lookup policy even if there's no route */
	if (in_dev) {
		fp = ah_update_tunnel_policy (skb, in_dev, f);
	}

	/* for non-broadcast pkts, fill in reverse flow info based on route */
	if (AH_FLOW_NEED_LINK_ROUTE(f) && f->out_dev) {

		/*
		 * after searching for the reverse tunnel, we have all the info to fill in
		 * the reverse flow zone here..
		 */
		if ((rp = ah_update_tunnel_policy (skb, f->out_dev, r))) {

			;

		} else if (is_ah_flow_to_self (f)) {

			// afe_detail_dbgk (skb, "to-self traffic, change outif to mgt0\n");

			/*
			 * change the outgoing interface to mgt0 for to-self pkts.
			 * otherwise reply pkts for this will come in on mgt0
			 * and create a different session.
			 */

			/*
			 * we donn't need this anymore, we now change mac session lookup based
			 * on zone instead of interface, so it should be to use interface based
			 * on route lookup.

			f->out_dev = g_mgt_if;

			*/

		}


		r->iif     = f->out_dev->ifindex;
		r->zone_id = get_ahdev_zone(dev2ahdev(f->out_dev));

	}

	/*
	 * always create flows for pkts from access interfaces
	 * and packets from or to self.
	 *
	 * for pkts on backhaul interfaces, create flows only
	 * 1. broadcast pkt and we have atleast one access interface.
	 * 2. unicast pkt and outgoing interface is an access interface.
	 *
	 * with this logic, note that pkts out a tunnel may also
	 * get forwarded with no flow if they are headed out
	 * to a backhaul interface after decapsulation.
	 */
	if ((!skb || (!is_skb_from_self (skb) && !is_skb_to_self (skb) && !is_skb_reinject(skb))) &&
		is_zone_id_backhaul(f->zone_id)) {

		if (is_ah_flow_dst_broadcast(f)) {

			/* if there are no access interfaces in the box,
			 * don't create flows, no special processing needed
			 */
			if (r->out_dev && ah_dev_get_access_if_cnt (r->out_dev) == 0) {
				afe_detail_dbgk (skb, "no access interfaces, flow not needed\n");
				set_ah_flow_no_flow(f);
			}

		} else {

			struct ah_abs_net_device  *out_dev = f->out_dev;

			if (out_dev == NULL || r->out_dev == NULL ||
				(is_zone_id_backhaul(dev2ahdev(out_dev)->zone_id) &&
				 is_backhaul_app_enabled(out_dev) == FALSE &&
				 is_backhaul_app_enabled(r->out_dev) == FALSE)) {
				afe_detail_dbgk (skb, "pkt from backhaul to backhaul, flow not needed\n");
				set_ah_flow_no_flow(f);
			}
		}
	}

	if (skb && (rc1 & AH_ROUTE_SELF_PAK)) {

		if (is_skb_from_self (skb)) {

			ah_logk (AH_SYS_KMOD_FE, AH_LOG_ERR,
					 "pkt from self to be delivered to self? drop pak\n");

			ah_inc_drop_pak_counter (ah_get_skb_input_dev(skb), AH_DROP_PAK_FROM_SELF_LOOP);
			drop_pak = TRUE;
			goto end_first_path;
		}

	}

	/*
	 * don't tunnel pkts destined to self or a locally attached
	 * station *even if* we have a policy to tunnel the
	 * pkt, send the pkt directly to station to avoid
	 * extra hops
	 */
	if (!is_ah_flow_dst_tun_loc_sta(f) && !is_ah_flow_to_self(f) && fp) {

		t = ah_get_tunnel_by_id (fp->tun_id);
		ah_kassert(t);

		f->tunnel_id = fp->tun_id;
		set_ah_flow_encap(f);
		set_ah_flow_encap_vec(f);

		if (skb && is_ah_flow_no_flow(f)) {
			clr_ah_flow_no_flow(f);
		}

	} else {

		/* here we know whether or not to create flow */
		if (skb && is_ah_flow_no_flow(f)) {
			skb2flow_vector_ptr(skb) = (void *)no_flow_fast_path_vector_list;
			afe_detail_dbgk(skb, "walk thru no flow vector list\n");
			goto end_first_path;
		}
	}

	/*
	 * main forwarding vector should be always present. If encap vector is there,
	 * pkt doens't need to go to main vector when encap vector is removed (because
	 * of policy change etc), the same flow will be updated. so in case main
	 * vector is not there, pkt will not get forwarded out.
	 */
	set_ah_flow_main_vec(f);
	set_ah_flow_qos_cp_vec(f);
	set_ah_flow_dhcp_vec(f);
	set_ah_flow_ipv6_misc_vec(f);
	set_ah_flow_arp_vec(f);
	set_ah_flow_alg_vec(f);
	set_ah_flow_l7_vec(f);
	if (g_sfl.ah_sfl_enabled) {
		set_ah_flow_sfl_vec(f);
	}
	set_ah_flow_cwp_vec(f);

	/* set the same vector on the reverse direction as well */
	if (!is_ah_flow_dst_broadcast(f)) {

		set_ah_flow_main_vec(r);
		set_ah_flow_qos_cp_vec(r);
		set_ah_flow_dhcp_vec(r);

		set_ah_flow_ipv6_misc_vec(r);

		set_ah_flow_arp_vec(r);
		set_ah_flow_alg_vec(r);
		set_ah_flow_l7_vec(r);
		if (g_sfl.ah_sfl_enabled) {
			set_ah_flow_sfl_vec(r);
		}
		set_ah_flow_cwp_vec(r);

		/* check whether policy is needed or not for the reverse flow */
		if (!is_ah_flow_dst_tun_loc_sta(r) && !is_ah_flow_to_self(r) && rp) {

			t = ah_get_tunnel_by_id (rp->tun_id);
			ah_kassert(t);

			r->tunnel_id = rp->tun_id;
			set_ah_flow_encap(r);
			set_ah_flow_encap_vec(r);

		}

	}

#if 0
	if ((!skb || !is_skb_reinject(skb)) && in_dev && AH_SYS_DEV_IS_CWP_ANY_ENABLED(dev2ahdev(in_dev))) {
		set_ah_flow_cwp_vec(f);
		set_ah_flow_cwp_vec(r);
	}

	/* check whether cwp vector is needed by the reverse flow */
	if (!is_ah_flow_dst_broadcast(f) && f->out_dev && AH_SYS_DEV_IS_CWP_ANY_ENABLED (dev2ahdev(f->out_dev))) {
		set_ah_flow_cwp_vec(r);
		set_ah_flow_cwp_vec(f);
	}
#endif

	if ((f = ah_create_mac_sess (f, skb, from_sync, &should_drop)) == NULL ||
		should_drop == 1) {
		drop_pak = TRUE;

		if (f) {
			afe_detail_dbgk(skb, "drop the first packet but keep mac-ses %d", s->id);
		}
		goto end_first_path;
	}
#if  defined(AH_SUPPORT_SDWAN_FE_PATCH) //kidy
	if (skb != NULL && f->out_dev != NULL && is_access_dev(f->out_dev)) {
		/*
		 * This is a trick here: At this point, skb->dev is mgt0.X, but we know from mac-sess/route lookup, that this packet
		 * would be eventually egress to a lan_port, so we set our field here.
		 */
		set_skb_br_lan_out(skb, f->out_dev);
	}
#endif
	if (skb) {

		skb2flow_vector_ptr(skb) = (void *)ah_get_vector_list_from_index(f->vector_index);
		skb2macflow(skb) = (void *)f;


#if AH_SUPPORT_USEG
		if ( ah_useg_enable && 
			pkt_get_eth_type (skb) != ETH_P_IP &&
			pkt_get_eth_type (skb) != ETH_P_IPV6 ) {

			ah_useg_set_src_metadata_for_nonip_pkt(skb);

			ah_useg_set_dst_metadata_for_nonip_pkt(skb);
			}
#endif


		if (unlikely (ah_ip_sess_create_needed ())) {

			/* change for IPv6 */
			if ( ( (pkt_get_eth_type (skb) == ETH_P_IP) || (pkt_get_eth_type (skb) == ETH_P_IPV6 && !is_skb_ipv6_skip_session6(skb) ) )
				 && (!is_skb_nat (skb))) {


				boolean create_needed = ah_pkt_is_ip_flow_needed(skb) || ah_force_ipsess;
				boolean find_needed = create_needed ||
									  (!is_skb_from_self(skb) && ah_ip_nat_sess_lookup_needed()) ||
									  (is_skb_from_self(skb) && ah_ip_nat_to_self_sess_lookup_needed());

				if (create_needed) {
					set_skb_ipsess_needed(skb);
				} else {
					clr_skb_ipsess_needed(skb);
				}

				if (find_needed) {
					if ((rc = ah_find_and_create_ip_sess_from_skb (skb)) < 0) {
						drop_pak = TRUE;
					}
				}
			}
		}

		/*
		 * update counters before Ip session is created,
		 * some counters like TFTP are depend on parent Ip session
		 */
		ah_update_counters(skb, is_skb_reinject(skb) ? DIR_REINJECT : DIR_INCOMING);
	}

end_first_path:

	if (drop_pak) {
		set_ah_flow_drop_pak (ah_get_cpu_var(gflow));
		ahcp_return(firstpathc);
		return NULL;
	}

	if (reinject_pak) {
		ahcp_return(firstpathc);
		return NULL;
	}

	/* invoke QoS 1st packet processing */
	if (in_dev) {
		ah_qos_process_first_pkt_mac_sess (skb, f, in_dev);
	}

	/* pkt is queued by defrag queue */
	if (rc == 1) {

		/* return NULL so that vector processing will stop */
		ahcp_return(firstpathc);
		return NULL;
	}

	ahcp_return(firstpathc);
	/* when from_sync == FALSE, skb is a non-NULL value so no sanity check here */
	return (from_sync == TRUE) ? (void *)f : (void *)skb2flow_vector_ptr(skb);
}

ahcpd(firstpathv);
void *ah_first_path_vector (struct ah_sk_buff *skb)
{
	ah_mac_flow_t *f = (ah_mac_flow_t *)skb2macflow(skb), *r = macflow2peer(f);
	ah_mac_t *smac, *dmac;

	ahcp_start();

	skb_trace_trip(skb);

	/* fill up the basics fields from the pkt.. */
	ah_fill_flow_lookup_params (skb, smac, dmac, f->iif, f->zone_id);

	memcpy(f->src_mac, (char *)smac, ETH_ALEN);
	memcpy(f->dst_mac, (char *)dmac, ETH_ALEN);

	if ((*(char *)dmac) & 1) {
		set_ah_flow_dst_broadcast (f);
		set_ah_flow_not_installed (r);
	}

	memcpy(r->src_mac, dmac, ETH_ALEN);
	memcpy(r->dst_mac, smac, ETH_ALEN);

	ahcp_return(firstpathv);
	return ah_first_path_common (skb, f, FALSE);
}

ahcpd(sflv);
void *ah_sfl_vector(struct ah_sk_buff *skb)
{

	ahcp_start();

	ah_sfl_run_pkt(skb);

	ahcp_return(sflv);

	return flow_next_vector(skb) ;
}

ahcpd(l7v);
void ah_l7_process(struct ah_sk_buff *skb, ah_ip_flow_t *f, ah_ip_sess_t *s)
{
	ah_ip_flow_t *r;

	boolean nn;
	boolean lp;
	uint p_cnt;

	int l7_pak_threshold;

	ahcp_start();

	/* first check global flags */
	if ( (ah_fe_l7_enable == 0) ||
		 (ah_is_stateful_fw_enabled() == 0 &&
		  is_l7_ip_sess_flag_set() == 0)) {

		ahcp_return(l7v);

        return;
    }

	nn = TRUE;
	lp = FALSE;
	p_cnt = 0;

	l7_pak_threshold = ah_fe_l7_pak_threshold;

	r = ipflow2peer(f);

	if (unlikely(ah_is_ip_sess_invalid(s))) {

		ahcp_return(l7v);

        return;
    }

	/* now check session level flags */
	if (s->l7_cxt.cls_needed == AH_L7_CLS_NOT_NEEDED) {
		ahcp_return(l7v);

        return;
    } else if (s->l7_cxt.cls_needed == AH_L7_CLS_INIT) {
        if (is_l7_classification_needed(skb, s, f, r) == FALSE) {
            s->l7_cxt.cls_needed = AH_L7_CLS_NOT_NEEDED;

			ahcp_return(l7v);

            return;
        } else {
            s->l7_cxt.cls_needed = AH_L7_CLS_NEEDED;
        }
    }

	if (f->proto == IPPROTO_TCP) {

		if (ah_is_tcp_rst_pkt(skb)) {
			nn = FALSE;
		}
	}

	if (s->l7_cxt.is_monitored) {

		l7_pak_threshold = 0xffff;
	}

	else {

		if (s->l7_cxt.maxpkts != 0) {

			l7_pak_threshold = s->l7_cxt.maxpkts;
		}
	}

	p_cnt = ah_ip_flow_get_pak(r) + ah_ip_flow_get_pak(f);

	if (!is_skb_from_pkt_queue(skb)
		&& (!ah_is_skb_frag(skb) || ah_is_skb_first_frag(skb))) {

		if ((s->l7_cxt.l7_id != AH_L7_APPID_INVALID && s->l7_cxt.is_inspecting == FALSE) && !s->l7_cxt.is_monitored) {

		}

		else if (ah_is_ip_sess_from_sync(s)) {
			/* TODO leave it as invalid appid for now, daemon will convert it to IP */
		}

		else if (l7_pak_threshold < p_cnt) {

		}

		else if (nn == FALSE && p_cnt <= 1) {

			s->l7_cxt.l7_id = AH_L7_APPID_UNKNOWN;
#ifdef AH_SUPPORT_SDWAN            
 			s->l7_cxt.appgrp_id = 0;
#endif            
		}

		else {

			if (l7_pak_threshold == p_cnt) {
				lp = TRUE;
			}

			ah_send_l7_pkts(f, skb, nn, lp);
		}
	}

	ahcp_return(l7v);

	return;
}

void *ah_l7_vector(struct ah_sk_buff *skb) 
{
	ah_ip_flow_t *f;
	ah_ip_sess_t *s;

	if (unlikely(!(f = skb2ipflow(skb)))) {
		return flow_next_vector(skb);
    }

	s = ipflow2sess(f);
    if (likely(!ah_is_ip_sess_nat(s))) {
        ah_l7_process(skb, f, s);
    }

    return flow_next_vector(skb);
}

ahcpd(dhcpv);

void *FEFASTPATH ah_dhcp_vector (struct ah_sk_buff *skb)
{
	const struct ah_ethhdr *e;
	int is_dhcp_pkt;

	ah_sta_t *s;
	ah_mac_flow_t *f;

	ahcp_start();

	if (likely(is_tcp_fastpath(skb))) {
		ahcp_return(dhcpv);
		return flow_next_vector(skb);
	}


	/*return early based on our coarse parsing logic*/
	if (likely(get_skb_ipv4_l5_proto(skb) != AHB_L5_PROTO_ID_DHCPV4)) {
		return flow_next_vector(skb);
	}


	e = ah_eth_hdr(skb);
	is_dhcp_pkt = 0;    /* you should be set to 0 here instead of 1. */

	skb_trace_trip(skb);

	/* if this is a DHCP REQ pkt, note that down */
	if (is_access_dev(skb->dev)) {
		is_dhcp_pkt = ah_log_dhcp_pkt (skb, e->h_source);
	}

	f = (ah_mac_flow_t *)skb2macflow(skb);
	s = f->station;
	if (is_dhcp_pkt && s && AH_IS_STA_DHCP_BLOCK(s)) {
		afe_detail_or_drop_dbgk (skb, "dhcp: blocked, drop pak\n");
		set_ah_flow_drop_pak (ah_get_cpu_var(gflow));
		ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_DHCP_DISALLOW);
		ahcp_return(dhcpv);
		return NULL;
	}
	/*
	 * convert dst-mac address for DHCP reply pkts
	 * if necessary and reinject the pkt back into FE.
	 */
	if (is_dhcp_pkt && ah_convert_dhcp_reply_pkt (skb)) {
		ah_dhcp_reinject_pak  (skb, ah_get_cpu_var(gflow));
		ahcp_return(dhcpv);
		return NULL;
	}

	ahcp_return(dhcpv);

	return flow_next_vector(skb);
}

static const char *apple_test_header = "GET /library/test/success.html HTTP/1.0\r\n";
#define APPLE_TEST_HEADER_LEN   41


#define HTTP_TYPE_NORMAL        0
#define HTTP_TYPE_APPLE_TEST    1
/**
 * We think HTTP packet has 2 types:
 * 1. Apple's test request. It's start by apple_test_header
 * 2. Normal HTTP request.
 */
static int ah_alg_http_check_type(const struct ah_sk_buff *skb)
{
	const struct ah_ethhdr *e;
	struct ah_iphdr *ip;
	struct ah_tcphdr *tcp;
	char *http_hdr, *p;
	int http_len;
	int tcp_hdr_len;

	/* add for IPv6 */
	if (unlikely(is_skb_ipv6(skb))) {
		return -1;
	}


	e = ah_eth_hdr(skb);
	ip = (struct ah_iphdr *) (e + 1);
	tcp = ah_get_tcp_from_ip (ip);
	if (unlikely(tcp == NULL)) {
		return -1;
	}

	tcp_hdr_len = (tcp->doff * 4);
	http_hdr = (char *)tcp + tcp_hdr_len;
	p = http_hdr;
	http_len = ntohs(ip->tot_len) - (ip->ihl * 4) - tcp_hdr_len;
	if (http_len < APPLE_TEST_HEADER_LEN) {
		return -1;
	}
	if (memcmp(http_hdr, apple_test_header, APPLE_TEST_HEADER_LEN) != 0) {
		return HTTP_TYPE_NORMAL;
	}

	return HTTP_TYPE_APPLE_TEST;
}

/**
 * If the body changed, the value of Content-Length in header
 * and macro AH_REJ_HTTP_RESP should be changed too.
 */
static const char *rej_http_resp =
	"HTTP/1.1 200 OK\r\n"
	"Server: unspecified, TeacherView/1.0\r\n"
	"Connection: close\r\n"
	"Content-Type: text/html; charset=\"utf-8\"\r\n"
	"Content-Length: 564\r\n"
	"\r\n"
	"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\">\n"
	"<html><head><title>Reject</title></head>\n"
	"<body style=\"margin:10px;color:#69788E;background:#F7F7F7;font-family:Verdana, Arial, Helvetica, sans-serif;font-size:75%;\">\n"
	"<div style=\"padding:10px;color:#000000;background:#FFFFFF;border:1px dashed #CECFCE;\">\n"
	"<h1 style=\"color:#333333;font-size:1.70em;font-weight:normal;margin-bottom:10px;margin-top:10px;\">Network Access Denied</h1>\n"
	"You do not have permission to access the network. Please contact the network administrator for assistance.\n"
	"</div></body></html>";
#define AH_REJ_HTTP_RESP 703

static int ah_quarantine(struct ah_sk_buff *skb)
{
	ah_sta_t *s;
	const struct ah_ethhdr *e;
	ah_mac_flow_t *f = (ah_mac_flow_t *)skb2macflow(skb), *r;
	ushort deliver_flags;
	int drop_idx;
	int http_type;

	/* special action for TeacherView regardless CWP status */
	if (unlikely(((s = f->station) && AH_IS_STA_LIMITED_ACCESS(s)))
		// ((s = r->station) && AH_IS_STA_LIMITED_ACCESS(s))
	   ) {
		e =  ah_eth_hdr(skb);
		r =  macflow2peer (f);
		drop_idx = AH_DROP_LIMITED_ACCESS;
		deliver_flags = 0;

		if ((ah_validate_limited_pkt (skb, s, &deliver_flags, TRUE) < 0)
			|| (deliver_flags & AH_IS_HTTPS_PKT)) {
			afe_detail_dbgk(skb, "drop pkt for limited access\n");
			return -1;
		}

		/* http pkt, reply a failure page */
		if (deliver_flags & AH_IS_HTTP_PKT) {
			afe_detail_dbgk(skb, "limited access\n");
			http_type = ah_alg_http_check_type(skb);
			if (http_type == HTTP_TYPE_NORMAL) {

				afe_detail_dbgk(skb, "send the reject page.\n");

				if (ah_alg_http_redirect_output(f, skb, rej_http_resp, strlen(rej_http_resp) + 1, FALSE)
					!= ALG_CMD_RC_OK) {
					afe_detail_dbgk(skb, "drop http pkt for normal response fail\n");
				}

				return -1;	//no matter what, FE outter logic would drop skb

			} else if (http_type == HTTP_TYPE_APPLE_TEST) {
				afe_detail_dbgk(skb, "forward http pkt for Apple's test request\n");
				return 0;
			}
			else {

				return -1;	//FE outter logic would drop skb.
			}
		}
	}

	return 0;
}

/*
 * ah_cwp_lookup
 *
 * decides what to do with the pkt based on the station's src-mac address
 * state in the CWP table.
 *
 * Return code
 * AH_CWP_DROP_PAK     --> drop the pkt (no entry in CWP table or ifp mismatch)
 * AH_CWP_FORWARD_PAK  --> forward the pkt in the usual manner (station is already registered)
 * AH_CWP_TO_SELF_PAK   --> deliver the pkt to the local stack  (station is authed but not registered)
 */
ahcpd(cwpv);
void *FEFASTPATH ah_cwp_vector (struct ah_sk_buff *skb)
{
	ah_sta_t *s;
	const struct ah_ethhdr *e;
	ah_mac_flow_t *f, *r;
	boolean  cwp_local_authed_phase;
	ushort deliver_flags;
	int rc;
	int drop_idx;

	uint16_t etype;
	struct ah_tcphdr *tcp;
	struct ah_iphdr *ip;
	struct ah_ipv6hdr *ip6 = NULL;


	boolean need_xlat = FALSE;


	ahcp_start();

	skb_trace_trip(skb);

	if (unlikely(is_skb_from_self (skb))) {
		ahcp_return(cwpv);
		return flow_next_vector(skb);
	}


	f = (ah_mac_flow_t *)skb2macflow(skb);

	if (likely(!AH_SYS_DEV_IS_CWP_ANY_ENABLED(dev2ahdev(skb->dev)))) {

		if ( f->station != NULL && AH_IS_STA_LIMITED_ACCESS(f->station) &&	/* duplicate these check out from ah_quarantine(), to early return avoiding function call */
			ah_quarantine(skb) < 0 ) {

			drop_idx = AH_DROP_LIMITED_ACCESS;
			goto drop_pak;
		}
		ahcp_return(cwpv);
		return flow_next_vector(skb);
	}

	drop_idx = AH_DROP_CWP_ERR;
	deliver_flags = 0;

	e =  ah_eth_hdr(skb);
	r =  macflow2peer (f);

	/* first clear the CWP flags */
	clr_ah_flow_cwp_dhcp_to_self (f);

	if (!(s = f->station) || !ah_is_cwp_entry (s)) {

		afe_detail_dbgk(NULL,  "cwp: no entry\n");

		/* if there's no cwp station entry, allow only preauth pkt */
		if (!is_ah_preauth_pkt(e)) {
			afe_detail_or_drop_dbgk (NULL, "cwp: not pre-auth, drop pak\n");
			goto drop_pak;
		}

		set_ah_flow_to_self (ah_get_cpu_var(gflow));
		afe_detail_dbgk(NULL,  "cwp: deliver pak to self\n");
		ahcp_return(cwpv);
		return NULL;

	}

	/* treat ifp mismatch as no entry */
	if (s->iif != skb->dev->ifindex) {
		afe_detail_or_drop_dbgk (NULL, "cwp: dev mismatch, entry [%u] != incoming dev [%u], drop pak\n",
								 s->iif, skb->dev->ifindex);
		goto drop_pak;
	}

	/*
	 * let all the CWP registered and MDM enrolled users go..
	 * also let authorized users go to the walled garden white list
	 */
	if (AH_IS_CWP_ENTRY_COMPLETED(s)) {

		if (AH_SYS_DEV_IS_CWP_POPUP_ENABLED(dev2ahdev(skb->dev))) {

			if ((rc = ah_xlate_http_cwp_pkt (skb, skb->dev, s)) == 1) {
				afe_detail_dbgk (NULL, "cwp: %m registered: deliver http pkt to self\n", s->mac);
				set_ah_flow_to_self (ah_get_cpu_var(gflow));
				ahcp_return(cwpv);
				return NULL;
			}

			/* error in xlate, drop pkt */
			if (rc == -1) {
				goto drop_pak;
			}
		}

		if (AH_SYS_DEV_IS_CWP_LOCAL_ENABLED(dev2ahdev(skb->dev))) {
			ah_ingress_dhcp_process (skb, s, f);
		}

		afe_detail_dbgk(NULL, "cwp: forward pak\n");
		ahcp_return(cwpv);
		if (ah_quarantine(skb) < 0) {
			drop_idx = AH_DROP_LIMITED_ACCESS;
			goto drop_pak;
		}
		return flow_next_vector(skb);
	}

	if (AH_IS_CWP_ENTRY_COMPLETED_WAIT(s)) {

		/**
		 * If CWP completed but src_port/dst_ip in the map table
		 * We should translate the external IP to internal IP.
		 *
		 * This is for the case:
		 * 1. TCP 1/3 was received when CWP not completed
		 *    The IPs was translated
		 * 2. TCP 3/3 was received when  CWP completed, the IPs won't be translated
		 * And this would result in some error.
		 *
		 * To avoid this issue, we should check if the src_port/dst_ip is in the map table.
		 * If yes, we should translate the IPs even if the CWP passed.
		 *
		 * For the performace, we won't check each packet. So we add a flag to indicate if need to check.
		 * This flag would be set when CWP completed and would be cleared 10 seconds later.
		 */
		if (!time_after ((unsigned long)ah_jiffies, s->chk_xlate_expire)) {
			etype = pkt_get_eth_type(skb);

			if (etype == ETH_P_IP) {
				ip = (struct ah_iphdr *) (e + 1);
				if (s && ip->protocol == IPPROTO_TCP) {
					tcp = ah_get_tcp_from_ip (ip);

					if (ah_ip_port_is_map(s, ip->daddr, ntohs(tcp->source))) {
						need_xlat = TRUE;
						afe_detail_dbgk (NULL, "cwp: %m wait complete status: deliver http pkt to self\n", s->mac);
					}
				}
			}

			/* add for IPv6 */
			if (unlikely(etype == ETH_P_IPV6)) {

				ip6 = (struct ah_ipv6hdr *) (e + 1);
				tcp = ah_get_tcp_from_ipv6_skb(skb);

				if (s && tcp != NULL) {

					if (ah_ipv6_port_is_map(s, &ip6->daddr, ntohs(tcp->source))) {
						need_xlat = TRUE;
						afe_detail_dbgk (NULL, "cwp6: %m wait complete status: deliver http6 pkt to self\n", s->mac);
					}
				}
			}


		} else {
			/* COMPLETED_WAIT is expired, move to COMPLETED */
			AH_CLR_CWP_ENTRY_COMPLETED_WAIT(s);
			AH_SET_CWP_ENTRY_COMPLETED(s);
		}

		if (!need_xlat) {
			if (AH_SYS_DEV_IS_CWP_POPUP_ENABLED(dev2ahdev(skb->dev))) {
				if ((rc = ah_xlate_http_cwp_pkt (skb, skb->dev, s)) == 1) {
					afe_detail_dbgk (NULL, "cwp: %m registered: deliver http pkt to self\n", s->mac);
					set_ah_flow_to_self (ah_get_cpu_var(gflow));
					ahcp_return(cwpv);
					return NULL;
				}

				/* error in xlate, drop pkt */
				if (rc == -1) {
					goto drop_pak;
				}
			}

			if (AH_SYS_DEV_IS_CWP_LOCAL_ENABLED(dev2ahdev(skb->dev))) {
				ah_ingress_dhcp_process (skb, s, f);
			}

			afe_detail_dbgk(NULL, "cwp: forward pak\n");
			ahcp_return(cwpv);
			if (ah_quarantine(skb) < 0) {
				drop_idx = AH_DROP_LIMITED_ACCESS;
				goto drop_pak;
			}
			return flow_next_vector(skb);
		}
	}

	/* station is only authed or registered but not completed here.. */
	if (ah_is_cwp_entry_incompleted(s) || need_xlat) {

		/* after registered for cwp local, neeed xlate dst-mac and dst-ip for http(s) */
		if (AH_SYS_DEV_IS_CWP_LOCAL_ENABLED(dev2ahdev(skb->dev)) &&
			AH_IS_CWP_ENTRY_AUTHORIZED(s)) {
			cwp_local_authed_phase = TRUE;
		} else {
			cwp_local_authed_phase = FALSE;

			/* cwp local but passed authorize phase */
			if (AH_SYS_DEV_IS_CWP_LOCAL_ENABLED(dev2ahdev(skb->dev))) {
				ah_ingress_dhcp_process (skb, s, f);

				/* we do not support local CWP IPv6 */
			}
		}

		/* walled garden is supported for pass-thru cwp or cwp local but passed authorize phase */
		if (!cwp_local_authed_phase &&
			AH_SYS_DEV_IS_CWP_WALLED_GARDEN_ENABLED(dev2ahdev(skb->dev)) &&
			ah_check_wgarden_white_list(skb)) {
			afe_detail_dbgk(NULL, "cwp pak passed walled garden white list check: forward pak\n");
			ahcp_return(cwpv);
			return flow_next_vector(skb);
		}

		/*
		 * do this the sanity test only when IP session is not created.
		 * otherwise, this check is already before IP session is created
		 */
		if (ah_validate_limited_pkt (skb, s, &deliver_flags, cwp_local_authed_phase) < 0) {
			goto drop_pak;
		}


		/*
		 * in cwp incompleted state, we want all the pkts to be delievered to wifi0.x. in both
		 * cwp_local and cwp-passthrough because will be communicating with wifi0.x IP address
		 * in this case. but with DHCP server, there's a change, we want the client to talk to
		 * mgt0.x IP address in this case for DHCP pkts alone.
		 */
		if (cwp_local_authed_phase || !ah_is_valid_pkt_to_dhcp_server (skb)) {
			set_ah_flow_cwp_dhcp_to_self (f);
		}

		if (cwp_local_authed_phase || (deliver_flags & AH_DELIVER_TO_SELF)) {
			/*
			 * dst-ip for cwp-local must match device ip
			 * we need to drop http traffic with ip address in url not
			 * matching device ip
			 */
			if (cwp_local_authed_phase) {
				boolean allow_arp = 0;
				ah_ipaddr_t dst_ip = 0;
				uint16_t etype = pkt_get_eth_type(skb);

				/*
				 * AH_IPV6_CWP - local CWP does not support IPv6, so we don't need to anything here.
				 */

				if (etype == ETH_P_IP || etype == ETH_P_ARP) {
					if (etype == ETH_P_IP) {
						dst_ip = ((struct ah_iphdr *)(e + 1))->daddr;
					} else if (ah_get_arp_dst_ip(skb, &dst_ip, &allow_arp) < 0) {
						goto drop_pak;
					}

					if (dst_ip != INADDR_BROADCAST && !(IN_MULTICAST(dst_ip)) &&
						!allow_arp && !(ah_match_dst_ip_with_ifp(skb->dev, dst_ip))) {
						afe_basic_dbgk (NULL, "dst-ip %i doesn't match ip device (%s) IP, drop pkt before cwp completed\n",
										dst_ip, skb->dev->name);
						goto drop_pak;
					}
				}
			}

			set_ah_flow_to_self (ah_get_cpu_var(gflow));
			afe_detail_dbgk(skb,  "cwp %m not registered: deliver pak to self\n",  s->mac);
			ahcp_return(cwpv);
			return NULL;

		} else {

			if (ah_quarantine(skb) < 0) {
				drop_idx = AH_DROP_LIMITED_ACCESS;
				goto drop_pak;
			}
			return flow_next_vector (skb);
		}

		ahcp_return(cwpv);
		return NULL;
	}

drop_pak:

	set_ah_flow_drop_pak (ah_get_cpu_var(gflow));
	ah_inc_drop_pak_counter (NULL, drop_idx);
	ahcp_return(cwpv);
	return NULL;
}

/* perform QoS classification and policing */
ahcpd(qoscpv);
void *FEFASTPATH ah_qos_cp_vector (struct ah_sk_buff *skb)
{
	struct net_device *old_dev = NULL, *wan_in_dev = NULL;		/* for BR QoS trick, see below. */

	ahcp_start();

	skb_trace_trip(skb);

	/*
	 * assign QoS class and user profile to self packets
	 */
	if (unlikely (is_skb_from_self (skb))) {
        
#if  defined(AH_SUPPORT_SDWAN_FE_PATCH)
		/*
		 * BR QoS
		 *
		 *		(LAN-in, routing, mgt0.X FE-ingress, LAN-out) packets.
		 *
		 * 		For (LAN-in, LAN-out) packet, we skip cls/plc here in mgt0.X FE-ingress stage, because it already went through cls/plc at LAN-in stage.
		 *
		 *      No need to make the packet through cls/plc again.
		 */
		if (unlikely(is_skb_br_lan_in(skb) && is_skb_br_lan_out(skb))) {

			afe_detail_dbgk (skb, "BR QoS, no need cls/plc again for lan-in-lan-out packet\n");
                
			ahcp_return(qoscpv);
			return flow_next_vector(skb);            
		}

		/*
		 * BR QoS
		 *
		 *		(WAN-in, routing, mgt0.X FE-ingress, LAN-out) packets need to be cls / plc, according to STA user profile.
		 *
		 *		We can not do this in early WAN-in stage, because at that time, packet DMAC & DIP is just BR, and no way
		 * to determine STA user profile.
		 *
		 *		Therefore, we have to defer cls/plc later at this moment in mgt0.X FE-ingress.
		 *
		 *
		 * The trick for (WAN-in, LAN-out) is:
		 *
		 *		At this moment, skb->dev is mgt0.X, which has no classifier and policier configured.
		 *		So we can not use skb->dev, but instead, use the wan_in_dev ( with classifier and policier), which was set by 
		 *		set_skb_br_wan_in() in early FE-ingress pipeline before routing.
		 *
		 *		We save mgt0.X in local variable "old_dev", set skb->dev to be wan_in_dev temporarily, and then push this packet into
		 *		ah_qos_ingress_proc_pkt().
		 *
		 *		This is because cls & plc internal func rely on skb->dev, and only wan_in_dev has cls & plc config and datastructure, but not mgt0.X.
		 *
		 *		We restore skb->dev to be "old_dev" mgt0.X, after it pass cls/plc
		 *
		 */

		if (unlikely(is_skb_br_wan_in(skb) && is_skb_br_lan_out(skb) && !is_skb_encap_pak (skb))) {   //tip: !is_skb_encap_pak (skb) is just to follow "tunnel pak..." condition in existing if below.

			if ((wan_in_dev = get_skb_br_phy_dev_in(skb)) != NULL) {

				afe_detail_dbgk (skb, "BR QoS, jump to cls/plc for wan-in-lan-out packet.\n");
				old_dev = skb->dev;
				skb->dev = wan_in_dev;
				goto qos_ingress_proc_pkt;
			}
		}
#endif /* AH_L3_ROUTER */
		/* 
		 * tunnel pak will have to retain inner pak's
		 * QOS settings, don't change them.
		 */
		if (!is_skb_encap_pak (skb)) {
			ah_qos_process_host_pkt (skb);
		}

		ahcp_return(qoscpv);
		return flow_next_vector(skb);
	}


qos_ingress_proc_pkt:
	if (likely (ah_qos_ingress_proc_pkt (skb) == 0) ) {
		/* forward pkt */
#if  defined(AH_SUPPORT_SDWAN_FE_PATCH)
		if (old_dev) {
			skb->dev = old_dev;
		}
#endif
		ahcp_return(qoscpv);
		return flow_next_vector(skb);
	} else {
#if defined(AH_SUPPORT_SDWAN_FE_PATCH)
		if (old_dev) {
			skb->dev = old_dev;
		}
#endif
		/* drop pkt */
		set_ah_flow_drop_pak (ah_get_cpu_var(gflow));
		ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_QOS_POLICER_DROP);
		ahcp_return(qoscpv);
		return NULL;
	}

}

ahcpd(algv);
void *FEFASTPATH ah_alg_vector (struct ah_sk_buff *skb)
{
	ah_ip_flow_t *f;
	ah_ip_sess_t *s;
	int           air_time, rc, http_alg_pak_threshold;
	int skip_alg = 0;   /* jira bug HOS-3221 fix: if seeing NAT or REDIRECT, then skip ALG. */


	ahcp_start();

	skb_trace_trip(skb);

	if (likely(!(f = skb2ipflow(skb)))) {
		ahcp_return(algv);
		return flow_next_vector(skb);
	}

	mb();   /* HOS-3711 trial fix: on br100 mips32, register "s2" (saving local variable "f" ) suddenly become a invalid value, so add a barrier here for try */

	http_alg_pak_threshold = 0;
	s = ipflow2sess(f);


	/*
	 * xzhao - 2014.06.26
	 *
	 *      the following code block need some explain, to avoid possible.
	 *
	 *      it is NOT purely about ALG, but:
	 *
	 *          tcp     -   not about ALG, just monitor tcp rst/fin, for quick invalidate tcp ip-sess.  # this logic is similar to native netfilter conntrack.
	 *
	 *          icmp    -   not about ALG, just track icmp ip-sess
	 *
	 *          udp     -   check if tftp, if yes, perform tftp ALG
	 */

	/* cases where the IP session needs to be torn down right away */
	if (f->proto == IPPROTO_TCP) {

		/*
		 *common to IPv4 & IPv6 TCP
		 *
		 *     IPPROTO_TCP
		 *
		 *     ah_is_tcp_rst_pkt() -> ah_get_tcp_hdr()
		 *
		 *     ah_is_tcp_4way_close_done() -> ah_get_tcp_hdr()
		 */

		if (ah_is_tcp_rst_pkt (skb)) {

			if (s->alg_type == AH_ALG_TYPE_HTTP && s->pkt_queue_head &&
				!hlist_empty(&s->pkt_queue_head->queue_head)) {

				afe_detail_dbgk (skb, "tcp RST pkt, but queue is not empty on IP sess id [%u]\n",
								 s->id);
				/* queue the RST pkt and stop FE vectors processing */
				ah_queue_skb_to_ip_sess (s, skb, 0, TRUE);
				return NULL;

			} else {
				afe_detail_dbgk (skb, "tcp RST pkt, invalidate IP sess id [%u]\n", s->id);
				ah_invalidate_ip_sess_by_traffic (skb, s, AH_TCP_CLOSE, AH_TCP_CLOSE_AGEOUT_TIME);
			}

			goto check_nat;
		}

		if (ah_is_tcp_4way_close_done (skb, f)) {
			afe_detail_dbgk (skb, "tcp 4-way close done, invalidate IP sess id [%u]\n", s->id);
			ah_invalidate_ip_sess_by_traffic (skb, s, AH_TCP_CLOSE, AH_TCP_CLOSE_AGEOUT_TIME);
			goto check_nat;
		}

	} else if (f->proto == IPPROTO_ICMP || f->proto == IPPROTO_ICMPV6) {

		if (ah_process_icmp_pak (skb, f) < 0) {
			ah_invalidate_ip_sess_by_traffic (skb, s, AH_ICMP_REPLY, AH_ICMP_REPLY_AGEOUT_TIME);
			goto check_nat;
		}

	} else if (f->proto == IPPROTO_UDP) {

		/*
		 * if this is TFTP data session, do some special processing so that
		 * we send the last data pkt ack to control plane ALG.
		 */
		if (ah_is_ip_sess_from_gate (s) &&
			s->parent_sess &&
			s->parent_sess->alg_type == AH_ALG_TYPE_TFTP) {

			ah_process_tftp_pak (skb, f);

		}

	}


	if (ah_is_ip_sess_to_be_invalid (s) && ah_is_skb_last_frag (skb)) {
		ah_invalidate_ip_sess (s, s->ageout_code, AH_FRAG_AGEOUT_TIME);        
		goto check_nat;
	}


	if (likely(!ah_is_ip_flow_ipv6(f))) {
		/* for dhcp request pkt, create a gate for reply */
		if ((int)INADDR_BROADCAST == f->dst_ip && ah_is_dhcp_discover_or_req (skb) == TRUE) {

			/*
			 * note the request pkt may have the hsrc-ip as 0, in that case we will
			 * create a gate where dst-ip 0, i.e. we can expect the reply pkt to have
			 * any dst-ip. if the src-ip is not 0, we expect the reply pkt only to be
			 * destined only the specified IP address.
			 */
			ah_create_gate (skb,
							0,
							f->src_ip,
							f->dst_port,
							f->src_port,
							f->proto,
							&s->cookie,
							s->id,
							AH_IP_GATE_DHCP_REFCNT,
							AH_ALG_TYPE_DHCP);

		}
	} else {

	/* open gate for dhcp6 reply */


		/*
		 * xzhao - 2015.08.25
		 *
		 *      dhcpv6 is quite different from dhcpv4 in a lot of detail, though basic terminology is similar.
		 *
		 *      for example:    ususally, it uses DUID instead of explicit assigned IPv6 address.       # unless IA Address Option.
		 *
		 *                      therefore, ususally no explicit way to get to know client address from DHCPv6 message. ( but we can snoop )
		 *
		 *
		 *      therefore, for DHCPv6 ALG, my thinking is:
		 *
		 *              on seeing any dhcp6 client->server (or agent->server) message, just open the gate for all reverse reply
		 *
		 *
		 *      and, for any non-ALG DHCPv6 logic ( like snooping, protection ), YangYunlei already implemnt them in:
		 *
		 *              ah_dhcpv6.c                 # seems also reuse code from 3rd-party opensource software.
		 *              ah_dhcpv6_priv.c
		 */

		if (ipv6_addr_equal(&in6addr_all_dhcp_relay_agents_and_servers, &f->dst_ipv6)
			|| ipv6_addr_equal(&in6addr_all_dhcp_servers, &f->dst_ipv6) ) {

			ah_create_gate6 (skb,
							 _ah_in6addr_any,
							 &f->src_ipv6,
							 f->dst_port,
							 f->src_port,
							 f->proto,
							 &s->cookie,
							 s->id,
							 AH_IP_GATE_DHCP_REFCNT,
							 AH_ALG_TYPE_DHCP);
		}
	}


	/*
	 * if this is ALG control session, deliver the pkt to ALG in data plane.
	 * if the pkt is reinjected from pkt queue then no need to send to ALG again
	 *
	 * also, for non-first fragmented pkts, since the fragments are injected in
	 * order and first fragment already passed through pkt queue, there's no need
	 * check for subsequent frags, let them follow..
	 *
	 * for http alg:
	 * 1. check the http action flag, deliver the http pkt to application
	 *    if there is no http action flag set
	 * 2. only send up max number of packets per flow threshold
	 */
	if (s->alg_type == AH_ALG_TYPE_HTTP) {
		if (ah_is_ip_sess_http_deny(s) && !is_skb_from_pkt_queue(skb)) {

			/* drop pkt */
			set_ah_flow_drop_pak (ah_get_cpu_var(gflow));
			ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_INVALID_HTTP_ALG_PKT);
			afe_detail_dbgk (skb, "http alg pkt is denied on IP sess id [%u], drop it\n", s->id);
			ahcp_return(algv);
			return NULL;

		} else if (ah_is_ip_sess_http_permit(s)) {

			afe_detail_dbgk (skb, "http alg pkt is permitted on IP sess id [%u]\n", s->id);
			goto check_nat;

		}

		http_alg_pak_threshold = ah_is_ip_flow_forward(f) ?
								 ah_get_http_alg_upstream_threshold() :
								 ah_get_http_alg_downstream_threshold();
	}


	skip_alg = ( ( ah_is_ip_sess_techview(s) || ah_is_ip_sess_nat (s) ) &&
					( ah_is_ip_sess_alg_control (s) && s->alg_type == AH_ALG_TYPE_HTTP )	/* CFD-4470 - problem #1: caused by HOS-3221, fix: don't skip all alg on redirect/nat, but just http alg -- then alg-dns can work and update walled-garden hostname/IP */
					);

	if (!skip_alg &&
		ah_is_ip_sess_alg_control (s) &&
		!is_skb_encap_pak(skb) &&
		!is_skb_from_pkt_queue (skb) &&
		(!ah_is_skb_frag (skb) || ah_is_skb_first_frag (skb)) &&
		(s->alg_type != AH_ALG_TYPE_HTTP || !http_alg_pak_threshold ||
		 http_alg_pak_threshold >= ah_ip_flow_get_pak(f))) {

		int pkt_cookie;

		rc = ah_send_control_alg_pkt (f, skb, AH_ALG_PKT_MORE, s->alg_type, &pkt_cookie);
		if (rc < 0 &&
			s->alg_type != AH_ALG_TYPE_DNS ) {	/* let DNS packet continue be forwarded even if trap to user-level alg fails, so network DNS transaction not be broken */

			/* drop pkt */
			set_ah_flow_drop_pak (ah_get_cpu_var(gflow));
			ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_INVALID_ALG_CONTROL);
			ahcp_return(algv);
			return NULL;

		} else if (rc) {

			/*
			 * queue the pkt only if it's sent to ALG. some alg session
			 * pkts like tcp-ack etc that do not have any payload
			 * don't need to be sent to alg or queued.
			 */
			if (ah_should_queue_pkt_to_ip_sess (skb, s->alg_type)) {
				/* queue the pkt and stop vector processing */
				ah_queue_skb_to_ip_sess (s, skb, pkt_cookie, FALSE);
				ahcp_return(algv);
				return NULL;
			}

		}

	}

	/* if it's a CAC session, deduct rx-airimte tokens */
	if (ah_get_cac_enable () && ah_is_ip_sess_cac (s) && is_access_dev (skb->dev) && is_wireless_dev (skb->dev)) {
		air_time = AH_GET_RX_AIRTIME (skb);
		ah_cac_deduct_pkt_tokens (air_time, skb, ah_is_ip_sess_from_sync(s), FALSE);
	}

check_nat:

	if (ah_is_ip_sess_techview(s)) {
		/*
		   * if there are any fragmented pkts associated with this,
		   * reinject them back into FE..
		   */
		if (unlikely (get_skb_next_frag (skb))) {
			ah_reinject_frags (skb);
		}

		/* addd for IPv6 */
		if (likely(ah_is_ip_sess_ipv4(s))) {
			rc = ah_nat_skb_toself_by_ip_flow (skb, f);
		} else {
			rc = ah_nat6_skb_toself_by_ip_flow (skb, f);
		}


		if (rc < 0) {

			/* drop pkt */
			set_ah_flow_drop_pak (ah_get_cpu_var(gflow));
			ah_inc_drop_pak_counter (skb ?  ah_get_skb_input_dev(skb) : NULL, AH_DROP_NAT_FAIL);
			ahcp_return(algv);
			return NULL;

		} else if (rc == 1) {

			/*
			 * pkt is accepted by NAT and reinjected back into flow
			 * or Linux stack, so don't need anymore processing.
			 */
			ahcp_return(algv);
			return NULL;
		}
	}


	/* do NAT if needed */
	if (ah_is_ip_sess_nat (s)) {
		/*
		 * if there are any fragmented pkts associated with this,
		 * reinject them back into FE..
		 */
		if (unlikely (get_skb_next_frag (skb))) {
			ah_reinject_frags (skb);
		}

		/* addd for IPv6 */
		if (likely(ah_is_ip_sess_ipv4(s))) {
			rc = ah_nat_skb_by_ip_flow (skb, f);
		} else {
			rc = ah_nat6_skb_by_ip_flow (skb, f);
		}

		if (rc < 0) {

			/* drop pkt */
			set_ah_flow_drop_pak (ah_get_cpu_var(gflow));
			ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_NAT_FAIL);
			ahcp_return(algv);
			return NULL;

		} else if (rc == 1) {

			/*
			 * pkt is accepted by NAT and reinjected back into flow
			 * or Linux stack, so don't need anymore processing.
			 */
			ahcp_return(algv);
			return NULL;
		}
	}

	ahcp_return(algv);
	return flow_next_vector(skb);
}

void ah_airtime_proc_tx_pkt (struct ah_sk_buff *skb, ah_ieee80211_airtime_callback_arg_t *arg)
{
	ah_ip_flow_t  *ipf = skb2ipflow (skb);
	ah_mac_flow_t *mf = skb2macflow (skb);
	ah_mac_flow_t *mr;
	ah_ip_sess_t  *is;
	ah_mac_sess_t *ms;
	int            air_time;

	if (unlikely (!mf || !ipf)) {
		afe_cac_dbgk (NULL, "airtime api callback, mac flow 0x%x, ip flow 0x%x\n", mf, ipf);
		return;
	}

	mr = macflow2peer (mf);
	ms = macflow2sess (mf);
	is = ipflow2sess (ipf);

	if (likely (!ah_is_mac_sess_invalid (ms) && !ah_is_ip_sess_invalid (is) && ah_is_ip_sess_cac (is))) {
		air_time = AH_GET_TX_AIRTIME (skb);
		ah_cac_deduct_pkt_tokens (air_time, skb, ah_is_ip_sess_from_sync(is), TRUE);
	}

}

void ah_encap_with_tunnel (struct ah_sk_buff *skb, ah_tunnel_t *t)
{
	/****Hack code here ****/
	/*
	ah_vlan_tag_t vtag;
	set_ah_tunnel_classic(t);


	AH_CLR_VLAN_CB(skb->cb);
	vtag.ether_type = ETHERTYPE_VLAN;
	vtag.tci = 2;
	AH_SET_VLAN_CB(skb->cb, vtag);
	*/


	switch (t->type) {

	case AH_TUNNEL_GRE:
		if (t->tunnel.gre.src_ip.af == AF_INET) {
			gre_encap_hook(skb, t);
		} else {
			gre_encap_ipv6_hook(skb, t);
		}

		return;

	default:
		ah_kassert(0);
		break;
	}
}

ahcpd(encapv);
void *ah_encap_vector (struct ah_sk_buff *skb)
{
	const struct ah_ethhdr *e = ah_eth_hdr(skb);
	ah_tunnel_t *t;
	ah_mac_flow_t *f = (ah_mac_flow_t *)skb2macflow(skb);
	int tid;
	ah_policy_t *pol;
	struct ah_abs_net_device *in_dev = skb->dev;
	struct ah_abs_net_device *mgt_dev;
	ah_mgt_if_t *mif;
	ah_port_t *p;
	struct ah_sk_buff *skb2;
	int drop_idx = AH_DROP_ENCAP_ERROR;
	boolean unlink_mac_sess_on_drop = TRUE;

	ahcp_start();
	skb_trace_trip(skb);

	/*
	 * if there are any fragmented pkts associated with this,
	 * reinject them back into FE..
	 */
	if (unlikely (get_skb_next_frag (skb))) {
		ah_reinject_frags (skb);
	}

	pol = f->policy;

	if (!pol) {
		ah_logk(AH_SYS_KMOD_FE, AH_LOG_ERR,
				"policy not found in %s\n", __FUNCTION__);

		drop_idx = AH_DROP_NO_TUNNEL_POLICY;

		goto drop_pkt;
	}


#if AH_SUPPORT_USEG

	/* if useg_rules_table permit, then we should do local forwarding for unicast as much as possible, 
	*	without encapping into tunnel unnecessarily.
	*/

	if ( unlikely(ah_useg_enable)) {

		if (f->out_dev != NULL && 
			is_access_dev(f->out_dev) &&
			useg_packet_dst_info_is_to_physical_local_sta(skb2ahb(skb)->useg_packet_dst_info)) {
			/*
			 * is_to_local_sta() also indicates !(e->h_dest[0] & 1) and  is_access_dev(f->out_dev), 
			 * but just for code clearness, write explicit if check.
			 */
	
			return flow_next_vector(skb);	/* goto ah_main_vector() for local forwarding */
		}
	}

#endif	/* #if AH_SUPPORT_USEG */


	ah_policy_inc_pak (pol);

	tid = f->tunnel_id;

	/*
	 * clear all flags that are check in the main flow
	 * because after encap, the pkt will be sent out
	 * directly from encap vector itelf, if the pkt is to
	 * dropped, it will be dropped in this vector itself,
	 * so there's no need to check and drop the pkt in the
	 * main loop again. otherwise the skb will be freed twice!!!
	 */
	clr_ah_flow_main_loop_flags (ah_get_cpu_var(gflow));

	t = ah_get_tunnel_by_id (tid);

	if (!t) {
		ah_logk(AH_SYS_KMOD_FE, AH_LOG_ERR,
				"tunnel with id [%u] not found in %s\n", tid, __FUNCTION__);
#if 0
		ah_kassert(t);
#endif

		drop_idx = AH_DROP_NO_TUNNEL;

		goto drop_pkt;
	}

	/*
	 * if it's a broadcast pkt and pkt is getting encapped by a
	 * tunnel anchored on access zone, pkt needs to flooded out
	 * on other access interfaces as well in this vector itself
	 * since this pkt processing will stop after this vector.
	 *
	 * note that there's an implicit assumption here. packets
	 * coming from bachhaul zone that need to be encrypted will
	 * not have the DMAC as a broadcast address, so no need to
	 * handle flooding of such packets here. so if DMAC is
	 * broadcast or the tunnel zone is backhaul, we just
	 * encap into the only tunnel.
	 */
	if (!(e->h_dest[0] & 1) || !is_zone_id_access(t->zone_id)) {
		goto tunnel_pak;
	}

	/*
	 * if it's a dhcp discover or request broadcast pkt
	 * do not flood it on other access interfaces
	 */
	if ( get_skb_ipv4_l5_proto(skb) == AHB_L5_PROTO_ID_DHCPV4 &&
		ah_is_dhcp_discover_or_req(skb) == TRUE) {

		goto tunnel_pak;
	}

	mgt_dev = is_mgt_dev(in_dev) ? in_dev : dev2mgt_dev (in_dev);
	mif = mgt_dev ? dev2ahdev(mgt_dev)->mif : NULL;

	if (unlikely(!mif)) {
		//ah_kassert(mif);
		afe_detail_or_drop_dbgk (NULL,
								 "%s no mgt ctxt, drop flood broadcast\n",
								 dev2name (in_dev));

		unlink_mac_sess_on_drop = FALSE;

		drop_idx = AH_DROP_NO_MIF_DEV;

		goto drop_pkt;
	}

	ah_read_lock_bh (&mif->ami_listlock);

	list_for_each_entry (p, &mif->ami_portlist, ahp_list) {

		if (unlikely(p->ahp_dev == NULL) ||
			unlikely(dev2ahdev(p->ahp_dev) == NULL)) {
			continue;
		}

		/*
		 * currently only wireless devices are access devcies
		 * so the check (!is_wireless_dev()) is not really needed
		 * here. But in future, if we want to extend access
		 * interfaces to wired devices as well, this should
		 * handle that case.
		 */
		if (!is_access_dev(p->ahp_dev) ||
			(p->ahp_dev == in_dev &&  !is_wireless_dev(in_dev))) {
			continue;
		}

#if AH_SUPPORT_USEG

		if ( unlikely(ah_useg_enable && !ah_useg_check_multicast_need_flood_in_encap_vector(skb, t, p->ahp_dev))) {

			afe_detail_dbgk (skb, "%s: useg sanity check for multicast: packet skip sent to interface %s\n",
					__FUNCTION__,
					dev2name (p->ahp_dev) );

			continue;
		}

#endif	/* #if AH_SUPPORT_USEG */


		if (AH_SYS_DEV_IS_MEMBER_INTERFACE (dev2ahdev(p->ahp_dev))) {
			afe_detail_dbgk (skb,
							 "%s is member interface, don't flood\n",
							 dev2name (in_dev));
			continue;
		}

		if (!(ah_wrapper_dev_get_flags (p->ahp_dev) & IFF_RUNNING) || !netif_carrier_ok(p->ahp_dev)) {
			afe_detail_dbgk (skb,
							 "interface down, don't flood on %s, flag 0x%x\n", dev2name (p->ahp_dev));
			continue;
		}

		/*
		 * Don't flood broadcast pkts from one SSID to another. Note
		 * that interface check alone is good enough, we also need to
		 * compare the ssid because same SSID may be bound to two
		 * different radios.
		 */
		if (ah_inter_ssid_flood_enabled == FALSE &&
			is_access_dev (in_dev) &&
			(p->ahp_dev != in_dev &&
			 AH_SYS_DEV_GET_SSID (dev2ahdev(in_dev)) != AH_SYS_DEV_GET_SSID (dev2ahdev(p->ahp_dev)))) {

            afe_detail_dbgk (skb,
                      "inter-ssid flood is disabled, don't flood on %s\n", dev2name (p->ahp_dev));
			continue;
		}

		/*
		 * if inter station flood is disabled on incoming or outgoing interface, then don't
		 * flood any pks from a station onto an access interface.
		 */
		if (is_access_dev (p->ahp_dev) &&
			(AH_SYS_DEV_IS_INTER_STATION_TRAFFIC_DISABLE (dev2ahdev(in_dev)) ||
			 AH_SYS_DEV_IS_INTER_STATION_TRAFFIC_DISABLE (dev2ahdev(p->ahp_dev)))) {

			ah_mac_flow_t *r = macflow2peer (f);

			if (is_ah_mac_flow_sta_real (r)) {

				afe_detail_dbgk (skb,
								 "traffic is from station, inter-station traffic is disabled, don't flood on %s\n",
								 dev2name (p->ahp_dev));

				continue;
			}
		}

		if (!(skb2 = ah_wrapper_skb_copy (skb, GFP_ATOMIC)) ) {
			afe_detail_dbgk (skb, "fail copy skb when flood pkt out %s\n",
							 dev2name (p->ahp_dev));
			break;
		}

		afe_detail_dbgk (skb, "broadcast out on access interface %s\n",
						 dev2name (p->ahp_dev));

		ah_dev_forward_pkt (skb2, p->ahp_dev, bcast_mac);
	}

	ah_read_unlock_bh (&mif->ami_listlock);

tunnel_pak:

	/* now encap the pkt into the tunnel */
	ah_encap_with_tunnel (skb, t);

	ahcp_return(encapv);
	return NULL;


drop_pkt:

	set_ah_flow_drop_pak (ah_get_cpu_var(gflow));

	ah_inc_drop_pak_counter ( ah_get_skb_input_dev(skb), drop_idx);

	if ( unlink_mac_sess_on_drop ) {

		ah_fdb_unlink_mac_sess (f, FALSE);
	}

	ahcp_return(encapv);

	return NULL;
}


/*
 * ARP spoof shield, see jira HOS-5298, HOS-5487
 *
 * --- note: we place the shield logic here early in FE ingress pipeline, instead of late in ah_arp_vector() ( which is doing proxy-arp )
 *
 *
 * return 0:   continue pkt processing
 * return !=0: pkt drop
 */
int ah_arp_spoof_shield_proc (struct ah_sk_buff *skb)
{
	int rc = 0;

	const struct ah_ethhdr *e = ah_eth_hdr (skb);
	int arp_op;
	ah_ipaddr_t arp_sip;
	ah_mac_t    arp_smac;
	ah_sta_t            *sta  = NULL;


	if (unlikely(ah_arp_spoof_shield_enabled)) {

		if (is_access_dev(skb->dev)) {

			if (unlikely(ah_arp_parse(skb, &arp_op, &arp_sip, &arp_smac, NULL, NULL) == 0)) {


				/*
				 * MRD HOS-5298
				 *
				 *Implement ARP inspection to ensure:
				 *
				 *      ARP queries, replies, and gratuitous ARP replies have correct MAC and IP addresses
				 * within the ARP frame payload that match the source MAC address in the Wi-Fi frame and
				 * the IP address learned through DHCP snooping
				 */


				/*
				 * #1. check both source MAC of both REQUEST and REPLY
				 */
				if (unlikely(memcmp(e->h_source, &arp_smac, ETH_ALEN) != 0)) {

					afe_detail_or_drop_dbgk (skb, "Detect ARP spoof, on %s, STA %m, invalid ARP %s, eth_smac [%m] != arp_smac [%m]\n",
											 skb->dev->name,
											 e->h_source,
											 (arp_op == ARPOP_REPLY) ? "REPLY" : "REQUEST",
											 e->h_source,
											 &arp_smac
											);


					rc = -1;

					goto out;

				}


				/*
				 * #2. check ARP sender_ip          # for both REQUEST and REPLY
				 *
				 *      let ARP pass for:
				 *
				 *          arp_sender_ip == 0
				 *
				 *          arp_sender_ip == linklocal IPv4 address
				 *
				 *          arp_sender_ip == STA dhcp_ip_addr
				 *
				 #
				 # Note that:
				 *
				 *      #1. MRD only require checking REPLY, but we need also check REQUEST, for the following reason:
				 *
				 *              If not checking REQUEST, then a macilious REQUEST would travel to ah_arp_vector(), then let ARP snoop learn
				 * STA_ip, further to roaming cache.
				 *
				 *              Then if STA roam from AP#1 to AP#2, AP#2 would have STA_ip in its roaming cache, then push to FE "sta->ip_addr" and "sta->dhcp_ip_addr",
				 * Then STA can make its attack on AP #2.
				 *
				 *
				 *
				 *      #2. we have this FE ingress pipeline:
				 *
				 *          ah_sanity_vector()  -> our ARP shield logic
				 *
				 *          ah_arp_vector() -> ARP snoop logic
				 *
				 *      We can assure macilious ARP not to mess up ARP snoop.
				 *
				 *
				 *      #3. See "ah_sta_t->dhcp_ip_addr" for more detail ( such as roaming issue).
				 */

				if (unlikely(arp_sip == 0)) {

					goto out;
				}

				if (unlikely(AH_IS_LINK_LOCAL_ADDR(ntohl(arp_sip)))) {

					goto out;
				}

				if (unlikely(AH_IN_MULTICAST(arp_sip))) {

					afe_detail_or_drop_dbgk (skb, "Detect ARP spoof, on %s, STA %m, invalid ARP %s, arp_sip[%i] is multicast IP address\n",
											 skb->dev->name,
											 e->h_source,
											 (arp_op == ARPOP_REPLY) ? "REPLY" : "REQUEST",
											 arp_sip
											);


					rc = -1;

					goto out;
				}


				sta = ah_get_valid_sta (arp_smac.am_bp, FALSE);
				if (likely(sta)) {

					if (sta->dhcp_ip_addr &&
						arp_sip != sta->dhcp_ip_addr) {

						afe_detail_or_drop_dbgk (skb, "Detect ARP spoof, on %s, STA %m, invalid ARP %s, arp_sip[%i] != STA_dhcp_ip[%i]\n",
												 skb->dev->name,
												 e->h_source,
												 (arp_op == ARPOP_REPLY) ? "REPLY" : "REQUEST",
												 arp_sip,
												 sta->dhcp_ip_addr
												);

						rc = -1;

						goto out;
					}


					/*
					 * Convention: if there is no address for a node interface yet, then all ARP packet should set 0 in ar$spa.
					 *
					 * As RFC 2131 DHCP specify             # after DHCPACK

					            the lease from the DHCPACK message.    The client SHOULD perform a
					       check on the suggested address to ensure that the address is not
					       already in use.  For example, if the client is on a network that
					       supports ARP, the client may issue an ARP request for the suggested
					       request.  When broadcasting an ARP request for the suggested address,
					       the client must fill in its own hardware address as the sender's
					       hardware address, and 0 as the sender's IP address, to avoid
					       confusing ARP caches in other hosts on the same subnet.  If the
					       network address appears to be in use, the client MUST send a
					       DHCPDECLINE message to the server. The client SHOULD broadcast an ARP
					       reply to announce the client's new IP address and clear any outdated
					       ARP cache entries in hosts on the client's subnet.

					 */
					if (!sta->dhcp_ip_addr &&
						arp_sip) {

						afe_detail_or_drop_dbgk (skb,
												 "Detect ARP spoof, on %s, STA %m, invalid ARP %s, arp_sip[%i], but STA_dhcp_ip is not snoop yet, STA is using manual set IP?\n",
												 skb->dev->name,
												 e->h_source,
												 (arp_op == ARPOP_REPLY) ? "REPLY" : "REQUEST",
												 arp_sip
												);

						rc = -1;

						goto out;
					}

				} else {

					afe_detail_or_drop_dbgk (skb, "Detect ARP spoof (possible), on %s, no match STA for eth_smac %m, ARP %s\n",
											 skb->dev->name,
											 e->h_source,
											 (arp_op == ARPOP_REPLY) ? "REPLY" : "REQUEST"
											);

					rc = -1;

					goto out;
				}

			}
		}
	}


out:

	if (rc) {

		//set_ah_flow_drop_pak (ah_get_cpu_var(gflow));      # this statement is run by caller on seeing -1 return value.

		ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_ARP_SPOOF_SHIELD);
	}

	return rc;
}


/* add for IPv6 */

#define EXPECT_ND_MSG_LEN   (sizeof(struct ah_ipv6hdr) + sizeof(struct ah_nd_msg) + sizeof(struct ah_nd_opt_hdr) + ETH_ALEN)

void *FEFASTPATH ah_nd_cache_vector (struct ah_sk_buff *skb)
{
	unsigned char l4_proto = 0;
	int rc, swap_dev_rc = 0;
	struct ah_icmp6hdr *icmp6_hdr = NULL;

	skb_trace_trip(skb);

	l4_proto = get_skb_ipv6_l4_proto(skb);
	if (likely(l4_proto != IPPROTO_ICMPV6)) {

		return flow_next_vector(skb);
	}


	if (unlikely(ah_proxy_nd_enabled == FALSE)) {
		return flow_next_vector(skb);
	}


	icmp6_hdr = ah_get_icmp_from_ipv6_skb(skb);
	if (icmp6_hdr == NULL) {

		return flow_next_vector(skb);
	}

	/* early check. */
	if (icmp6_hdr->icmp6_type != NDISC_NEIGHBOUR_SOLICITATION &&
		icmp6_hdr->icmp6_type != NDISC_NEIGHBOUR_ADVERTISEMENT ) {

		return flow_next_vector(skb);
	}

	/* early length check */
	if (unlikely(skb->len != EXPECT_ND_MSG_LEN)) {

		return flow_next_vector(skb);
	}

	if (icmp6_hdr->icmp6_type == NDISC_NEIGHBOUR_SOLICITATION) {

		rc = ah_nd_process_neighbor_solicitation(skb);

		switch (rc) {

		case AH_ARP_PKT_CHANGED:         /*ARP Request has been modified to ARP Reply packet*/

			afe_detail_dbgk(skb, "proxy-ND, reinject ND reply pak back to flow\n");

			set_arp_reply_from_self(skb);       ////reuse ARP func

			if (is_skb_decap_pak(skb)) {
				/*
				 * clear decap_pak before reinjecting back to FE,
				 * otherwise, fe will try to contiue to process this as a decap packet
				 */
				clr_skb_decap_pak(skb);
			}
			ah_arp_reinject_pak (skb, ah_get_cpu_var(gflow));    //reuse ARP func

			return NULL;

		case AH_ARP_PKT_DROP:            /*This ARP packet should be dropped*/
			afe_detail_or_drop_dbgk(skb, "drop ND SOLICITATION\n");
			set_ah_flow_drop_pak (ah_get_cpu_var(gflow));
			ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_DROP_ARP_REQ);
			return NULL;

		case AH_ARP_PKT_HOST:            /* ARP caching has found the IP is our host IP*/
			afe_detail_dbgk(skb, "ND SOLICITATION is to host\n");

			if (!is_skb_from_self (skb)) {

				if ((swap_dev_rc = ah_swap_dev(skb)) < 0) {
					afe_detail_or_drop_dbgk(skb, "error is dev swap, drop pak\n");
					set_ah_flow_drop_pak (ah_get_cpu_var(gflow));
					if (swap_dev_rc == -2) {
						ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_NO_MATCHING_VLAN);
					} else {
						ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_SWAP_DEV_ERR);
					}
				} else {
					set_ah_flow_to_self (ah_get_cpu_var(gflow));
				}

				return NULL;
			}
			break;

		default:
			afe_detail_dbgk (skb, "ND SOLICITATION return code %d, no action\n", rc);
			break;

		}

	} else if (icmp6_hdr->icmp6_type == NDISC_NEIGHBOUR_ADVERTISEMENT) {

		afe_detail_dbgk(skb, "process ND ADVERTISEMENT\n");

		ah_nd_process_neighbor_advertisement(skb);
	}


	return flow_next_vector(skb);
}



ahctd(reinja, 0x10);
ahctd(arpvector, 0x20);
ahcpd(arpv);
void *FEFASTPATH ah_arp_vector (struct ah_sk_buff *skb)
{
	int rc, swap_dev_rc;
	uint16_t zone;
	struct ah_arphdr *arph = NULL;

	ahcp_start();

	skb_trace_trip(skb);

	if (likely(is_tcp_fastpath(skb))) {	/* TCP fastpath packet is not IPv4 ARP or ICMPv6 ND, so return earily */
		ahcp_return(arpv);
		return flow_next_vector(skb);
	}

	/* return early based on our coarse parsing logic */
	if ( likely(!is_skb_arp(skb) && get_skb_ipv6_l4_proto(skb) != IPPROTO_ICMPV6)) {    /* not IPv4 ARP && not IPv6 ND (ICMPv6)*/
		return flow_next_vector(skb);
	}

	/********************
	 *
	 * redirect to IPv6 ND func
	 *
	 ***********************/
	if (unlikely(is_skb_ipv6(skb))) { /* redirect to IPv6 func */
		return ah_nd_cache_vector(skb);
	}
	/********************
	 *
	 * the following logic about IPv4 ARP
	 *
	 ***********************/


	/*
	 * ARP spoof shield, see jira HOS-5298, HOS-5487
	 *
	 * --- originally, it is in early ah_sanity_vector(), but it need STA table, which is constructed in ah_first_path_vector(),
	 *      so have to place in later ah_arp_vector().
	 */
	if (unlikely(ah_arp_spoof_shield_proc(skb) != 0)) {

		afe_detail_dbgk (skb, "Pkt dropped by if arp spoof shield (if=%s)\n", dev2name(skb->dev));

		set_ah_flow_drop_pak (ah_get_cpu_var(gflow));

		return NULL;
	}


	if (unlikely (ah_proxy_arp_enabled == FALSE)) {
		ah_arp_process_when_proxy_disabled(skb);
		return flow_next_vector(skb);
	}


	if (is_skb_decap_pak(skb)) {
		zone = ah_get_zone_by_tunnel_id(get_skb_tunnel_id(skb));
		ahct(arpvector[2]);
	}


	arph = (struct ah_arphdr *)skb_network_header(skb);

	if (arph->ar_op == __constant_htons (ARPOP_REQUEST)) {

		rc = ah_arp_process_request (skb);

		switch (rc) {

		case AH_ARP_PKT_CHANGED:         /*ARP Request has been modified to ARP Reply packet*/

			ahct(arpvector[4]);

            afe_detail_dbgk(skb, "proxy-ARP, reinject ARP reply pak back to flow\n");
				/* catch you Sreekanth, copy&free? show do copy only when needed! ykang */            

			set_arp_reply_from_self(skb);

			if (is_skb_decap_pak(skb)) {
				/*
				 * clear decap_pak before reinjecting back to FE,
				 * otherwise, fe will try to contiue to process this as a decap packet
				 */
				ahct(arpvector[6]);
				clr_skb_decap_pak(skb);
			}
			ah_arp_reinject_pak (skb, ah_get_cpu_var(gflow));
			ahcp_return(arpv);
			return NULL;

		case AH_ARP_PKT_DROP:            /*This ARP packet should be dropped*/
			afe_detail_or_drop_dbgk(skb, "drop ARP REQ\n");
			set_ah_flow_drop_pak (ah_get_cpu_var(gflow));
			ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_DROP_ARP_REQ);
			ahcp_return(arpv);
			return NULL;

		case AH_ARP_PKT_HOST:            /* ARP caching has found the IP is our host IP*/
			afe_detail_dbgk(skb, "ARP REQ is to host\n");

			if (!is_skb_from_self (skb)) {

				if ((swap_dev_rc = ah_swap_dev(skb)) < 0) {
					afe_detail_or_drop_dbgk(skb, "error is dev swap, drop pak\n");
					set_ah_flow_drop_pak (ah_get_cpu_var(gflow));
					if (swap_dev_rc == -2) {
						ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_NO_MATCHING_VLAN);
					} else {
						ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_SWAP_DEV_ERR);
					}
				} else {
					set_ah_flow_to_self (ah_get_cpu_var(gflow));
				}

				ahcp_return(arpv);
				return NULL;
			}
			break;

		default:
			afe_detail_dbgk (skb, "ARP REQ return code %d, no action\n", rc);
			break;

		}

	}
	else if (arph->ar_op == __constant_htons (ARPOP_REPLY)) {
       afe_detail_dbgk(skb, "process ARP reply\n");
		ah_arp_process_reply(skb);

	}

	ahcp_return(arpv);
	return flow_next_vector(skb);
}


/* add for IPv6 */

void *FEFASTPATH ah_ipv6_misc_vector (struct ah_sk_buff *skb)
{
	skb_trace_trip(skb);

	if (likely(is_tcp_fastpath(skb))) {	/* logic of ipv6_misc_vector() does not handle TCP packet, so return early */

		return flow_next_vector(skb);
	}

	if (likely (skb->protocol != __constant_htons(ETH_P_IPV6))) {

		return flow_next_vector(skb);
	}


	if (ah_ipv6_snoop_guard(skb) < 0) {

		set_ah_flow_drop_pak(ah_get_cpu_var(gflow));

		return NULL;
	}


	return flow_next_vector(skb);

}



static inline void ah_init_global_sess (ah_mac_sess_t *s)
{
	ah_mac_flow_t *f = &s->f_flow;

	memset(s, 0, sizeof(ah_mac_sess_t));

	rwlock_init (&s->mac_sess_rw_lock);

	set_ah_mac_flow_forward (f);
}

void ah_bypass_fe (struct ah_sk_buff *skb)
{
#if defined(AH_SUPPORT_FE_VCON)
	struct ah_abs_net_device *out_dev = NULL;
	int if_index;

	if (likely (((if_index = ah_get_vwire_interface_peer (skb->dev->ifindex)) >= 0) &&
				(out_dev = ah_dev_get_by_index (if_index)))) {

		ah_dev_forward_pkt (skb, out_dev, ah_vwire_nhop);
		dev_put(out_dev);

	} else  {

		afe_detail_dbgk (NULL, "no vwire interface, drop pkt\n");

	}
#endif
}



static inline void ah_mark_tcp_fastpath(struct ah_sk_buff *skb)
{
	ah_sys_dev_t        *ah_dev;

	/*
	 * if the pkt arrived on member interface, change it's
	 * incoming interface to parent interface
	 */

	if (likely( get_skb_ipv46_l4_proto(skb) == IPPROTO_TCP &&	/* TCP over IPv4 or IPv6 */
				!is_skb_ipv46_fragment(skb) ) ) {				/* Not a IPv4 or IPv6 fragment */

			ah_dev = dev2ahdev(skb->dev);

			if (likely(ah_dev->ah_port &&
					   !AH_SYS_DEV_IS_MEMBER_INTERFACE(ah_dev) &&
					   skb->len && skb->data && skb->pkt_type != PACKET_LOOPBACK &&
					   !is_skb_decap_pak(skb))) {

				set_tcp_fastpath(skb);
				return;
			}
	}
	clr_tcp_fastpath(skb);
}

ahcpd(run_vectors);
static inline void ah_run_vectors(struct ah_sk_buff *skb)
{
	ah_flow_vector_t *v, *tv;

	ahcp_start();

	for (v = flow_fast_vector_list, skb2flow_vector_ptr(skb) = (void *)v; v && *v; ) {
		tv = (ah_flow_vector_t *) (*v)(skb);
		v = tv;
	}

	ahcp_return(run_vectors);
}


/* compact conditions of ah_if_rate_limit_proc_fastpath() */
static inline boolean ah_if_rate_limit_proc_fastpath__early_check(ah_sys_dev_t *ah_dev)
{
	ushort      enable = ah_dev->qos.priv->if_rate.enable;

	if (enable & AH_IF_RATE_LIMIT_UCAST_ENABLE ) {
		return TRUE;
	}

	return FALSE;
}

/*
 * return 0:   continue pkt processing
 * return !=0: pkt drop due to interface-based rate limiting
 */
int ah_if_rate_limit_proc_fastpath (struct ah_sk_buff *skb, struct ah_ethhdr *e)
{
	ah_if_rate_limit_config_t *rate_ptr;

	rate_ptr = &((dev2ahdev(skb->dev)->qos.priv)->if_rate);

	if (likely(!(rate_ptr->enable & AH_IF_RATE_LIMIT_UCAST_ENABLE)
			   || rate_ptr->ah_ucast_plc == NULL)) {
		return 0;
	}

	if (likely(AH_BUCKET_TOKEN_ALLOW(rate_ptr->ah_ucast_plc, skb->len))) {
		AH_BUCKET_TOKEN_UPDATE(rate_ptr->ah_ucast_plc, skb->len);
		return 0;
	}

		afe_detail_dbgk(skb,
						"%m->%m Unicast pkt dropped due to eth0 rate limiting\n",
						e->h_source, e->h_dest);

	return 1;
}


static inline boolean ah_if_rate_limit_proc__early_check(ah_sys_dev_t *ah_dev, struct ah_ethhdr *e)	/* we don't need skb arguments. */
{
	/*
	 * rate limit is only for ethX/agg0/red0
	 *
	 * see:
	 *		
	 *		CLI "interface <ethx|aggx|redx> rate-limit [options] enable"
	 *
	 *		ah_config_default_rate_limit()		# and UC is not rate limiting by default.
	 *
	 * 
	 * we do early check if necessary.
	 */

	ushort      enable = ah_dev->qos.priv->if_rate.enable;


	if (likely( !(e->h_dest[0] & 0x1) && 						/* uc packet */
				!(enable & AH_IF_RATE_LIMIT_UCAST_ENABLE) )) {	/* uc rate limiting not enabled by default */

		return FALSE;
	}

	return TRUE;
}


/*
 * return 0:   continue pkt processing
 * return !=0: pkt drop due to interface-based rate limiting
 */
static int ah_if_rate_limit_proc (struct ah_sk_buff *skb, struct ah_ethhdr *e)
{
	ah_if_rate_limit_config_t *rate_ptr;

	rate_ptr = &((dev2ahdev(skb->dev)->qos.priv)->if_rate);
	if (rate_ptr->enable == 0) {
		return 0;
	}

	if (likely(!(e->h_dest[0] & 0x1))) {
		/* unicast traffic */
		if (ah_if_rate_limit_proc_fastpath(skb, e) == 0) {
			return 0;
		}
	} else if (e->h_dest[0] == AH_IF_RATE_LIMIT_BCAST_ADDR) {
		/* broadcast traffic */
		if (!(rate_ptr->enable & AH_IF_RATE_LIMIT_BCAST_ENABLE)
			|| rate_ptr->ah_bcast_plc == NULL) {
			return 0;
		}

		if (AH_BUCKET_TOKEN_ALLOW(rate_ptr->ah_bcast_plc, skb->len)) {
			AH_BUCKET_TOKEN_UPDATE(rate_ptr->ah_bcast_plc, skb->len);

			return 0;
		} else {
			/* pkt got dropped due to interface-based rate limiting */
			afe_detail_dbgk(skb,
							"%m->%m Broadcast pkt dropped due to rate limiting\n",
							e->h_source, e->h_dest);
		}

	} else {
		/* multicast traffic */
		if (!(rate_ptr->enable & AH_IF_RATE_LIMIT_MCAST_ENABLE)
			|| rate_ptr->ah_mcast_plc == NULL) {
			return 0;
		}

		if (AH_BUCKET_TOKEN_ALLOW(rate_ptr->ah_mcast_plc, skb->len)) {
			AH_BUCKET_TOKEN_UPDATE(rate_ptr->ah_mcast_plc, skb->len);

			return 0;
		} else {
			/* pkt got dropped due to interface-based rate limiting */
			afe_detail_dbgk(skb,
							"%m->%m Multicast pkt dropped due to rate limiting\n",
							e->h_source, e->h_dest);
		}
	}

	/* pkt will be dropped */
	AH_QOS_DBG_DROP("if rate limit: pkt dropped %m->%m dev=%s\n",
					e->h_source, e->h_dest, skb->dev->name);
	return 1;
}



/**
 * @brief determine early if FE should drop an ingress packet early in ingress pipline, without forwarding it or upload it to TCP/IP stack.
 * @param[in] skb the ingress packet
 * @return non-zero for drop, and the value also as drop counter index, zero for not drop and let the packet pass.
 * @note the determination is based on packet metadata parsed from ah_ingress_parse_ipv4_pak() and etc, therefore this function must be
 *      called after those parsing functions.
 */
static int ah_ingress_pkt_should_drop_early(struct ah_sk_buff *skb)
{
    int drop_idx = 0;

    /* If we are not bounjour gateway( we are client ), then drop any Bonjour MDNS packet with TTL<=1, not flooding them and upload to TCP/IP stack */
	/* If we are bounjour gateway, we still need to flooding all bonjour packets ( but just not upload to TCP/IP stack, this check would be done later when flow_to_self )*/
    if (unlikely(!ah_bonjour_gateway_enabled && get_skb_ipv46_l5_proto(skb) == AHB_L5_PROTO_ID_BONJOUR)) {     //get_skb_ip46_XXX() is common for ipv4 and ipv6

        if (is_skb_ipv4(skb)) {

            if (ip_hdr(skb)->ttl <= 1) {
                drop_idx = AH_DROP_BONJOUR_TTL_1_PACKET;
                goto out;
            }
        }
        else {      //we dont need to check is_skb_ipv6(skb), because if bonjour, it must be ipv4 or ipv6. then omit one branch instruction....

            if (ipv6_hdr(skb)->hop_limit <= 1) {
                drop_idx = AH_DROP_BONJOUR_TTL_1_PACKET;
                goto out;
            }
        } 
    }

out:
    return drop_idx;
}


/**
 * @brief determine early if FE early drop an ingress packet flow to self (upload it to TCP/IP stack).
 * @param[in] skb the ingress packet
 * @return non-zero for drop, and the value also as drop counter index, zero for not drop and let the packet pass.
 * @note the determination is based on packet metadata parsed from ah_ingress_parse_ipv4_pak() and etc, therefore this function must be
 *      called after those parsing functions.
 */
static int ah_ingress_pkt_should_drop_early_flow_to_self(struct ah_sk_buff *skb)
{
    int drop_idx = 0;

    /* either we are bounjour client or gateway, we don't handle Bonjour MDNS packet with TTL<=1 at all */
    if (unlikely(get_skb_ipv46_l5_proto(skb) == AHB_L5_PROTO_ID_BONJOUR)) {     //get_skb_ip46_XXX() is common for ipv4 and ipv6

        if (is_skb_ipv4(skb)) {

            if (ip_hdr(skb)->ttl <= 1) {
                drop_idx = AH_DROP_BONJOUR_TTL_1_PACKET;
                goto out;
            }
        }
        else {      //we dont need to check is_skb_ipv6(skb), because if bonjour, it must be ipv4 or ipv6. then omit one branch instruction....

            if (ipv6_hdr(skb)->hop_limit <= 1) {
                drop_idx = AH_DROP_BONJOUR_TTL_1_PACKET;
                goto out;
            }
        } 
    }

out:
    return drop_idx;
}


#ifdef CONFIG_NETFILTER
static void ah_set_skb_upid_in_fw_mark_for_netfilter(struct ah_sk_buff *skb)
{
			/* set upid in skb fw mark */
			ah_mac_flow_t *r = NULL;
			if (likely(skb2macflow(skb))) {
				r = macflow2peer(skb2macflow(skb));
			}

			if (likely(r)) {
				ah_set_skb_fw_mark(skb, ah_user_profile_attribute2id(r->upid));
			} else {
				ah_set_skb_fw_mark(skb, AH_USER_PROFILE_ENTRY_MAX);
			}

#ifdef AH_SUPPORT_MULTIWAN
			/*
			 * The following logic is to support policy based routing
			 * based on incoming LAN interface for ethX or pX
			 */
			if (is_access_dev((struct ah_abs_net_device *)ah_get_skb_input_dev(skb))) {
				if (!strncmp(((struct ah_abs_net_device *)ah_get_skb_input_dev(skb))->name,
							 AH_IF_NETIF_PREFIX,
							 strlen(AH_IF_NETIF_PREFIX))) {
					int iifname_idx;
					int num_scanned_items = 0;

					num_scanned_items = sscanf(((struct ah_abs_net_device *)ah_get_skb_input_dev(skb))->name,
											   AH_IF_NETIF_PREFIX"%d",
											   &iifname_idx);
					if ((num_scanned_items == 1) &&
						(iifname_idx <= AH_MAX_IF)) {
						ah_set_skb_fw_mark_ifnameidx(skb, (iifname_idx + 1));
					}
				}
			}
#endif

			afe_detail_dbgk(skb, "deliver pak to self on %s input %s with fw mark 0x%x\n",
							dev2name(skb->dev), ((struct ah_abs_net_device *)ah_get_skb_input_dev(skb))->name, ah_get_skb_fw_mark(skb));
}
#endif	/* #ifdef CONFIG_NETFILTER */

#ifdef AH_FWD_STATS
uint32_t fe_thrd_drop = 0;
EXPORT_SYMBOL(fe_thrd_drop);
#endif
/*
 * Descri: this routine is called within netif_receive_skb.
 *         return 1 will cause netif_receive_skb() to return,
 *         and we take over the skb.
 *         return 0 will cause linux keep process this skb
 *         as if we are not here.
 *
 * Return: 0 if skb will be continue processed by Linux.
 *     1 if skb is taken by aerohive forwarding engine.
 *
 * NOTICE: (1). when skb arrive at netif_receive_skb(), driver
 *              MUST have pulled skb->data right after Ethernet
 *              header, and have remember it at skb->mac.raw,
 *              which can be retrieved by ah_eth_hdr(skb). Our code
 *              count on this to be correct!
 *         (2). ah_rcu_read_lock() has been called right before
 *              enter this routine.
 */
ahctd(pktcatch, 0x30);
ahcpd(pktcatch);

#define PRINTFLOWCONTEXT()
static inline void fun5_(void);
static        void (*fun5)(void) = &fun5_;
static inline void fun5_(void) {
    PRINTFLOWCONTEXT();
    return;
}
static inline void fun4_(void);
static        void (*fun4)(void) = &fun4_;
static inline void fun4_(void) {
    PRINTFLOWCONTEXT();
    fun5();
}
static void fun3_(void) {
    PRINTFLOWCONTEXT();
    fun4();
} void (*fun3)(void) = &fun3_;
void fun2(void) {
    PRINTFLOWCONTEXT();
    fun3();
}
void fun1(void) {
    PRINTFLOWCONTEXT();
    fun2();
}
void fun0(void) {
    PRINTFLOWCONTEXT();
    fun1();
}
#include <linux/spinlock_types.h>
extern spinlock_t __fcon_caller_func_spinlock;
extern unsigned int __fcon_logdepth;
extern char __fcon_caller_func_str[200];
extern int trigarr[100];

int FEFASTPATH ah_ingress_pkt_catch (struct ah_sk_buff *skb)
{
	ah_buff_t *ahb = skb2ahb(skb);
	int swap_dev_rc, rc = 1;
	const struct ah_ethhdr *e;
	struct ah_iphdr        *ip;
	int early_drop_idx;

	ah_mgt_if_t *mif;

	ahcp_start();
    ahct(pktcatch[0]);

	spin_lock(&__fcon_caller_func_spinlock);
	if (trigarr[5] /*&& ah_ff_match(skb, skb->dev, NULL)*/) {   // manual trigger once
	    __fcon_logdepth = 1;
	    sprintf(__fcon_caller_func_str, "%s [fe]", __func__); // sup:

	    __pkt_dbg_decode(skb, DIR_INCOMING, skb->dev);
	    trigarr[5] = 0; // manual trigger reset
	}
	spin_unlock(&__fcon_caller_func_spinlock);

	fun0();

	skb_trace_trip(skb);
#ifdef AH_FWD_STATS
    if (fe_thrd_drop) {
        fe_qos_counters.ah_fe_input_drop_cnt++;
        ah_kfree_skb(skb);
		return 1;
    }
#endif

	/* protect route, policy, dev changes with this lock */
	ah_read_lock_bh (&ah_fe_global_lock);
	//this is to protect against unlist_netdevice() which could
	//make the if index recorded in skb->skb_iif invalid
	ah_read_lock(_ah_dev_base_lock);



do_flow_again:


	if ((skb->dev != NULL) &&
		(skb->dev->ah_dev) &&
		(AH_SYS_DEV_ST_IS_DELETED((ah_sys_dev_t *)skb->dev->ah_dev))) {

		ah_kfree_skb(skb);

		goto end_flow_loop;    // this case rc = 1;
	}


	if (likely (!is_ahb_valid(ahb))) {
		init_ahb_values(skb);
	}


#if AH_SUPPORT_USEG
	ah_useg_init_metadata(ahb);
#endif


	/*
	 * Note that, don't put this into the "if (likely (!is_ahb_valid(ahb)))" above. we need to parse packet every "do_flow_again",
	 * because for fragment packet, the following trace would break our previous parsing result:
	 *
	 *          ah_frag_reinject_pak()
	 *              init_ahb_values(skb);
	 *
	 *
	 * In fact, we can have:
	 *
	 *          if ( !is_skb_reinject(skb)) ) {
	 *              ah_ingress_parse_pak(skb);
	 *          }
	 *
	 * But reinjected packets are minor, no bother parse it again.
	 *
	 * [*][*] Besides If fragments ( or other reinjection ) happens, the network performance is dragged down anyway.
	 */
	ah_parse_pak(skb);


	ah_mark_tcp_fastpath(skb);


#if AH_SUPPORT_DHCP_OPT_82
	if ( unlikely( ah_dhcp_opt82_enabled &&
					!is_tcp_fastpath(skb) ) ) {		/* TCP fastpath packet is not DHCP, no need to call into function */

		skb = ah_dhcp_opt82_ingress_proc(skb, skb->dev);

		if (skb == NULL) {

			goto end_flow_loop;
		}
	}
#endif	/* #if AH_SUPPORT_DHCP_OPT_82 */


	if (unlikely (!is_tcp_fastpath(skb) &&		/* TCP fastpath don't go through drop early check */
				(early_drop_idx = ah_ingress_pkt_should_drop_early(skb)) != 0)) {

		ah_inc_drop_pak_counter (ah_get_skb_input_dev(skb), early_drop_idx);

		ah_kfree_skb(skb);
		goto end_flow_loop;    // this case rc = 1;
	}

	if (unlikely(is_wan_dev(skb->dev))) {
		/* bypass fe procesing and deliver WAN pak directly to stack */
#if  defined(AH_SUPPORT_SDWAN_FE_PATCH)
		set_skb_br_wan_in(skb, skb->dev);
#endif
		/*
		 * mark skb to self
		 */
		//skb->pkt_type = PACKET_HOST;
		set_skb_dbg_flag(skb);
		pkt_decode(skb, DIR_INCOMING, skb->dev);
		afe_detail_dbgk (skb, "bypass fe ingress procesing and deliver packet to stack on %s\n", dev2name(skb->dev));

		/* clear skb cb before sending to stack */
		//memset(skb->cb, 0, sizeof(skb->cb));
		rc = 0;
		goto end_flow_loop;
	}
#if defined(AH_SUPPORT_SDWAN_FE_PATCH)
	if (is_access_dev(skb->dev)) {
		set_skb_br_lan_in(skb, skb->dev);	/* we need check and set early here, otherwise ah_swap_dev() would assign skb->dev to mgt0.X*/	
	}
#endif
	/* TODO: global session overhead is big, need to optimize it */
	ah_init_global_sess (&ah_get_cpu_var(gsess));

	ah_get_cpu_var(gflow) = &(ah_get_cpu_var(gsess).f_flow);
	skb2macflow(skb) = (void *)ah_get_cpu_var(gflow);


	ah_run_vectors(skb);


	/* remove the route links of global flow (if any) */
	ah_fdb_unlink_mac_sess_global (ah_get_cpu_var(gflow));
	ah_fdb_unlink_mac_sess_global (macflow2peer(ah_get_cpu_var(gflow)));

	/****** IMPORTANT NOTE ****************/
	/*
	 * all the flags that we check here that are used
	 * in modifying skb, like free skb etc, need to be
	 * cleared in ah_encap_vector() in the macro
	 * clr_ah_flow_main_loop_flags(). Otherwise, pkt will
	 * get sent out directly from ah_encap_vector() and
	 * if we try to free the skb here, it will end up
	 * as skb double free.
	 */

	if (unlikely (is_ah_flow_virtual_wire (ah_get_cpu_var(gflow)))) {
		goto end_flow_loop;
	}
	if (unlikely (is_ah_flow_reinject_pak (ah_get_cpu_var(gflow)))) {
		ahct(pktcatch[4]);
		goto do_flow_again;
	}

	/* for pkts that are marked "skip host", don't deliver to stack */
	if (unlikely (is_ah_flow_skip_self (ah_get_cpu_var(gflow)))) {
		ahct(pktcatch[6]);
		ah_kfree_skb(skb);
		goto end_flow_loop;
	}

	if (unlikely (is_ah_flow_drop_pak (ah_get_cpu_var(gflow)))) {
		ahct(pktcatch[20]);
		pkt_decode(skb, DIR_DROPPKT, skb->dev);
		ah_kfree_skb(skb);
		goto end_flow_loop;
	}

	if (unlikely (is_ah_flow_to_self (ah_get_cpu_var(gflow)))) {

		/* check if the dst-ip is valid */
		ahct(pktcatch[10]);

		/* check if need to drop this ingress packet early */
        if (unlikely ((early_drop_idx = ah_ingress_pkt_should_drop_early_flow_to_self(skb)) != 0)) {
		
			ah_inc_drop_pak_counter (ah_get_skb_input_dev(skb), early_drop_idx);
		
			ah_kfree_skb(skb);
			goto end_flow_loop;    // this case rc = 1;
		}


		if (is_ah_skb_vrrp_pkt(skb)) {
			afe_detail_dbgk(skb, "vrrp pak to self on %s\n", dev2name(skb->dev));
			if ((swap_dev_rc = ah_swap_dev(skb)) < 0) {
				afe_detail_dbgk(skb, "vrrp pkt interface can't be changed, rc=%d.\n", swap_dev_rc);
				pkt_decode(skb, DIR_DROPPKT, skb->dev);
				ah_kfree_skb(skb);
				goto end_flow_loop;
			}
		}

		if (!is_skb_loopback_pak(skb) &&
			ah_validate_dst_ip__early_check(skb) &&
			(ah_validate_dst_ip(skb) < 0)) {

			e = ah_eth_hdr (skb);
			if (unlikely (!(e->h_dest[0] & 0x1))) {
				ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_INVALID_TO_SELF_DST_IP);
			}

			ah_kfree_skb(skb);
			goto end_flow_loop;
		}

		if (is_skb_from_self(skb)) {

			uint16_t etype = pkt_get_eth_type(skb);

			e = ah_eth_hdr (skb);

			ah_logk(AH_SYS_KMOD_FE, AH_LOG_ERR,
					"self packet to be delivered to self? smac %m, dmac %m etype %d, incoming dev %s, dev %s, unused2 %d, pkt_type %d is_mgt_dev %d\n",
					e->h_source, e->h_dest, etype,
					dev2name((struct ah_abs_net_device *)ah_get_skb_input_dev(skb)), dev2name(skb->dev),
					skb2ahb(skb)->unused2, skb->pkt_type, is_mgt_dev(skb->dev));

			if (likely (etype == ETH_P_IP)) {
				ip = (struct ah_iphdr *)(e + 1);
				ah_logk(AH_SYS_KMOD_FE, AH_LOG_ERR,
						"src-ip %i dst-ip %i protocol %d",
						ip->saddr, ip->daddr, ip->protocol);
			}

			ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_PAK_FROM_SELF_LOOP);
			ah_kfree_skb(skb);
			goto end_flow_loop;
		}

		if (!is_skb_loopback_pak(skb) &&
			ah_validate_mng_pkt__early_check(skb) &&
			(ah_validate_mng_pkt (skb) < 0)) {

			ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_INVALID_TO_SELF_PROTOCOL);
			ah_kfree_skb(skb);
			goto end_flow_loop;
		}

		/* demux the udp pkts to right applications */
		if (!is_skb_loopback_pak (skb)&&
			get_skb_ipv46_l4_proto(skb) == IPPROTO_UDP ) {

			ah_demux_udp_pkt (skb);
		}

		/* increment counters for mgt0 or mgt0.x, vlan interface */
		if (is_mgt_dev(skb->dev) || is_mgt_sub_dev(skb->dev)) {
			mif = dev2ahdev(skb->dev)->mif;
			if (likely(mif)) {
				mif->ami_stats.rx_packets++;
				mif->ami_stats.rx_bytes += skb->len;
			}
		}

#ifdef CONFIG_NETFILTER
#if defined(AH_DYNAMIC_BYPASS_NETFILTER)

		if ( ah_raw_nf_enable_count != 0 )
#endif
			ah_set_skb_upid_in_fw_mark_for_netfilter(skb);
#endif


		afe_detail_dbgk(skb, "deliver pak to self on %s\n", dev2name(skb->dev));


		/*
		 * mark skb to self
		 */
		skb->pkt_type = PACKET_HOST;
		pkt_decode(skb, DIR_UPSTACK, skb->dev);
		ah_update_counters (skb, DIR_UPSTACK);

		/* clear skb cb before sending to stack */
		memset(skb->cb, 0, sizeof(skb->cb));
		rc = 0;
	}

end_flow_loop:

	/* protect route changes with this lock */
    ah_read_unlock(_ah_dev_base_lock);
	ah_read_unlock_bh (&ah_fe_global_lock);

    ahct(pktcatch[8]);
	ahcp_return(pktcatch);
	return rc;
}



/*
 * DHCP spoof shield, see jira HOS-5298, HOS-5486
 *
 *
 * return 0:   continue pkt processing
 * return !=0: pkt drop
 */
int ah_dhcp_spoof_shield_proc (struct ah_sk_buff *skb)
{
	int rc = 0;
	struct ah_ethhdr *e;
	boolean dhcp_s2c, dhcp_c2s;

	/* return early based on our coarse parsing logic */
	if ( likely(get_skb_ipv4_l5_proto(skb) != AHB_L5_PROTO_ID_DHCPV4)) {
		return 0;
	}

	if (likely(ah_dhcp_spoof_shield_enabled)) {

		if (is_access_dev(skb->dev)) {

			if (unlikely(ah_get_dhcp_direction(skb, &dhcp_s2c, &dhcp_c2s) == 0)) {

				if (unlikely(dhcp_s2c)) {

					e = ah_eth_hdr (skb);

					afe_detail_or_drop_dbgk (skb, "Detect DHCP spoof, DHCP response sent from access interface [%s] by STA %m\n",
											 skb->dev->name,
											 e->h_source);

					rc = -1;    //drop me.

					goto out;
				}

			}

		}

	}


out:

	if (rc) {

		//set_ah_flow_drop_pak (ah_get_cpu_var(gflow));      # this statement is run by caller on seeing -1 return value.

		ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_DHCPV4_SPOOF_SHIELD);
	}

	return rc;
}

/**
 * @brief send a multicast prune event
 * @param[in] skb packet sk_buff data structure
 * @param[in] f forward MAC flow
 * @param[in] r reverse MAC flow
 * @return 0 for success, others for failure
 */
int ah_send_mcast_prune_event (struct ah_sk_buff *skb, ah_mac_flow_t *f, ah_mac_flow_t *r)
{
    ah_mcast_prune_event_t mcast_prune_event;

    mcast_prune_event.ifindex = f->iif;
    memcpy(mcast_prune_event.src_mac, f->src_mac, ETH_ALEN);

    if (is_wmmode_dev(skb->dev)) {
        memcpy(mcast_prune_event.wifi_sender_mac, r->nhop, ETH_ALEN); 
    } else {
        memset(mcast_prune_event.wifi_sender_mac, 0, ETH_ALEN);
    }

    return (ah_kevent_send(AH_KEVENT_MCAST_PRUNE, &mcast_prune_event,
                           sizeof(ah_mcast_prune_event_t)));
}

static inline int ah_per_pkt_route_lookup_check (struct ah_sk_buff *skb, ah_mac_flow_t *f)
{
    ah_buff_t *ahb;
    ah_mac_flow_t *r;
    ah_mac_sess_t *s;
    int rc = 0;

    if (unlikely (is_ah_flow_dst_broadcast(f))) {

        r = macflow2peer (f);
        s = macflow2sess (f);

        ah_read_lock_bh (&s->mac_sess_rw_lock);

        if (r->out_dev && is_ah_flow_to_self (r) && !is_skb_from_self (skb)) {

            afe_detail_or_drop_dbgk (skb, "%s drop self broadcast from %m\n",
                                     dev2name(skb->dev), f->src_mac);
            rc = AH_ROUTE_DROP_PAK;

        } else if (is_wmmode_dev (skb->dev) &&
                   (ahb = skb2ahb(skb)) &&
                   ah_compare_ether_addr(ahb->ab_phop, r->nhop)) {

            afe_detail_or_drop_dbgk (skb, "%s drop illegal broadcast from %m, route-nhop %m\n",
                                     dev2name(skb->dev), ahb->ab_phop, r->nhop);
            rc = AH_ROUTE_DROP_PAK;

        }

        if (amrp_has_wifi_neighbor && (ah_mcast_prune_threshold != 0)) {
            f->mcast_count++;

            if (fe_seconds_tic >= (f->mcast_last_timestamp + AH_MCAST_PRUNE_INTERVAL)) {
                afe_detail_dbgk(skb, "timestamp changed for mac-sess id %u, src-mac %m count %u, fe_tic %u, mcast_timestamp %u!\n",
                                s->id, f->src_mac, f->mcast_count, fe_seconds_tic,
                                f->mcast_last_timestamp);

                if (f->mcast_count >= ah_mcast_prune_threshold) {
                    afe_detail_dbgk(NULL, "send prune event for src-mac %m count %lu, ah_mcast_prune_threshold %d\n", f->src_mac, f->mcast_count, ah_mcast_prune_threshold);
                    ah_send_mcast_prune_event(skb, f, r);
                }

                f->mcast_count = 0;
                f->mcast_last_timestamp = fe_seconds_tic;
            }
        }

        ah_read_unlock_bh (&s->mac_sess_rw_lock);
    }


    /*
     * note that there's no need to do reverse check for non backahul wireless pkts,
     * in that case we check only the interface and that's already done when the
     * session is being setup.
     */

    return rc;
}

ahcpd(sanityv);
void *FEFASTPATH ah_sanity_vector (struct ah_sk_buff *skb)
{
	int rc;
	struct ah_abs_net_device *parent_dev;
	struct ah_ethhdr *e = ah_eth_hdr (skb);
	ah_sys_dev_t *ah_dev = dev2ahdev(skb->dev);
	boolean probe;
	int drop_idx = AH_DROP_PAK_ERR;
#if defined(CONFIG_AP550) || defined(CONFIG_BMW)
    struct ah_abs_net_device *pse_dev;
#endif

	ahcp_start();

	skb_trace_trip(skb);

	e = ah_eth_hdr (skb);

	if (is_tcp_fastpath(skb)) {

		ingress_init_ah_buff_tcp_fastpath(skb);

		pkt_decode(skb, is_skb_reinject(skb) ? DIR_REINJECT : DIR_INCOMING,
				   skb->dev);



		if (unlikely( ah_if_rate_limit_proc_fastpath__early_check(ah_dev) && 
						ah_if_rate_limit_proc_fastpath(skb, e))) {

			drop_idx = AH_DROP_NO_MEMORY;	//no memory to handle that many packets.

			goto drop_pkt;
		}

	} else {

		/*
		 * sanity check
		 */
		rc = is_good_skb (skb, &drop_idx);

		if (rc == 0) {	/* most skb is good, should if (rc == 0) should put at first. */

			/*
			 * code reach here, we will process this pkt
			 */
			ingress_init_ah_buff(skb);

			if (unlikely (is_skb_decap_pak (skb))) {

				if (unlikely (ah_is_valid_tunnel_seq_num (get_skb_tunnel_seq(skb), get_skb_tunnel_id(skb)) == 0)) {

					afe_detail_or_drop_dbgk (skb, "no tunnel (tid %d) or tunnel seq number (%d) mismatch for decap pkt\n",
											 get_skb_tunnel_id(skb), get_skb_tunnel_seq(skb));


					drop_idx = AH_DROP_DECAP_ERR;

					goto drop_pkt;
				}

				clr_skb_bypass_xps(skb);	/* tunnel decap pkt should not bypass XPS. */
			}

			ah_dev = dev2ahdev(skb->dev);

			parent_dev = NULL;
			/*
			 * if the pkt arrived on member interface, change it's
			 * incoming interface to parent interface
			 */
			if (unlikely (AH_SYS_DEV_IS_MEMBER_INTERFACE (ah_dev))) {

				parent_dev = AH_GET_PARENT_DEVICE(ah_dev);

				if (!parent_dev) {
					pkt_decode(skb, DIR_DROPPKT, skb->dev);
					afe_detail_or_drop_dbgk (NULL, "parent interface not found for [%s]\n", dev2name(skb->dev));

					drop_idx = AH_DROP_PARENT_INTERFACE_NOT_FOUND;

					goto drop_pkt;
				}

			}

			pkt_decode(skb, is_skb_reinject(skb) ? DIR_REINJECT : DIR_INCOMING, parent_dev ? parent_dev : skb->dev);


			if (unlikely (parent_dev)) {


				/* if both member interfaces are active, for red0, accept pkts only on primary */
				if (AH_IS_DEVICE_RED0(dev2ahdev(parent_dev)) &&
					dev2ahdev(parent_dev)->mem_dev_cnt > 1 &&
					dev2ahdev(parent_dev)->active_mem_dev_cnt > 1 &&
					!AH_SYS_DEV_IS_PRIMARY_MEMBER_INTERFACE(dev2ahdev(skb->dev))) {
#if defined(CONFIG_AP550) || defined(CONFIG_BMW)
                    /* HOS-15315: To make sure get correct power state from POE switch,
                     * receive LLDP packet on no-primrary interface if the interface is power supply port
                     */
                    if (unlikely(   (is_ah_lldp_pkt(e) || is_ah_cdp_pkt (e))
                                 && (pse_dev = ah_get_supply_member_dev_from_redx(parent_dev))
                                 && (pse_dev == skb->dev) )) {
                       afe_detail_dbgk(skb, "Red: recv lldp pkt on interface %s power supply port\n", pse_dev->name);
                    } else {
#endif
					afe_detail_or_drop_dbgk (NULL, "drop broadcast pkt on non-primrary interface %s\n", dev2name(skb->dev));

					drop_idx = AH_DROP_BROADCAST_ON_NON_PRIMARY_INTERFACE;

					goto drop_pkt;
#if defined(CONFIG_AP550) || defined(CONFIG_BMW)
                    }
#endif
				}

				skb->dev = parent_dev;

				if (is_red_agg_dev(skb->dev)) {     //same effect as AH_IS_DEVICE_RED0 || AH_IS_DEVICE_AGG0
					ah_red_agg_update_rx_counter(skb->dev, skb);
				}
			}

#if defined(AH_SUPPORT_FE_VCON)
			//here to insert virtual connection here.
			if (unlikely( g_ah_vconn_enable && ah_is_vconn_bypass_fe(skb) )) {
				set_ah_flow_virtual_wire(ah_get_cpu_var(gflow));
				return NULL;
			}
#endif

			if (unlikely ( ah_if_rate_limit_proc__early_check(ah_dev, e) && 
							ah_if_rate_limit_proc (skb, e))) {

				drop_idx = AH_DROP_NO_MEMORY;	//no memory to handle that many packets.

				goto drop_pkt;
			}

		}
		else if (rc == 1) {

			set_ah_flow_to_self (ah_get_cpu_var(gflow));

			set_skb_loopback_pak (skb);

			ahcp_return(sanityv);

			return NULL;

		}
		else {	//rc = -1

			goto drop_pkt;	
		}

	}

	if (unlikely (ah_is_likely_self_mac((ah_mac_t *)e->h_source) &&	/* put this coarse check at first */
					ah_is_self_mac ((ah_mac_t *)e->h_source) && 
					!is_skb_from_self (skb) && 
					!is_ah_amrp_eth(e) )) {

		afe_detail_or_drop_dbgk (skb, "pkt with self mac %m dropped\n", e->h_source);

		drop_idx = AH_DROP_SELF_MAC;

		goto drop_pkt;
	}



	/*
	 * DHCP spoof shield, see jira HOS-5298, HOS-5486
	 */
	if (unlikely(get_skb_ipv4_l5_proto(skb) == AHB_L5_PROTO_ID_DHCPV4 &&		/* only handle real DHCPv4 packet */
				ah_dhcp_spoof_shield_enabled &&
				ah_dhcp_spoof_shield_proc(skb) != 0)) {

		afe_detail_dbgk (skb, "Pkt dropped by if dhcp spoof shield (if=%s)\n", dev2name(skb->dev));

		drop_idx = AH_DROP_DHCPV4_SPOOF_SHIELD;
		
		goto drop_pkt;
	}


	ff_skb_dump(skb);

	/*
	 * perform MAC adddress learning for source MAC address
	 */
	{
		extern int enable_backhaul_mac_learn;
		if (unlikely (AH_SYS_DEV_IS_MAC_LEARN(dev2ahdev(skb->dev)) &&
					  ah_mac_learn_enable) ||
			(enable_backhaul_mac_learn &&  AH_SYS_DEV_IS_BACKHAUL(dev2ahdev(skb->dev))))

			/*
			 * ignore DHCP probe pak, it will use different vid but with same MAC
			 */
			if ( ( get_skb_ipv4_l5_proto(skb) != AHB_L5_PROTO_ID_DHCPV4)  ||
				( ah_validate_dhcp_pak(skb, NULL, &probe) == 0 || probe == FALSE) ) {

				ah_mac_table_process_pkt (skb);
			}
	}


	ahcp_return(sanityv);
	return flow_next_vector(skb);


drop_pkt:

	ah_inc_drop_pak_counter ( ah_get_skb_input_dev(skb), drop_idx);

	set_ah_flow_drop_pak (ah_get_cpu_var(gflow));

	ahcp_return(sanityv);

	return NULL;
}

ahcpd(findflowv);
void *FEFASTPATH ah_find_flow_vector (struct ah_sk_buff *skb)
{
	ah_mac_flow_t *f, *r;
	struct ah_ethhdr *e = ah_eth_hdr(skb);
	ah_mac_sess_t *s;
	int rc, rc1, old_zone_id, new_zone_id, old_upid;
	boolean lookup_route;
	ah_acl_grp_t *old_in_acl_grp, *old_out_acl_grp;
	int rc2;
	uint hash;

	ahcp_start();

	skb_trace_trip(skb);

	/*
	 * no need for any flow processing for armp/ndp
	 * ctrl pkt, just deliver it to self
	 */
	if (unlikely(!is_tcp_fastpath(skb))) {		/* TCP fastpath packet is not any of those below, so no need to go though those check */

		/*
		 *  PAE, BRCM, LLDP, CDP,
		 *
		 *		should flow to self.
		 */
		
		if ( is_ah_pae_pkt (e) || is_ah_brcm_pkt(e) || is_ah_lldp_pkt (e) || is_ah_cdp_pkt (e) ) {

			goto flow_to_self;
		}


		/*
		 * 		MC packet, not sent from self.
		 *
		 *		if it is:
		 *
		 *			AH_LLC non-probe packet or
		 *
		 *			VRRP packet
		 *
		 *		then should flow to self.
		 */
		if ( (e->h_dest[0] & 1) && !is_skb_from_self (skb) && 
				 	( ( is_ah_llc_pkt(e) && !is_ah_probe_pkt(e) ) ||
				 		is_ah_vrrp_pkt(e) ) ) {

			goto flow_to_self;
		}

	}

	if (likely (!(f = skb2macflow2 (skb)) || ah_is_mac_sess_invalid (macflow2sess (f)))) {
		f = ah_find_flow_by_skb (skb);
	}

	if (unlikely (!f)) {
		afe_detail_dbgk(skb, "no MAC sess found\n");
		return (skb2flow_vector_ptr(skb) = (void *)first_path_vector_list);
	}

	r = macflow2peer(f);
	s = macflow2sess(f);

    afe_dbgk_skb(skb, KDBG_FE_DETAIL, "skb=%lx, MAC session (id [%u]) found\n"
                 "fflow %m->%m flag 0x%x upid %d vid %d, rflow %m->%m flag 0x%x upid %d vid %d\n"
                 "fflow acl 0x%x/0x%x, rflow acl 0x%x/0x%x\n",
				 (unsigned long)skb,
				 s->id,
				 f->src_mac, f->dst_mac, f->flag, f->upid, f->vlan,
				 r->src_mac, r->dst_mac, r->flag, r->upid, r->vlan,
				 f->in_acl_grp, f->out_acl_grp, r->in_acl_grp, r->out_acl_grp);


	if (unlikely( (ah_is_mac_sess_deny_ipv4(s) || ah_is_mac_sess_deny_ipv6(s) ) && 	//if mac_sess is not denying IPv4 or IPv6, then no need to call into should_deny_ipv4_or_v6() function.
					should_deny_ipv4_or_v6(s, skb))) {

			goto drop_pak;
	}


	skb2macflow(skb) = (void *)f;
	ah_mac_flow_inc_pak(f);
	ah_mac_flow_add_byte (f, skb->len);
#if  defined(AH_SUPPORT_SDWAN_FE_PATCH)
	if (skb != NULL && f->out_dev != NULL && is_access_dev(f->out_dev)) {
		/*
		 * This is a trick here: At this point, skb->dev is mgt0.X, but we know from mac-sess/route lookup, that this packet
		 * would be eventually egress to a lan_port, so we set our field here.
		 */
		set_skb_br_lan_out(skb, f->out_dev);
	}
#endif
	/* resolve the route in case flow's route is gone */
	rc = 0;
	lookup_route = FALSE;

	if (unlikely(!IS_AH_FLOW_IN_ROUTE_LIST(f))) {
		/* route lookup changes the session parameters, so grab the lock before anything changes */
		ah_write_lock_bh (&s->mac_sess_rw_lock);

		if (unlikely (!IS_AH_FLOW_IN_ROUTE_LIST (f) && AH_FLOW_NEED_LINK_ROUTE (f))) {

			lookup_route = TRUE;
			old_zone_id = r->zone_id;
			old_upid = f->upid;
			old_in_acl_grp = f->in_acl_grp;
			old_out_acl_grp = f->out_acl_grp;

			rc = ah_kafe_forward_lookup (skb, skb->dev, f);

			if (rc & AH_ROUTE_DROP_PAK) {

				/*
				 * restore old-upid for now.. in case station disconnects and comes back, the route for station
				 * will change to default route, there by the UPID changes to 0. so when the station joins back,
				 * the upid will comparison will fail and the session will get invlidated. Not a very clean design
				 * but keep it for now.
				 */
				f->upid = old_upid;

				ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_NO_ROUTE);
				ah_write_unlock_bh (&s->mac_sess_rw_lock);
				goto drop_pak;
			}

			if (f->out_dev != NULL && old_zone_id != (new_zone_id = get_ahdev_zone (dev2ahdev(f->out_dev)))) {

				/*
				 * let this pkt get forwarded out, but invalidate this session
				 * so that next pkt will create a new session.
				 */
				afe_detail_dbgk (skb, "change in zone for mac sess id [%u], old-zone %s/%u new-zone %s/%u\n",
								 s->id, ah_zone_str(old_zone_id), old_zone_id, ah_zone_str (new_zone_id), new_zone_id);


				/*
				 * restore old-upid for now.. in case station disconnects and comes back, the route for station
				 * will change to default route, there by the UPID changes to 0. so when the station joins back,
				 * the upid will comparison will fail and the session will get invlidated. Not a very clean design
				 * but keep it for now.
				 */
				f->upid = old_upid;

				ah_route_unlink_mac_sess (f, FALSE);
				ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_NO_ROUTE);

				if (is_ah_pae_pkt (e) || is_ah_flow_to_self(f)) {
					afe_detail_dbgk (skb, "to-self or PAE pkt, skip zone change check\n");
				} else {
					afe_detail_or_drop_dbgk (skb, "zone change, drop pkt\n");
					ah_write_unlock_bh (&s->mac_sess_rw_lock);
					goto drop_pak;
				}

				/*
				ah_invalidate_mac_sess (s, AH_OIF_CHANGE, AH_OIF_CHANGE_AGEOUT_TIME);
				*/

			} else if (f->upid != old_upid) {

				afe_detail_dbgk (skb, "change in upid, invalidate mac sess id [%u], old-upid %d new-upid %d\n",
								 s->id, old_upid, f->upid);
				ah_invalidate_mac_sess (s, AH_UPID_CHANGE, AH_UPID_CHANGE_AGEOUT_TIME);

			} else if (f->in_acl_grp != old_in_acl_grp || f->out_acl_grp != old_out_acl_grp) {

				afe_detail_dbgk (skb, "change in dynamic ip acl grp in forward flow, invalidate mac sess id [%u]\n",
								 s->id);
				ah_invalidate_mac_sess (s, AH_UPID_CHANGE, AH_UPID_CHANGE_AGEOUT_TIME);

			} else {

				hash = ah_sta_mac_hash(f->dst_mac);
				ah_write_lock_bh(&sta_list->sta_hash_lock[hash]);

				if (f->out_dev && is_access_dev (f->out_dev) && !is_ah_flow_to_self (f)) {
					r->station = ah_create_and_get_station (f->dst_mac);
				}

				if (r->station) {
					ah_start_monitor_station (r->station, f->monitor_duration, f->monitor_pkt_cnt, FALSE);
				}

				ah_write_unlock_bh(&sta_list->sta_hash_lock[hash]);

			}

			/* set the incoming interface of the other wing based on route lookup */
			r->iif = f->oif;

			/* update tunnel policy in the flow (if any) */
			if (f->out_dev) {
				ah_update_tunnel_policy (skb, f->out_dev, r);
			}

		}

		ah_write_unlock_bh (&s->mac_sess_rw_lock);
	}

	if (unlikely (!IS_AH_FLOW_IN_ROUTE_LIST (r))) {
		ah_write_lock_bh (&s->mac_sess_rw_lock);

		/* check this again after acquiring lock to avoid race condition */
		if (unlikely (!IS_AH_FLOW_IN_ROUTE_LIST (r))) {

			lookup_route = TRUE;
			old_zone_id = f->zone_id;
			old_upid = r->upid;
			old_in_acl_grp = r->in_acl_grp;
			old_out_acl_grp = r->out_acl_grp;

			rc1 = ah_kafe_reverse_lookup (skb, skb->dev, r);

			if (rc1 & AH_ROUTE_DROP_PAK) {

				/*
				 * restore old-upid for now.. in case station disconnects and comes back, the route for station
				 * will change to default route, there by the UPID changes to 0. so when the station joins back,
				 * the upid will comparison will fail and the session will get invlidated. Not a very clean design
				 * but keep it for now.
				 */
				r->upid = old_upid;

				/* allow to self pkts even if theres' no route to smac */
				if (is_ah_flow_to_self(f)) {
					goto end_reverse_route_check;
				}

				/* don't drop ah-llc pkt if indicated by reverse route lookup,
				 * otherwise amrp register won't go through to portal from > 3 wifi-mesh hop away */
				if (is_ah_llc_pkt(e)) {
					goto end_reverse_route_check;
				}

				ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_NO_ROUTE);
				ah_write_unlock_bh (&s->mac_sess_rw_lock);
				goto drop_pak;

			}

			if (rc1 & AH_ROUTE_NO_FORW) {

				/*
				 * no route exists for SMAC, but we can deliver this pkt to the stack,
				 * so let it fall through, since there's no route, flow is not added
				 * to the route linked also.
				 */

			} else if (old_zone_id != (new_zone_id = get_ahdev_zone (dev2ahdev(r->out_dev)))) {

				/*
				 * let this pkt get forwarded out, but invalidate this session
				 * so that next pkt will create a new session.
				 */
				afe_detail_dbgk (skb, "change in zone for ac sess id [%u], old-zone %s/%u new-zone %s/%u\n",
								 s->id, ah_zone_str(old_zone_id), old_zone_id, ah_zone_str (new_zone_id), new_zone_id);

				ah_invalidate_mac_sess (s, AH_UPID_CHANGE, AH_UPID_CHANGE_AGEOUT_TIME);

			} else if (r->upid != old_upid) {

				afe_detail_dbgk (skb, "change in upid, invalidate mac sess id [%u], old upid %d new upid %d\n",
								 s->id, old_upid, r->upid);
				ah_invalidate_mac_sess (s, AH_UPID_CHANGE, AH_UPID_CHANGE_AGEOUT_TIME);

			} else if (r->in_acl_grp != old_in_acl_grp || r->out_acl_grp != old_out_acl_grp) {

				afe_detail_dbgk (skb, "change in dynamic ip acl grp in reverse flow, invalidate mac sess id [%u]\n",
								 s->id);
				ah_invalidate_mac_sess (s, AH_UPID_CHANGE, AH_UPID_CHANGE_AGEOUT_TIME);

			} else {

				hash = ah_sta_mac_hash(f->src_mac);
				ah_write_lock_bh(&sta_list->sta_hash_lock[hash]);

				if (r->out_dev && is_access_dev (r->out_dev) && !is_ah_flow_to_self (r)) {
					f->station = ah_create_and_get_station (f->src_mac);
				}

				if (f->station) {
					ah_start_monitor_station (f->station, r->monitor_duration, r->monitor_pkt_cnt, FALSE);
				}

				ah_write_unlock_bh(&sta_list->sta_hash_lock[hash]);

			}

			/* set the incoming interface of the other wing based on route lookup */
			f->iif = r->oif;

			/* update tunnel policy (if any ) */
			if (r->out_dev) {
				ah_update_tunnel_policy (skb, r->out_dev, f);
			}
		}

end_reverse_route_check:

		ah_write_unlock_bh (&s->mac_sess_rw_lock);
	}

	if (likely(is_tcp_fastpath(skb))) {
		if (unlikely(ah_common_pkt_proc_fastpath(skb, f, r, e) < 0)) {
			goto drop_pak;
		}
	} else {
		if (unlikely (is_ah_probe_pkt (e))) {
			rc2 = ah_proc_probe_pkt (skb, rc & AH_ROUTE_DROP_PAK, is_ah_flow_to_self (f));
			if (rc2) {
				rc = rc2;
			}
		}

		if (unlikely (rc & AH_ROUTE_REINJECT_PAK)) {
			ah_probe_reinject_pak (skb, ah_get_cpu_var(gflow));
			ahcp_return(findflowv);
			return NULL;
		}

		/* do per pkt checking for route */
		rc = ah_per_pkt_route_lookup_check (skb, f);

		if (unlikely (rc & AH_ROUTE_DROP_PAK)) {
			ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_NO_ROUTE);
			goto drop_pak;
		}


		if (unlikely (ah_common_pkt_proc (skb, f, e) < 0)) {
			goto drop_pak;
		}
	}

	/*
	 * expire the existing flow in case the route
	 * lookup indicates that no session is needed
	 * and if there's no policy required
	 */
	if (unlikely (is_ah_flow_no_flow(f) && !f->policy)) {
		afe_detail_dbgk (skb, " flow not needed, invalidate flow [%u]\n", s->id);
		ah_invalidate_mac_sess (s, AH_NO_SESS_NEEDED, AH_NO_SESS_NEEDED_AGEOUT_TIME);
	}

	if (unlikely (is_ah_mac_flow_tunnel_transition (f) &&	/* if not in tun transition, then no need to call transition function */
					ah_flow_handle_tunnel_transition (f) < 0) ) {

		ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_FLOW_TUNNEL_CONVERSION_ERR);
		afe_detail_dbgk (skb, "error in flow tunnel vector transition, drop pak\n");
		goto drop_pak;
	}


#if 0
	if (unlikely (ah_flow_handle_cwp_transiton (skb, f) < 0)) {
		ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_FLOW_CWP_CONVERSION_ERR);
		afe_detail_dbgk (skb, "error in flow cwp vector transition, drop pak\n");
		goto drop_pak;
	}
#endif


	if (unlikely (ah_ip_sess_create_needed ())) {

#if AH_SUPPORT_USEG
		if ( ah_useg_enable && 
			pkt_get_eth_type (skb) != ETH_P_IP &&
			pkt_get_eth_type (skb) != ETH_P_IPV6 ) {

			ah_useg_set_src_metadata_for_nonip_pkt(skb);

			ah_useg_set_dst_metadata_for_nonip_pkt(skb);
		}
#endif

		/*
		 * don't create IP sessions for packets that are NATtted, otherwise, this will
		 * overwrite the previous IP session that's created for NAT and NAT will not
		 * work correctly
		 */
		/* change for IPv6 */
		if ( ( (pkt_get_eth_type (skb) == ETH_P_IP) || (pkt_get_eth_type (skb) == ETH_P_IPV6 && !is_skb_ipv6_skip_session6(skb) ) )
			 && (!is_skb_nat (skb))) {


			boolean create_needed = ah_pkt_is_ip_flow_needed(skb) || ah_force_ipsess;
			boolean find_needed = create_needed ||
								  (!is_skb_from_self(skb) && ah_ip_nat_sess_lookup_needed()) ||
								  (is_skb_from_self(skb) && ah_ip_nat_to_self_sess_lookup_needed());

			if (create_needed) {
				set_skb_ipsess_needed(skb);
			} else {
				clr_skb_ipsess_needed(skb);
			}

			if (find_needed) {

				if (unlikely((rc = ah_find_and_create_ip_sess_from_skb(skb)) < 0)) {
					afe_detail_dbgk(skb, "[%s, %d] error in creating IP sess, drop pak skb=%lx\n",
									__FUNCTION__, __LINE__, (unsigned long )skb);

					goto drop_pak;
				}

				if (unlikely (rc == 1)) {
					/* pkt is queued in defrag queue */
					ahcp_return(findflowv);
					return NULL;
				}

			}
		}

	}

	/*
	 * update counters before Ip session is created or found,
	 * some counters like TFTP are depend on parent Ip session
	 */
	if (likely(is_tcp_fastpath(skb))) {
		ah_update_counters_fastpath(skb, is_skb_reinject(skb) ? DIR_REINJECT : DIR_INCOMING);
	} else {
		ah_update_counters(skb, is_skb_reinject(skb) ? DIR_REINJECT : DIR_INCOMING);
	}

	ah_refresh_mac_sess (s);

	ahcp_return(findflowv);

	return (skb2flow_vector_ptr(skb) =
				(void *)ah_get_vector_list_from_index(f->vector_index));

drop_pak:

	set_ah_flow_drop_pak (ah_get_cpu_var(gflow));
	return NULL;


flow_to_self:

	set_ah_flow_to_self (ah_get_cpu_var(gflow));
	ahcp_return(findflowv);
	afe_detail_dbgk(skb, "ah ctrl pak, to self\n");
	return NULL;
}

ahcpd(mainv);
void *FEFASTPATH ah_main_vector (struct ah_sk_buff *skb)
{
	ah_mac_flow_t *f = (ah_mac_flow_t *)skb2macflow(skb), *r;
	const struct ah_ethhdr *e = ah_eth_hdr(skb);
	struct ah_sk_buff *skb2;
	int swap_dev_rc = 0;

	ahcp_start();

	skb_trace_trip(skb);

	/*
	 * if there are any fragmented pkts associated with this,
	 * reinject them back into FE..
	 */
	if (unlikely (get_skb_next_frag (skb))) {
		ah_reinject_frags (skb);
	}

	/* only unicast sessions can be marked to self. so if they
	 * are self sessions, just swap device and get out
	 */
	if (unlikely (is_ah_flow_to_self(f))) {

		if (likely (!is_skb_from_self (skb))) {

			afe_detail_dbgk(skb, "set pkt to self\n");
			set_ah_flow_to_self (ah_get_cpu_var(gflow));

			/*
			 * there's no need to swap device in this case, we want to
			 * deliver the pkt on the incoming interface directly.
			 */
			if (AH_SYS_DEV_IS_ACCESS_CONSOLE (dev2ahdev (skb->dev))) {
				ahcp_return(mainv);
				return NULL;
			} else {
				goto swap_dev;
			}

		} else {

			ah_kassert("pkt from self destined to self?\n");
			set_ah_flow_drop_pak (ah_get_cpu_var(gflow));
			ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_PAK_FROM_SELF_LOOP);
			goto exit_main;

		}
	}

	if (unlikely (e->h_dest[0] & 0x1)) {

		char nhop[ETH_ALEN];
		int ifindex;
		ushort vlan;

		/* if ttl == 0, drop through traffic */
		if ( unlikely (!skb2ahb(skb)->ab_ttl) ) {
			afe_detail_dbgk (skb, "drop ttl 0 pkt %m -> %m(0x%04x)\n",
							 e->h_dest, e->h_source, ntohs(e->h_proto));

			if (is_skb_from_self (skb)) {
				ah_kassert ("self-pkt with ttl 0?\n");
				set_ah_flow_drop_pak (ah_get_cpu_var(gflow));
				ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_TTL_ZERO);
				goto exit_main;
			}

			set_ah_flow_to_self (ah_get_cpu_var(gflow));

			goto swap_dev;
		}

		/*
		 * special handling for pkts from access console interface
		 * there's no need to swap device in this case, we want to
		 * deliver the pkt on the incoming interface directly.
		 * also, we shoudn't forward the broadcast pkts on other
		 * interfaces.
		 */
		if (AH_SYS_DEV_IS_ACCESS_CONSOLE (dev2ahdev (skb->dev))) {
			afe_detail_dbgk(skb, "set pkt to self\n");
			set_ah_flow_to_self (ah_get_cpu_var(gflow));

			ahcp_return(mainv);
			return NULL;
		}

		/* special handling for dhcp pak */

		/*
		 * always try to unicast out DCHP request pkts even
		 * though DMAC is a broadcast pkt.
		 * Otherwise, since we flood the pkt on all interface, there
		 * may be responses both on eth0 and wifi1.1 depending on how
		 * the network configuation is setup and if DHCP servers on eth0
		 * and wifi1.1 give out diffent subnet IP address, it may lead
		 * to issues. So for now, do a route lookup for the default route
		 * and based on it's result of oif/nhop, send out DHCP request pkts
		 * in a unicast manner.
		 *
		 * do the special handling if we are meant to forward the pkt
		 * out, otherwise it will lead to loops. Note the rule that
		 * we should deliver broadcast pkts only to self instead of
		 * forwarding them out IF there's no route to SMAC, which
		 * is when 'no_forward' bit is set.
		 *
		 */
		if (unlikely (!is_ah_flow_no_forw (f) &&
					  get_skb_ipv4_l5_proto(skb) == AHB_L5_PROTO_ID_DHCPV4 &&
					  ah_is_dhcp_discover_or_req (skb) == TRUE &&
					  (ah_default_route_lookup (NULL, nhop, &ifindex) == GET_ROUTE))) {

			struct ah_abs_net_device *dev = ah_dev_get_by_index(ifindex);

			if (!dev) {
				afe_detail_or_drop_dbgk (skb, "dev not found with index %d\n", ifindex);
				set_ah_flow_drop_pak (ah_get_cpu_var(gflow));
				ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_DEV_NOT_FOUND);
				ahcp_return(mainv);
				return NULL;
			}

			/*
			 * earlier we used to the special handling only for self-generated
			 * DHCP traffic, but now we extended it to through traffic also.
			 * Make sure we don't this special handling for pkts received
			 * if incoming and outgoing interfaces are the same,
			 * otherwise it will lead to loops :).
			 */
			if (is_portal_dev (skb->dev)) {

				afe_detail_or_drop_dbgk (skb, "DHCP discover/req pkt on portal ifp, deliver to self, don't flood\n");

			} else if (skb->dev != dev || (is_wmmode_dev (dev) && ah_compare_ether_addr(skb2ahb(skb)->ab_phop, nhop))) {

                afe_detail_dbgk (skb, "special handling for DHCP request pkts, tx on %s\n",
                                dev2name(dev));

				/* catch you Sreekanth, copy&free? show do copy only when needed! ykang */
				if (!(skb2 = ah_wrapper_skb_copy (skb, GFP_ATOMIC)) ) {

					afe_detail_dbgk (skb, "fail copy skb when sending DHCP pkt out\n");

				} else {

					if (!is_wireless_dev (dev)) { /* no non-wireless ifp, need check own-bit before forward bcast pkt */

						r = macflow2peer (f);

						/*
						 * check ownership of src-mac before sending on portal dev
						 */
						if (!is_ah_flow_in_proxy_list(r) || !ah_is_dev_mac_own (dev, r->mp)) {

							afe_detail_dbgk (skb, "%s route: mesh point %m (mac %m) %s on dev %s, don't flood\n",
											 is_ah_flow_in_proxy_list(r) ? "has" : "no", r->mp, f->src_mac, ah_is_dev_mac_own(dev, r->mp) ? "own" : "not own", dev2name (dev));
							ah_wrapper_kfree_skb (skb2);

						} else {
							/*
							 * on portal dev, update out_dev based on vlan defoult route.
							 *
							 * dhcp probe will flood dhcp discover in different VLANs,
							 * but the mac-session will not update f->vlan for different dhcp probe pkt.
							 * so fetch the vlan id from skb->cb
							 */
							if (!ah_def_route_vlan_list_empty() && (vlan = AH_GET_VLAN_VID(AH_VLAN_TAG_P(skb->cb)))) {

								dev_put (dev);

								dev = ah_dev_lookup_def_route_vlan(vlan);
								afe_detail_dbgk (skb, "def-route lookup based on vlan %d, out_dev %s\n",
												 vlan, dev2name(dev));
							}

							ah_dev_forward_pkt (skb2, dev, bcast_mac);
						}

					} else if (is_wmmode_dev (dev)) {

						ah_dev_forward_pkt (skb2, dev, bcast_mac);

					} else {

						afe_detail_or_drop_dbgk (skb, "invalid ifp %s in sending out DHCP pak\n", dev2name(dev));
						set_ah_flow_drop_pak (ah_get_cpu_var(gflow));
						ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_INVALID_DEV_IN_DHCP);
						ah_wrapper_kfree_skb (skb2);
					}

				}

			} else {

				afe_detail_dbgk (skb, "incoming and outgoing %s are same, dont' foward pkt out\n",
								 is_portal_dev (dev) ? "interface" : "hop");

			}

			dev_put (dev);

		} else if (likely (!is_ah_flow_no_forw (f))) {

			ah_dev_flood_forward (skb);

		} else {

			afe_detail_dbgk (skb, "don't forward the pkt out\n");

		}

		if (unlikely (is_skb_from_self (skb))) {
			set_ah_flow_skip_self (ah_get_cpu_var(gflow));
			goto exit_main;
		} else {
			set_ah_flow_to_self (ah_get_cpu_var(gflow));
		}

	} else {

		/* if ttl == 0, drop through traffic */
		if ( unlikely (!skb2ahb(skb)->ab_ttl) ) {
			afe_detail_or_drop_dbgk (skb, "drop ttl 0 pkt %m -> %m(0x%04x)\n",
									 e->h_dest, e->h_source, ntohs(e->h_proto));
			set_ah_flow_drop_pak (ah_get_cpu_var(gflow));
			ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_TTL_ZERO);
			goto exit_main;
		}

		/* check if incoming and outgoing interfaces are ethx and if so, drop the pkt */
		if (unlikely(skb->dev == f->out_dev && !is_wireless_dev (skb->dev))) {
			afe_detail_dbgk (skb, "incoming and outgoing interface both %s, drop pkt\n", dev2name(skb->dev));
			set_ah_flow_drop_pak (ah_get_cpu_var(gflow));
			ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_ETH_LOOP);
			goto exit_main;
		}

		if (unlikely(!f->out_dev || !dev2ahdev((f->out_dev)))) {
			afe_detail_or_drop_dbgk(skb, "fe integrity issue, flow route deleted, drop pkt\n");
			set_ah_flow_drop_pak (ah_get_cpu_var(gflow));
			ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_INVALID_OUT_DEV);
			goto exit_main;
		}
//todo
		ah_dev_forward_pkt (skb, f->out_dev, f->nhop);

		/* here we have only unicast sessions that are not to self, so get out */
		goto exit_main;
	}

swap_dev:

	/* some pkt need swap in_dev before pkt back to Linux */

	/*
	 * don't swap device for dhcp paks from CWP stations after
	 * their registration. we want the DHCP server that's running
	 * on the interface to reply with NAK so that station will
	 * broadcast out DHCP REQ to get the new IP address faster.
	 */
	if (unlikely(!is_ah_flow_cwp_dhcp_to_self(f) &&
				 (swap_dev_rc = ah_swap_dev(skb)) < 0)) {
		afe_detail_or_drop_dbgk(skb, "self pkt interface can't be changed, drop pak\n");
		set_ah_flow_drop_pak (ah_get_cpu_var(gflow));

		if (swap_dev_rc == -2) {
			ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_NO_MATCHING_VLAN);
		} else {
			ah_inc_drop_pak_counter (skb ? ah_get_skb_input_dev(skb) : NULL, AH_DROP_SWAP_DEV_ERR);
		}
	}

exit_main:

	ahcp_return(mainv);
	return NULL;
}

static void ah_mac_sess_timeout_callback (unsigned long data)
{
	int cur_tic = ah_mac_sess_timer_table->mac_sess_timer_cur_tick;
	ah_mac_sess_t *s;
	struct ah_hlist_node *h1;
	int diff;
	boolean empty;

	/*
	 * remove all the sessions from the invalidated list and move them into ager ring.
	 * If the FE module deletes and inserts from the ager ring in the fowarding path,
	 * it may lead to deadlocks.
	 */

	ah_abs_spin_lock_bh (&ah_mac_sess_invalid_list_lock);

    ah_hlist_for_each_entry_safe (s, h1, h2, &ah_mac_sess_invalid_list, invalid_sess_list) {

		hlist_del (&s->invalid_sess_list);

		/* delete the current session from ager ring ... */
		ah_del_mac_sess_from_ager_ring (s, FALSE);

		/* .. and it to current tic + 2 position */
		ah_add_mac_sess_to_ager_with_timeout (s, s->invalidate_period + 5 * HZ);

		ah_set_mac_sess_invalid_timeout(s);
	}

	ah_abs_spin_unlock_bh (&ah_mac_sess_invalid_list_lock);

	/* now walk thru the session ager ring and ageout/refresh each session */

	ah_abs_spin_lock_bh (&ah_mac_sess_timer_table->mac_sess_tick_lock [cur_tic]);

    ah_hlist_for_each_entry_safe (s, h1, h2, &ah_mac_sess_timer_table->mac_sess_tick[cur_tic], ageout_list) {

		/* first delete the session from ager ring */

		if (s->ageout_tic != cur_tic) {
			ah_kassert (0);
			ah_logk (AH_SYS_KMOD_FE, AH_LOG_ERR,
					 "MAC session (id %u) ageout tic (%u) not in the correct tic (%u) list ???\n",
					 s->id, s->ageout_tic, cur_tic);
		}

		ah_del_mac_sess_from_ager_ring (s, TRUE);

		ah_abs_spin_lock_bh (&s->f_flow.ip_flow_list_lock);
		empty = hlist_empty (&s->f_flow.ip_flow_head);
		ah_abs_spin_unlock_bh (&s->f_flow.ip_flow_list_lock);

		if (empty) {
			ah_abs_spin_lock_bh (&s->r_flow.ip_flow_list_lock);
			empty = hlist_empty (&s->r_flow.ip_flow_head);
			ah_abs_spin_unlock_bh (&s->r_flow.ip_flow_list_lock);
		}

		/* if the session is already expired.. */
		if (ah_abs_atomic_read(&s->next_ageout_tic) == s->ageout_tic) {

			/* session is already invalidated, so now delete it */
			if (ah_is_mac_sess_invalid_timeout(s)) {
				if (!empty) {

					afe_mac_sess_dbgk (s, "mac sess id [%u] has IP sessions, delay ageout\n", s->id);

					/*
					 * ageout of this MAC session a few seconds later. the idea
					 * is to give a chance to all the IP sessions that are
					 * bound to this MAC session to ageout first.
					 */
					ah_add_mac_sess_to_ager_with_timeout (s, AH_MAC_SESS_AGEOUT_DELAY_TIME);

					continue;
				}

				afe_mac_sess_dbgk (s, "delete MAC sess id [%u]\n", s->id);
				ah_delete_mac_sess (s, FALSE);

			} else {

				/* otherwise, invalidate it  */
				ah_invalidate_mac_sess (s, AH_NATURAL_AGEOUT, AH_NATURAL_AGEOUT_TIME);
				ah_add_mac_sess_to_ager_with_timeout (s, 2 * HZ);
				ah_set_mac_sess_invalid_timeout(s);
			}

			continue;
		}

		/* make sure that next tick is within bounds */
		/*
		 * we also make sure that next_ageout_tic and current age out (ageout_tic) are
		 * not the same, otherwise since we already have the current age out tic lock,
		 * we will end up deadlocking ourselves as we move this session to next ageout tick.
		 */
		if ((diff = ah_mac_sess_timer_offset (ah_abs_atomic_read(&s->next_ageout_tic),
											  s->ageout_tic)) > (AH_MAX_MAC_SESS_AGEOUT / AH_MAC_SESS_AGER_TIC_MULTIPLIER) || diff == 0) {

			ah_kassert (0);
			ah_logk (AH_SYS_KMOD_FE, AH_LOG_ERR,
					 "MAC session (id %u) next_ageout_tic (%u) is (far from/same as) cur-tic (%u)\n",
					 s->id, ah_abs_atomic_read (&s->next_ageout_tic), s->ageout_tic);

			/* session is already invalidated, so now delete it
			   use invalid_timeout flag instead of invalid flag because
			   invalid_flag can be set outside this timer, and race
			   condition can occur if we use invalid flag. invalid_timeout
			   flag must be set inside timer after an invalid session is put
			   into ager ring */
			if (ah_is_mac_sess_invalid_timeout (s)) {

				ah_logk (AH_SYS_KMOD_FE, AH_LOG_ERR,
						 "next ageout time [%u] sec greater than max ageout [%u] sec\n",
						 diff, AH_MAX_MAC_SESS_AGEOUT);

				afe_mac_sess_dbgk (s, "delete MAC sess id [%u]\n", s->id);
				ah_delete_mac_sess (s, FALSE);

			} else {

				/* otherwise, invalidate it first.. */
				ah_invalidate_mac_sess (s, AH_NATURAL_AGEOUT, AH_NATURAL_AGEOUT_TIME);
				ah_add_mac_sess_to_ager_with_timeout (s, 2 * HZ);
				ah_set_mac_sess_invalid_timeout(s);
			}

			continue;
		}

		/* otherwise move it to the next ageout tick ring */
		ah_add_mac_sess_to_ager_ring (s, ah_abs_atomic_read (&s->next_ageout_tic));

	}

	ah_abs_spin_unlock_bh (&ah_mac_sess_timer_table->mac_sess_tick_lock [cur_tic]);

	/* move to next tick */
	cur_tic = (cur_tic + 1) % AH_MAC_SESS_TIMER_ENTRIES;
	ah_mac_sess_timer_table->mac_sess_timer_cur_tick = cur_tic;

	fe_seconds_tic++;
	if (ah_mac_learn_enable == 0 && fe_seconds_tic > MAC_LEARN_ENABLE_WAIT) {
		ah_mac_learn_enable = 1;
		afe_detail_dbgk (NULL, "MAC learning is allowed\n");
	}

	/* set up for the next poll. timer tic is every 2 second */
	ah_wrapper_mod_timer (&flow_timer, ah_jiffies + (AH_MAC_SESS_AGER_TIC_MULTIPLIER * HZ));
}

/*
 *  with a given SMAC and another hash for all sessions with
 *  a given DMAC
 *
 * Locks held:
 * 1. ah_fe_global_lock
 * 2. ah_mac_acl_grp_t->cfg_list_lock
 * 3. policy_list->hlist_lock[]
 */
void ah_update_mac_sessions_policies_by_smac (ah_policy_t *p, boolean add)
{
	ah_mac_flow_t *f, *r;
	ah_mac_sess_t *s;

	ah_rcu_read_lock ();

	list_for_each_entry_rcu (s, &ah_mac_sess_list, mac_sess_list) {

		f = &s->f_flow;
		r = macflow2peer(f);

		if (add == FALSE) {

			if (f->policy == p) {
				f->policy = NULL;
				f->tunnel_id = 0;
				set_ah_mac_flow_tunnel_transition (f);
			}

			if (r->policy == p) {
				r->policy = NULL;
				r->tunnel_id = 0;
				set_ah_mac_flow_tunnel_transition (r);
			}


		} else {

			if (!ah_compare_ether_addr(f->src_mac, p->mac) && r->out_dev) {

				ah_update_tunnel_policy (NULL, r->out_dev, f);

			} else if (!is_ah_flow_not_installed(r) && !ah_compare_ether_addr(r->src_mac, p->mac) && f->out_dev) {

				ah_update_tunnel_policy (NULL, f->out_dev, r);

			}

		}

	}

	ah_rcu_read_unlock ();
}

/*
 * this function can be called when the following locks are held
 *
 * 1. ah_fe_global_lock
 * 3. policy_list->hlist_lock[i]
 */
void ah_update_mac_sessions_policies_by_dmac (ah_policy_t *p, boolean add)
{
	ah_mac_flow_t *f, *r;
	ah_mac_sess_t *s;

	ah_rcu_read_lock ();

	list_for_each_entry_rcu (s, &ah_mac_sess_list, mac_sess_list) {

		f = &s->f_flow;
		r = macflow2peer(f);

		if (add == FALSE) {

			if (f->policy == p) {
				f->policy = NULL;
				f->tunnel_id = 0;
				set_ah_mac_flow_tunnel_transition (f);
			}

			if (r->policy == p) {
				r->policy = NULL;
				r->tunnel_id = 0;
				set_ah_mac_flow_tunnel_transition (r);
			}

		} else {



			if (!ah_compare_ether_addr(f->dst_mac, p->mac) && r->out_dev) {

				ah_update_tunnel_policy (NULL, r->out_dev, f);
				// update CAVC port
				if (p->is_cavc) {
					f->cavc_port_index = p->if_index;
				}

			} else if (!is_ah_flow_not_installed(r) && !ah_compare_ether_addr(r->dst_mac, p->mac) && f->out_dev) {

				ah_update_tunnel_policy (NULL, f->out_dev, r);
				// update CAVC port
				if (p->is_cavc) {
					r->cavc_port_index = p->if_index;
				}

			}

		}

	}

	ah_rcu_read_unlock ();

}

/*
 * called only in case all policies are deleted.
 * only when the FE module is uninstalled.
 */
void ah_invalidate_policy_flows (void)
{
	ah_mac_flow_t *f, *r;
	ah_mac_sess_t *s;

	afe_detail_dbgk (NULL, "delete all flows that point to policy\n");

	ah_rcu_read_lock ();

	list_for_each_entry_rcu (s, &ah_mac_sess_list, mac_sess_list) {

		f = &s->f_flow;
		r = macflow2peer(f);

		if (f->policy || (!is_ah_flow_not_installed(r) && r->policy)) {

			ah_invalidate_mac_sess (s, AH_POLICY_CHANGE, AH_POLICY_CHANGE_AGEOUT_TIME);

		}

	}

	ah_rcu_read_unlock ();
}

/*
 * ah_mac_sess_policy_change_callback
 *
 * set a bit in all session that have flows that point to
 * a FW policy rule that's changed.
 *
 * this function can be called when the following locks are held
 * 1. ah_mac_acl_grp_list->list_lock
 * 2. ah_mac_acl_grp_t->cfg_list_lock
 * 3. ah_fe_global_lock
 */
void ah_mac_sess_fw_policy_change_callback (ah_mac_acl_grp_t *grp, int rule_id)
{
	ah_mac_sess_t *s;

	ah_rcu_read_lock ();

	list_for_each_entry_rcu (s, &ah_mac_sess_list, mac_sess_list) {

		if (ah_is_mac_sess_invalid (s)) {
			continue;
		}

		if (s->ingress_mac_fw_policy == grp &&
			((rule_id == 0) || s->ingress_mac_fw_policy_rule->id == rule_id)) {

			s->ingress_mac_fw_policy = NULL;
			s->ingress_mac_fw_policy_rule = NULL;
			ah_invalidate_mac_sess (s, AH_FW_POLICY_CHANGE, AH_FW_POLICY_CHANGE_AGEOUT_TIME);

		}  else if (s->egress_mac_fw_policy == grp &&
					((rule_id == 0) || s->egress_mac_fw_policy_rule->id == rule_id)) {

			s->egress_mac_fw_policy = NULL;
			s->egress_mac_fw_policy_rule = NULL;
			ah_invalidate_mac_sess (s, AH_FW_POLICY_CHANGE, AH_FW_POLICY_CHANGE_AGEOUT_TIME);

		}

	}

	ah_rcu_read_unlock ();
}

/*
 *
 * Locks held when this function is called
 *
 * ah_fe_global_lock
 */
static void ah_invalidate_sess_by_dev (struct ah_abs_net_device *dev)
{
	ah_mac_sess_t *s;
	ah_mac_flow_t *f, *r;

	ah_rcu_read_lock ();

	list_for_each_entry_rcu (s, &ah_mac_sess_list, mac_sess_list) {

		if (ah_is_mac_sess_invalid (s)) {
			continue;
		}

		f = &s->f_flow;
		r = &s->r_flow;

		if (!is_ah_flow_dst_broadcast (f) && f->out_dev == dev) {
			ah_invalidate_mac_sess (s, AH_FW_POLICY_CHANGE, AH_FW_POLICY_CHANGE_AGEOUT_TIME);
		} else if (r->out_dev == dev) {
			ah_invalidate_mac_sess (s, AH_FW_POLICY_CHANGE, AH_FW_POLICY_CHANGE_AGEOUT_TIME);
		}

	}

	ah_rcu_read_unlock ();
}

static inline void ah_update_mac_flow_with_def_route (char *route_mac, ah_mac_flow_t *f)
{
	if (is_ah_flow_dst_broadcast (f)) {
		return;
	}

	if (!is_ah_flow_in_proxy_list (f)) {
		return;
	}

	if (is_ah_mac_flow_use_def_route (f) && !ah_compare_ether_addr(f->dst_mac, route_mac)) {

		afe_detail_dbgk (NULL, "update flow %m->%m, sess [%u] route\n",
						 f->src_mac, f->dst_mac, macflow2sess(f)->id);

		ah_route_unlink_mac_sess (f, FALSE);

	}
}

void ah_update_mac_sess_with_def_route (char *route_mac)
{
	ah_mac_sess_t *s;

	if (ah_flow_initialized == FALSE) {
		return;
	}

	ah_rcu_read_lock ();

	list_for_each_entry_rcu (s, &ah_mac_sess_list, mac_sess_list) {

		if (ah_is_mac_sess_invalid (s)) {
			continue;
		}

		ah_update_mac_flow_with_def_route (route_mac, &s->f_flow);
		ah_update_mac_flow_with_def_route (route_mac, &s->r_flow);

	}

	ah_rcu_read_unlock ();

}

/*
 * Locks held
 *
 * 1. ah_fe_global_lock
 */
static void ah_reinstall_mac_flow (ah_mac_flow_t *f, int new_zone_id)
{
	ah_mac_flow_t *tf = &(ah_get_cpu_var(tmp_flow));
	ah_mac_sess_t *s = macflow2sess (f);

	/* make sure that there's no existing sesson with the new zone */
	ah_abs_spin_lock_bh (&ah_mac_sess_create_lock);

	if (ah_find_flow_by_tuple (tf, (ah_mac_t *)f->src_mac, (ah_mac_t *)f->dst_mac, new_zone_id)) {

		ah_abs_spin_unlock_bh (&ah_mac_sess_create_lock);

		/* invalidate this session */
		ah_invalidate_mac_sess (s, AH_STA_ROAM, AH_STA_ROAM_AGEOUT_TIME);
		return;
	}

	/* uninstall it from the old zone first */
	ah_abs_spin_lock_bh(&flow_table->flow_hash_lock[f->hash]);
	ah_del_flow_from_hash (f);
	ah_abs_spin_unlock_bh(&flow_table->flow_hash_lock[f->hash]);

	/* compute new hash */
	f->zone_id = new_zone_id;
	f->hash = ah_flow_hash((ah_mac_t *)f->src_mac, (ah_mac_t *)f->dst_mac, new_zone_id);


	/* install it with new zone */
	ah_abs_spin_lock_bh(&flow_table->flow_hash_lock[f->hash]);
	ah_add_flow_to_hash (f);
	ah_abs_spin_unlock_bh(&flow_table->flow_hash_lock[f->hash]);

	ah_abs_spin_unlock_bh (&ah_mac_sess_create_lock);
}

void ah_update_mac_session_by_sta_roam (const uchar *sta_mac, boolean away, boolean  have_global_lock)
{
	ah_mac_sess_t *s;
	ah_mac_flow_t *f, *r;

	if (!have_global_lock) {
		ah_write_lock_bh (&ah_fe_global_lock);
	}

	ah_rcu_read_lock ();

	list_for_each_entry_rcu (s, &ah_mac_sess_list, mac_sess_list) {

		if (ah_is_mac_sess_invalid (s)) {
			continue;
		}

		f = &s->f_flow;
		r = macflow2peer(f);

		if (away) {

			if (is_zone_id_access(f->zone_id) && !ah_compare_ether_addr(f->src_mac, sta_mac)) {

				afe_mac_sess_dbgk (s, "change flow %m -> %m of session [%d] to backhaul zone\n",
								   f->src_mac, f->dst_mac, s->id);

				/* remove it from the route linked list, so it will be forced to do route lookup again */
				ah_route_unlink_mac_sess (r, FALSE);
				ah_reinstall_mac_flow (f, AH_ZONE_BACKHAUL);
				continue;
			}

			if (!is_ah_flow_not_installed (r) && is_zone_id_access(r->zone_id) && !ah_compare_ether_addr(r->src_mac, sta_mac)) {

				afe_mac_sess_dbgk (s, "change flow %m -> %m of session [%d] to backhaul zone\n",
								   r->src_mac, r->dst_mac, s->id);

				ah_route_unlink_mac_sess (f, FALSE);
				ah_reinstall_mac_flow (r, AH_ZONE_BACKHAUL);
				continue;

			}

		} else {

			if (is_zone_id_backhaul (f->zone_id) && !ah_compare_ether_addr(f->src_mac, sta_mac)) {

				afe_mac_sess_dbgk (s, "change flow %m -> %m of session [%d] to access zone\n",
								   f->src_mac, f->dst_mac, s->id);

				ah_route_unlink_mac_sess (r, FALSE);
				ah_reinstall_mac_flow (f, AH_ZONE_ACCESS);
				continue;
			}

			if (!is_ah_flow_not_installed (r) && is_zone_id_backhaul (r->zone_id) && !ah_compare_ether_addr(r->src_mac, sta_mac)) {

				afe_mac_sess_dbgk (s, "change flow %m -> %m of session [%d] to access zone\n",
								   r->src_mac, r->dst_mac, s->id);

				ah_route_unlink_mac_sess (f, FALSE);
				ah_reinstall_mac_flow (r, AH_ZONE_ACCESS);
				continue;

			}

		}

	}

	ah_rcu_read_unlock ();

	if (!have_global_lock) {
		ah_write_unlock_bh (&ah_fe_global_lock);
	}
}

#if AH_SUPPORT_USEG

boolean ah_useg_if_need_invalidate_mac_sess_right_now(ah_mac_sess_t *s, ah_sta_t *src_sta, ah_sta_t *dst_sta)
{

	if (src_sta == NULL || dst_sta == NULL) {

		return FALSE;
	}


/*
	 * If src_sta and dst_sta are silibing APCG sta connecting on nonanchor AP
 *
	 * No need to check keyid.
	 */
	if ( USEG_LOCAL_STA_GET_MODE(src_sta) == USEG_MODE_APCG && 
		USEG_LOCAL_STA_GET_MODE(dst_sta) == USEG_MODE_APCG && 
		USEG_LOCAL_STA_IS_APCG_NONANCHOR(src_sta) &&
		USEG_LOCAL_STA_IS_APCG_NONANCHOR(dst_sta) &&
		memcmp(src_sta->apcg_anchor_ap_mac, dst_sta->apcg_anchor_ap_mac, ETH_ALEN) == 0
	) {

		return TRUE;
	}

	return FALSE;
}

#endif	/* #if AH_SUPPORT_USEG */

/*
 *
 * Locks held
 *
 * 1. ah_fe_global_lock
 * 2. sta_list->sta_hash_lock
 */
int ah_update_mac_sess_by_sta_delete (ah_sta_t *sta)
{
	ah_mac_flow_t *f, *r;
	ah_mac_sess_t *s;

	local_bh_disable ();

	ah_rcu_read_lock ();

	list_for_each_entry_rcu (s, &ah_mac_sess_list, mac_sess_list) {

		f = &s->f_flow;
		r = macflow2peer(f);

		/* station is stored in the flow based on it's src-mac */

		if (sta && f->station == sta && r->station == sta) {
			ah_logk(AH_SYS_KMOD_FE, AH_LOG_INFO,
					"%s f->station %x and r->station %x are the same\n",
					__func__, f->station, r->station);
		}

		if (f->station == sta) {

#if AH_SUPPORT_USEG
			if ( ah_useg_enable &&
					ah_useg_if_need_invalidate_mac_sess_right_now(s, f->station, r->station) ) {

				ah_invalidate_mac_sess (s, AH_STA_DELETE, AH_CLI_DELETE_AGEOUT_TIME);	// use cli_timeout = 0 for quick invalidate.

				continue;
			}
#endif


			f->station = NULL;

			/*
			 * we shouldn't invalidate the sessions that have both the ends as stations (access)
			 * and if one of the stations roams away. Invaliadate it only if the other end is
			 * backhaul
			 */
			if (is_zone_id_backhaul (r->zone_id)) {
				afe_detail_dbgk (NULL, "invalidate MAC session id [%d] station %m delete\n",
								 s->id, sta->mac);
				ah_invalidate_mac_sess (s, AH_STA_DELETE, AH_STA_DELETE_AGEOUT_TIME);
			}
		}

		if (!is_ah_flow_not_installed (r) && (r->station == sta)) {

#if AH_SUPPORT_USEG
			if ( ah_useg_enable &&
					ah_useg_if_need_invalidate_mac_sess_right_now(s, f->station, r->station) ) {

				ah_invalidate_mac_sess (s, AH_STA_DELETE, AH_CLI_DELETE_AGEOUT_TIME);	// use cli_timeout = 0 for quick invalidate.

				continue;
			}
#endif


			r->station = NULL;

			/*
			 * we shouldn't invalidate the sessions that have both the ends as stations (access)
			 * and if one of the stations roams away. Invaliadate it only if the other end is
			 * backhaul
			 */
			if (is_zone_id_backhaul (f->zone_id)) {
				afe_detail_dbgk (NULL, "invalidate MAC session id [%d] station %m delete\n",
								 s->id, sta->mac);
				ah_invalidate_mac_sess (s, AH_STA_DELETE, AH_STA_DELETE_AGEOUT_TIME);
			}

		}

	}

	ah_rcu_read_unlock ();

	ah_local_bh_enable ();

	return 0;
}


static inline void ah_fill_get_flow_flags  (ah_get_mac_flow_entry_t *fe, ah_mac_flow_t *f)
{
	ah_mac_flow_t *r = macflow2peer (f);

	if (is_ah_flow_encap(f)) {
		fe->flag |= AH_GET_FLOW_ENCAP;
	}

	if (is_ah_flow_to_self (f)) {
		fe->flag |= AH_GET_FLOW_TO_SELF;
	}

	if (is_ah_flow_not_installed (f)) {
		fe->flag |= AH_GET_FLOW_NOT_INSTALLED;
	}

	if (!is_ah_flow_in_proxy_list (r)) {
		fe->flag |= AH_GET_FLOW_ROUTE_INVALID;
	}

	fe->flow_flag = f->flag;

}

static void ah_dump_one_mac_flow (ah_get_mac_flow_entry_t *to, ah_mac_flow_t *from)
{

	ah_mac_flow_t *peer = macflow2peer(from);

	ah_fill_get_flow_flags (to, from);

	memcpy(to->src_mac, from->src_mac, ETH_ALEN);
	memcpy(to->dst_mac, from->dst_mac, ETH_ALEN);

	if (is_ah_flow_not_installed (from)) {
		return;
	}

	//to->ifindex      = from->iif;
	to->ifindex      = peer->out_dev ? peer->out_dev->ifindex : -1;
	to->zone_id      = from->zone_id;
	to->vector_index = from->vector_index;
	to->tunnel_id    = from->tunnel_id;
	to->policy_id    = from->policy ? from->policy->id : 0;
	to->pak          = ah_mac_flow_get_pak (from);
	to->byte_count   = ah_mac_flow_get_byte (from);
	to->oif          = from->out_dev ? from->out_dev->ifindex : -1;
	to->nhop_metric  = from->nhop_metric;
	to->agg_mem_hash = from->agg_mem_hash;
	to->cavc_port_index = from->cavc_port_index;
	memcpy(to->nhop, from->nhop, ETH_ALEN);

	/*
	 * for broadcast sessions, we should display the peer flows' upid because
	 * that's where the src-mac address upid is stored.
	 */
	to->upid         = (from->dst_mac[0] & 0x1) ? peer->upid : from->upid;

	if (from->station) {

		to->flag |= AH_GET_FLOW_STA;

		if (ah_is_cwp_entry(from->station)) {
			to->flag |= AH_GET_FLOW_CWP;
			memcpy(to->ci.mac , from->station->mac, ETH_ALEN);
			to->ci.iif  = from->station->iif;
			to->ci.flag = from->station->flag & AH_CWP_FLAG_MASK;
		}
	}

	if (is_ah_mac_flow_vlan_present (from) && from->vlan) {
		to->flag |= AH_GET_FLOW_VLAN;
		to->vlan = from->vlan;
	}
}

static void ah_dump_one_mac_sess (ah_get_mac_sess_entry_t *to, ah_mac_sess_t *from)
{
	memset(to, 0, sizeof(ah_get_mac_sess_entry_t ));

	ah_dump_one_mac_flow (&to->f_flow, &from->f_flow);
	ah_dump_one_mac_flow (&to->r_flow, &from->r_flow);

	to->id           = from->id;
	to->flag         = from->flag;
	to->upid         = from->upid;
	to->ageout_time  = /* ah_is_mac_sess_timed_out(from)? 0 : */ah_get_mac_sess_age_out_time(from);
	to->up_time      = get_mac_sess_life_time (from) / HZ;

	if (from->ingress_mac_fw_policy) {
		strncpy(to->ingress_fw_policy, from->ingress_mac_fw_policy->name, AH_MAC_ACL_GRP_NAME_LEN);
		to->ingress_fw_policy_rule_id = from->ingress_mac_fw_policy_rule ? from->ingress_mac_fw_policy_rule->id : 0;
	}

	if (from->egress_mac_fw_policy) {
		strncpy(to->egress_fw_policy, from->egress_mac_fw_policy->name, AH_MAC_ACL_GRP_NAME_LEN);
		to->egress_fw_policy_rule_id = from->egress_mac_fw_policy_rule ? from->egress_mac_fw_policy_rule->id : 0;
	}

}

int ah_dump_mac_sessions (char *buf, int size, int start_cnt, char *src_mac, char *dst_mac, int16_t vlan)
{
	ah_get_mac_sess_entry_t *fe = (ah_get_mac_sess_entry_t *)buf ;
	ah_mac_sess_t *s;
	ah_mac_flow_t *f, *r;
	int cnt = 0;
	boolean cmp_src_mac = 0, cmp_dst_mac = 0;

	if (ah_compare_ether_addr(src_mac, zero_mac)) {
		cmp_src_mac = 1;
	}

	if (ah_compare_ether_addr(dst_mac, zero_mac)) {
		cmp_dst_mac = 1;
	}

	ah_rcu_read_lock ();

	list_for_each_entry_rcu (s, &ah_mac_sess_list, mac_sess_list) {

		if (start_cnt) {
			start_cnt--;
			continue;
		}

		if (cnt >= size) {
			ah_rcu_read_unlock ();
			return cnt;
		}

		f = &s->f_flow;
		r = &s->r_flow;

		if (cmp_src_mac &&
			ah_compare_ether_addr(src_mac, f->src_mac) &&
			(is_ah_flow_not_installed (r) || ah_compare_ether_addr(src_mac, r->src_mac))) {
			continue;
		}

		if (cmp_dst_mac &&
			ah_compare_ether_addr(dst_mac, f->dst_mac) &&
			(is_ah_flow_not_installed (r) || ah_compare_ether_addr(dst_mac, r->dst_mac))) {
			continue;
		}

		if (vlan &&
			(!is_ah_mac_flow_vlan_present(f) || f->vlan != vlan) &&
			(is_ah_flow_not_installed (r) || !is_ah_mac_flow_vlan_present (r) || r->vlan != vlan)) {
			continue;
		}

		ah_dump_one_mac_sess (fe, s);

		fe++;
		cnt++;

	}

	ah_rcu_read_unlock ();

	return cnt;
}

int ah_dump_one_mac_sess_by_id (char *buf, int sess_id)
{
	ah_get_mac_sess_entry_t *dst = (ah_get_mac_sess_entry_t *)buf ;
	ah_mac_sess_t *s;

	if (!(s = ah_find_mac_sess_by_id (sess_id))) {
		return -1;
	}

	ah_dump_one_mac_sess (dst, s);
	return 0;
}

void ah_set_mac_sess_debug_by_filter (ah_ffilter_t *f)
{
	ah_mac_sess_t *s;

	ah_rcu_read_lock ();

	list_for_each_entry_rcu (s, &ah_mac_sess_list, mac_sess_list) {

		if (ah_ff_match_for_mac_sess_by_filter (s, f)) {
			set_mac_sess_match_ff_id (s, f->id);
		}

	}

	ah_rcu_read_unlock ();
}

void ah_unset_mac_sess_debug_by_filter (int filter_id)
{
	ah_mac_sess_t *s;

	ah_rcu_read_lock ();

	list_for_each_entry_rcu (s, &ah_mac_sess_list, mac_sess_list) {

		if (ah_is_mac_sess_debug (s) && get_mac_sess_match_ff_id (s) == filter_id) {
			clr_mac_sess_match_ff_id (s);
			ah_ff_match_for_mac_sess (s);
		}

	}

	ah_rcu_read_unlock ();
}

static void ah_flow_lock(void)
{
	ah_write_lock_bh(&ah_fe_global_lock);
}

static void ah_flow_unlock(void)
{
	ah_write_unlock_bh(&ah_fe_global_lock);
}

int ah_flow_init (void)
{
	int i;

	ah_ff_init ();

	ah_sta_init ();
	ah_vlan_disc_init();

	if (ah_frag_init () < 0) {
		return ENOMEM;
	}

	if (ah_ip_gate_init () < 0) {
		ah_frag_exit ();
		return ENOMEM;
	}

	if (ah_ip_flow_init () < 0) {
		ah_frag_exit ();
		ah_ip_gate_exit();
		return ENOMEM;
	}

	if (ah_sess_sync_init () < 0) {
		ah_frag_exit ();
		ah_ip_gate_exit();
		ah_ip_flow_exit ();
		return ENOMEM;
	}

	if (ah_tunnel_init () < 0) {
		ah_frag_exit ();
		ah_ip_gate_exit();
		ah_ip_flow_exit ();
		ah_sess_sync_exit ();
		return ENOMEM;
	}

	if (ah_acl_init () < 0) {
		ah_frag_exit ();
		ah_ip_gate_exit();
		ah_ip_flow_exit ();
		ah_sess_sync_exit ();
		ah_tunnel_exit ();
		return ENOMEM;
	}

	if (ah_mac_acl_init() < 0) {
		ah_frag_exit ();
		ah_ip_gate_exit();
		ah_acl_exit ();
		ah_tunnel_exit ();
		ah_ip_flow_exit ();
	}

	if (ah_policy_init () < 0) {
		ah_frag_exit ();
		ah_ip_gate_exit();
		ah_tunnel_exit ();
		ah_acl_exit ();
		ah_mac_acl_exit ();
		ah_ip_flow_exit ();
		ah_sess_sync_exit ();
		return ENOMEM;
	}

	if (ah_nat_init () < 0) {
		ah_frag_exit ();
		ah_ip_gate_exit();
		ah_tunnel_exit ();
		ah_acl_exit ();
		ah_mac_acl_exit ();
		ah_ip_flow_exit ();
		ah_sess_sync_exit ();
		ah_policy_exit ();
		return ENOMEM;
	}

	flow_table = ah_kcalloc_atomic (sizeof(ah_flow_table_t));

	if (!flow_table) {
		ah_logk(AH_SYS_KMOD_FE, AH_LOG_ERR, "can't alloc kernel memory\n");
		ah_frag_exit ();
		ah_ip_gate_exit();
		ah_tunnel_exit ();
		ah_acl_exit ();
		ah_mac_acl_exit ();
		ah_policy_exit ();
		ah_ip_flow_exit ();
		ah_sess_sync_exit ();
		ah_nat_exit ();
		return ENOMEM;
	}

	/*
	    kflow_mh = ah_kmpi_open(AH_KMOD_ID_FLOW, ah_kflow_recv);
	    if (kflow_mh < 0)  {
	        ah_kfree(flow_table);
	        ah_frag_exit ();
	        ah_ip_gate_exit();
	        flow_table = NULL;
	        ah_tunnel_exit ();
	        ah_acl_exit ();
	        ah_mac_acl_exit ();
	        ah_policy_exit ();
	        ah_ip_flow_exit ();
	        ah_sess_sync_exit ();
	        ah_nat_exit ();
	        return ENOMEM;
	    }
	*/


	if ((ah_mac_sess_timer_table = ah_kcalloc_atomic (sizeof(ah_mac_sess_timer_table_t))) == NULL) {
		ah_logk(AH_SYS_KMOD_FE, AH_LOG_ERR, "can't alloc kernel memory\n");
		ah_kfree(flow_table);
		ah_frag_exit ();
		ah_ip_gate_exit();
		flow_table = NULL;
		ah_tunnel_exit ();
		ah_acl_exit ();
		ah_mac_acl_exit ();
		ah_policy_exit ();
		ah_ip_flow_exit ();
		ah_sess_sync_exit ();
		ah_kmpi_close (kflow_mh);
		ah_nat_exit ();
		kflow_mh = -1;
		return ENOMEM;
	}
#if defined(AH_SUPPORT_FE_VCON)
	if (ah_vconn_init() < 0) {
		ah_logk(AH_SYS_KMOD_FE, AH_LOG_ERR, "can't alloc kernel memory\n");
		ah_kfree(ah_mac_sess_timer_table);
		ah_kfree(flow_table);
		ah_frag_exit ();
		ah_ip_gate_exit();
		flow_table = NULL;
		ah_tunnel_exit ();
		ah_acl_exit ();
		ah_mac_acl_exit ();
		ah_policy_exit ();
		ah_ip_flow_exit ();
		ah_sess_sync_exit ();
		ah_kmpi_close (kflow_mh);
		ah_nat_exit ();
		kflow_mh = -1;
		return ENOMEM;
	}
#endif

	for (i = 0; i < AH_FLOW_HASH_SIZE; i++) {
		INIT_HLIST_HEAD(&flow_table->flow_hash[i]);
		ah_abs_spin_lock_init(&flow_table->flow_hash_lock[i]);
		ah_abs_atomic_set (&flow_table->flow_hash_cnt[i], 0);
	}

	for (i = 0; i < AH_MAC_SESS_TIMER_ENTRIES; i++) {
		ah_abs_spin_lock_init (&ah_mac_sess_timer_table->mac_sess_tick_lock [i]);
		INIT_HLIST_HEAD (&ah_mac_sess_timer_table->mac_sess_tick[i]);
	}

	INIT_LIST_HEAD (&ah_mac_sess_list);
	ah_abs_spin_lock_init (&ah_mac_sess_list_lock);

	/* init and start flow table timer */
	ah_init_timer (&flow_timer);

	/* kick start the 1st timer after 2 seconds */
	flow_timer.expires = ah_jiffies + (1 * HZ);
	flow_timer.data = 0;
	flow_timer.function = ah_mac_sess_timeout_callback;

	ah_wrapper_add_timer (&flow_timer);

	ah_vector_init();


	/* don't alloc session with id '0', mark it as taken */
	ah_get_mac_sess_avail_id ();

	ah_wrapper_register_netdevice_notifier (&ah_flow_device_notifier);
	ah_dev_register_notifier (&ah_dev_flow_device_notifier);

	ah_init_self_mng_ip ();


	INIT_HLIST_HEAD (&ah_mac_sess_invalid_list);
	ah_abs_spin_lock_init (&ah_mac_sess_invalid_list_lock);

	ah_abs_spin_lock_init (&ah_mac_sess_create_lock);
	rwlock_init (&ah_fe_global_lock);
	ah_fe_lock_handler = ah_flow_lock;
	ah_fe_unlock_handler = ah_flow_unlock;

	ah_init_pak_counter ();

	ah_flow_initialized = TRUE;

#if defined(AH_SUPPORT_FE_VCON)
	memset (ah_vwire_nhop, 0, ETH_ALEN);
#endif

	ah_cwp_init ();

	ah_tb_init ();
	ah_pkt_queue_init ();
	ah_to_self_init ();
	ah_http_alg_init ();

	ah_sfl_init ();

	return 0;

}

void ah_flow_destroy (void)
{

	ah_sfl_exit ();

	ah_http_alg_exit ();
	ah_pkt_queue_exit ();
	ah_tb_exit ();

	ah_cwp_exit ();
	ah_ff_exit ();

	ah_sta_destroy ();

	ah_frag_exit ();
	ah_vector_exit ();
	ah_policy_exit ();
	ah_tunnel_exit ();
	ah_acl_exit ();
	ah_mac_acl_exit ();
	ah_delete_all_mac_sessions ();
	ah_ip_flow_exit ();
	ah_ip_gate_exit ();
	ah_sess_sync_exit ();
	ah_nat_exit ();

	if (kflow_mh != -1) {
		ah_kmpi_close (kflow_mh);
		kflow_mh = -1;
	}


	ah_wrapper_del_timer (&flow_timer);

	if (flow_table) {
		ah_kfree (flow_table);
		flow_table = NULL;
	}

	if (ah_mac_sess_timer_table) {
		ah_kfree (ah_mac_sess_timer_table);
		ah_mac_sess_timer_table = NULL;
	}

	ah_fe_lock_handler = NULL;
	ah_fe_unlock_handler = NULL;

	ah_wrapper_unregister_netdevice_notifier (&ah_flow_device_notifier);
	ah_dev_unregister_notifier (&ah_dev_flow_device_notifier);

	ah_flow_initialized = FALSE;

}

#ifdef AH_DEBUG_MAC_SESS_LIST
#define list_for_each_entry_rcu_no_prefetch(pos, head, member) \
	for (pos = list_entry((head)->next, typeof(*pos), member); \
		 &(rcu_dereference(pos)->member) != (head); \
		 pos = list_entry(pos->member.next, typeof(*pos), member))
void dump_mac_sess(ah_mac_sess_t *s)
{
	/* dump mac session data */
	*_ah_kernel_trace_dump = 1;
	if (s > ((ah_mac_sess_t *) 0x80000000) &&
		!(((int) s) & 0x03)) {
		ah_abs_printk(KERN_ERR "bad mac sess: 0x%p\n", s);
		ah_abs_printk(KERN_ERR "bad mac sess: id %d sig1 0x%x sig2 0x%x sess_next 0x%p sess_prev 0x%p\n",
			   s->id, s->mac_sess_sig1, s->mac_sess_sig2, s->mac_sess_list.next, s->mac_sess_list.prev);
		ah_abs_printk(KERN_ERR "bad mac sess: id %d ageout_next 0x%p ageout_pprev 0x%p flag 0x%lx\n",
			   s->id, s->ageout_list.next, s->ageout_list.pprev, s->flag);
	}
	ah_abs_printk(KERN_ERR "&ah_mac_sess_list 0x%p next 0x%p prev 0x%p\n",
		   &ah_mac_sess_list, ah_mac_sess_list.next, ah_mac_sess_list.prev);
	ah_abs_printk(KERN_ERR "dumping mac sess 0x%p:\n", s);
	ah_hexdumpk((unsigned char *) s, sizeof(ah_mac_sess_t));
	*_ah_kernel_trace_dump = 0;
}

int ah_check_bad_mac_sess(ah_mac_sess_t *s)
{
	struct ah_list_head *next = s->mac_sess_list.next;
	struct ah_list_head *prev = s->mac_sess_list.prev;

	if (s->mac_sess_sig1 != AH_MAC_SESS_SIG ||
		s->mac_sess_sig2 != AH_MAC_SESS_SIG ||
		!next || (((int)next) & 0x3) || next < ((struct ah_list_head *) 0x80000000) ||
		!prev || (((int)prev) & 0x3) || prev < ((struct ah_list_head *) 0x80000000)) {
		dump_mac_sess(s);
		if (next && (next != &ah_mac_sess_list)) {
			dump_mac_sess(list_entry(next, typeof(*s), mac_sess_list));
		}
		if (prev && (prev != &ah_mac_sess_list)) {
			dump_mac_sess(list_entry(prev, typeof(*s), mac_sess_list));
		}
		return 1;
	} else {
		return 0;
	}
}

int ah_check_mac_sess_list(void)
{
	ah_mac_sess_t *s;
	int bad_list_ptr = 0, sess_cnt = 0;

	ah_rcu_read_lock ();

	list_for_each_entry_rcu_no_prefetch (s, &ah_mac_sess_list, mac_sess_list) {
		++sess_cnt;
		if (ah_check_bad_mac_sess(s)) {
			bad_list_ptr = 1;
			break;
		}
	}

	ah_rcu_read_unlock ();

	if (bad_list_ptr) {
		/* dump mac session data */
		*_ah_kernel_trace_dump = 1;
		ah_abs_printk(KERN_ERR "%s: sess_cnt %d mac_sess 0x%p\n", __func__, sess_cnt, s);
		*_ah_kernel_trace_dump = 0;
	}

	return bad_list_ptr;
}
#endif


#if defined(AH_SUPPORT_FE_VCON)

static ah_vconn_hash_table_t *ah_vconn_hash_table;

uint32_t g_ah_vconn_entries_cnt = 0;

boolean g_ah_vconn_enable = FALSE;

#define ah_get_vconn_cnt()      (g_ah_vconn_entries_cnt)
#define ah_inc_vconn_cnt()      (g_ah_vconn_entries_cnt++)
#define ah_dec_vconn_cnt()      (g_ah_vconn_entries_cnt--)

unsigned int ah_DJBHash(char *str, unsigned int len, unsigned int hash)
{
	unsigned int i = 0;

	for (i = 0; i < len; str++, i++) {
		hash = ((hash << 5) + hash) + (*str);
	}

	return hash;
}

static uint32_t ah_vconn_hash (uint32_t index, char *dst, char *pre_hop)
{
	uint32_t hash = 0;

	hash = ah_DJBHash((char *) &index, sizeof(uint32_t), 5381);
	// hash = ah_DJBHash(src,ETH_ALEN,hash); //src mac may be ignored.
	hash = ah_DJBHash(dst, ETH_ALEN, hash);
	hash = ah_DJBHash(pre_hop, ETH_ALEN, hash);

	hash %= AH_VCONN_HASH_SIZE;
	return hash;
}


static inline boolean  ah_vconn_compare(ah_vconn_rule_t *p_rule, uint32_t index, char *src, char *dst, char *phop)
{
	int len;

	if (p_rule->srcmacignored == 0) {

		if (p_rule->srcouienabled) {
			len = 3;
		} else {
			len = ETH_ALEN;
		}

		if (memcmp(src, p_rule->src_mac, len )) {
			return FALSE;
		}
	}

	if ((index == p_rule->iifindex) && (0 == ah_compare_ether_addr(dst, p_rule->dst_mac)) &&
		(0 == ah_compare_ether_addr(phop, p_rule->pre_hop))) {
		return TRUE;
	} else {
		return FALSE;
	}
}

ah_vconn_rule_t *ah_match_vconn_hit(uint index, char *src, char *dst, char *phop)
{
	uint32_t  hash;
	struct ah_hlist_node *g, *h;

	hash = ah_vconn_hash(index, dst, phop);

	hlist_for_each_safe(h, g, &ah_vconn_hash_table->hash[hash]) {

		ah_vconn_rule_t *t = hlist_entry(h, ah_vconn_rule_t, hlist);

		if (ah_vconn_compare(t, index, src, dst, phop)) {
			return t;
		}
	}
	return NULL;
}

boolean ah_is_vconn_bypass_fe(struct ah_sk_buff *skb)
{
	ah_vconn_rule_t *p;
	const struct ah_ethhdr    *e;
	ah_buff_t         *ahb;
	struct ah_abs_net_device *outdev;

	if (likely(ah_get_vconn_cnt() == 0)) {
		return FALSE;
	}

	e = ah_eth_hdr(skb);
	ahb = skb2ahb(skb);

    afe_detail_dbgk(skb, "matching vconnection rule by src %m dst %m phop %m\n", 
				e->h_source, e->h_dest, ahb->ab_phop);
	p = ah_match_vconn_hit(skb->dev->ifindex, (char *)e->h_source, (char *)e->h_dest, (char *)ahb->ab_phop);
	if (p) {

		outdev = ah_dev_get_by_index(p->oifindex);
		if (outdev) {

            afe_detail_dbgk(skb, "forward vconnection data to interface %s by rule  %s\n", 
                    outdev->name, p->rulename);
			set_skb_vconn(skb);
			ah_dev_forward_pkt (skb, outdev, p->next_hop);
			dev_put(outdev);

		} else {
        
            afe_detail_dbgk(skb, "fellow vconnection rule(%s), drop pkt\n", p->rulename);
			ah_kfree_skb(skb);
		}
		return TRUE;
	}

	return FALSE;
}


int ah_add_vconn_rule(ah_vconn_msg_t *msg)
{
	uint32_t hash;
	ah_vconn_rule_t *t;


	if (AH_VCONN_MAX_LIMIT <= ah_get_vconn_cnt()) {
		return -1;
	}

	hash = ah_vconn_hash(msg->iifindex, msg->dst_mac, msg->pre_hop);
	if ((t = ah_kcalloc_atomic(sizeof(ah_vconn_rule_t))) == NULL) {
		ah_logk(AH_SYS_KMOD_FE, AH_LOG_ERR, "alloc mem in kernel failed\n");
		return -1;
	}

	t->iifindex = msg->iifindex;//index and name
	t->srcouienabled = msg->srcouienabled;
	t->srcmacignored = msg->srcignored;
	memcpy(t->src_mac, msg->src_mac, ETH_ALEN);
	memcpy(t->dst_mac, msg->dst_mac, ETH_ALEN);
	memcpy(t->pre_hop, msg->pre_hop, ETH_ALEN);
	t->oifindex = msg->oifindex;
	memcpy(t->rulename, msg->rulename, AH_TB_MAX_NAME);
	t->rulename[AH_TB_MAX_NAME] = '\0';
	memcpy(t->next_hop, msg->next_hop, ETH_ALEN);

	ah_wrapper_ah_rcu_read_lock_bh();
	hlist_add_head_rcu(&t->hlist, &ah_vconn_hash_table->hash[hash]);

	ah_inc_vconn_cnt();
	g_ah_vconn_enable = TRUE;

	ah_wrapper_ah_rcu_read_unlock_bh();

	return 0;

}

void ah_del_vconn(ah_vconn_rule_t *t)
{
	if (t) {
		ah_wrapper_ah_rcu_read_lock_bh();
		hlist_del_rcu(&t->hlist);
		ah_dec_vconn_cnt();
		if ( ah_get_vconn_cnt() == 0 ) {
			g_ah_vconn_enable = FALSE;
		}
		ah_kfree(t);
		ah_wrapper_ah_rcu_read_unlock_bh();
	}
}

int32_t ah_del_vconn_rule(ah_vconn_msg_t *msg)
{
	ah_vconn_rule_t *t;
	struct ah_hlist_node *h;
	int i;

	for (i = 0; i < AH_VCONN_HASH_SIZE; i++) {

       ah_hlist_for_each_entry (t, h, &ah_vconn_hash_table->hash[i], hlist) {

			if (msg->flag) {
				afe_detail_dbgk(NULL, "delete all the vconn rule\n");
				ah_del_vconn(t);
			} else {

				if (memcmp(msg->rulename, t->rulename, AH_TB_MAX_NAME) == 0) {
					afe_detail_dbgk(NULL, "delete the vconn rule %s\n", msg->rulename);
					ah_del_vconn(t);
					break;
				}
			}
		}
	}

	return 0;
}

int ah_dump_vconn_entries (char *buf, int size)
{
	ah_vconn_msg_t *te = (ah_vconn_msg_t *)buf ;
	int i, cnt = 0;
	ah_vconn_rule_t *t;
	struct ah_hlist_node *h;

	ah_wrapper_ah_rcu_read_lock_bh();
	for (i = 0; i < AH_VCONN_HASH_SIZE; i++) {

        ah_hlist_for_each_entry (t, h, &ah_vconn_hash_table->hash[i], hlist) {

			if (cnt >= size) {
				goto finish_dump;
			}

			te->iifindex = t->iifindex;
			te->flag = 0;
			memcpy(te->rulename, t->rulename, AH_TB_MAX_NAME);
			te->srcouienabled = t->srcouienabled;
			te->srcignored = t->srcmacignored;
			memcpy(te->src_mac, t->src_mac, ETH_ALEN);
			memcpy(te->dst_mac, t->dst_mac, ETH_ALEN);
			memcpy(te->pre_hop, t->pre_hop, ETH_ALEN);
			te->oifindex = t->oifindex;
			memcpy(te->next_hop, t->next_hop, ETH_ALEN);
			te++;
			cnt++;
		}
	}

finish_dump:
	ah_wrapper_ah_rcu_read_unlock_bh();
	return cnt;
}

int ah_vconn_init(void)
{
	int i;

	ah_vconn_hash_table = ah_kcalloc_atomic(sizeof(ah_vconn_hash_table_t));
	if (!ah_vconn_hash_table) {
		ah_logk(AH_SYS_KMOD_FE, AH_LOG_ERR, "vconn alloc mem in kernel failed\n");
		return -1;
	}

	for (i = 0; i < AH_VCONN_HASH_SIZE; i++) {
		INIT_HLIST_HEAD(&ah_vconn_hash_table->hash[i]);
	}

	return 0;
}

void ah_set_wifi_nbr(int has_wifi_neighbor)
{
    amrp_has_wifi_neighbor = has_wifi_neighbor;
}
#endif //#if defined(AH_SUPPORT_FE_VCON)

