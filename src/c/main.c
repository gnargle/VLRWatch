#include <pebble.h>
#include <stdlib.h>

static Window *s_main_window;

static TextLayer *s_time_layer;
static TextLayer *s_group_layer;

static GFont s_time_font;
static GFont s_group_font;

static GBitmap *s_digit;
static GBitmap *s_digitzero;
static GBitmap *s_digitone;
static GBitmap *s_digittwo;
static GBitmap *s_digitthree;
static GBitmap *s_digitfour;
static GBitmap *s_digitfive;
static GBitmap *s_digitsix;
static GBitmap *s_digitseven;
static GBitmap *s_digiteight;
static GBitmap *s_digitnine;
static GBitmap *s_digit2;
static GBitmap *s_skull;

static BitmapLayer *s_digit_layer;
static BitmapLayer *s_digit2_layer;
static BitmapLayer *s_skull_layer;

int rand_lim(int limit) {
/* return a random number between 0 and limit inclusive.
 */

    int divisor = RAND_MAX/(limit+1);
    int retval;

    do { 
        retval = rand() / divisor;
    } while (retval > limit);

    return retval;
}

static void determine_colour(int r){
  GColor* pal = gbitmap_get_palette(s_digit);
  GColor* pal2 = gbitmap_get_palette(s_digit2);
  if (r == 0){
    text_layer_set_text_color(s_time_layer,GColorRed);
    text_layer_set_text_color(s_group_layer,GColorRed);
    pal[1] = GColorRed;
    pal2[1] = GColorRed;
  }
  if (r==1){
    text_layer_set_text_color(s_time_layer,GColorGreen);
    text_layer_set_text_color(s_group_layer,GColorGreen);
    pal[1] = GColorGreen;
    pal2[1] = GColorGreen;
  }
  if (r==2){
    text_layer_set_text_color(s_time_layer,GColorBlue);
    text_layer_set_text_color(s_group_layer,GColorBlue);
    pal[1] = GColorBlue;
    pal2[1] = GColorBlue;
  }
  if (r==3){
    text_layer_set_text_color(s_time_layer,GColorCyan);
    text_layer_set_text_color(s_group_layer,GColorCyan);
    pal[1] = GColorCyan;
    pal2[1] = GColorCyan;
  }
  if (r==4){
    text_layer_set_text_color(s_time_layer,GColorMagenta);
    text_layer_set_text_color(s_group_layer,GColorMagenta);
    pal[1] = GColorMagenta;
    pal2[1] = GColorMagenta;
  }
  if (r==5){
    text_layer_set_text_color(s_time_layer,GColorYellow);
    text_layer_set_text_color(s_group_layer,GColorYellow);
    pal[1] = GColorYellow;
    pal2[1] = GColorYellow;
  }
}

static void determine_group(int r){
  static const char solo[6] = "SOLO";
  static const char pair[6] = "PAIR";
  if (r==0){
    text_layer_set_text(s_group_layer,solo);
  }
  if (r==1){
    text_layer_set_text(s_group_layer,pair);
  }
}

static void determine_big(){
  time_t temp = time(NULL);
  struct tm *big_time = localtime(&temp);
  char s_buffer1[8];
  strftime(s_buffer1, sizeof(s_buffer1), "%I:%M", big_time);
  layer_set_hidden(text_layer_get_layer(s_time_layer),true);
  //bitmap_layer_destroy(s_digit_layer);
  //bitmap_layer_destroy(s_digit2_layer);
  if (s_buffer1[0] == '1'){
    s_digit2 = s_digitone;
    
    layer_set_hidden(bitmap_layer_get_layer(s_digit2_layer),false);
    if (s_buffer1[1] == '0'){
      //gbitmap_destroy(s_bitmap);
      s_digit = s_digitzero;
    }
    else if (s_buffer1[1] == '1'){
      //gbitmap_destroy(s_bitmap);
      s_digit = s_digitone;
    }
    else{
      //gbitmap_destroy(s_bitmap);
      s_digit = s_digittwo;
    }
    if (s_buffer1[0] == '1' && s_buffer1[1] == '2' && s_buffer1[3] == '0' && s_buffer1[4] == '0'){
      layer_set_hidden(bitmap_layer_get_layer(s_skull_layer),false);
      layer_set_hidden(bitmap_layer_get_layer(s_digit_layer),true);
      layer_set_hidden(bitmap_layer_get_layer(s_digit2_layer),true);
    }
  }
  else{
    s_digit2 = s_digitone;
    layer_set_hidden(bitmap_layer_get_layer(s_digit2_layer),true);
    if (s_buffer1[1] == '1'){
      //gbitmap_destroy(s_bitmap);
      s_digit = s_digitone;
    }
    else if (s_buffer1[1] == '2'){
      //gbitmap_destroy(s_bitmap);
      s_digit = s_digittwo;
    }
    else if (s_buffer1[1] == '3'){
      //gbitmap_destroy(s_bitmap);
      s_digit = s_digitthree;
    }
    else if (s_buffer1[1] == '4'){
      //gbitmap_destroy(s_bitmap);
      s_digit = s_digitfour;
    }
    else if (s_buffer1[1] == '5'){
      //gbitmap_destroy(s_bitmap);
      s_digit = s_digitfive;
    }
    else if (s_buffer1[1] == '6'){
      //gbitmap_destroy(s_bitmap);
      s_digit = s_digitsix;
    }
    else if (s_buffer1[1] == '7'){
      //gbitmap_destroy(s_bitmap);
      s_digit = s_digitseven;
    }
    else if (s_buffer1[1] == '8'){
      //gbitmap_destroy(s_bitmap);
      s_digit = s_digiteight;
    }
    else{
      //gbitmap_destroy(s_bitmap);
      s_digit = s_digitnine;
    }
    if (s_buffer1[0] == '0' && s_buffer1[1] == '0' && s_buffer1[3] == '0' && s_buffer1[4] == '0'){
      layer_set_hidden(bitmap_layer_get_layer(s_skull_layer),false);
      layer_set_hidden(bitmap_layer_get_layer(s_digit_layer),true);
      layer_set_hidden(bitmap_layer_get_layer(s_digit2_layer),true);
    }
  }
}


