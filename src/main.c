#include "pebble.h"

#define KEY_COLOR_NR 0
#define KEY_INVERTED 1
#define KEY_TWELVEHR 2	
#define KEY_SHOWDATE 3

#define KEY_COL00 4
#define KEY_COL01 5
#define KEY_COL02 6

#define KEY_COL10 7
#define KEY_COL11 8
#define KEY_COL12 9

#define KEY_COL20 10
#define KEY_COL21 11
#define KEY_COL22 12

#define KEY_COL30 13
#define KEY_COL31 14
#define KEY_COL32 15

// #define ALTFONT 1
 #define NEUFONT 1

static int incer =0 ;
static bool twelvehr = false;
static bool showdate =false;
static bool altfont = true;
static GColor col0;
static GColor col1;
static GColor col2;
static GColor col3;

static char *s_buffer;
static Window *s_main_window;

static BitmapLayer *links_oben_layer;
static BitmapLayer *rechts_oben_layer;
static BitmapLayer *bar_layer;
static BitmapLayer *links_unten_layer;
static BitmapLayer *rechts_unten_layer;

static TextLayer *status_layer;
static GBitmap *IMAGE_BARNULL;

#if defined(ALTFONT)

static GBitmap *IMAGE_0;
static GBitmap *IMAGE_1;
static GBitmap *IMAGE_2;
static GBitmap *IMAGE_3;
static GBitmap *IMAGE_4;
static GBitmap *IMAGE_5;
static GBitmap *IMAGE_6;
static GBitmap *IMAGE_7;
static GBitmap *IMAGE_8;
static GBitmap *IMAGE_9;
static GBitmap *IMAGE_BAR;

#elif defined(NEUFONT)

static GBitmap *IMAGE_0_alt;
static GBitmap *IMAGE_1_alt;
static GBitmap *IMAGE_2_alt;
static GBitmap *IMAGE_3_alt;
static GBitmap *IMAGE_4_alt;
static GBitmap *IMAGE_5_alt;
static GBitmap *IMAGE_6_alt;
static GBitmap *IMAGE_7_alt;
static GBitmap *IMAGE_8_alt;
static GBitmap *IMAGE_9_alt;
static GBitmap *IMAGE_BAR_alt;

#endif

static GBitmap *V_IMAGE;

static GColor Textcolour; 
static bool prev_bt_status = false;

// Placeholders
static void redaw_entire_screen();


