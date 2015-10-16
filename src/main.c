#include "pebble.h"
 

#define KEY_COLOR_NR 0
#define KEY_INVERTED 1
#define KEY_TWELVEHR 2	
#define KEY_SHOWDATE 3

static bool directbool = false;
static int director = 0; 
static int colourptr = 0;  
static int incer =0 ;
static bool twelvehr = false;
static bool showdate =false;

static Window *s_main_window;

static BitmapLayer *links_oben_layer;
static BitmapLayer *rechts_oben_layer;
static BitmapLayer *bar_layer;
static BitmapLayer *links_unten_layer;
static BitmapLayer *rechts_unten_layer;

static TextLayer *status_layer;

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
static GBitmap *V_IMAGE;

static GColor Textcolour; 

// Placeholders
static void redaw_entire_screen();
//  BLIT DIGIT ------------------------------------------------------------------------------------------------------------------------------
static void inbox_received_handler(DictionaryIterator *iter, void *context) {
  Tuple *c_t = dict_find(iter, KEY_COLOR_NR);
  Tuple *director_t = dict_find(iter, KEY_INVERTED);
  Tuple *twelve_t = dict_find(iter, KEY_TWELVEHR);
  Tuple *show_t = dict_find(iter, KEY_SHOWDATE);
	
	if (c_t != 0) {
		colourptr = c_t->value->int16;
  //  persist_write_int(KEY_COLOR_NR, colourptr);
//		APP_LOG(APP_LOG_LEVEL_DEBUG, "Colour: %d", colourptr);
	}

  if (director_t) {
		//var test=director_t->value->cstring;
	//	if (director_t->value->cstring=='true'){director=1;} else {director=0;}
     director= director_t->value->int8;
	//if (directbool==true) {director=1;} else {director=0;} 
	persist_write_int(KEY_INVERTED, director);
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Inverted: %d", director);
 }
	
	
	if (twelve_t) {
		twelvehr = twelve_t->value->int8;
//		persist_write_bool(KEY_TWELVEHR, twelvehr);
//		if (twelvehrd==1){twelvehd=true;} else {twelvehd=false;}
//		APP_LOG(APP_LOG_LEVEL_DEBUG, "twelvehr: %d", twelvehr);
	}	
	
	if (show_t) {
		showdate = show_t->value->int8;		
//	 	persist_write_bool(KEY_SHOWDATE, showdate);
//				APP_LOG(APP_LOG_LEVEL_DEBUG, "showdate: %d", showdate);
	}
	
  redaw_entire_screen();
}
static void blit_digit(void *bitmap_layer, int image) {
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
                }

							bitmap_layer_set_bitmap(bitmap_layer, V_IMAGE);
}

// PRINT_STATUS -----------------------------------------------------------------------------------------------------------------------------

