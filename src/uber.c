#include <pebble.h>
#include <uber.h>
#include <stdlib.h>

void createResultsMenu(int count){
  if(count<=0){
    APP_LOG(APP_LOG_LEVEL_DEBUG, "[PEBBLE] No results found"); 


    r_items[0]=(SimpleMenuItem){
      .title="No results found",
      .subtitle=":("
    };
    count++;


  }
  else{
    APP_LOG(APP_LOG_LEVEL_DEBUG, "[PEBBLE] %d results found",count); 

    for(int i=0;i<count;i++){
      r_items[i]=(SimpleMenuItem){
        .title=names[i],
        .subtitle=estimates[i]        

      };
    }
  }

  r_sections[0] = (SimpleMenuSection){
    .title="Nearby cars",
    .num_items = count,
    .items = r_items
  };

}


//AppMessage handlers

/**
 * Called when message is received from PebbleKitJS
 * @param received 
 * @param context
 */
 void in_received_handler(DictionaryIterator *received, void *context) {  




  int count = 0;
  Tuple *tuple = dict_read_first(received);
  while(tuple){
    count ++;
    tuple = dict_read_next(received);
  }
  APP_LOG(APP_LOG_LEVEL_DEBUG, "[PEBBLE] Message received -- %d cars",count); 


  for (int i=0;i<count;i++) {
    Tuple *car = dict_find(received, i); 


    if (car) {
      char* name=car->value->cstring;

          //Formatting
      char* estimate = name;
      while(*estimate!= '|') {
        estimate++;
      }


      *estimate++ = '\0';





  //End of Formatting
      


      
      


      
      names[i]=name;
      estimates[i]=estimate;

      
    }
  }


  createResultsMenu(count);

  
  


  bounds = layer_get_frame(window_get_root_layer(r_window));

  r_menu_layer = simple_menu_layer_create(bounds,r_window, r_sections, 1, NULL);


  layer_add_child(window_get_root_layer(r_window), simple_menu_layer_get_layer(r_menu_layer));
      window_stack_pop(animated);  // pop out searching window
      window_stack_push(r_window, animated);  // pusg results window


    }

    


/**
 * Called when message from PebbleKitJS is dropped due to some reason
 * @param reason
 * @param context
 */
 void in_dropped_handler(AppMessageResult reason, void *context) {  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "[PEBBLE] Message dropped"+reason); 


}


/**
 * Called when message is sent to PebbleKitJS
 * @param sent
 * @param context
 */
 void out_sent_handler(DictionaryIterator *sent, void *context) { 
  APP_LOG(APP_LOG_LEVEL_DEBUG, "[PEBBLE] Message delivered");
}


/**
 * Called when message is not sent to PebbleKitJS
 * @param failed
 * @param reason
 * @param context
 */
 void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) { 

  APP_LOG(APP_LOG_LEVEL_DEBUG, "[PEBBLE] Message delivery failed %d",reason); 
}





//Window  load & unload Handlers


static void window_load(Window *window) {


}


static void splash_window_load(Window *window) {

 splash=gbitmap_create_with_resource( RESOURCE_ID_SPLASH);

 Layer *window_layer = window_get_root_layer(window);
 GRect bounds = layer_get_frame(window_layer);


  // The bitmap layer holds the image for display
 image_layer = bitmap_layer_create(bounds);
 bitmap_layer_set_bitmap(image_layer, splash);
 bitmap_layer_set_alignment(image_layer, GAlignCenter);
 layer_add_child(window_layer, bitmap_layer_get_layer(image_layer));


}



void bluetooth_connection_callback(bool connected) {
 APP_LOG(APP_LOG_LEVEL_INFO, "bluetooth connected=%d", (int) connected);
 if(!connected){
  msg="Bluetooth connection \n lost";
  window_stack_pop_all(animated);
  window_stack_push(window, animated); 


}


}


/**
 * Initializer function to be called when WatchApp is started
 */
 static void init(void) {



  splash_window = window_create(); 
  r_window = window_create(); 

  window_set_window_handlers(splash_window, (WindowHandlers) {

   .load = splash_window_load, 
 });

  window = window_create(); 

  window_set_window_handlers(window, (WindowHandlers) {

   .load = window_load, 
 });




  if (bluetooth_connection_service_peek()) {
    APP_LOG(APP_LOG_LEVEL_INFO, "Phone is connected!");


    bluetooth_connection_service_subscribe(bluetooth_connection_callback);

    


    

    

  //Registering handlers

    app_message_register_inbox_received(in_received_handler); 
    app_message_register_inbox_dropped(in_dropped_handler);
    app_message_register_outbox_sent(out_sent_handler); 
    app_message_register_outbox_failed(out_failed_handler);

    const uint32_t inbound_size = app_message_inbox_size_maximum(); 
    const uint32_t outbound_size = app_message_outbox_size_maximum();

  //open channel
    app_message_open(inbound_size, outbound_size);

  //create category menu

    window_stack_push(splash_window, animated); 

  } else {
    msg="Connect your phone to pebble";
    window_stack_push(window, animated); 

  }
  
}



/**
 * Deinitializer function to be called when WatchApp is closed
 */
 static void deinit(void) {
  window_destroy(window);
  window_destroy(splash_window);
  gbitmap_destroy(splash);

  
}

/**
 * Main function
 * @return
 */
 int main(void) {
  init();
  app_event_loop();
  deinit();
}