//  BLIT DIGIT ------------------------------------------------------------------------------------------------------------------------------
static void inbox_received_handler(DictionaryIterator *iter, void *context) {

  Tuple *twelve_t = dict_find(iter, KEY_TWELVEHR);
  Tuple *show_t = dict_find(iter, KEY_SHOWDATE);
	
  Tuple *col00_t = dict_find(iter, KEY_COL00);
  Tuple *col01_t = dict_find(iter, KEY_COL01);
  Tuple *col02_t = dict_find(iter, KEY_COL02);

  Tuple *col10_t = dict_find(iter, KEY_COL10);
  Tuple *col11_t = dict_find(iter, KEY_COL11);
  Tuple *col12_t = dict_find(iter, KEY_COL12);

	Tuple *col20_t = dict_find(iter, KEY_COL20);
  Tuple *col21_t = dict_find(iter, KEY_COL21);
  Tuple *col22_t = dict_find(iter, KEY_COL22);

	Tuple *col30_t = dict_find(iter, KEY_COL30);
  Tuple *col31_t = dict_find(iter, KEY_COL31);
  Tuple *col32_t = dict_find(iter, KEY_COL32);
	
	int red;
	int green;
	int blue;
	
	if (twelve_t) {
		twelvehr = twelve_t->value->int8;
	}	
	
	if (show_t) {
		showdate = show_t->value->int8;		
	}
	
	if (col00_t != 0) {
		red = col00_t->value->int32;
		green= col01_t->value->int32;
		blue=col02_t->value->int32;
	
		col0=GColorFromRGB(red, green, blue);
//		APP_LOG(APP_LOG_LEVEL_DEBUG, "Color0: %d %d %d", red, green, blue);
	
	  persist_write_int(KEY_COL00, red);
    persist_write_int(KEY_COL01, green);
    persist_write_int(KEY_COL02, blue);
	}
	
	if (col10_t != 0) {
		red = col10_t->value->int32;
		green= col11_t->value->int32;
		blue=col12_t->value->int32;
	
		col1=GColorFromRGB(red, green, blue);
//		APP_LOG(APP_LOG_LEVEL_DEBUG, "Color0: %d %d %d", red, green, blue);

	  persist_write_int(KEY_COL10, red);
    persist_write_int(KEY_COL11, green);
    persist_write_int(KEY_COL12, blue);		
	}
	
	if (col20_t != 0) {
		red = col20_t->value->int32;
		green= col21_t->value->int32;
		blue=col22_t->value->int32;
	
		col2=GColorFromRGB(red, green, blue);
//		APP_LOG(APP_LOG_LEVEL_DEBUG, "Color0: %d %d %d", red, green, blue);

	  persist_write_int(KEY_COL20, red);
    persist_write_int(KEY_COL21, green);
    persist_write_int(KEY_COL22, blue);
	}
	
	if (col30_t != 0) {
		red = col30_t->value->int32;
		green= col31_t->value->int32;
		blue=col32_t->value->int32;
	
		col3=GColorFromRGB(red, green, blue);
//		APP_LOG(APP_LOG_LEVEL_DEBUG, "Color0: %d %d %d", red, green, blue);

	  persist_write_int(KEY_COL30, red);
    persist_write_int(KEY_COL31, green);
    persist_write_int(KEY_COL32, blue);
}

		persist_write_bool(KEY_TWELVEHR, twelvehr);
		persist_write_bool(KEY_SHOWDATE, showdate);
	
  redaw_entire_screen();
}

static void blit_digit(void *bitmap_layer, int image) {

#if defined(NEUFONT)  
		switch (image)
								{
                               case 0:
                                               V_IMAGE=IMAGE_0_alt;
                                              break;
                               case 1:
                                               V_IMAGE=IMAGE_1_alt;
                                              break;
                               case 2:
                                               V_IMAGE=IMAGE_2_alt;
                                              break;
                               case 3:
                                               V_IMAGE=IMAGE_3_alt;
                                              break;
                               case 4:
                                               V_IMAGE=IMAGE_4_alt;
                                              break;
                               case 5:
                                               V_IMAGE=IMAGE_5_alt;
                                              break;
                               case 6:
                                               V_IMAGE=IMAGE_6_alt;
                                              break;
                               case 7:
                                               V_IMAGE=IMAGE_7_alt;
                                              break;
                               case 8:
                                               V_IMAGE=IMAGE_8_alt;
                                              break;
                               case 9:
                                               V_IMAGE=IMAGE_9_alt;
                                              break;
                               case 10:
                                               V_IMAGE=IMAGE_BAR_alt;
																							 break;
									 						 case 11:
                                               V_IMAGE=IMAGE_BARNULL;
                }

#elif defined(ALTFONT)
	
		switch (image)
								{
                               case 0:
                                               V_IMAGE=IMAGE_0;
                                              break;
                               case 1:
                                               V_IMAGE=IMAGE_1;
                                              break;
                               case 2:
                                               V_IMAGE=IMAGE_2;
                                              break;
                               case 3:
                                               V_IMAGE=IMAGE_3;
                                              break;
                               case 4:
                                               V_IMAGE=IMAGE_4;
                                              break;
                               case 5:
                                               V_IMAGE=IMAGE_5;
                                              break;
                               case 6:
                                               V_IMAGE=IMAGE_6;
                                              break;
                               case 7:
                                               V_IMAGE=IMAGE_7;
                                              break;
                               case 8:
                                               V_IMAGE=IMAGE_8;
                                              break;
                               case 9:
                                               V_IMAGE=IMAGE_9;
                                              break;
                               case 10:
                                               V_IMAGE=IMAGE_BAR;
																							 break;
									 						 case 11:
                                               V_IMAGE=IMAGE_BARNULL;
                }
#endif
							bitmap_layer_set_bitmap(bitmap_layer, V_IMAGE);
}

