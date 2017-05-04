//#ifndef __DMA_UTIL_
//#define __DMA_UTIL_
//
//#define ALIGN64(some_ptr) (int*)(some_ptr + XAXIDMA_BD_MINIMUM_ALIGNMENT-some_ptr%XAXIDMA_BD_MINIMUM_ALIGNMENT);
//
//int configure_DMA(XAxiDma * AxiDMA) {
//    Config = XAxiDma_LookupConfig(XPAR_AXIDMA_0_DEVICE_ID);
//	if (!Config) {
//		xil_printf("No config found for %d\r\n", XPAR_AXIDMA_0_DEVICE_ID);
//
//		return XST_FAILURE;
//	}
//
//	status = XAxiDma_CfgInitialize(AxiDma, Config);
//
//	if (status != XST_SUCCESS) {
//		xil_printf("Initialization failed %d\r\n", status);
//		return XST_FAILURE;
//	}
//
//	if(XAxiDma_HasSg(AxiDma)){
//		xil_printf("Device configured as SG mode \r\n");
//	}
//
//	XAxiDma_Reset(AxiDma);
//	while (!XAxiDma_ResetIsDone(AxiDma)) {}
//
//    return XST_SUCESS
//}
//
//XAxiDma_BdRing * create_BD(XAxiDMA * AxiDMA, int num_bds, int is_rx) {
//
//	XAxiDma_Bd      bd_template;
//    XAxiDma_BdRing *bd_ring;
//
//    if (is_rx) {
//        bd_ring = XAxiDma_GetRxRing(&AxiDma);
//    } else {
//        bd_ring = XAxiDma_GetTxRing(&AxiDma);
//    }
//
//    int * ring_mem = (int*) malloc((num_bds+1)*XAXIDMA_BD_MINIMUM_ALIGNMENT); // +1 to account for worst case misalignment
//    ring_mem = ALIGN64((int) ring_mem);
//
//    status = XAxiDma_BdRingCreate(bd_ring, (int) ring_mem, (int) ring_mem, XAXIDMA_BD_MINIMUM_ALIGNMENT, num_bds);
//	if (status != XST_SUCCESS)
//	{
//        xil_printf("ERROR! Failed create %s buffer descriptor ring.\r\n", (is_rx ? "s2mm" : "mm2s"));
//		return DMA_PASSTHROUGH_BD_SETUP_FAIL;
//	}
//
//    XAxiDma_UpdateBdRingCDesc(bd_ring);
//
//	// Allocate BDs from template
//	XAxiDma_BdClear(&bd_template);
//	status = XAxiDma_BdRingClone(bd_ring, &bd_template);
//	if (status != XST_SUCCESS)
//	{
//		xil_printf("ERROR! Failed allocate buffer descriptors.\r\n");
//		return -1;
//	}
//
//	// Ring buffer allocate
//	status = XAxiDma_BdRingAlloc(bd_ring, num_bds, &tx_bd_ptr);
//	if (status != XST_SUCCESS)
//	{
//		xil_printf("ERROR! Failed to allocate locations in the buffer descriptor ring.\r\n");
//		return -1;
//	}
//
//    cur_bd_ptr = bd_ptr;
//	buf_addr   = (int)p_dma_buf;
//
//    int i;
//	for(i = 0; i < num_bds; i++)
//	{
//
//		// Set source address of buffer described by current BD
//		status = XAxiDma_BdSetBufAddr(cur_bd_ptr, buf_addr);
//		if (status != XST_SUCCESS)
//		{
//			xil_printf("ERROR! Failed to set source address for this BD.\r\n");
//			return DMA_PASSTHROUGH_BD_SETUP_FAIL;
//		}
//
//		// Set length of buffer
//		status = XAxiDma_BdSetLength(cur_bd_ptr, num_samples_per_bd*sample_size_bytes, ring->MaxTransferLen);
//		if (status != XST_SUCCESS)
//		{
//			xil_printf("ERROR! Failed to set buffer length for this BD.\r\n");
//			return DMA_PASSTHROUGH_BD_SETUP_FAIL;
//		}
//
//		// Set control bits for TX side
//		if (!is_rx)
//		{
//			if (ii == 0)
//			{
//				cr_bits |= XAXIDMA_BD_CTRL_TXSOF_MASK; // Set SOF (tuser) for first BD
//			}
//
//			cr_bits |= XAXIDMA_BD_CTRL_TXEOF_MASK; // Sets tlast to generate an interrupt. Coalescing is set to interrupt once per ring
//		}
//
//		// Set control register
//		XAxiDma_BdSetCtrl(cur_bd_ptr, cr_bits);
//
//		// Increment pointer address to the next BD
//		buf_addr += num_samples_per_bd*sample_size_bytes;
//
//		// Advance current BD pointer for next iteration
//		cur_bd_ptr = XAxiDma_BdRingNext(ring, cur_bd_ptr);
//
//	}
//}
//
//#endif
