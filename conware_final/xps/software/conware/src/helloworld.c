

#include <stdio.h>
#include <xparameters.h>
#include "xvtc.h"
#include "xaxivdma.h"

#define for_x for (x = 0; x < w; x++)
#define for_y for (y = 0; y < h; y++)
#define for_xy for_x for_y
#define screen_iterate for(i = 0; i < 480; i++) for (j = 1; j < 640; j++)
#define game_iterate for(ii = 0; ii < 32; ii++) for (j = 1; j < 640; jj++)

#define DEAD_CELL 0x0000
#define ALIVE_CELL 0x00F0

#define GAME_HEIGHT 32
#define GAME_WIDTH 32


u16 test_image[480][640];
u8 game_grid[GAME_HEIGHT][GAME_WIDTH];
u8 temp_grid[GAME_HEIGHT][GAME_WIDTH];

void raster_game(int x, int y, int width, int height){
	int i,j;

	int x_ratio = (int) ((GAME_WIDTH << 16) / width) + 1;
	int y_ratio = (int) ((GAME_HEIGHT << 16) / height) + 1;
	int x2, y2;

	for (i=1; i<width; i++){
		for (j=1; j<height; j++){
			x2 = ((j*x_ratio)>> 16);
			y2 = ((i*y_ratio)>>16);

			if (game_grid[x2][y2] == 1){
				test_image[x+i][y+j] = ALIVE_CELL;
			} else {
				test_image[x+i][y+j] = DEAD_CELL;
			}
		}
	}

//	for (j=0; j<GAME_HEIGHT; j++){
//		for (i=0; i<GAME_WIDTH; i++){
//			if (game_grid[i][j] == 1){
//				test_image[x+i][y+j] = ALIVE_CELL;
//			} else {
//				test_image[x+i][y+j] = DEAD_CELL;
//			}
//		}
//	}
}

void place_block(int x, int y) {
	if (x >=0 && x < GAME_WIDTH - 1){
		if (y >=0 && y < GAME_HEIGHT -1) {
			game_grid[x+0][y+1] = 1;
			game_grid[x+0][y+0] = 1;
			game_grid[x+1][y+1] = 1;
			game_grid[x+1][y+0] = 1;
		}
	}
}

void evolve(int w, int h)
{
	int x,y,x1,y1,i,j;

	for (j=0; j<GAME_HEIGHT; j++){
		for (i=0; i<GAME_WIDTH; i++){
			int nearest_sum = game_grid[i-1][j-1] + game_grid[i][j-1] + game_grid[i+1][j-1] +
					          game_grid[i-1][j]   +     0             + game_grid[i+1][j]   +
					          game_grid[i-1][j+1] + game_grid[i][j+1] + game_grid[i+1][j+1];
			int current = game_grid[i][j];
			if (current) {
				if (nearest_sum < 2){
					temp_grid[i][j] = 0;
				} else if (nearest_sum > 3){
					temp_grid[i][j] = 0;
				} else {
					temp_grid[i][j] = 1;
				}
			} else {
				if (nearest_sum == 3) {
					temp_grid[i][j] = 1;
				}
			}
		}
	}
	for (i=0; i<GAME_WIDTH; i++){
		for (j=0; j<GAME_HEIGHT; j++){
			game_grid[i][j] = temp_grid[i][j];
			temp_grid[i][j] = 0;
		}
	}
}

int main() {

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

//    XVtc_GetGenerator(&Vtc, original);
//
//    printf("Origin Mode: %d\nHTotal:%d\nHFrontPorchStart:%d\nHSyncStart:%d\nHBackPorchStart:%d\nHActiveStart:%d\n", original->OriginMode, original->HTotal, original->HFrontPorchStart, original->HSyncStart, original->HBackPorchStart, original->HActiveStart);


    print("XVtc_Enable\r\n");
    XVtc_Enable(&Vtc, XVTC_EN_GENERATOR);

    print("Filling frame...\r\n");


    screen_iterate {
    	test_image[i][j] = 0x0abc;
    }

    for (i=0; i<32; i++){
    	for (j=0; j<32; j++){
    		game_grid[i][j] = 0;
    		temp_grid[i][j] = 0;
    	}
    }

    test_image[300][300] = 0x000F;
    game_grid[3][1] = 1;
    game_grid[3][2] = 1;
    game_grid[3][3] = 1;
    game_grid[20][2] = 1;
    //game_grid[0][0] = 1;
    game_grid[31][31] = 1;
    game_grid[0][31] = 1;
    game_grid[31][0] = 1;

    place_block(10,10);



    print("Configuring VDMA...\r\n");
    XAxiVdma_WriteReg(XPAR_AXI_VDMA_0_BASEADDR, XAXIVDMA_CR_OFFSET,  0x03);  // Circular Mode and Start bits set, VDMA MM2S Control (p. 34)
    XAxiVdma_WriteReg(XPAR_AXI_VDMA_0_BASEADDR, XAXIVDMA_HI_FRMBUF_OFFSET, 0x00);  // VDMA MM2S Reg_Index (P. 44)
    XAxiVdma_WriteReg(XPAR_AXI_VDMA_0_BASEADDR, XAXIVDMA_FRMSTORE_OFFSET, 0x01);  // VDMA MM2S Number FRM_Stores (p. 45)
    XAxiVdma_WriteReg(XPAR_AXI_VDMA_0_BASEADDR, XAXIVDMA_MM2S_ADDR_OFFSET + XAXIVDMA_START_ADDR_OFFSET, test_image);  // VDMA MM2S Start Addr 1
    XAxiVdma_WriteReg(XPAR_AXI_VDMA_0_BASEADDR, XAXIVDMA_MM2S_ADDR_OFFSET + XAXIVDMA_STRD_FRMDLY_OFFSET, 1280);  // 1280 bytes, VDMA MM2S FRM_Delay, and Stride
    XAxiVdma_WriteReg(XPAR_AXI_VDMA_0_BASEADDR, XAXIVDMA_MM2S_ADDR_OFFSET + XAXIVDMA_HSIZE_OFFSET, 1280);  // 1280 bytes, VDMA MM2S HSIZE
    XAxiVdma_WriteReg(XPAR_AXI_VDMA_0_BASEADDR, XAXIVDMA_MM2S_ADDR_OFFSET + XAXIVDMA_VSIZE_OFFSET, 480);  // 480 lines, VDMA MM2S VSIZE  (Note: Starts VDMA transaction
    print("Something should be happening!\r\n");

//    raster_game(50,50,200,200);
//    Xil_DCacheFlush();
//    sleep(5);

    while(1){

        evolve(32, 32);
        raster_game(50,50,150,200);
        Xil_DCacheFlush();
        //sleep(1);
    }
    return 0;
}