// PRINT_STATUS -----------------------------------------------------------------------------------------------------------------------------

static void battery_handler(BatteryChargeState charge_state)  {                                                  //DATUM UND AKKU AKTUALISIEREN
 if (showdate) {	
			static char battery_buffer[6];

			if (charge_state.is_charging) {
											snprintf(battery_buffer, sizeof(battery_buffer), "Zzz");
				} else {
											snprintf(battery_buffer, sizeof(battery_buffer), "%d%%", charge_state.charge_percent);
				}

				// Date
				time_t temp = time(NULL);
				struct tm *tick_time = localtime(&temp);

				static char date_buffer[30]= "00:00";
	 
				char *date_format = "%a %x";																							// all supported languages get their format
			  const	char *current_locale =setlocale(LC_ALL, "");

	 			if (twelvehr == false && strcmp("en_US", current_locale) == 0) {					// Pebble only knows en_US - typical...
									date_format= "%a %d/%m/%y";																			// if not 12 hour format and english this must be britain 
				}
	 
				strftime(date_buffer, 22, date_format, tick_time);

	 			strcat(strcat(date_buffer, "  "),battery_buffer);

				// Update time TextLayer
				
	 			text_layer_set_text_color(status_layer, Textcolour);
				text_layer_set_text(status_layer, date_buffer);

 } else {
					text_layer_set_text(status_layer,"");
	}												
}  

void handle_bluetooth( bool connected ) {
	
	if ( prev_bt_status != connected ) {vibes_double_pulse();}
	
	if ( connected ) {
		blit_digit(bar_layer, 10);       
		Textcolour=col0;

	} else {
		blit_digit(bar_layer, 11);       
		Textcolour=col3;
	}

	battery_handler(battery_state_service_peek());
	prev_bt_status = connected;
}
// UPDATE_TIME -----------------------------------------------------------------------------------------------------------------------------------

static void update_time() {                                                            // UHRZEIT AKTUALISIEREN
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
 
  int hour = tick_time->tm_hour;
  int minute = tick_time->tm_min;
   
  if(minute == 0) {
  
            // NEUE STUNDE
               
          blit_digit(links_unten_layer, 0 );                                          			//Minute1
          blit_digit(rechts_unten_layer, 0 );     																					//Minute2
          if (twelvehr && hour==13) {blit_digit(links_oben_layer, 0);}
					if (twelvehr && hour>12) {hour-=12;}																							//12-Stunden-Format                    																			
		
					if(( hour % 10)== 0) {blit_digit(links_oben_layer, hour / 10 ); }                 //Stunde1
          blit_digit(rechts_oben_layer, hour % 10 );                          							//Stunde2
 
					if (showdate && hour == 0) {battery_handler(battery_state_service_peek());}			  //neuer Tag - Datum anzeigen
				
  } else {
            // NEUE MINUTE
    
           if((minute % 10) == 0) {blit_digit(links_unten_layer, minute / 10);}        			// Minute1
           blit_digit(rechts_unten_layer, (minute % 10));                              			// Minute2
	
	}
}

// TICK_HANDLER ------------------------------------------------------------------------------------------------------------------------------------

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

