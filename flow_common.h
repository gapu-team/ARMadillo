/*
 * Copyright (c) 2022 NVIDIA CORPORATION & AFFILIATES, ALL RIGHTS RESERVED.
 *
 * This software product is a proprietary product of NVIDIA CORPORATION &
 * AFFILIATES (the "Company") and all right, title, and interest in and to the
 * software product, including all associated intellectual property rights, are
 * and shall remain exclusively with the Company.
 *
 * This software product is governed by the End User License Agreement
 * provided with the software product.
 *
 */

#ifndef FLOW_COMMON_H_
#define FLOW_COMMON_H_

#include <rte_byteorder.h>

#include <doca_flow.h>

#define BE_IPV4_ADDR(a, b, c, d) (RTE_BE32(((uint32_t)a << 24) + (b << 16) + (c << 8) + d))	/* create IPV4 address */
#define SET_MAC_ADDR(addr, a, b, c, d, e, f)\
do {\
	addr[0] = a & 0xff;\
	addr[1] = b & 0xff;\
	addr[2] = c & 0xff;\
	addr[3] = d & 0xff;\
	addr[4] = e & 0xff;\
	addr[5] = f & 0xff;\
} while (0)									/* create source mac address */
#define BUILD_VNI(uint24_vni) (RTE_BE32((uint32_t)uint24_vni << 8))		/* create VNI */
#define DEFAULT_TIMEOUT_US (10000)						/* default timeout for processing entries */
#define NB_ACTIONS_ARR (1)							/* default length for action array */

/*
 * Initialize DOCA Flow library
 *
 * @nb_queues [in]: number of queues the sample will use
 * @mode [in]: doca flow architecture mode
 * @resource [in]: number of meters and counters to configure
 * @nr_shared_resources [in]: total shared resource per type
 * @error [out]: output error
 * @return: 0 on success, negative errno value otherwise and error is set.
 */
int init_doca_flow(int nb_queues, const char *mode, struct doca_flow_resources resource, uint32_t nr_shared_resources[],
	 struct doca_flow_error *error);

/*
 * Initialize DOCA Flow ports
 *
 * @nb_ports [in]: number of ports to create
 * @ports [in]: array of ports to create
 * @is_hairpin [in]: port pair should run if is_hairpin = true
 * @return: 0 on success and negative value otherwise.
 */
int init_doca_flow_ports(int nb_ports, struct doca_flow_port *ports[], bool is_hairpin);

/*
 * Destroy DOCA Flow ports
 *
 * @nb_ports [in]: number of ports to destroy
 * @ports [in]: array of ports to destroy
 */
void destroy_doca_flow_ports(int nb_ports, struct doca_flow_port *ports[]);

#endif /* FLOW_COMMON_H_ */
