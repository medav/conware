

#include <stdio.h>
#include <xparameters.h>
#include "xvtc.h"
#include "xaxivdma.h"
#include "conway.h"

#define for_x for (x = 0; x < w; x++)
#define for_y for (y = 0; y < h; y++)
#define for_xy for_x for_y
#define screen_iterate for(i = 0; i < 480; i++) for (j = 1; j < 640; j++)
#define game_iterate for(ii = 0; ii < 32; ii++) for (j = 1; j < 640; jj++)


short test_image[480][640];
char game_grid[GAME_HEIGHT][GAME_WIDTH];


int main2() {

	XVtc Vtc;
    XVtc_Config *VtcCfgPtr;
    XVtc_Signal *original;
    XVtc_HoriOffsets * offsets;

    int i, j;

    print("Hello, World!\r\n");


    // Enable the VTC module
    print("XVtc_LookupConfig\r\n");
    VtcCfgPtr = XVtc_LookupConfig(XPAR_AXI_VDMA_0_DEVICE_ID);
    print("XVtc_CfgInitialize\r\n");
    XVtc_CfgInitialize(&Vtc, VtcCfgPtr, VtcCfgPtr->BaseAddress);
    print("XVtc_Enable\r\n");
    XVtc_Enable(&Vtc, XVTC_EN_GENERATOR);

    screen_iterate {
    	test_image[i][j] = 0x0abc;
    }

    for (i=0; i<8; i++){
    	for (j=0; j<8; j++){
    		game_grid[i][j] = 0;
    	}
    }

    test_image[300][300] = 0x000F;

    place_block(game_grid, 10,10);
    place_glider(game_grid, 10, 2);

    place_glider(game_grid, 1, 10);
    place_blinker(game_grid, 20, 30);


    print("Configuring VDMA...\r\n");
    XAxiVdma_WriteReg(XPAR_AXI_VDMA_0_BASEADDR, XAXIVDMA_CR_OFFSET,  0x03);  // Circular Mode and Start bits set, VDMA MM2S Control (p. 34)
    XAxiVdma_WriteReg(XPAR_AXI_VDMA_0_BASEADDR, XAXIVDMA_HI_FRMBUF_OFFSET, 0x00);  // VDMA MM2S Reg_Index (P. 44)
    XAxiVdma_WriteReg(XPAR_AXI_VDMA_0_BASEADDR, XAXIVDMA_FRMSTORE_OFFSET, 0x01);  // VDMA MM2S Number FRM_Stores (p. 45)
    XAxiVdma_WriteReg(XPAR_AXI_VDMA_0_BASEADDR, XAXIVDMA_MM2S_ADDR_OFFSET + XAXIVDMA_START_ADDR_OFFSET, test_image);  // VDMA MM2S Start Addr 1
    XAxiVdma_WriteReg(XPAR_AXI_VDMA_0_BASEADDR, XAXIVDMA_MM2S_ADDR_OFFSET + XAXIVDMA_STRD_FRMDLY_OFFSET, 1280);  // 1280 bytes, VDMA MM2S FRM_Delay, and Stride
    XAxiVdma_WriteReg(XPAR_AXI_VDMA_0_BASEADDR, XAXIVDMA_MM2S_ADDR_OFFSET + XAXIVDMA_HSIZE_OFFSET, 1280);  // 1280 bytes, VDMA MM2S HSIZE
    XAxiVdma_WriteReg(XPAR_AXI_VDMA_0_BASEADDR, XAXIVDMA_MM2S_ADDR_OFFSET + XAXIVDMA_VSIZE_OFFSET, 480);  // 480 lines, VDMA MM2S VSIZE  (Note: Starts VDMA transaction
    print("Something should be happening!\r\n");


    while(1){

        evolve(game_grid);
        raster_game(game_grid, test_image, 50,50,8,8);
        Xil_DCacheFlush();
    }
    return 0;
}