// MAIN_WINDOW_LOAD --------------------------------------------------------------------------------------------------------------------------------

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
 
  /*   Define images                         */
 
	#if defined(ALTFONT)  
  IMAGE_0 = gbitmap_create_with_resource(RESOURCE_ID_digit_0);
  IMAGE_1 = gbitmap_create_with_resource(RESOURCE_ID_digit_1);
  IMAGE_2 = gbitmap_create_with_resource(RESOURCE_ID_digit_2);
  IMAGE_3 = gbitmap_create_with_resource(RESOURCE_ID_digit_3);
  IMAGE_4 = gbitmap_create_with_resource(RESOURCE_ID_digit_4);
  IMAGE_5 = gbitmap_create_with_resource(RESOURCE_ID_digit_5);
  IMAGE_6 = gbitmap_create_with_resource(RESOURCE_ID_digit_6);
  IMAGE_7 = gbitmap_create_with_resource(RESOURCE_ID_digit_7);
  IMAGE_8 = gbitmap_create_with_resource(RESOURCE_ID_digit_8);
  IMAGE_9 = gbitmap_create_with_resource(RESOURCE_ID_digit_9);
  IMAGE_BAR = gbitmap_create_with_resource(RESOURCE_ID_bar);
	#elif defined(NEUFONT)  
	IMAGE_0_alt = gbitmap_create_with_resource(RESOURCE_ID_image_0_alt);
  IMAGE_1_alt = gbitmap_create_with_resource(RESOURCE_ID_image_1_alt);
  IMAGE_2_alt = gbitmap_create_with_resource(RESOURCE_ID_image_2_alt);
  IMAGE_3_alt = gbitmap_create_with_resource(RESOURCE_ID_image_3_alt);
  IMAGE_4_alt = gbitmap_create_with_resource(RESOURCE_ID_image_4_alt);
  IMAGE_5_alt = gbitmap_create_with_resource(RESOURCE_ID_image_5_alt);
  IMAGE_6_alt = gbitmap_create_with_resource(RESOURCE_ID_image_6_alt);
  IMAGE_7_alt = gbitmap_create_with_resource(RESOURCE_ID_image_7_alt);
  IMAGE_8_alt = gbitmap_create_with_resource(RESOURCE_ID_image_8_alt);
  IMAGE_9_alt = gbitmap_create_with_resource(RESOURCE_ID_image_9_alt);
  IMAGE_BAR_alt = gbitmap_create_with_resource(RESOURCE_ID_image_bar_alt);
	#endif
	IMAGE_BARNULL = gbitmap_create_with_resource(RESOURCE_ID_BARNULL);
	
  /* Define graphic areas              */
 
	#if defined(PBL_RECT)
  int xadd=0;
	int yadd=0;
	#elif defined(PBL_ROUND)
  int xadd=18;
	int yadd=5;
	#endif
	
  links_oben_layer = bitmap_layer_create(GRect(4+xadd, 4+yadd, 66, 66));
  layer_add_child(window_layer, bitmap_layer_get_layer(links_oben_layer));
  
  rechts_oben_layer = bitmap_layer_create(GRect(74+xadd, 4+yadd, 66, 66));
  layer_add_child(window_layer, bitmap_layer_get_layer(rechts_oben_layer));
  
  links_unten_layer = bitmap_layer_create(GRect(4+xadd, 99+yadd, 66, 66));
  layer_add_child(window_layer, bitmap_layer_get_layer(links_unten_layer));
  
  rechts_unten_layer = bitmap_layer_create(GRect(74+xadd, 99+yadd, 66, 66));
  layer_add_child(window_layer, bitmap_layer_get_layer(rechts_unten_layer));
 
  bar_layer = bitmap_layer_create(GRect(4+xadd, 74+yadd, 136, 22));
	
	#if defined(NEUFONT)  
  bitmap_layer_set_bitmap(bar_layer, IMAGE_BAR_alt);
	#elif defined(ALTFONT)  
  bitmap_layer_set_bitmap(bar_layer, IMAGE_BAR);
	#endif
  layer_add_child(window_layer, bitmap_layer_get_layer(bar_layer));
 
  // Status Text Layer
	
  status_layer = text_layer_create(GRect(4+xadd, 72+yadd, 139, 19));
  text_layer_set_background_color(status_layer, GColorClear);
  text_layer_set_font(status_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_alignment(status_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(status_layer));
	
	redaw_entire_screen();
}
 
// REDRAW WINDOW - DIRTY DIANA ----------------------------------------------------------------------------------------------------------

