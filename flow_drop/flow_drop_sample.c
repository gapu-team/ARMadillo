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

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <rte_byteorder.h>

#include <doca_log.h>
#include <doca_flow.h>

#include "flow_common.h"

DOCA_LOG_REGISTER(FLOW_DROP);

/*
 * Create DOCA Flow pipe that forwards all the traffic to the other port
 *
 * @port [in]: port of the pipe
 * @port_id [in]: port ID of the pipe
 * @pipe [out]: created pipe pointer
 * @error [out]: output error
 * @return: 0 on success, negative value otherwise and error is set.
 */
int
create_hairpin_pipe(struct doca_flow_port *port, int port_id, struct doca_flow_pipe **pipe, struct doca_flow_error *error)
{
	struct doca_flow_match match;
	struct doca_flow_actions actions, *actions_arr[NB_ACTIONS_ARR];
	struct doca_flow_fwd fwd;
	struct doca_flow_pipe_cfg pipe_cfg = {0};

	memset(&match, 0, sizeof(match));
	memset(&actions, 0, sizeof(actions));
	memset(&fwd, 0, sizeof(fwd));
	memset(&pipe_cfg, 0, sizeof(pipe_cfg));

	pipe_cfg.attr.name = "HAIRPIN_PIPE";
	pipe_cfg.attr.type = DOCA_FLOW_PIPE_BASIC;
	pipe_cfg.match = &match;
	actions_arr[0] = &actions;
	pipe_cfg.actions = actions_arr;
	pipe_cfg.attr.is_root = false;
	pipe_cfg.attr.nb_actions = NB_ACTIONS_ARR;
	pipe_cfg.port = port;

	/* forwarding traffic to other port */
	fwd.type = DOCA_FLOW_FWD_PORT;
	fwd.port_id = port_id ^ 1;

	*pipe = doca_flow_pipe_create(&pipe_cfg, &fwd, NULL, error);
	if (*pipe == NULL)
		return -1;
	return 0;
}

/*
 * Add DOCA Flow pipe entry to the hairpin pipe
 *
 * @pipe [in]: pipe of the entry
 * @port [in]: port of the entry
 * @error [out]: output error
 * @return: 0 on success, negative value otherwise and error is set.
 */
int
add_hairpin_pipe_entry(struct doca_flow_pipe *pipe, struct doca_flow_port *port, struct doca_flow_error *error)
{
	struct doca_flow_match match;
	struct doca_flow_actions actions;
	struct doca_flow_pipe_entry *entry;
	int result;
	int num_of_entries = 1;

	memset(&match, 0, sizeof(match));
	memset(&actions, 0, sizeof(actions));

	entry = doca_flow_pipe_add_entry(0, pipe, &match, &actions, NULL, NULL, 0, NULL, error);
	result = doca_flow_entries_process(port, 0, DEFAULT_TIMEOUT_US, num_of_entries);
	if (result != num_of_entries || doca_flow_pipe_entry_get_status(entry) != DOCA_FLOW_ENTRY_STATUS_SUCCESS)
		return -1;

	return 0;
}

/*
 * Create DOCA Flow pipe with 5 tuple match and fwd drop action. miss fwd to hairpin pipe
 *
 * @port [in]: port of the pipe
 * @hairpin_pipe [in]: pipe to forward the traffic that didn't hit the pipe rules
 * @pipe [out]: created pipe pointer
 * @error [out]: output error
 * @return: 0 on success, negative value otherwise and error is set.
 */
