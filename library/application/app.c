#include "app.h"

/* Process The Uint32_t "*temp" Value And Uint8_t "*data"  */
// void APP_Temp_Processing(uint32_t *temp, uint8_t *data, uint8_t **count, uint8_t *enable_flash)
// {
//     if (*count == 0)
//         {
//             *temp |= ((*data*16) << 24);
//             *count++;
//             // Queue_ForcePush*data(&queue, *data);
//         }
//         else if (*count == 1)
//         {
//             *temp |= (*data << 24);
//             *count++;
//             // Queue_ForcePush*data(&queue, *data);
//         }
//         else if (*count == 2)
//         {
//             *temp |= ((*data*16) << 16);
//             *count++;
//             // Queue_ForcePush*data(&queue, *data);
//         }
//         else if (*count == 3)
//         {
//             *temp |= (*data << 16);
//             *count++;
//             // Queue_ForcePush*data(&queue, *data);
//         }
//         else if (*count == 4)
//         {
//             *temp |= ((*data*16) << 8);
//             *count++;
//             // Queue_ForcePush*data(&queue, *data);
//         }
//         else if (*count == 5)
//         {
//             *temp |= (*data << 8);
//             *count++;
//             // Queue_ForcePush*data(&queue, *data);
//         }
//         else if (*count == 6)
//         {
//             *temp |= (*data*16);
//             *count++;
//             // Queue_ForcePush*data(&queue, *data);
//         }
//         else if (*count == 7)
//         {
//             *temp |= (*data);
//             if (enable_flash == 1)
//             {
//                 Flash_Program(address, *temp);
//             }
//             *temp = 0;
//             address += 0x4;
//             *count = 0;
//             // Queue_ForcePush*data(&queue, *data);
//         }

// }