static void redaw_entire_screen(){ 

	// All Images
	
	GColor *current_palettebarnull = gbitmap_get_palette(IMAGE_BARNULL);
	
	#if defined(ALTFONT) 

	GColor *current_palette0 = gbitmap_get_palette(IMAGE_0);
  GColor *current_palette1 = gbitmap_get_palette(IMAGE_1);
  GColor *current_palette2 = gbitmap_get_palette(IMAGE_2);
  GColor *current_palette3 = gbitmap_get_palette(IMAGE_3);
  GColor *current_palette4 = gbitmap_get_palette(IMAGE_4);
  GColor *current_palette5 = gbitmap_get_palette(IMAGE_5);
  GColor *current_palette6 = gbitmap_get_palette(IMAGE_6);
  GColor *current_palette7 = gbitmap_get_palette(IMAGE_7);
  GColor *current_palette8 = gbitmap_get_palette(IMAGE_8);
  GColor *current_palette9 = gbitmap_get_palette(IMAGE_9);
  GColor *current_palettebar = gbitmap_get_palette(IMAGE_BAR);
	GColor *Paletten[]={current_palette0,current_palette1, current_palette2,current_palette3,current_palette4,current_palette5, current_palette6,current_palette7,current_palette8,current_palette9,current_palettebar,current_palettebarnull};
	
	#elif defined(NEUFONT) 
	GColor *current_palette0_alt = gbitmap_get_palette(IMAGE_0_alt);
  GColor *current_palette1_alt = gbitmap_get_palette(IMAGE_1_alt);
  GColor *current_palette2_alt = gbitmap_get_palette(IMAGE_2_alt);
  GColor *current_palette3_alt = gbitmap_get_palette(IMAGE_3_alt);
  GColor *current_palette4_alt = gbitmap_get_palette(IMAGE_4_alt);
  GColor *current_palette5_alt = gbitmap_get_palette(IMAGE_5_alt);
  GColor *current_palette6_alt = gbitmap_get_palette(IMAGE_6_alt);
  GColor *current_palette7_alt = gbitmap_get_palette(IMAGE_7_alt);
  GColor *current_palette8_alt = gbitmap_get_palette(IMAGE_8_alt);
  GColor *current_palette9_alt = gbitmap_get_palette(IMAGE_9_alt);
  GColor *current_palettebar_alt = gbitmap_get_palette(IMAGE_BAR_alt);
  GColor *Paletten[]={current_palette0_alt,current_palette1_alt, current_palette2_alt,current_palette3_alt,current_palette4_alt,current_palette5_alt, current_palette6_alt,current_palette7_alt,current_palette8_alt,current_palette9_alt,current_palettebar_alt,current_palettebarnull};
	#endif
	
	
	GColor Farben[]={col0, col1, col2, col3};

	for(int pals=0; pals<12;pals++){
		incer=0;
		for(int i=0; i<4;i++){
			Paletten[pals][incer]=Farben[i];
			incer++;
		}	
	}
		
	time_t temp = time(NULL);
	struct tm *tick_time = localtime(&temp);
	int minute = tick_time->tm_min;
	int hour = tick_time->tm_hour;
	
	if (twelvehr && hour>12) {hour-=12;}																					//12-Stunden-Format
	
	blit_digit(links_oben_layer, hour / 10 );                   
	blit_digit(rechts_oben_layer, hour % 10 );
	blit_digit(links_unten_layer, minute / 10);                             
	blit_digit(rechts_unten_layer, minute % 10);  
	window_set_background_color(s_main_window, Paletten[0][0]);
	Textcolour=Paletten[0][0];

																																							//Datum anzeigen	
	handle_bluetooth(bluetooth_connection_service_peek());										// calls battery_handler(battery_state_service_peek());												
									
}

// SYSTEM RELATED -----------------------------------------------------------------------------------------------------------------------