int
create_drop_pipe(struct doca_flow_port *port, struct doca_flow_pipe *hairpin_pipe, struct doca_flow_pipe **pipe, struct doca_flow_error *error)
{
	struct doca_flow_match match;
	struct doca_flow_actions actions, *actions_arr[NB_ACTIONS_ARR];
	struct doca_flow_monitor monitor;
	struct doca_flow_fwd fwd;
	struct doca_flow_fwd fwd_miss;
	struct doca_flow_pipe_cfg pipe_cfg = {0};

	memset(&match, 0, sizeof(match));
	memset(&actions, 0, sizeof(actions));
	memset(&monitor, 0, sizeof(monitor));
	memset(&fwd, 0, sizeof(fwd));
	memset(&fwd_miss, 0, sizeof(fwd_miss));
	memset(&pipe_cfg, 0, sizeof(pipe_cfg));

	pipe_cfg.attr.name = "DROP_PIPE";
	pipe_cfg.attr.type = DOCA_FLOW_PIPE_BASIC;
	pipe_cfg.match = &match;
	actions_arr[0] = &actions;
	pipe_cfg.actions = actions_arr;
	pipe_cfg.monitor = &monitor;
	pipe_cfg.attr.is_root = true;
	pipe_cfg.attr.nb_actions = NB_ACTIONS_ARR;
	pipe_cfg.port = port;

	/* 5 tuple match */
	match.out_l4_type = DOCA_PROTO_TCP;
	match.out_src_ip.type = DOCA_FLOW_IP4_ADDR;
	match.out_src_ip.ipv4_addr = 0xffffffff;
	match.out_dst_ip.type = DOCA_FLOW_IP4_ADDR;
	match.out_dst_ip.ipv4_addr = 0xffffffff;
	match.out_src_port = 0xffff;
	match.out_dst_port = 0xffff;

	monitor.flags |= DOCA_FLOW_MONITOR_COUNT;

	fwd.type = DOCA_FLOW_FWD_DROP;

	fwd_miss.type = DOCA_FLOW_FWD_PIPE;
	fwd_miss.next_pipe = hairpin_pipe;

	*pipe = doca_flow_pipe_create(&pipe_cfg, &fwd, &fwd_miss, error);
	if (*pipe == NULL)
		return -1;
	return 0;
}

/*
 * Add DOCA Flow pipe entry to the drop pipe with example 5 tuple to match
 *
 * @pipe [in]: pipe of the entry
 * @port [in]: port of the entry
 * @entry [out]: created entry pointer
 * @error [out]: output error
 * @return: 0 on success, negative value otherwise and error is set.
 */
int
add_drop_pipe_entry(struct doca_flow_pipe *pipe, struct doca_flow_port *port, struct doca_flow_pipe_entry **entry, struct doca_flow_error *error, int* src_ip, int* dest_ip, int src_p, int dest_p)
{
	struct doca_flow_match match;
	struct doca_flow_actions actions;
	int result;
	int num_of_entries = 1;

	/* example 5-tuple to drop */
	doca_be32_t dst_ip_addr = BE_IPV4_ADDR(*dest_ip, *(dest_ip + 1), *(dest_ip + 2), *(dest_ip + 3));
	doca_be32_t src_ip_addr = BE_IPV4_ADDR(*src_ip, *(src_ip + 1), *(src_ip + 2), *(src_ip + 3));
	doca_be16_t dst_port = rte_cpu_to_be_16(dest_p);
	doca_be16_t src_port = rte_cpu_to_be_16(src_p);

	memset(&match, 0, sizeof(match));
	memset(&actions, 0, sizeof(actions));

	match.out_dst_ip.ipv4_addr = dst_ip_addr;
	match.out_src_ip.ipv4_addr = src_ip_addr;
	match.out_dst_port = dst_port;
	match.out_src_port = src_port;

	*entry = doca_flow_pipe_add_entry(0, pipe, &match, &actions, NULL, NULL, 0, NULL, error);
	result = doca_flow_entries_process(port, 0, DEFAULT_TIMEOUT_US, num_of_entries);
	if (result != num_of_entries || doca_flow_pipe_entry_get_status(*entry) != DOCA_FLOW_ENTRY_STATUS_SUCCESS)
		return -1;

	return 0;
}

/*
 * Run flow_drop sample
 *
 * @nb_queues [in]: number of queues the sample will use
 * @return: 0 on success and negative value otherwise.
 */
