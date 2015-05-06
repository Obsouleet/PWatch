#include "pebble.h"
 
#define TAP_TIME 2000	
#define EDIT_MODE_DURATION 3000
#define TRIPPLE_TAP_TIME 900
#define THROTTLE_TIME 100
	
static int director = 1; 
static int colourcounter=0*4;  
static int colsetno=0;
static int incer=0;
static int ctr=0;
static bool is_throttled=false;
static bool editmode=false;


static Window *s_main_window;

static BitmapLayer *links_oben_layer;
static BitmapLayer *rechts_oben_layer;
static BitmapLayer *bar_layer;
static BitmapLayer *links_unten_layer;
static BitmapLayer *rechts_unten_layer;

static TextLayer *status_layer;

static AppTimer *tap_timer = NULL;	
static AppTimer *throttle_timer = NULL;	
static AppTimer *enter_mode_timer = NULL;	
static AppTimer *edit_mode_timer = NULL;	

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

static void print_status(Window *window) {                                                  //DATUM UND AKKU AKTUALISIEREN
 
		// Battery        
		static char battery_buffer[6];

		BatteryChargeState charge_state = battery_state_service_peek();
		if (charge_state.is_charging) {
									snprintf(battery_buffer, sizeof(battery_buffer), "ZzZz");
		} else {
									snprintf(battery_buffer, sizeof(battery_buffer), "%d%%", charge_state.charge_percent);
		}

		// Date
		time_t temp = time(NULL);
		struct tm *tick_time = localtime(&temp);

		static char date_buffer[30]= "00:00";
		strftime(date_buffer, 22, "%a %d.%m.%Y", tick_time);
		strcat(strcat(date_buffer, " - "),battery_buffer);

		// Update time TextLayer
		text_layer_set_text_color(status_layer, Textcolour);
		text_layer_set_text(status_layer, date_buffer);
}

// TIMER_CALLBACK ---------------------------------------------------------------------------------------------------------------------------

static void timer_callback() {
  text_layer_set_text(status_layer, "");
  // DEBUG ONLY - VIBE TRIGGERS ANOTHER TAP
  // vibes_short_pulse();
}

static void reset_throttle() {
	is_throttled=false;
}

static void edit_mode_timer_reset() {
	editmode=false;
	text_layer_set_text(status_layer, "");
}

static void enter_mode_timer_reset() {
	if(editmode==false) {																					// trippleclick and outside editmode 
			if(ctr>2) {
				app_timer_cancel(tap_timer);
				editmode=true;
				text_layer_set_text_color(status_layer, Textcolour);
				text_layer_set_text(status_layer, ">>>> Edit Mode <<<<");
				edit_mode_timer = app_timer_register(EDIT_MODE_DURATION, edit_mode_timer_reset, NULL);
			}
	}
ctr=0;	
}


// TAP-HANDLER -------------------------------------------------------------------------------------------------------------------------------

static void tap_handler(AccelAxisType axis, int32_t direction) {

if(is_throttled==false) {
		is_throttled=true;
		throttle_timer = app_timer_register(THROTTLE_TIME, reset_throttle, NULL);
	
		switch (axis) {
		case ACCEL_AXIS_X:
				if (direction > 0) {
					APP_LOG(APP_LOG_LEVEL_INFO, "X axis positive.");
				} else {
					APP_LOG(APP_LOG_LEVEL_INFO, "X axis negative.");
				}
				break;
			
		case ACCEL_AXIS_Y:
				if (direction > 0) {
					if(editmode==true) {
						colsetno++;
						if(colsetno>3) {colsetno=0;}
					}
					APP_LOG(APP_LOG_LEVEL_INFO, "Y axis positive.");
				} else {
					APP_LOG(APP_LOG_LEVEL_INFO, "Y axis negative.");
				}
				break;
			
		case ACCEL_AXIS_Z:
				if (direction > 0) {
																// display date if not in edit mode
						if(editmode==false){ 
								if(app_timer_reschedule(tap_timer, TAP_TIME)==false) {
									tap_timer = app_timer_register(TAP_TIME, timer_callback, NULL);														// reschedule TAPtimer else set em 				
									text_layer_set_text_color(status_layer, Textcolour);
									print_status(s_main_window);
								}	
								APP_LOG(APP_LOG_LEVEL_INFO, "normal single tap");
																
								if(ctr==0){
										enter_mode_timer = app_timer_register(TRIPPLE_TAP_TIME, enter_mode_timer_reset, NULL);
										ctr=1;
										APP_LOG(APP_LOG_LEVEL_INFO, "ctr = 1");
								} else {
										ctr++;
										APP_LOG(APP_LOG_LEVEL_INFO, "ctr ++");
								}
						} else {																												// editmode single tap
									app_timer_cancel(tap_timer);
									redaw_entire_screen();
							    text_layer_set_text_color(status_layer, Textcolour);
									text_layer_set_text(status_layer, ">>>> Edit Mode <<<<");
									if(app_timer_reschedule(edit_mode_timer, EDIT_MODE_DURATION)==false) {
										edit_mode_timer = app_timer_register(EDIT_MODE_DURATION, edit_mode_timer_reset, NULL);					// keep editmode for another 3 secs
										editmode=true;
										text_layer_set_text_color(status_layer, Textcolour);
										text_layer_set_text(status_layer, ">>>> Edit Mode <<<<");				
									}									
						}

				} else {
					APP_LOG(APP_LOG_LEVEL_INFO, "Z axis negative.");
				}
				break;
		}
  }
}