static void main_window_unload(Window *window) {          
		bitmap_layer_destroy(links_oben_layer);
		bitmap_layer_destroy(rechts_oben_layer);
		bitmap_layer_destroy(links_unten_layer);
		bitmap_layer_destroy(rechts_unten_layer);
		bitmap_layer_destroy(bar_layer);
		text_layer_destroy(status_layer);
	
	#if defined(ALTFONT) 	
		gbitmap_destroy(IMAGE_0);
		gbitmap_destroy(IMAGE_1);
		gbitmap_destroy(IMAGE_2);
		gbitmap_destroy(IMAGE_3);
		gbitmap_destroy(IMAGE_4);
		gbitmap_destroy(IMAGE_5);
		gbitmap_destroy(IMAGE_6);
		gbitmap_destroy(IMAGE_7);
		gbitmap_destroy(IMAGE_8);
		gbitmap_destroy(IMAGE_9);
		gbitmap_destroy(IMAGE_BAR);

	#elif defined(NEUFONT) 
		gbitmap_destroy(IMAGE_0_alt);
		gbitmap_destroy(IMAGE_1_alt);
		gbitmap_destroy(IMAGE_2_alt);
		gbitmap_destroy(IMAGE_3_alt);
		gbitmap_destroy(IMAGE_4_alt);
		gbitmap_destroy(IMAGE_5_alt);
		gbitmap_destroy(IMAGE_6_alt);
		gbitmap_destroy(IMAGE_7_alt);
		gbitmap_destroy(IMAGE_8_alt);
		gbitmap_destroy(IMAGE_9_alt);
		gbitmap_destroy(IMAGE_BAR_alt);
	#endif
		gbitmap_destroy(IMAGE_BARNULL);
	
}

static void init() {
	
	  int red = persist_exists(KEY_COL00) ? persist_read_int(KEY_COL00): 0;
    int green = persist_exists(KEY_COL01) ? persist_read_int(KEY_COL01) : 0;
    int blue = persist_exists(KEY_COL02) ? persist_read_int(KEY_COL02) : 0;
    col0 = GColorFromRGB(red, green, blue);
	
	  red = persist_exists(KEY_COL10) ? persist_read_int(KEY_COL10) : 85;
    green = persist_exists(KEY_COL11) ? persist_read_int(KEY_COL11) : 85;
    blue = persist_exists(KEY_COL12) ? persist_read_int(KEY_COL12) : 85;
    col1 = GColorFromRGB(red, green, blue);
		
	  red = persist_exists(KEY_COL20) ? persist_read_int(KEY_COL20) : 170;
    green = persist_exists(KEY_COL21) ? persist_read_int(KEY_COL21) : 170;
    blue = persist_exists(KEY_COL22) ? persist_read_int(KEY_COL22) : 170;
    col2 = GColorFromRGB(red, green, blue);
	
	  red = persist_exists(KEY_COL30) ? persist_read_int(KEY_COL30) : 255;
    green = persist_exists(KEY_COL31) ? persist_read_int(KEY_COL31) : 255;
    blue = persist_exists(KEY_COL32) ? persist_read_int(KEY_COL32) : 255;
    col3 = GColorFromRGB(red, green, blue);
	
		twelvehr=persist_exists(KEY_TWELVEHR) ? persist_read_bool(KEY_TWELVEHR) : false ;

		showdate=persist_exists(KEY_SHOWDATE) ? persist_read_bool(KEY_SHOWDATE) : false ;
		prev_bt_status=bluetooth_connection_service_peek();
		s_main_window = window_create();

		tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
		
		battery_state_service_subscribe(battery_handler);
		bluetooth_connection_service_subscribe(handle_bluetooth);	

		window_set_window_handlers(s_main_window, (WindowHandlers) {
				.load = main_window_load,
				.unload = main_window_unload,
		});
		window_stack_push(s_main_window, true);
		app_message_register_inbox_received(inbox_received_handler);
  	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
	
}
 
static void deinit() {
	free(s_buffer);
	
 	window_destroy(s_main_window);
	battery_state_service_unsubscribe();
	tick_timer_service_unsubscribe();
	bluetooth_connection_service_unsubscribe();
	app_message_deregister_callbacks();
}
 
int main(void) {
		init();
		app_event_loop();
		deinit();
}
