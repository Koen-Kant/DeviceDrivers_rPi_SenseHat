#include "oslib/wimp.h"

int main()
{
   wimp_t task;
   wimp_event_no event;
   wimp_block block;
   osbool quit = FALSE;

   task = wimp_initialise(wimp_VERSION_RO3, "test file", NULL, NULL);
   while(!quit)
   {
      event = wimp_poll(wimp_MASK_NULL, &block, NULL);
      switch(event)
      {
         case wimp_USER_MESSAGE:
         case wimp_USER_MESSAGE_RECORDED:
            if(block.message.action == message_QUIT)
            {
               quit = TRUE;
            }
            break;
      }
   }
   wimp_close_down(task);
   return 0;
}