static void battery_handler(BatteryChargeState charge_state)  {                                                  //DATUM UND AKKU AKTUALISIEREN
 //		APP_LOG(APP_LOG_LEVEL_DEBUG, "Battery: %d", charge_state.charge_percent);
		// Battery        
		static char battery_buffer[6];

//		BatteryChargeState charge_state = battery_state_service_peek();

	if (charge_state.is_charging) {
									snprintf(battery_buffer, sizeof(battery_buffer), "Zzz");
		} else {
									snprintf(battery_buffer, sizeof(battery_buffer), "%d%%", charge_state.charge_percent);
		}

		// Date
		time_t temp = time(NULL);
		struct tm *tick_time = localtime(&temp);

		static char date_buffer[30]= "00:00";

	   if (twelvehr) {strftime(date_buffer, 22, "%a %m/%d/%Y", tick_time);} else {strftime(date_buffer, 22, "%a %d.%m.%Y", tick_time);}	
	
		strcat(strcat(date_buffer, "  "),battery_buffer);

		// Update time TextLayer
		text_layer_set_text_color(status_layer, Textcolour);
		text_layer_set_text(status_layer, date_buffer);
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
 
  /* Define graphic areas              */
 
  links_oben_layer = bitmap_layer_create(GRect(4, 4, 66, 66));
  layer_add_child(window_layer, bitmap_layer_get_layer(links_oben_layer));
  
  rechts_oben_layer = bitmap_layer_create(GRect(74, 4, 66, 66));
  layer_add_child(window_layer, bitmap_layer_get_layer(rechts_oben_layer));
  
  links_unten_layer = bitmap_layer_create(GRect(4, 99, 66, 66));
  layer_add_child(window_layer, bitmap_layer_get_layer(links_unten_layer));
  
  rechts_unten_layer = bitmap_layer_create(GRect(74, 99, 66, 66));
  layer_add_child(window_layer, bitmap_layer_get_layer(rechts_unten_layer));
 
  bar_layer = bitmap_layer_create(GRect(4, 74, 136, 22));
  bitmap_layer_set_bitmap(bar_layer, IMAGE_BAR);
  layer_add_child(window_layer, bitmap_layer_get_layer(bar_layer));
 
  // Status Text Layer
	
  status_layer = text_layer_create(GRect(4, 72, 139, 19));
  text_layer_set_background_color(status_layer, GColorClear);
  text_layer_set_font(status_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_alignment(status_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(status_layer));
	
	redaw_entire_screen();
}
 
// REDRAW WINDOW - DIRTY DIANA ----------------------------------------------------------------------------------------------------------

static void redaw_entire_screen(){ 
 GColor farben[]={	GColorBulgarianRose, GColorRoseVale, GColorMelon, GColorWhite, 
GColorImperialPurple, GColorPurpureus, GColorRichBrilliantLavender, GColorWhite, 
GColorDarkCandyAppleRed, GColorSunsetOrange, GColorMelon, GColorWhite, 
GColorDarkCandyAppleRed, GColorBulgarianRose, GColorBulgarianRose, GColorBlack, 
GColorRed, GColorDarkCandyAppleRed, GColorBulgarianRose, GColorBlack,
GColorBlueMoon, GColorPictonBlue, GColorCeleste, GColorWhite, 
GColorDukeBlue, GColorVeryLightBlue, GColorBabyBlueEyes, GColorWhite, 
GColorMidnightGreen, GColorCadetBlue, GColorCeleste, GColorWhite, 
GColorOxfordBlue, GColorLiberty, GColorBabyBlueEyes, GColorWhite, 
GColorBlueMoon, GColorDukeBlue, GColorOxfordBlue, GColorBlack, 
GColorDarkGreen, GColorMayGreen, GColorMelon, GColorWhite, 
GColorIslamicGreen, GColorScreaminGreen, GColorMelon, GColorWhite, 
GColorArmyGreen, GColorBrass, GColorPastelYellow, GColorWhite, 
GColorGreen, GColorScreaminGreen, GColorMelon, GColorWhite, 
GColorGreen, GColorIslamicGreen, GColorDarkGreen, GColorBlack, 
GColorMediumSpringGreen, GColorJaegerGreen, GColorDarkGreen, GColorBlack,
GColorSunsetOrange, GColorMelon, GColorCeleste, GColorElectricBlue, 
GColorMediumSpringGreen, GColorScreaminGreen, GColorSpringBud, GColorYellow, 
GColorBlueMoon, GColorCadetBlue, GColorInchworm, GColorYellow, 
GColorYellow, GColorChromeYellow, GColorOrange, GColorRed, 
GColorMidnightGreen, GColorDarkGray, GColorBrass, GColorIcterine, 
GColorSunsetOrange, GColorSunsetOrange, GColorRajah, GColorYellow,
GColorBlack, GColorDarkGray, GColorLightGray, GColorWhite, 
GColorWindsorTan, GColorRajah, GColorIcterine, GColorPastelYellow, 
GColorWindsorTan, GColorRajah, GColorPastelYellow, GColorWhite, 
GColorWindsorTan, GColorArmyGreen, GColorDarkGreen, GColorBlack, 
GColorDarkGray, GColorLightGray, GColorLightGray, GColorWhite}; 
	// All Images
	
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
  GColor *Paletten[]={current_palette0,current_palette1, current_palette2,current_palette3,current_palette4,current_palette5, current_palette6,current_palette7,current_palette8,current_palette9,current_palettebar};

		 //  Color Sets


		
	for(int pals=0;pals<11;pals++){                                                                                                         // walk through all images of the project
		incer=0; 
			for(int i=(colourptr*4+(3*director));i!=(colourptr*4+abs((4*director)-4)-director);i=(i+((director-1+director))*-1)) {          		// up- or down the colour array depending on director
				Paletten[pals][incer]=farben[i];  
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

	if (showdate) {																															//Datum anzeigen	
						battery_handler(battery_state_service_peek());
	} else {
						text_layer_set_text(status_layer,"");
	}																		
									
}

// SYSTEM RELATED -----------------------------------------------------------------------------------------------------------------------

static void main_window_unload(Window *window) {          
		bitmap_layer_destroy(links_oben_layer);
		bitmap_layer_destroy(rechts_oben_layer);
		bitmap_layer_destroy(links_unten_layer);
		bitmap_layer_destroy(rechts_unten_layer);
		bitmap_layer_destroy(bar_layer);
		text_layer_destroy(status_layer);
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
}
 

static void init() {
	
		colourptr = persist_exists(KEY_COLOR_NR) ? persist_read_int(KEY_COLOR_NR) : 1 ;
//		if (persist_exists(KEY_COLOR_NR)) {
 // 		  colourptr = persist_read_int(KEY_COLOR_NR);
//   APP_LOG(APP_LOG_LEVEL_DEBUG,"loaded color: %d", colourptr);
//   	}
	director=persist_exists(KEY_INVERTED) ? persist_read_int(KEY_INVERTED) : 0 ;
//		if (persist_exists(KEY_INVERTED)) {
 //   		director = persist_read_int(KEY_INVERTED); }

	twelvehr=persist_exists(KEY_TWELVEHR) ? persist_read_bool(KEY_TWELVEHR) : false ;
//	if (persist_exists(KEY_TWELVEHR)) {
//  	  twelvehr = persist_read_bool(KEY_TWELVEHR);
//	}

	showdate=persist_exists(KEY_SHOWDATE) ? persist_read_bool(KEY_SHOWDATE) : false ;
	
//	if (persist_exists(KEY_SHOWDATE)) {
// 		showdate = persist_read_bool(KEY_SHOWDATE);
//	}
	
		s_main_window = window_create();
//		window_set_background_color(s_main_window, GColorBlack);
		tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
		
		battery_state_service_subscribe(battery_handler);
	
	

		
		window_set_window_handlers(s_main_window, (WindowHandlers) {
				.load = main_window_load,
				.unload = main_window_unload,
		});
		window_stack_push(s_main_window, true);
		app_message_register_inbox_received(inbox_received_handler);
  	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
	
}
 
static void deinit() {
	
	persist_write_int(KEY_COLOR_NR, colourptr);
	persist_write_int(KEY_INVERTED, director);
	persist_write_bool(KEY_TWELVEHR, twelvehr);
	persist_write_bool(KEY_SHOWDATE, showdate);
	
 	window_destroy(s_main_window);
	battery_state_service_unsubscribe();
	tick_timer_service_unsubscribe();
}
 
int main(void) {
		init();
		app_event_loop();
		deinit();
}
