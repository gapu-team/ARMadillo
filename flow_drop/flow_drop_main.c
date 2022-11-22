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
#include <stdlib.h>

#include <doca_argp.h>
#include <doca_flow.h>
#include <doca_log.h>

#include <dpdk_utils.h>

DOCA_LOG_REGISTER(FLOW_DROP::MAIN);

/* Sample's Logic */
int flow_drop(int nb_queues, int* src_ip, int* dest_ip, int src_port, int dest_port);

/*
 * Sample main function
 *
 * @argc [in]: command line arguments size
 * @argv [in]: array of command line arguments
 * @return: EXIT_SUCCESS on success and EXIT_FAILURE otherwise
 */
int
main(int argc, char **argv)
{
	FILE * fp;
        fp = fopen("/var/log/doca/doca_drop_flow.log", "w+");
        if(fp == NULL)
        {
            freopen("/var/log/doca/doca_drop_flow.log", "wb", fp);
        }
        doca_log_stream_redirect(fp);

	doca_error_t result;
	int ret;
	int exit_status = EXIT_SUCCESS;
	struct application_dpdk_config dpdk_config = {
		.port_config.nb_ports = 2,
		.port_config.nb_queues = 1,
		.port_config.nb_hairpin_q = 1,
		.sft_config = {0},
	};

	/*
	 * Try modifing flow_drop() signature
	 */

	FILE *config = fopen("config.txt", "r");

	if (config == NULL)
	{
		printf("Error! Could not open config file\n");
		return EXIT_FAILURE;
	}

	int src_ip[4], dest_ip[4], src_port, dest_port;

	fscanf(config, "%d.%d.%d.%d,%d.%d.%d.%d,%d,%d", src_ip, src_ip + 1, src_ip + 2, src_ip + 3, dest_ip, dest_ip + 1, dest_ip + 2, dest_ip + 3, &src_port, &dest_port);
	fclose(config);

	/*
	 * Original code
	 */

	result = doca_argp_init("flow_drop", NULL);
	if (result != DOCA_SUCCESS) {
		DOCA_LOG_ERR("Failed to init ARGP resources: %s", doca_get_error_string(result));
		return EXIT_FAILURE;
	}
	doca_argp_set_dpdk_program(dpdk_init);
	result = doca_argp_start(argc, argv);
	if (result != DOCA_SUCCESS) {
		DOCA_LOG_ERR("Failed to parse sample input: %s", doca_get_error_string(result));
		doca_argp_destroy();
		return EXIT_FAILURE;
	}

	/* update queues and ports */
	result = dpdk_queues_and_ports_init(&dpdk_config);
	if (result != DOCA_SUCCESS) {
		DOCA_LOG_ERR("Failed to update ports and queues");
		dpdk_fini(&dpdk_config);
		doca_argp_destroy();
		return EXIT_FAILURE;
	}

	/* run sample */
	//ret = flow_drop(dpdk_config.port_config.nb_queues);
	ret = flow_drop(dpdk_config.port_config.nb_queues, src_ip, dest_ip, src_port, dest_port);
	if (ret < 0) {
		DOCA_LOG_ERR("flow_drop sample encountered errors");
		exit_status = EXIT_FAILURE;
	}

	/* cleanup resources */
	dpdk_queues_and_ports_fini(&dpdk_config);
	dpdk_fini();

	/* ARGP cleanup */
	doca_argp_destroy();
	return exit_status;
}