// UPDATE_TIME -----------------------------------------------------------------------------------------------------------------------------------

static void update_time() {                                                            // UHRZEIT AKTUALISIEREN
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
 
   // Display the time on the Pebble

  int hour = tick_time->tm_hour;
  int minute = tick_time->tm_min;
                
  if(minute == 0) {
  
            // NEUE STUNDE
               
           blit_digit(links_unten_layer, 0 );                                          //Minute1
           blit_digit(rechts_unten_layer, 0 );                                         //Minute2
  
           if((hour % 10) == 0) {
                     blit_digit(links_oben_layer, hour / 10 );                           //Stunde1
                     blit_digit(rechts_oben_layer, hour % 10 );                          //Stunde2
           } else {
                     blit_digit(rechts_oben_layer, hour % 10 );                          //Stunde2                                         
           }
 
  } else {
            // NEUE MINUTE
    
           if((minute % 10) == 0) {blit_digit(links_unten_layer, minute / 10);}        // Minute1
           blit_digit(rechts_unten_layer, (minute % 10));                              // Minute2
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
  status_layer = text_layer_create(GRect(7, 76, 130, 17));
  text_layer_set_background_color(status_layer, GColorClear);
  text_layer_set_font(status_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(status_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(status_layer));
	
  redaw_entire_screen();
	
}
 
// REDRAW WINDOW - DIRTY DIANA ----------------------------------------------------------------------------------------------------------

static void redaw_entire_screen(){ 
	
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

GColor colours[]={	GColorBlueMoon,	GColorPictonBlue,	GColorCeleste,GColorWhite,
					GColorWindsorTan,	GColorRajah,	GColorIcterine,	GColorPastelYellow,		
					GColorDarkGreen,	GColorMayGreen,	GColorMelon,	GColorWhite,		
					GColorBlack,	GColorDarkGray,	GColorLightGray,	GColorWhite,		
					GColorIslamicGreen,	GColorScreaminGreen,	GColorMelon,	GColorWhite,		
					GColorDukeBlue,	GColorVeryLightBlue,	GColorBabyBlueEyes,	GColorWhite,		
					GColorArmyGreen,	GColorBrass,	GColorPastelYellow,	GColorWhite,		
					GColorBulgarianRose,	GColorRoseVale,	GColorMelon,	GColorWhite,		
					GColorImperialPurple,	GColorPurpureus,	GColorRichBrilliantLavender,	GColorWhite,		
					GColorMidnightGreen,	GColorCadetBlue,	GColorCeleste,	GColorWhite,		
					GColorOxfordBlue,	GColorLiberty,	GColorBabyBlueEyes,	GColorWhite,		
					GColorDarkCandyAppleRed,	GColorSunsetOrange,	GColorMelon,	GColorWhite,		
					GColorWindsorTan,	GColorRajah,	GColorPastelYellow,	GColorWhite,		
					GColorGreen,	GColorScreaminGreen,	GColorMelon,	GColorWhite	};

GColor reds[]={	GColorBulgarianRose, GColorRoseVale, GColorMelon, GColorWhite, 
GColorImperialPurple, GColorPurpureus, GColorRichBrilliantLavender, GColorWhite, 
GColorDarkCandyAppleRed, GColorSunsetOrange, GColorMelon, GColorWhite, 
GColorDarkCandyAppleRed, GColorBulgarianRose, GColorBulgarianRose, GColorBlack, 
GColorRed, GColorDarkCandyAppleRed, GColorBulgarianRose, GColorBlack, 
GColorYellow, GColorLimerick, GColorArmyGreen, GColorBlack, 
GColorYellow, GColorIcterine, GColorPastelYellow, GColorWhite};
  
GColor blues[]={GColorBlueMoon, GColorPictonBlue, GColorCeleste, GColorWhite, 
GColorDukeBlue, GColorVeryLightBlue, GColorBabyBlueEyes, GColorWhite, 
GColorMidnightGreen, GColorCadetBlue, GColorCeleste, GColorWhite, 
GColorOxfordBlue, GColorLiberty, GColorBabyBlueEyes, GColorWhite, 
GColorBlueMoon, GColorDukeBlue, GColorOxfordBlue, GColorBlack, 
GColorBlueMoon, GColorDukeBlue, GColorOxfordBlue, GColorBlack};
					
GColor greens[]={GColorDarkGreen, GColorMayGreen, GColorMelon, GColorWhite, 
GColorIslamicGreen, GColorScreaminGreen, GColorMelon, GColorWhite, 
GColorArmyGreen, GColorBrass, GColorPastelYellow, GColorWhite, 
GColorGreen, GColorScreaminGreen, GColorMelon, GColorWhite, 
GColorGreen, GColorIslamicGreen, GColorDarkGreen, GColorBlack, 
GColorMediumSpringGreen, GColorJaegerGreen, GColorDarkGreen, GColorBlack };

GColor others[]={GColorSunsetOrange, GColorMelon, GColorCeleste, GColorElectricBlue, 
GColorMediumSpringGreen, GColorScreaminGreen, GColorSpringBud, GColorYellow, 
GColorBlueMoon, GColorCadetBlue, GColorInchworm, GColorYellow, 
GColorYellow, GColorChromeYellow, GColorOrange, GColorRed, 
GColorMidnightGreen, GColorDarkGray, GColorBrass, GColorIcterine, 
GColorSunsetOrange, GColorSunsetOrange, GColorRajah, GColorYellow};
 
GColor darks[]={GColorBlack, GColorDarkGray, GColorLightGray, GColorWhite, 
GColorWindsorTan, GColorRajah, GColorIcterine, GColorPastelYellow, 
GColorWindsorTan, GColorRajah, GColorPastelYellow, GColorWhite, 
GColorWindsorTan, GColorArmyGreen, GColorDarkGreen, GColorBlack, 
GColorDarkGray, GColorLightGray, GColorLightGray, GColorWhite}; 

	GColor *Colsets[]={reds,blues,greens,darks,others};
		
	for(int pals=0;pals<11;pals++){                                                                                                         // walk through all images of the project

			incer=0; 																																																														// PURE MAGIC !!

			for(int i=(colourcounter+(3*director));i!=(colourcounter+abs((4*director)-4)-director);i=(i+((director-1+director))*-1)) {          // up- or down the colour array 
				Paletten[pals][incer]=Colsets[colsetno][i];                                                                                         
				incer++;
			}
	}  
	
	static int n = sizeof(Colsets[colsetno])/sizeof(Colsets[colsetno][0]);
	if (director==1){
				director = 0;                                       // change direction
				colourcounter=colourcounter + 4;                    // next colourset
				if(colourcounter>n){colourcounter=0;}              // reset Colourcycle
	}  else {
				director = 1;                                       // change direction
	}
	 
	time_t temp = time(NULL);
	struct tm *tick_time = localtime(&temp);
	int minute = tick_time->tm_min;
	int hour = tick_time->tm_hour;
		
	blit_digit(links_oben_layer, hour / 10 );                   
	blit_digit(rechts_oben_layer, hour % 10 );
	blit_digit(links_unten_layer, minute / 10);                             
	blit_digit(rechts_unten_layer, minute % 10);  
	window_set_background_color(s_main_window, Paletten[0][0]);
	Textcolour=Paletten[0][0];

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
		s_main_window = window_create();
//		window_set_fullscreen(s_main_window, true);
		window_set_background_color(s_main_window, GColorBlack);
		accel_tap_service_subscribe(tap_handler);
		tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
		window_set_window_handlers(s_main_window, (WindowHandlers) {
				.load = main_window_load,
				.unload = main_window_unload,
		});
		window_stack_push(s_main_window, true);
}
 
static void deinit() {
 	 window_destroy(s_main_window);
}
 
int main(void) {
		init();
		app_event_loop();
		deinit();
}