int
flow_drop(int nb_queues, int* src_ip, int* dest_ip, int src_port, int dest_port)
{
	/*
	 * src_ip - int array of 4 octets for source ip
	 * dest_ip - int array of 4 octets for destination ip
	 * src_port - int number of source port
	 * dest_port - int number of destination port
	 */

	FILE * fp;
        fp = fopen("/var/log/doca/doca_drop_flow.log", "w+");
        if(fp == NULL)
        {
            freopen("/var/log/doca/doca_drop_flow.log", "wb", fp);
        }
        doca_log_stream_redirect(fp);	

	const int nb_ports = 2;
	struct doca_flow_resources resource = {0};
	uint32_t nr_shared_resources[DOCA_FLOW_SHARED_RESOURCE_MAX] = {0};
	struct doca_flow_port *ports[nb_ports];
	struct doca_flow_pipe *pipe;
	struct doca_flow_pipe *hairpin_pipe;
	struct doca_flow_pipe_entry *entry[nb_ports];
	struct doca_flow_query query_stats;
	struct doca_flow_error error;
	int port_id;
	int result;
	FILE *fd;
	char file_name[128];

	resource.nb_counters = 2;

	if (init_doca_flow(nb_queues, "vnf,hws", resource, nr_shared_resources, &error) < 0) {
		DOCA_LOG_ERR("Failed to init DOCA Flow - %s (%u)", error.message, error.type);
		return -1;
	}

	if (init_doca_flow_ports(nb_ports, ports, true)) {
		DOCA_LOG_ERR("Failed to init DOCA ports");
		doca_flow_destroy();
		return -1;
	}

	for (port_id = 0; port_id < nb_ports; port_id++) {
		result = create_hairpin_pipe(ports[port_id], port_id, &hairpin_pipe, &error);
		if (result < 0) {
			DOCA_LOG_ERR("Failed to add hairpin pipe - %s (%u)", error.message, error.type);
			destroy_doca_flow_ports(nb_ports, ports);
			doca_flow_destroy();
			return -1;
		}

		result = add_hairpin_pipe_entry(hairpin_pipe, ports[port_id], &error);
		if (result < 0) {
			DOCA_LOG_ERR("Failed to add hairpin entry - %s (%u)", error.message, error.type);
			destroy_doca_flow_ports(nb_ports, ports);
			doca_flow_destroy();
			return -1;
		}

		result = create_drop_pipe(ports[port_id], hairpin_pipe, &pipe, &error);
		if (result < 0) {
			DOCA_LOG_ERR("Failed to create pipe - %s (%u)", error.message, error.type);
			destroy_doca_flow_ports(nb_ports, ports);
			doca_flow_destroy();
			return -1;
		}

		result = add_drop_pipe_entry(pipe, ports[port_id], &entry[port_id], &error, src_ip, dest_ip, src_port, dest_port);
		if (result < 0) {
			DOCA_LOG_ERR("Failed to add entry - %s (%u)", error.message, error.type);
			destroy_doca_flow_ports(nb_ports, ports);
			doca_flow_destroy();
			return -1;
		}
	}

	/* wait few seconds for packets to arrive so query will not return zero */
	DOCA_LOG_INFO("Wait few seconds for packets to arrive");
	while(1)
	{
		sleep(5);

		for (port_id = 0; port_id < nb_ports; port_id++) {
			snprintf(file_name, sizeof(file_name) - 1, "port_%d_info.txt", port_id);

			fd = fopen(file_name, "w");
			if (fd == NULL) {
				DOCA_LOG_ERR("Failed to open the file %s", file_name);
				destroy_doca_flow_ports(nb_ports, ports);
				doca_flow_destroy();
				return -1;
			}

			/* dump port info to a file */
			doca_flow_port_pipes_dump(ports[port_id], fd);
			fclose(fd);

			if (doca_flow_query(entry[port_id], &query_stats) < 0) {
				DOCA_LOG_ERR("Failed to query entry - %s (%u)", error.message, error.type);
				destroy_doca_flow_ports(nb_ports, ports);
				doca_flow_destroy();
				return -1;
			}
			DOCA_LOG_INFO("Port %d:", port_id);
			DOCA_LOG_INFO("Total bytes: %ld", query_stats.total_bytes);
			DOCA_LOG_INFO("Total packets: %ld", query_stats.total_pkts);
		}
	}

	destroy_doca_flow_ports(nb_ports, ports);
	doca_flow_destroy();
	return 0;
}
