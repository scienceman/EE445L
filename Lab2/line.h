//*************RIT128x96x4_ClearImage**************************************
// Clears the RAM version of the image
// Inputs: none
// Outputs: none
void RIT128x96x4_ClearImage(void);

//*************RIT128x96x4_Line********************************************
// Draws one line in the RAM version of the image
// Inputs: (x1,y1) is the start point
// (x2,y2) is the end point
// color is 0 (off) to 15 (white)
// coordinates range from 0 to MAX, 0 is bottom or left, MAX is top or right
// Outputs: none
void RIT128x96x4_Line(int x1, int y1, int x2, int y2, unsigned char color);

//*************RIT128x96x4_ShowImage**************************************
// Copies the RAM version of the image to the OLED
// Inputs: none
// Outputs: none
void RIT128x96x4_ShowImage(void);