static void update_time(){
  //Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  //Write the current hours and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ? 
          "%H:%M" : "%I:%M", tick_time);
  
  //Display this time on the TextLayer
  text_layer_set_text(s_time_layer,s_buffer);
  if ((s_buffer[0] == '1' && s_buffer[1] == '2' && s_buffer[3] == '0' && s_buffer[4] == '0')
     || (s_buffer[0] == '0' && s_buffer[1] == '0' && s_buffer[3] == '0' && s_buffer[4] == '0')){
    layer_set_hidden(bitmap_layer_get_layer(s_skull_layer),false);
    layer_set_hidden(bitmap_layer_get_layer(s_digit_layer),true);
    layer_set_hidden(bitmap_layer_get_layer(s_digit2_layer),true);
  }
  else{
    layer_set_hidden(bitmap_layer_get_layer(s_skull_layer),true);
    layer_set_hidden(bitmap_layer_get_layer(s_digit_layer),false);
    time_t temp1 = time(NULL);
    struct tm *tick1_time = localtime(&temp1);
    char s_buffer1[8];
    strftime(s_buffer1, sizeof(s_buffer1), "%I:%M", tick1_time);
    if ((s_buffer1[0] == '1') || (s_buffer1[0] == '2' && s_buffer1[1] != '4')){
      layer_set_hidden(bitmap_layer_get_layer(s_digit2_layer),false);
    }
    else{
      layer_set_hidden(bitmap_layer_get_layer(s_digit2_layer),true);
    }
  }
}

static void update_time_colour_change(){
  //Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  //Write the current hours and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ? 
          "%H:%M" : "%I:%M", tick_time);
  gbitmap_destroy(s_digit);
  gbitmap_destroy(s_digit2);
  determine_big();
  int r = rand_lim(5);
  determine_colour(r);
  int i = rand_lim(1);
  determine_group(i);
  //Display this time on the TextLayer
  text_layer_set_text(s_time_layer,s_buffer);
  bitmap_layer_set_bitmap(s_digit_layer,s_digit);
  bitmap_layer_set_bitmap(s_digit2_layer,s_digit2);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed){
  if((units_changed & MINUTE_UNIT)!= 0){
    update_time();
  }
  if((units_changed & HOUR_UNIT)!= 0){
    update_time_colour_change();
  }
}

static void timer_callback(void *context){
  layer_set_hidden(text_layer_get_layer(s_time_layer),true);
  layer_set_hidden(bitmap_layer_get_layer(s_digit_layer),false);
  time_t temp = time(NULL);
  struct tm *timer_time = localtime(&temp);
  char s_buffer1[8];
  strftime(s_buffer1, sizeof(s_buffer1), "%I:%M", timer_time);
  if (s_buffer1[0] == '1' || (s_buffer1[0] == '2' && s_buffer1[1] != '4')){
    layer_set_hidden(bitmap_layer_get_layer(s_digit2_layer),false);
  }
  else{
    layer_set_hidden(bitmap_layer_get_layer(s_digit2_layer),true);
  }
  layer_set_hidden(bitmap_layer_get_layer(s_skull_layer),true);
}

static void accel_tap_handler(AccelAxisType axis, int32_t direction){
  // A tap event occured
  
  time_t temp = time(NULL);
  struct tm *tap_time = localtime(&temp);
  char s_buffer1[8];
  strftime(s_buffer1, sizeof(s_buffer1), clock_is_24h_style() ? 
          "%H:%M" : "%I:%M", tap_time);
  if ((s_buffer1[0] == '1' && s_buffer1[1] == '2' && s_buffer1[3] == '0' && s_buffer1[4] == '0')
     || (s_buffer1[0] == '0' && s_buffer1[1] == '0' && s_buffer1[3] == '0' && s_buffer1[4] == '0')){
      layer_set_hidden(bitmap_layer_get_layer(s_skull_layer),false);
      layer_set_hidden(bitmap_layer_get_layer(s_digit_layer),true);
      layer_set_hidden(bitmap_layer_get_layer(s_digit2_layer),true);
  }
  else{
    layer_set_hidden(text_layer_get_layer(s_time_layer),false);
    layer_set_hidden(bitmap_layer_get_layer(s_digit_layer),true);
    layer_set_hidden(bitmap_layer_get_layer(s_digit2_layer),true);
    const int delay_ms = 5000;
  
    //Schedule the timer
    app_timer_register(delay_ms, timer_callback, NULL);
  }
  
}

static void main_window_load(Window *window){
  //Get info about Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  //Create the TextLayer with specific bounds
  s_time_layer = text_layer_create(
  GRect(4,PBL_IF_ROUND_ELSE(58,52), bounds.size.w, 50));
  s_group_layer = text_layer_create(
  GRect(0,PBL_IF_ROUND_ELSE(120,114),bounds.size.w,48));
  
  //Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer,GColorBlack);
  text_layer_set_background_color(s_group_layer,GColorBlack);
  
  int r = rand_lim(5);
  s_skull = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SKULL);
  s_skull_layer = bitmap_layer_create(GRect(PBL_IF_ROUND_ELSE(34,22),22,PBL_IF_ROUND_ELSE(107,95),105));
  layer_set_hidden(bitmap_layer_get_layer(s_skull_layer),true);
  
  s_digit_layer = bitmap_layer_create(GRect(64,30,100,86));
  s_digit2_layer = bitmap_layer_create(GRect(40,30,76,86));
  s_digit = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ZERO);
  s_digit2 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ZERO);
  s_digitzero = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ZERO);
  s_digitone = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ONE);
  s_digittwo = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TWO);
  s_digitthree = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_THREE);
  s_digitfour = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_FOUR);
  s_digitfive = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_FIVE);
  s_digitsix = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SIX);
  s_digitseven = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SEVEN);
  s_digiteight = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_EIGHT);
  s_digitnine = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NINE);
  
  determine_big();
  determine_colour(r);
  int i = rand_lim(1);
  determine_group(i);
  
  //Create GFont
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PENDULE_ORNAMENTAL_48));
  s_group_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DIGITAL_32));
  
  //Apply to TextLayer
  text_layer_set_font(s_time_layer,s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  text_layer_set_font(s_group_layer,s_group_font);
  text_layer_set_text_alignment(s_group_layer,GTextAlignmentCenter);
  
  bitmap_layer_set_compositing_mode(s_digit_layer, GCompOpSet);
  bitmap_layer_set_bitmap(s_digit_layer,s_digit);
  bitmap_layer_set_compositing_mode(s_digit2_layer, GCompOpSet);
  bitmap_layer_set_bitmap(s_digit2_layer,s_digit2);
  bitmap_layer_set_compositing_mode(s_skull_layer, GCompOpSet);
  bitmap_layer_set_bitmap(s_skull_layer,s_skull);
  
  //Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_group_layer));
  layer_add_child(window_layer, bitmap_layer_get_layer(s_digit_layer));
  layer_add_child(window_layer, bitmap_layer_get_layer(s_digit2_layer));
  layer_add_child(window_layer, bitmap_layer_get_layer(s_skull_layer));
}

static void main_window_unload(Window *window){
 // gbitmap_destroy(s_digit);
 // gbitmap_destroy(s_digit2);
  gbitmap_destroy(s_digitone);
  gbitmap_destroy(s_digittwo);
  gbitmap_destroy(s_digitthree);
  gbitmap_destroy(s_digitfour);
  gbitmap_destroy(s_digitfive);
  gbitmap_destroy(s_digitsix);
  gbitmap_destroy(s_digitseven);
  gbitmap_destroy(s_digiteight);
  gbitmap_destroy(s_digitnine);
  bitmap_layer_destroy(s_digit_layer);
  bitmap_layer_destroy(s_digit2_layer);
  gbitmap_destroy(s_skull);
  bitmap_layer_destroy(s_skull_layer);
  
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_group_layer);
  //Unload GFont
  fonts_unload_custom_font(s_time_font);
  fonts_unload_custom_font(s_group_font);
}

static void init() {
  srand(time(NULL));
  //Create main window element and assign to pointer
  s_main_window = window_create();
  
  //Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers){
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  window_set_background_color(s_main_window,GColorBlack);
  
  //Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT|HOUR_UNIT, tick_handler);
  accel_tap_service_subscribe(accel_tap_handler);
  
  //Show the window on the watch, with animated = true
  window_stack_push(s_main_window, true);
  
  update_time();
}

static void deinit(){
  window_destroy(s_main_window);
  tick_timer_service_unsubscribe();
  accel_tap_service_unsubscribe();
}

int main(void){
  init();
  app_event_loop();
  deinit();
}